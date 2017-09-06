#ifndef GNU_GUITAR_QT_CONTROLLER_HPP
#define GNU_GUITAR_QT_CONTROLLER_HPP

#include <QObject>

namespace GnuGuitar {

namespace Qt {

class Driver;
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
  void onPreferencesClicked();
  void onStopClicked();
  void onQuitClicked();
protected:
  void addEffect(const QString &name);
  void updateEffectList();
private:
  Driver *driver;
  MainWindow *mainWindow;
}; /* class Controller */

} /* namespace Qt */

} /* namespace GnuGuitar */

#endif /* GNU_GUITAR_QT_CONTROLLER_HPP */
