#include <gnu-guitar-qt/main-window.h>

namespace Gnuitar {

namespace Qt {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  menu_bar = new MenuBar();

  audio_panel = new AudioPanel();
  audio_panel->resize(640, 120);

  rack = new Rack();

  layout = new QVBoxLayout;
  layout->addWidget(audio_panel);
  layout->addWidget(rack);

  central_widget = new QWidget(this);
  central_widget->setLayout(layout);

  setCentralWidget(central_widget);
  setMenuBar(menu_bar);

  connect(rack, &Rack::effect_changed, this, &MainWindow::on_effect_changed);
  connect(menu_bar, &MenuBar::effect_selected, this,
          &MainWindow::on_effect_selected);
  connect(menu_bar, &MenuBar::quit_selected, this,
          &MainWindow::on_quit_selected);
  connect(audio_panel, &AudioPanel::play_triggered, this,
          &MainWindow::on_play_selected);
  connect(audio_panel, &AudioPanel::stop_triggered, this,
          &MainWindow::on_stop_selected);
}

MainWindow::~MainWindow(void) {}

void MainWindow::add_effect(EffectView *effect_view) {
  rack->add_effect(effect_view);
}

void MainWindow::add_effect_choice(const QString &effect_name) {
  menu_bar->add_effect(effect_name);
}

void MainWindow::on_play_selected(void) { emit play_selected(); }

void MainWindow::on_stop_selected(void) { emit stop_selected(); }

void MainWindow::on_effect_changed(const QString &effect_name,
                                   const QString &control_name,
                                   int control_value) {
  emit effect_changed(effect_name, control_name, control_value);
}

void MainWindow::on_effect_selected(const QString &effect_name) {
  emit effect_selected(effect_name);
}

void MainWindow::on_quit_selected(void) {}

} /* namespace Qt */

} /* namespace Gnuitar */
