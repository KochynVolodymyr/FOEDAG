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

#include "SynthesisReportManager.h"

#include <QFile>
#include <QRegularExpression>
#include <QTextStream>

#include "CompilerDefines.h"
#include "TableReport.h"

namespace {
static constexpr const char *REPORT_NAME{"Synthesis report"};
static constexpr const char *MAX_LVL_STR{"Maximum logic level"};
static constexpr const char *AVG_LVL_STR{"Average logic level"};
}  // namespace

namespace FOEDAG {

QStringList SynthesisReportManager::getAvailableReportIds() const {
  return {QString(REPORT_NAME)};
}

ITaskReport::TableData SynthesisReportManager::getStatistics(
    const QString &statsStr) const {
  auto res = ITaskReport::TableData{};

  auto line = QString{};        // Unmodified line from the log file
  auto dataLine = QString{};    // Simplified line
  auto parentItem = QString{};  // Parent item for lines starting with tab

  auto statTable =
      QRegExp("Number.*");  // Drop the beginning and start with stats
  if (statTable.indexIn(statsStr) == -1) return res;

  QTextStream in(statTable.cap().toLatin1());

  while (in.readLineInto(&line)) {
    if (line.startsWith("     ") && parentItem.isEmpty())
      parentItem = dataLine.left(dataLine.indexOf(':'));

    dataLine = line.simplified();
    if (dataLine.isEmpty()) break;

    auto reportLine = QStringList{};
    auto lineStrs = dataLine.contains(":") ? dataLine.split(":")
                                           : dataLine.split(QChar::Space);

    for (auto i = 0; i < lineStrs.size(); ++i) {
      auto lineStr = lineStrs[i];
      // The first string represents statistic name. Parent item should only be
      // added there.
      if (!i && !parentItem.isEmpty())
        lineStr = QString("%1:%2").arg(parentItem, lineStr);
      reportLine << lineStr;
    }

    res.push_back(std::move(reportLine));
  }

  return res;
}

void SynthesisReportManager::fillLevels(const QString &line,
                                        ITaskReport::TableData &stats) const {
  const QRegularExpression findLvls{
      "^DE:.*Max Lvl =\\s*(([0-9]*[.])?[0-9]+)\\s*Avg Lvl "
      "=\\s*(([0-9]*[.])?[0-9]+)"};

  auto match = findLvls.match(line);
  if (match.hasMatch()) {
    stats.push_back({MAX_LVL_STR, match.captured(1)});
    stats.push_back({AVG_LVL_STR, match.captured(3)});
  }
}

std::unique_ptr<ITaskReport> SynthesisReportManager::createReport(
    const QString &reportId) {
  auto logFile = createLogFile(QString(SYNTHESIS_LOG));
  if (!logFile) return nullptr;

  // To save the last report statistics
  auto stats = ITaskReport::TableData{};

  auto fileStr = QTextStream(logFile.get()).readAll();
  logFile->close();

  auto findStats = QRegExp("Printing statistics.*\n\n===.*===\n\n.*[^\n{2}]+");

  if (findStats.lastIndexIn(fileStr) != -1)
    stats = getStatistics(findStats.cap());

  auto findLvls = QRegExp{"DE:([^\n]+)"};
  if (findLvls.lastIndexIn(fileStr) != -1) {
    fillLevels(findLvls.cap(), stats);
  }

  emit reportCreated(QString(REPORT_NAME));

  auto columnNames = QStringList{"Statistics", "Value"};
  return std::make_unique<TableReport>(std::move(columnNames), std::move(stats),
                                       "Synthesis report");
}

QMap<size_t, QString> SynthesisReportManager::getMessages() { return {}; }

}  // namespace FOEDAG
