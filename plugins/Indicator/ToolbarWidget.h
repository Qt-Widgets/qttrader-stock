/*
 *  QtTrader stock charter
 *
 *  Copyright (C) 2012 - Mattias Johansson
 *
 */

#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QSplitter>
#include <QListWidget>
#include <QTextEdit>
#include <QScrollBar>
#include <QSettings>
#include <QComboBox>

#include "bar/Bars.h"
#include "LengthButton.h"
#include "plot/Plot.h"
#include "DataBase.h"

class ToolbarWidget : public QWidget
{
  Q_OBJECT
public:
  explicit ToolbarWidget(QWidget *parent = 0);

  int count();
  Bars currentSymbol();
  int length();
  QString lengthText();
  void setLength(int);

signals:
  void signalSelected();
  void signalIndicator();
  void signalLength();
  void signalRemovePlot();
  void signalEditPlot();

public slots:
  void updateSymbols();
  void previousSymbol();
  void nextSymbol();
  void buttonStatus();
  void listSelected();
  
private:
  LengthButton *_lengthButton;
  QToolButton *_prevButton;
  QToolButton *_nextButton;
  QToolButton *_optionButton;
  QList<Bars> _symbols;
  QComboBox *_list;

};

#endif // TOOLBARWIDGET_H
