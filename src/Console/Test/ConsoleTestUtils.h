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

#include <QApplication>
#include <QKeyEvent>
#include <QMetaType>

#include "Main/Foedag.h"
#include "StreamBuffer.h"
#include "Tcl/TclInterpreter.h"
#include "TclConsole.h"
#include "TclConsoleBuilder.h"

namespace FOEDAG {

void sendCommand(const QString& command, QObject* receiver);
TclConsoleWidget* InitConsole(void* clientData);
static const QChar controlC{0x3};

class StateCheck : public QObject {
  Q_OBJECT
  QString m_text;
  TclConsoleWidget* m_console;
  TclInterp *m_interp;
  bool m_pass{false};
  uint m_commandCount{1};

 public:
  StateCheck(const QString& textToCheck, FOEDAG::TclConsoleWidget* console, TclInterp *interp)
      : m_text(textToCheck), m_console(console), m_interp(interp) {
    connect(console, &FOEDAG::TclConsoleWidget::stateChanged, this,
            &StateCheck::stateChanged);
    connect(this, &StateCheck::check, this, &StateCheck::stateChanged,
            Qt::QueuedConnection);
  }
  ~StateCheck();

  /*!
   * \brief checkStateQueue. Put into the queue cheching console state
   */
  void checkStateQueue() { emit check(FOEDAG::State::IDLE); }
  void setCommandCount(uint count);

 signals:
  void check(FOEDAG::State);

 public slots:
  void stateChanged(FOEDAG::State st) {
    if (st == FOEDAG::State::IDLE) {
      m_commandCount--;
      if (m_commandCount != 0) {
        return;
      }
      QString consoleText = m_console->toPlainText();
      if (consoleText != m_text) {
        qDebug() << "FAILED";
        qDebug() << "Expected: " << m_text;
        qDebug() << "Actual:   " << consoleText;
        testFail("");
      } else {
        qDebug() << "SUCCESS";
        m_pass = true;
        Tcl_Eval(m_interp, "after 10 test_done");

        // it is important to disconnect everything
        deleteLater();
      }
    }
  }

 private:
  void testFail(const QString& message);
};

/*!
 *  sent one command \a cmd to the console and compare with \a expectedOut
 */
#define CHECK_EXPECTED(cmd, expectedOut)                                    \
  FOEDAG::StateCheck* check = new FOEDAG::StateCheck{expectedOut, console, interp}; \
  Q_UNUSED(check)                                                           \
  sendCommand(cmd, console);

/*!
 *  sent few commands \a cmd to the console and compare with \a expectedOut
 *  only after last command done.
 */
#define CHECK_EXPECTED_FOR_FEW_COMMANDS(cmd, expectedOut, commandCount)     \
  FOEDAG::StateCheck* check = new FOEDAG::StateCheck{expectedOut, console, interp}; \
  check->setCommandCount(commandCount);                                     \
  sendCommand(cmd, console);

/*!
 * send one command \a cmd to the console and check the results immediately. It
 * doesn't wait for anything.
 */
#define CHECK_EXPECTED_NOW(cmd, expectedOut)                                \
  FOEDAG::StateCheck* check = new FOEDAG::StateCheck{expectedOut, console, interp}; \
  sendCommand(cmd, console);                                                \
  check->checkStateQueue();

}  // namespace FOEDAG
Q_DECLARE_METATYPE(FOEDAG::State);
