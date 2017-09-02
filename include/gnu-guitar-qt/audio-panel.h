#ifndef GNUITAR_AUDIO_PANEL_H
#define GNUITAR_AUDIO_PANEL_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;

namespace Gnuitar {

namespace Qt {

class AudioPanel : public QWidget {
  Q_OBJECT
public:
  AudioPanel(QWidget *parent = nullptr);
  ~AudioPanel(void);
public slots:
  void onPowerButtonClicked(void);
signals:
  void play_triggered(void);
  void stop_triggered(void);

private:
  bool on;
  QHBoxLayout *layout;
  QPushButton *powerButton;
}; /* class AudioPanel */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_AUDIO_PANEL_H */
