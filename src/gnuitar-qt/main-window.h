#ifndef GNUITAR_MAIN_WINDOW_H
#define GNUITAR_MAIN_WINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>

#include "../track.h"
#include "../package-manager.h"

#include "audio-panel.h"
#include "menu-bar.h"
#include "rack.h"

namespace Gnuitar {

namespace Qt {

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow(void);
private:
    QWidget *central_widget;
    QVBoxLayout *layout;
    MenuBar *menu_bar;
    AudioPanel *audio_panel;
    Rack *rack;

    GnuitarTrack track;
    GnuitarPackageManager package_manager;
}; /* class MainWindow */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_MAIN_WINDOW_H */

