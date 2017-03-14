#ifndef GNUITAR_QT_MAIN_WINDOW_H
#define GNUITAR_QT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include <gnuitar-qt/audio-panel.h>
#include <gnuitar-qt/menu-bar.h>
#include <gnuitar-qt/rack.h>

namespace Gnuitar {

namespace Qt {

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  MainWindow (QWidget *parent = 0);
  ~MainWindow (void);
signals:
  void effect_selected (const QString& effect_name);
private slots:
  void on_play_selected (void);
  void on_stop_selected (void);
  void on_effect_selected (const QString& effect_name);
  void on_quit_selected (void);
private:
  QWidget *central_widget;
  QVBoxLayout *layout;
  MenuBar *menu_bar;
  AudioPanel *audio_panel;
  Rack *rack;
}; /* class MainWindow */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_MAIN_WINDOW_H */

