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
#include "ConsolePlugin.h"
#include "StreamBuffer.h"
#include "TclConsole.h"
#include "TclConsoleBuilder.h"
#include "DummyParser.h"
#include "Tcl/TclInterpreter.h"

#include <QWidget>
#include <memory>

namespace FOEDAG {

QWidget* ConsolePlugin::getConsole(TclInterpreter *interp, QWidget **consolePtr) {
  StreamBuffer* buffer = new StreamBuffer;
  auto tclConsole = std::make_unique<TclConsole>(
      interp->getInterp(), buffer->getStream());
//  FOEDAG::TclConsole* c = tclConsole.get();
  TclConsoleWidget* console{nullptr};
  QWidget* w =
      createConsole(interp, std::move(tclConsole), buffer, nullptr, &console);
  //  connect(console, &TclConsoleWidget::linkActivated, textEditor,
  //          &TextEditor::SlotOpenFile);
    console->addParser(new DummyParser{});
  qDebug() << (w->findChild<TclConsoleWidget *>() == nullptr);
    if (consolePtr) *consolePtr = console;
  return w;
}

void ConsolePlugin::runCommand(TclInterpreter *interp, const QString &cmd) {
  interp->evalCmd(cmd.toStdString());
}

}  // namespace FOEDAG
