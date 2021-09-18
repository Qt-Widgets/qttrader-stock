#include "ToolbarWidget.h"

#include "PluginFactory.h"
#include "bar/BarLength.h"
#include "DateRange.h"

#include <QtDebug>
#include <QSettings>
#include <QLayout>
#include <QMessageBox>
#include <QAction>
#include <QMenu>
#include <QTimer>
#include <QInputDialog>

ToolbarWidget::ToolbarWidget(QWidget *parent) :  QWidget(parent)
{
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->setSpacing(0);
  hbox->setMargin(0);
  setLayout(hbox);

  // bar length
  _lengthButton = new LengthButton();
  connect(_lengthButton, SIGNAL(signalLength()), this, SIGNAL(signalLength()));
  hbox->addWidget(_lengthButton);

  // symbol combo
  _list = new QComboBox();
  connect(_list, SIGNAL(currentIndexChanged(int)), this, SLOT(listSelected()));
  hbox->addWidget(_list);

  // create actions for options menu
  QMenu *menu = new QMenu(this);

  QAction* a = new QAction(QIcon(":icons/add"), tr("&Add Indicator"), this);
  a->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
  a->setIconVisibleInMenu(true);
  a->setToolTip(tr("Add Indicator"));
  a->setStatusTip(tr("Add Indicator"));
  connect(a, SIGNAL(triggered()), this, SIGNAL(signalIndicator()));
  menu->addAction(a);

  a = new QAction(QIcon(":icons/edit"), tr("&Edit Indicator"), this);
  a->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
  a->setIconVisibleInMenu(true);
  a->setToolTip(tr("Edit Indicator"));
  a->setStatusTip(tr("Edit Indicator"));
  connect(a, SIGNAL(activated()), this, SIGNAL(signalEditPlot()));
  menu->addAction(a);

  a = new QAction(QIcon(":icons/delete"), tr("&Remove Indicator"), this);
  a->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
  a->setIconVisibleInMenu(true);
  a->setToolTip(tr("Remove Indicator"));
  a->setStatusTip(tr("Remove Indicator"));
  connect(a, SIGNAL(activated()), this, SIGNAL(signalRemovePlot()));
  menu->addAction(a);

  // options button
  _optionButton = new QToolButton;
  _optionButton->setPopupMode(QToolButton::InstantPopup);
  _optionButton->setIcon(QIcon(":icons/config"));
  _optionButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
  _optionButton->setToolTip(tr("Options"));
  _optionButton->setStatusTip(tr("Options"));
//  _optionButton->setMaximumSize(bs);
  _optionButton->setMenu(menu);
  hbox->addWidget(_optionButton);

  // prev button
  _prevButton = new QToolButton;
  _prevButton->setIcon(QIcon(":icons/back"));
  _prevButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
  _prevButton->setToolTip(tr("Previous Symbol"));
  _prevButton->setStatusTip(tr("Previous Symbol"));
//  _prevButton->setMaximumSize(bs);
  connect(_prevButton, SIGNAL(clicked()), this, SLOT(previousSymbol()));
  hbox->addWidget(_prevButton);

  // next button
  _nextButton = new QToolButton;
  _nextButton->setIcon(QIcon(":icons/forward"));
  _nextButton->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
  _nextButton->setToolTip(tr("Next Symbol"));
  _nextButton->setStatusTip(tr("Next Symbol"));
//  _nextButton->setMaximumSize(bs);
  connect(_nextButton, SIGNAL(clicked()), this, SLOT(nextSymbol()));
  hbox->addWidget(_nextButton);

  //must update symbols last!
  updateSymbols();
}


void ToolbarWidget::updateSymbols()
{
  IDBPlugin *plug = dynamic_cast<IDBPlugin*>(((PluginFactory*)PluginFactory::getPluginFactory())->loadPlugin(QString("Database")));
  if (! plug)
    return;

  _symbols.clear();

  QList<Bars> symbols = plug->search("%");

  _symbols = symbols;

  QStringList tl;
  for (int pos = 0; pos < _symbols.size(); pos++)
  {
    Bars sym = _symbols.at(pos);
    tl << sym.symbol();
  }

  _list->blockSignals(true);
  _list->clear();
  _list->addItems(tl);
  _list->blockSignals(false);

  buttonStatus();
}

void ToolbarWidget::previousSymbol ()
{
  _list->setCurrentIndex(_list->currentIndex() - 1);
  buttonStatus();
}

void ToolbarWidget::nextSymbol ()
{
  _list->setCurrentIndex(_list->currentIndex() + 1);
  buttonStatus();
}

void ToolbarWidget::buttonStatus ()
{
  // update symbol navigation buttons
  if (_symbols.size() < 2)
  {
    _prevButton->setEnabled(false);
    _nextButton->setEnabled(false);
  }
  else
  {
    if (_list->currentIndex() == 0)
    {
      _prevButton->setEnabled(false);
      _nextButton->setEnabled(true);
    }
    else
    {
      if (_list->currentIndex() == _symbols.size() - 1)
      {
        _prevButton->setEnabled(true);
        _nextButton->setEnabled(false);
      }
      else
      {
        _prevButton->setEnabled(true);
        _nextButton->setEnabled(true);
      }
    }
  }
}

int ToolbarWidget::count ()
{
  return _list->count();
}

Bars ToolbarWidget::currentSymbol ()
{
  if (! _list->count() || _list->currentIndex() < 0)
    return Bars();

  return _symbols.at(_list->currentIndex());
}

int ToolbarWidget::length ()
{
  return _lengthButton->length();
}

QString ToolbarWidget::lengthText ()
{
  return _lengthButton->lengthText();
}

void ToolbarWidget::setLength (int d)
{
  _lengthButton->setLength(d);
}

void ToolbarWidget::listSelected ()
{
  emit signalSelected();
  buttonStatus();
}
