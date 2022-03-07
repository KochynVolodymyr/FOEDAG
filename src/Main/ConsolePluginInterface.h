/*
Copyright 2022 The Foedag team

GPL License

Copyright (c) 2022 The Open-Source FPGA Foundation

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
#pragma once
#include <QtPlugin>

class Tcl_Interp;
namespace FOEDAG {
class TclInterpreter;

class ConsolePluginInterface {
 public:
  virtual ~ConsolePluginInterface() = default;
  virtual QWidget* getConsole(TclInterpreter *interp, QWidget **console) = 0;
  virtual void runCommand(TclInterpreter *interp, const QString &cmd) = 0;
};

class ConsoleWidgetInterface {
 public:
  virtual ~ConsoleWidgetInterface() = default;
};

}  // namespace FOEDAG
#define ConsolePluginInterface_iid "foedag.ConsolePluginInterface/1.0"
Q_DECLARE_INTERFACE(FOEDAG::ConsolePluginInterface, ConsolePluginInterface_iid)

#define ConsoleWidgetInterface_iid "foedag.ConsoleWidgetInterface/1.0"
Q_DECLARE_INTERFACE(FOEDAG::ConsoleWidgetInterface, ConsoleWidgetInterface_iid)
