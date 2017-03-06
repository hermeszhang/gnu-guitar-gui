#include "main-window.h"

namespace Gnuitar {

namespace Qt {

MainWindow::MainWindow (QWidget *parent) : QMainWindow (parent)
{
  menu_bar = new MenuBar ();

  audio_panel = new AudioPanel ();
  audio_panel->resize (640, 120);

  rack = new Rack ();

  layout = new QVBoxLayout;
  layout->addWidget (audio_panel);
  layout->addWidget (rack);

  central_widget = new QWidget (this);
  central_widget->setLayout (layout);

  setCentralWidget (central_widget);
  setMenuBar (menu_bar);

  connect (menu_bar, &MenuBar::quit_requested, this, &MainWindow::close);
  connect (audio_panel, &AudioPanel::play_triggered, this, &MainWindow::on_play_triggered);
  connect (audio_panel, &AudioPanel::stop_triggered, this, &MainWindow::on_stop_triggered);

  driver = Driver::make();
}

MainWindow::~MainWindow (void)
{
  if (driver != nullptr)
    delete driver;
}

void
MainWindow::on_play_triggered (void)
{
  if (driver != nullptr)
    driver->start ();
}

void
MainWindow::on_stop_triggered (void)
{
  if (driver != nullptr)
    driver->stop ();
}

} /* namespace Qt */

} /* namespace Gnuitar */

