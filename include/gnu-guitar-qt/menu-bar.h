#ifndef GNUITAR_QT_MENU_BAR_H
#define GNUITAR_QT_MENU_BAR_H

#include <QMenuBar>

#include <gnu-guitar-qt/doc-browser.h>

namespace GnuGuitar {

namespace Qt {

class MenuBar final : public QMenuBar {
  Q_OBJECT
public:
  MenuBar(QWidget *parent = nullptr);
  ~MenuBar(void);
  void add_effect(const QString &plugin_name);
private slots:
  void on_quit_selected(void);
  void on_effect_selected(const QString &plugin_name);
  void on_view_documentation_selected(void);
  void on_report_an_issue_selected(void);
  void on_about_selected(void);
  void onPreferencesClicked();
signals:
  void quit_selected(void);
  void effect_selected(const QString &effect_name);
  void preferencesClicked();

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

  QMenu *effects_menu;

  QMenu *help_menu;
  QAction *about_action;
  QAction *view_documentation_action;
  QAction *report_an_issue_action;

  DocBrowser *doc_browser;
}; /* class MenuBar */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_MENU_BAR_H */
