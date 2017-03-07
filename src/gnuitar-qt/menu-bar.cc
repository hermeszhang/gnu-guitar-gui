#include "menu-bar.h"

#include <QDesktopServices>

#ifndef GNUITAR_ISSUES_LINK
#define GNUITAR_ISSUES_LINK "https://github.com/gnuitar/gnuitar/issues/new"
#endif /* GNUITAR_ISSUES_LINK */

namespace Gnuitar {

namespace Qt {

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent)
{
    file_menu = addMenu("File");
    open_preset_action = file_menu->addAction("Open Preset");
    save_preset_action = file_menu->addAction("Save Preset");
    file_menu->addSeparator();
    quit_action = file_menu->addAction("Quit");
    connect(quit_action, &QAction::triggered, this, &MenuBar::quit_triggered);

    edit_menu = addMenu("Edit");
    undo_action = edit_menu->addAction("Undo");
    redo_action = edit_menu->addAction("Redo");
    edit_menu->addSeparator();
    preferences_action = edit_menu->addAction("Preferences");

    view_menu = addMenu("View");
    zoom_in_action = view_menu->addAction("Zoom In");
    zoom_out_action = view_menu->addAction("Zoom Out");
    normal_size_action = view_menu->addAction("Normal Size");
    view_menu->addSeparator();
    fullscreen_action = view_menu->addAction("Full Screen");

    plugins_menu = addMenu("Plugins");
    ladspa_plugins_menu = plugins_menu->addMenu("LADSPA");
    lv2_plugins_menu = plugins_menu->addMenu("LV2");

    help_menu = addMenu("Help");
    view_documentation_action = help_menu->addAction("View Documentation");
    report_an_issue_action = help_menu->addAction("Report an Issue");
    help_menu->addSeparator();
    about_action = help_menu->addAction("About");
    connect(view_documentation_action, &QAction::triggered, this, &MenuBar::view_documentation_triggered);
    connect(report_an_issue_action, &QAction::triggered, this, &MenuBar::report_an_issue_triggered);
    connect(about_action, &QAction::triggered, this, &MenuBar::about_triggered);

    doc_browser = new DocBrowser();
    doc_browser->hide();
}

MenuBar::~MenuBar(void)
{

}

void
MenuBar::add_ladspa_plugin(const QString& plugin_name)
{
    auto plugin_action = ladspa_plugins_menu->addAction(plugin_name);
    connect(plugin_action, &QAction::triggered, this, [this, plugin_name]{ ladspa_plugin_triggered (plugin_name); });
}

void
MenuBar::add_lv2_plugin(const QString& plugin_name)
{
    lv2_plugins_menu->addAction(plugin_name);
}

void
MenuBar::quit_triggered(void)
{
    emit quit_requested();
}

void
MenuBar::ladspa_plugin_triggered(const QString& plugin_name)
{
    emit ladspa_plugin_selected(plugin_name);
}

void
MenuBar::lv2_plugin_triggered(const QString& plugin_name)
{
    emit lv2_plugin_selected(plugin_name);
}

void
MenuBar::view_documentation_triggered(void)
{
    doc_browser->show();
}

void
MenuBar::report_an_issue_triggered(void)
{
    QDesktopServices::openUrl(QUrl(GNUITAR_ISSUES_LINK));
}

void
MenuBar::about_triggered(void)
{

}

} /* namespace Qt */

} /* namespace Gnuitar */

