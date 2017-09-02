#ifndef GNU_GUITAR_QT_CONTROLLER_HPP
#define GNU_GUITAR_QT_CONTROLLER_HPP

#include <QObject>

#include <rtaudio/ladspa-plugins.hpp>
#include <rtaudio/session.hpp>

namespace RtAudio {

class CompositeProcessor;

} // namespace RtAudio

namespace GnuGuitar {

namespace Qt {

class MainWindow;

class Controller : public QObject {
  Q_OBJECT
public:
  Controller();
  ~Controller();
protected slots:
  void onEffectChanged(const QString &effectName,
                       const QString &controlName,
                       int value);
  void onPlayClicked();
  void onStopClicked();
  void onQuitClicked();

protected:
  void addEffect(const QString &name);
  void updateEffectList();

private:
  RtAudio::LadspaPlugins ladspaPlugins;
  RtAudio::CompositeProcessor *processor;
  RtAudio::Session session;
  MainWindow *mainWindow;
}; /* class Controller */

} /* namespace Qt */

} /* namespace GnuGuitar */

#endif /* GNU_GUITAR_QT_CONTROLLER_HPP */
