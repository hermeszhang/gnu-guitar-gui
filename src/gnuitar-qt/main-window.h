#ifndef GNUITAR_MAIN_WINDOW_H
#define GNUITAR_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include "audio-panel.h"
#include "menu-bar.h"
#include "rack.h"

#include <libgnuitar/driver.h>
#include <libgnuitar/plugin-manager.h>

namespace Gnuitar {

namespace Qt {

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow(void);
private slots:
  void on_play_triggered(void);
  void on_stop_triggered(void);
protected:
  void show_plugin(const Plugin *plugin) noexcept;
private:
  QWidget *central_widget;
  QVBoxLayout *layout;
  MenuBar *menu_bar;
  AudioPanel *audio_panel;
  Rack *rack;
  Driver *driver;
  PluginManager plugin_manager;
}; /* class MainWindow */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_MAIN_WINDOW_H */

