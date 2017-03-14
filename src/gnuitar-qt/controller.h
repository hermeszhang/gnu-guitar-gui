#ifndef GNUITAR_QT_CONTROLLER_H
#define GNUITAR_QT_CONTROLLER_H

#include <gnuitar-qt/main-window.h>

#include <libgnuitar/driver-manager.h>

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
private:
  Gnuitar::DriverManager driver_manager;
  Gnuitar::Qt::MainWindow main_window;
}; /* class Controller */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_CONTROLLER_H */

