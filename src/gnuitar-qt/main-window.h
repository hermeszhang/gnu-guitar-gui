#ifndef GNUITAR_MAIN_WINDOW_H
#define GNUITAR_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include "audio-panel.h"
#include "menu-bar.h"
#include "rack.h"

#include <libgnuitar/driver-manager.h>

namespace Gnuitar {

namespace Qt {

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow (QWidget *parent = 0);
  ~MainWindow (void);
private slots:
  void on_play_triggered (void);
  void on_stop_triggered (void);
  void ladspa_plugin_selected (const QString& name);
protected:
  void add_ladspa_plugin (const LADSPA_Descriptor *descriptor) noexcept;
  void show_ladspa_plugins (void) noexcept;
  void show_ladspa_plugin (LADSPA_Descriptor_Function descriptor) noexcept;
private:
  QWidget *central_widget;
  QVBoxLayout *layout;
  MenuBar *menu_bar;
  AudioPanel *audio_panel;
  Rack *rack;
  DriverManager driver_manager;
}; /* class MainWindow */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_MAIN_WINDOW_H */

