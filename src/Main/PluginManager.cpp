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
#include "PluginManager.h"

#include <QDebug>
#include <QDir>
#include <QPluginLoader>

PluginManager &PluginManager::instance() {
  static PluginManager plManager;
  return plManager;
}

PluginManager::~PluginManager() { qDeleteAll(m_plugins); }

void PluginManager::load(const QString &path, const QStringList &plugins) {
  qDeleteAll(m_plugins);
  m_plugins = loadPlugins(path, plugins);
}

QObjectList PluginManager::loadPlugins(const QString &path,
                                       const QStringList &plugins) {
  QObjectList pluginList;
  const QDir dir(path);
  for (const auto &plugin : plugins) {
    QPluginLoader loader{dir.absoluteFilePath(plugin)};
    bool ok = loader.load();
    if (!ok) {
      qDebug() << "plugin not loaded: " << loader.errorString();
      continue;
    }
    if (auto instance = loader.instance())
      pluginList += instance;
    else
      qDebug() << "plugin not loaded: " << loader.errorString();
  }
  return pluginList;
}

const QObjectList PluginManager::plugins() const { return m_plugins; }
