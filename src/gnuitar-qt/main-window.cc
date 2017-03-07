#include "main-window.h"

#include <iostream>

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

  connect (menu_bar, &MenuBar::ladspa_plugin_selected, this, &MainWindow::ladspa_plugin_selected);
  connect (menu_bar, &MenuBar::quit_requested, this, &MainWindow::close);
  connect (audio_panel, &AudioPanel::play_triggered, this, &MainWindow::on_play_triggered);
  connect (audio_panel, &AudioPanel::stop_triggered, this, &MainWindow::on_stop_triggered);

  driver = Driver::make();

  ladspa_plugin_manager.parse_env();
  ladspa_plugin_manager.find_all_plugins();

  for (size_t i = 0; i < ladspa_plugin_manager.plugin_count(); i++)
    {
      auto plugin = ladspa_plugin_manager.plugin(i);
      if (plugin == nullptr)
        continue;
      show_ladspa_plugin(*plugin);
    }
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

void
MainWindow::ladspa_plugin_selected (const QString& plugin_name)
{
  auto effect = ladspa_plugin_manager.get_effect(plugin_name.toStdString());
  if (effect == nullptr)
    return;

  effect->instantiate(driver->get_rate());
  effect->activate();
  if (driver->add_effect(effect) != 0)
    {
      delete effect;
      return;
    }

  auto effect_view = new Effect(plugin_name, rack);

  rack->add_effect(effect_view);
}

void
MainWindow::show_ladspa_plugin (const LADSPA::Plugin& plugin) noexcept
{
  for (size_t i = 0; i < SIZE_MAX; i++)
    {
      auto effect = plugin.get_effect(i);
      if (effect == nullptr)
        break;
      auto name = effect->get_name();
      if (name.size() == 0)
        {
          delete effect;
          continue;
        }
      menu_bar->add_ladspa_plugin(name.c_str());
      delete effect;
    }
}

} /* namespace Qt */

} /* namespace Gnuitar */

