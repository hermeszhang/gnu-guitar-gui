#include <QApplication>
#include <QMainWindow>

#include "menu-bar.h"

int
main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QMainWindow window;
    window.show();

    Gnuitar::Qt::MenuBar menu_bar(&window);
    menu_bar.add_ladspa_plugin("Reverb");
    menu_bar.add_ladspa_plugin("Echo");

    menu_bar.add_lv2_plugin("Tube Amplifier");
    menu_bar.add_lv2_plugin("Delay");
    menu_bar.add_lv2_plugin("Equalizer");

    return app.exec();
}

