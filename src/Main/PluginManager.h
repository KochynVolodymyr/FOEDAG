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
#include <QObject>
#include <QString>

class PluginManager {
 public:
  PluginManager(const PluginManager&) = delete;
  PluginManager& operator=(const PluginManager&) = delete;
  static PluginManager& instance();

  ~PluginManager();
  void load(const QString &path, const QStringList &plugins);
  const QObjectList plugins() const;

 private:
  PluginManager() = default;
  static QObjectList loadPlugins(const QString &path,
                                 const QStringList &plugins);

 private:
  QObjectList m_plugins;
};
