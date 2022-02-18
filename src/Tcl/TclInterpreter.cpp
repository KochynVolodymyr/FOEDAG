/*
Copyright 2021 The Foedag team

GPL License

Copyright (c) 2021 The Open-Source FPGA Foundation

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "TclInterpreter.h"

#include <mutex>

using namespace FOEDAG;

#include <tcl.h>

TclInterpreter::TclInterpreter(const char *argv0) : interp(nullptr) {
  static bool initLib;
  if (!initLib) {
    Tcl_FindExecutable(argv0);
    initLib = true;
  }
  interp = Tcl_CreateInterp();
  Tcl_Init(interp);
  if (!interp) throw new std::runtime_error("failed to initialise Tcl library");
  evalCmd(TclHistoryScript());
}

TclInterpreter::~TclInterpreter() {
  if (interp) Tcl_DeleteInterp(interp);
}

std::string TclInterpreter::evalFile(const std::string &filename) {
  int code = Tcl_EvalFile(interp, filename.c_str());

  if (code >= TCL_ERROR) {
    return std::string("Tcl Error: " +
                       std::string(Tcl_GetStringResult(interp)));
  }
  return std::string(Tcl_GetStringResult(interp));
}

std::string TclInterpreter::evalCmd(const std::string cmd, int *ret) {
  int code = Tcl_Eval(interp, cmd.c_str());
  if (ret) *ret = code;

  if (code >= TCL_ERROR) {
    return std::string("Tcl Error: " +
                       std::string(Tcl_GetStringResult(interp)));
  }
  return std::string(Tcl_GetStringResult(interp));
}

void TclInterpreter::registerCmd(const std::string &cmdName, Tcl_CmdProc proc,
                                 ClientData clientData,
                                 Tcl_CmdDeleteProc *deleteProc) {
  Tcl_CreateCommand(interp, cmdName.c_str(), proc, clientData, deleteProc);
}

std::string TclInterpreter::evalGuiTestFile(const std::string &filename) {
  std::string testHarness = R"(
  set TEST_ENABLES 1
  set cmds [list]
  set state 0
  proc test_enables {} {
    global TEST_ENABLES
    puts "YES, WE ARE HERE"
    incr TEST_ENABLES
  }
  proc test_done {} {
    global TEST_ENABLES state
    set TEST_ENABLES [expr $TEST_ENABLES - 1]
    incr state
    puts "TEST DONE $TEST_ENABLES"
  }
  proc run_cmd {cmdNumber cmdCount} {
    global cmds state
    if {$cmdNumber == $cmdCount} {
      return
    }
    set cmd [lindex $cmds $cmdNumber]
    if {$cmd == ""} {
      error "Empty command"
      exit 1
    }
    puts "$cmdNumber/$cmdCount $cmd"
    eval $cmd
    process_qt_events
    after 1000
    vwait state
    set state 0
    after 10 "run_cmd [expr $cmdNumber+1] $cmdCount"
  }
  proc test_harness { gui_script } {
    global CONT errorInfo TEST_ENABLES cmds
    set fid [open $gui_script]
    set content [read $fid]
    set cmd_count 0
    close $fid
    set errorInfo ""

    catch {
        
        # Schedule commands
        set lines [split $content "\n"]
        set STEP 500
        set time $STEP
        foreach line $lines {
            if {[regexp {^#} $line]} {
                continue
            }
            if {$line == ""} {
                continue
            }
            lappend cmds $line
            #after $time $line
            #after $time process_qt_events
            set time [expr $time + $STEP]
        }
        lappend cmds test_done
        set cmd_count [llength $cmds]
        set time [expr $time + $STEP]
        #set cmd_count [expr $cmd_count-1]
        after $STEP "run_cmd 0 $cmd_count"
    }
    
    # Schedule GUI exit
    set time [expr $time + 50000]
    after $time "error \"GUI EXIT TIMEOUT \" ; flush stderr; set CONT 0"
    #after $time "puts \"GUI EXIT\" ; flush stdout; set CONT 0"

    # Enter loop
    set CONT 1 
    puts TEST_LOOP_ENTERED
    flush stdout
    while {$TEST_ENABLES && $CONT} {
        set a 0
        after 10 set a 1
        vwait a
        if {$errorInfo != ""} {
          puts $errorInfo
          exit 1
        }
    }
    puts TEST_LOOP_EXITED$TEST_ENABLES
    flush stdout
    if {$errorInfo != ""} {
        puts $errorInfo
        exit 1
    }
    
    puts "Tcl Exit" ; flush stdout
    tcl_exit
  }

  )";

  std::string call_test = "proc call_test { } {\n";
  call_test += "test_harness " + filename + "\n";
  call_test += "}\n";

  std::string completeScript = testHarness + "\n" + call_test;

  int code = Tcl_Eval(interp, completeScript.c_str());

  if (code >= TCL_ERROR) {
    return std::string("Tcl Error: " +
                       std::string(Tcl_GetStringResult(interp)));
  }
  return std::string(Tcl_GetStringResult(interp));
}
