/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2001-2010 Stefan S. Stratigakos
 *  Copyright (C) 2013 Mattias Johansson
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307,
 *  USA.
 */

#include <QtDebug>
#include <QApplication>
#include <QSettings>
#include <QStatusBar>
#include <QInputDialog>
#include <QToolButton>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProcess>

#include "qttrader.h"
#include "PluginFactory.h"
#include "Setup.h"
#include "Global.h"
#include "Widget.h"


QtTrader::QtTrader (QString session, QString plugin)
{
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(shutDown()));

  setWindowIcon(QIcon(":/icons/qttrader"));

  QCoreApplication::setOrganizationName("QtTrader");
//  QCoreApplication::setOrganizationDomain("QtTrader.com");
  QCoreApplication::setApplicationName("QtTrader");

  Setup setup;
  setup.setup(session);
  setup.scanPlugins();

  createGUI();

  loadSettings();

  setWindowTitle(g_title);

  QString tplugin = plugin;
  if (tplugin.isEmpty())
  {
    QSettings settings(g_settings);
    settings.beginGroup(g_session);
    tplugin = settings.value("plugin").toString();
    qDebug() << "Using value from settings: " << tplugin;
  }

  loadPlugin(tplugin);
}

void QtTrader::shutDown ()
{
  save();
}

void QtTrader::createGUI ()
{
  QMenuBar *menuBar = new QMenuBar();
  setMenuBar(menuBar);
  QMenu *menu = new QMenu("&Help");

  QAction* a = new QAction(QIcon(":icons/info"), tr("&About"), this);
  a->setIconVisibleInMenu(true);
  connect(a, SIGNAL(activated()), this, SLOT(about()));
  menu->addAction(a);
  menuBar->addMenu(menu);

  statusBar()->showMessage(tr("Ready"), 2000);

  QSettings settings(g_settings);
  settings.beginGroup("plugins");
  QStringList value = settings.value("gui").toStringList();
  settings.endGroup();

  QToolBar* toolbar = new QToolBar();
  for(int i = 0; i<value.size(); i++){
      toolbar->addAction(value.at(i), this, SLOT(pluginLoader()));
  }
  toolbar->show();
  toolbar->setObjectName("MainToolbar");

  addToolBar(toolbar);
  setUnifiedTitleAndToolBarOnMac(true);
}

void QtTrader::loadSettings ()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
  
  restoreGeometry(settings.value("main_window_geometry").toByteArray());
  
  restoreState(settings.value("main_window_state").toByteArray());

  // restore the size of the app
  QSize sz = settings.value("main_window_size", QSize(500,300)).toSize();
  resize(sz);

  // restore the position of the app
  QPoint p = settings.value("main_window_pos", QPoint(0,0)).toPoint();
  move(p);
}

void QtTrader::save()
{
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
  
  settings.setValue("main_window_geometry", saveGeometry());
  settings.setValue("main_window_state", saveState());
  settings.setValue("main_window_size", size());
  settings.setValue("main_window_pos", pos());
}

void QtTrader::statusMessage (QString d)
{
  // update the status bar with a new message from somewhere
  statusBar()->showMessage(d, 0);
  wakeup();
}

void QtTrader::wakeup ()
{
  // force app to process the event que so we keep from any blocking
  qApp->processEvents();
}

void QtTrader::pluginLoader()
{
  QAction* pAction =  qobject_cast<QAction*>(QObject::sender());
  QString name = pAction->text();
  QSettings settings(g_settings);
  settings.beginGroup(g_session);
  settings.setValue("plugin", name);
  settings.sync();

  loadPlugin(name);
}

int QtTrader::loadPlugin (QString name)
{
  if (name.isEmpty())
    return 0;

  IGUIPlugin *pPlugin = 0;
  pPlugin = dynamic_cast<IGUIPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(name));

  if(pPlugin){
      Widget* pWidget = pPlugin->create();
      // if the plugin returns a widget, set it to main window
      if(pWidget != NULL)
      {
        //remove old central Widget
        Widget *cw = (Widget *) centralWidget();
        if (cw)
        {
          QToolBar *tb = cw->toolbar();
          if (tb)
          {
            removeToolBar(cw->toolbar());
            delete tb;
          }

          delete cw;
        }
        //And then set up the new one
        setCentralWidget(pWidget);
        connect(pWidget, SIGNAL(signalMessage(QString)), this, SLOT(statusMessage(QString)));
        connect(pWidget, SIGNAL(signalTitle(QString)), this, SLOT(setWindowTitle(QString)));

        QToolBar *pToolBar = pWidget->toolbar();
        if (pToolBar){
          addToolBar(pToolBar);
        }
    }
  }
  return 1;
}

/*
 * Do not remove. To apply to the license agreement for the icons in this software,
 * credits for the icons have to be displayed in the about dialog.
 */
void QtTrader::about(){
  QMessageBox::information(this, "QtTrader!",
    "QtTrader is a Open Source application for technical analysis of stocks.\n\n"\
    "QtTrader is based in part on the work of Qwt project (http://qwt.sf.net).\n\n"\
    "QtTrader is based in part on the work of TA-lib (http://ta-lib.org).\n\n"\
    "QtTrader uses icons from the following sources under the Creative Commons licence (CC-BY-SA and/or CC-BY):\n"\
    "http://www.aha-soft.com/\n"\
    "http://www.gentleface.com/\n"\
    "http://www.oxygen-icons.org/\n"\
    "http://www.visualpharm.com/)");
}
