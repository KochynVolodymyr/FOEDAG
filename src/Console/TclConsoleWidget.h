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
#pragma once

#include <QPlainTextEdit>
#include <QTextBlock>
#include <memory>
#include <ostream>

#include "ConsoleDefines.h"
#include "ConsoleInterface.h"
#include "OutputFormatter.h"
#include "QConsole/qconsole.h"

namespace FOEDAG {

enum class State {
  IDLE,
  IN_PROGRESS,
};

class StreamBuffer;
class TclInterpreter;
class TclConsoleWidget : public QConsole {
  Q_OBJECT
 public:
  explicit TclConsoleWidget(TclInterpreter *interp,
                            std::unique_ptr<ConsoleInterface> iConsole,
                            StreamBuffer *buffer, QWidget *parent = nullptr);
  bool isRunning() const override;
  QString getPrompt() const;
  StreamBuffer *getBuffer();
  static const char *consoleObjectName();

  State state() const;

  /*!
   * \brief setParsers. Console will take ownership of the parsers
   */
  void setParsers(const std::vector<LineParser *> &parsers);
  /*!
   * \brief addParser. Add parser to the list and take ownership of the pointer
   */
  void addParser(LineParser *parser);
  void registerCommands(TclInterp *interp);

 public slots:
  void clearText();

 signals:
  void searchEnable();
  void stateChanged(FOEDAG::State);
  void linkActivated(const QString &);
  void sendCommand(const QString &);

 protected:
  QString interpretCommand(const QString &command, int *res) override;
  QStringList suggestCommand(const QString &cmd, QString &prefix) override;
  bool isCommandComplete(const QString &command) override;
  void handleSearch() override;
  void handleTerminateCommand() override;
  void mouseReleaseEvent(QMouseEvent *e) override;
  void mousePressEvent(QMouseEvent *e) override;
  void mouseMoveEvent(QMouseEvent *e) override;

 private slots:
  void put(const QString &str) override;
  void commandDone();

 private:
  void setState(const State &state);
  void handleLink(const QPoint &p);
  bool hasPrompt() const;
  bool handleCommandFromHistory(const QString &command,
                                QString &commandFromHist);

  bool hasOpenBracket(const QString &str) const;
  bool hasCloseBracket(const QString &str) const;

 private:
  std::unique_ptr<ConsoleInterface> m_console;
  StreamBuffer *m_buffer;
  State m_state = State::IDLE;

  bool m_linkActivated{true};
  Qt::MouseButton m_mouseButtonPressed{Qt::NoButton};
  OutputFormatter m_formatter;
  TclInterpreter *m_interp;
};

}  // namespace FOEDAG
