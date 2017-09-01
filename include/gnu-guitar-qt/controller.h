#ifndef GNUITAR_QT_CONTROLLER_H
#define GNUITAR_QT_CONTROLLER_H

#include <gnu-guitar-qt/main-window.h>

#include <rtaudio/ladspa-plugins.hpp>
#include <rtaudio/session.hpp>

namespace Gnuitar
{

namespace Qt
{

class Controller : public QObject
{
  Q_OBJECT
public:
  Controller (void);
  ~Controller (void);
  void add_effect (const QString& effect_name);
protected slots:
  void on_effect_changed (const QString& effect_name, const QString& control_name, int value);
  void on_play_selected (void);
  void on_stop_selected (void);
protected:
  void update_effect_list (void);
private:
  RtAudio::LadspaPlugins ladspa_plugins;
  RtAudio::Session session;
  Gnuitar::Qt::MainWindow main_window;
}; /* class Controller */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_CONTROLLER_H */

