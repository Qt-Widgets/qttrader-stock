/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2013 Mattias Johansson
 */

#include "PluginFactory.h"
#include <QDir>
#include <QtDebug>
#include <QPluginLoader>
#include "qttrader_defines.h"


void* PluginFactory::m_pPluginFactory = 0;

PluginFactory::PluginFactory ()
{
}

void* PluginFactory::getPluginFactory ()
{
    if(!m_pPluginFactory){
        m_pPluginFactory = new PluginFactory();
    }
    return m_pPluginFactory;
}

QObject* PluginFactory::loadPluginFromString (QString d)
{

#ifdef DEBUG
    QString s = QDir::currentPath();
    s.append("/lib").append(d);
#else
    QString s = INSTALL_PLUGIN_DIR;
    s.append("/lib").append(d);
#endif

#if defined(Q_OS_MAC)  
  s.append(".dylib");
#endif
#if defined(Q_OS_UNIX)  
  s.append(".so");
#endif

  QPluginLoader pluginLoader(s);
  QObject *pObject = pluginLoader.instance();
  if (! pObject)
  {
      qDebug() << "PluginFactory::loadPluginFromString:" << pluginLoader.errorString() << " : " << s;
    return 0;
  }
  return pObject;
}

QtTraderPlugin* PluginFactory::loadPlugin(QString d)
{
  QtTraderPlugin* pQtTraderPlugin = 0;
//  if(mLoadedPlugins.contains(d))
//  {
//      pQtTraderPlugin = mLoadedPlugins.value(d);
//  }
//  else
  {
      QObject* pObject = loadPluginFromString(d);
      pQtTraderPlugin = qobject_cast<QtTraderPlugin *>(pObject);
      if (!pQtTraderPlugin){
        qDebug() << "PluginFactory::loadPlugin: error casting Plugin : " <<d;
        Q_ASSERT(pQtTraderPlugin);
      }
      mLoadedPlugins.insert(d,pQtTraderPlugin);
  }
  return pQtTraderPlugin;
}
