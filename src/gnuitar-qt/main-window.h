#ifndef GNUITAR_MAIN_WINDOW_H
#define GNUITAR_MAIN_WINDOW_H

#include <QMainWindow>

#include "../gnuitar.h"

#include "audio-panel.h"
#include "doc-browser.h"
#include "rack.h"

namespace Ui {

class MainWindow;

} /* namespace Ui */

namespace Gnuitar {

namespace Qt {

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(void);
protected:
    void open_documentation(void);
    void populate_known_packages(void);
    void populate_known_drivers(QMenu *menu, GnuitarPackage *package);
    void populate_known_effects(QMenu *menu, GnuitarPackage *package);
private slots:
    void add_effect(const char *package_name, const char *effect_name);
    void set_driver(const char *package_name, const char *driver_name);
    void on_actionFull_Screen_triggered(void);
    void on_quit_triggered(void);
    void on_report_an_issue_triggered(void);
    void on_view_documentation_triggered(void);
private:

    Ui::MainWindow *ui;

    AudioPanel *audio_panel;
    DocBrowser *doc_browser;
    Rack *rack;

    GnuitarTrack track;
    GnuitarPackageManager package_manager;

}; /* class MainWindow */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_MAIN_WINDOW_H */

