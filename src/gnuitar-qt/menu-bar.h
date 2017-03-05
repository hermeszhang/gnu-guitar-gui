#ifndef GNUITAR_QT_MENU_BAR_H
#define GNUITAR_QT_MENU_BAR_H

#include <QMenuBar>

#include "doc-browser.h"

namespace Gnuitar {

namespace Qt {

class MenuBar : public QMenuBar {

    Q_OBJECT

public:

    MenuBar(QWidget *parent = nullptr);
    ~MenuBar(void);
    void add_ladspa_plugin(const QString& plugin_name);
    void add_lv2_plugin(const QString& plugin_name);

private slots:

    void quit_triggered(void);

    void ladspa_plugin_triggered(const QString& plugin_name);
    void lv2_plugin_triggered(const QString& plugin_name);

    void view_documentation_triggered(void);
    void report_an_issue_triggered(void);
    void about_triggered(void);

signals:

    void quit_requested(void);
    void ladspa_plugin_selected(const QString& plugin_name);
    void lv2_plugin_selected(const QString& plugin_name);

private:

    QMenu *file_menu;
    QAction *open_preset_action;
    QAction *save_preset_action;
    QAction *quit_action;

    QMenu *edit_menu;
    QAction *redo_action;
    QAction *undo_action;
    QAction *preferences_action;

    QMenu *view_menu;
    QAction *zoom_in_action;
    QAction *zoom_out_action;
    QAction *normal_size_action;
    QAction *fullscreen_action;

    QMenu *plugins_menu;
    QMenu *ladspa_plugins_menu;
    QMenu *lv2_plugins_menu;

    QMenu *help_menu;
    QAction *about_action;
    QAction *view_documentation_action;
    QAction *report_an_issue_action;

    DocBrowser *doc_browser;

}; /* class MenuBar */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_MENU_BAR_H */

