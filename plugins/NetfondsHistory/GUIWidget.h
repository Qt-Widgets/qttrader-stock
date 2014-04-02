/*
 *  Qtstalker stock charter
 *
 *  Copyright (C) 2013 Mattias Johansson
 *
 */


#ifndef PLUGIN_NETFONDS_HISTORY_GUI_WIDGET_H
#define PLUGIN_NETFONDS_HISTORY_GUI_WIDGET_H

#include <QStringList>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QTimer>
#include <QTableWidget>
#include "Widget.h"

class GUIWidget : public Widget
{
  Q_OBJECT

  public:
    GUIWidget ();
    ~GUIWidget ();
    void createGUI ();
    void loadSettings ();
    void saveSettings ();

  public slots:
    void downloadHistory ();
    void buttonStatus ();
    void updateGUI ();
    void addTableRow();
    void contextMenuRequseted(QPoint point);

  private:
    QStringList getTickers();

    QPushButton *m_pAddRowButton;
    QTableWidget *m_pTableWidget;
    QPushButton *_okButton;
    QPushButton *_cancelButton;
    QComboBox *_templates;
    QComboBox *_range;
    QPushButton *_symbolButton;
    QTextEdit *_log;
    QTimer *_timer;
};

#endif
