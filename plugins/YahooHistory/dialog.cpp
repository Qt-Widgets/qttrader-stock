#include "dialog.h"
#include "GUIWidget.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
      GUIWidget* gui = new GUIWidget();
      gui->show();
}
