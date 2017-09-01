#include <QApplication>
#include <QMainWindow>

#include <gnuitar-qt/menu-bar.h>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  QMainWindow window;

  window.show();

  Gnuitar::Qt::MenuBar menu_bar(&window);
  menu_bar.add_effect("Reverb");
  menu_bar.add_effect("Echo");

  return app.exec();
}
