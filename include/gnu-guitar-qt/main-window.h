#ifndef GNUITAR_QT_MAIN_WINDOW_H
#define GNUITAR_QT_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include <gnu-guitar-qt/audio-panel.h>
#include <gnu-guitar-qt/menu-bar.h>
#include <gnu-guitar-qt/rack.h>

namespace Gnuitar {

namespace Qt {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow(void);
  void add_effect(EffectView *effect_view);
  void add_effect_choice(const QString &effect_name);
signals:
  void effect_changed(const QString &effect_name, const QString &control_name,
                      int control_value);
  void effect_selected(const QString &effect_name);
  void play_selected(void);
  void stop_selected(void);
private slots:
  void on_play_selected(void);
  void on_stop_selected(void);
  void on_effect_changed(const QString &effect_name,
                         const QString &control_name, int control_value);
  void on_effect_selected(const QString &effect_name);
  void on_quit_selected(void);

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
