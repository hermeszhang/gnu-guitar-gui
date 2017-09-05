#ifndef GNUITAR_AUDIO_PANEL_H
#define GNUITAR_AUDIO_PANEL_H

#include <QWidget>

class QHBoxLayout;

namespace GnuGuitar {

namespace Qt {

class PowerButton;

class AudioPanel : public QWidget {
  Q_OBJECT
public:
  AudioPanel(QWidget *parent = nullptr);
  ~AudioPanel(void);
signals:
  void play_triggered(void);
  void stop_triggered(void);
protected slots:
  void onPowerButtonActivated();
  void onPowerButtonDeactivated();
  void setupColors();
private:
  QHBoxLayout *layout;
  PowerButton *powerButton;
}; /* class AudioPanel */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_AUDIO_PANEL_H */
