#include <gnu-guitar-qt/menu-bar.h>

#include <QDesktopServices>

#ifndef GNUITAR_ISSUES_LINK
#define GNUITAR_ISSUES_LINK                                                    \
  "https://github.com/gnu-guitar/gnu-guitar-qt/issues/new"
#endif /* GNUITAR_ISSUES_LINK */

namespace GnuGuitar {

namespace Qt {

MenuBar::MenuBar(QWidget *parent) : QMenuBar(parent) {
  file_menu = addMenu("File");
  open_preset_action = file_menu->addAction("Open Preset");
  save_preset_action = file_menu->addAction("Save Preset");
  file_menu->addSeparator();
  quit_action = file_menu->addAction("Quit");
  connect(quit_action, &QAction::triggered, this, &MenuBar::on_quit_selected);

  edit_menu = addMenu("Edit");
  undo_action = edit_menu->addAction("Undo");
  redo_action = edit_menu->addAction("Redo");
  edit_menu->addSeparator();
  preferences_action = edit_menu->addAction("Preferences");
  connect(preferences_action, &QAction::triggered,
          this, &MenuBar::onPreferencesClicked);

  view_menu = addMenu("View");
  zoom_in_action = view_menu->addAction("Zoom In");
  zoom_out_action = view_menu->addAction("Zoom Out");
  normal_size_action = view_menu->addAction("Normal Size");
  view_menu->addSeparator();
  fullscreen_action = view_menu->addAction("Full Screen");

  effects_menu = addMenu("Effects");

  help_menu = addMenu("Help");
  view_documentation_action = help_menu->addAction("View Documentation");
  report_an_issue_action = help_menu->addAction("Report an Issue");
  help_menu->addSeparator();
  about_action = help_menu->addAction("About");
  connect(view_documentation_action, &QAction::triggered, this,
          &MenuBar::on_view_documentation_selected);
  connect(report_an_issue_action, &QAction::triggered, this,
          &MenuBar::on_report_an_issue_selected);
  connect(about_action, &QAction::triggered, this, &MenuBar::on_about_selected);

  doc_browser = new DocBrowser();
  doc_browser->hide();
}

MenuBar::~MenuBar(void) {}

void MenuBar::add_effect(const QString &effect_name) {
  auto effect_action = effects_menu->addAction(effect_name);

  auto effect_callback = [this, effect_name] {
    on_effect_selected(effect_name);
  };

  connect(effect_action, &QAction::triggered, this, effect_callback);
}

void MenuBar::on_quit_selected(void) { emit quit_selected(); }

void MenuBar::on_effect_selected(const QString &effect_name) {
  emit effect_selected(effect_name);
}

void MenuBar::on_view_documentation_selected(void) { doc_browser->show(); }

void MenuBar::on_report_an_issue_selected(void) {
  QDesktopServices::openUrl(QUrl(GNUITAR_ISSUES_LINK));
}

void MenuBar::on_about_selected(void) {}

void MenuBar::onPreferencesClicked() {
  emit preferencesClicked();
}

} /* namespace Qt */

} /* namespace Gnuitar */
