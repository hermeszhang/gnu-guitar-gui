#include <gnuitar-qt/controller.h>

namespace Gnuitar
{

namespace Qt
{

Controller::Controller (void)
{
  driver_manager.update_libs ();

  main_window.resize (640, 480);
  main_window.show ();

  main_window.connect (&main_window, &Gnuitar::Qt::MainWindow::effect_selected, this, &Controller::add_effect);
}

Controller::~Controller (void)
{

}

void
Controller::add_effect (const QString& effect_name)
{
  driver_manager.add_effect (effect_name.toStdString ());
}


} /* namespace Qt */

} /* namespace Gnuitar */

