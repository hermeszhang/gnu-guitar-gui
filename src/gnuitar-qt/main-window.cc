#include "main-window.h"

#include "ui_main-window.h"

#include <QStringList>
#include <QStringListModel>
#include <QFileDialog>
#include <QDesktopServices>

#ifndef GNUITAR_ISSUES_LINK
#define GNUITAR_ISSUES_LINK "https://github.com/gnuitar/gnuitar/issues/new"
#endif

#ifndef GNUITAR_INSTALL_PREFIX
#ifdef _WIN32
#define GNUITAR_INSTALL_PREFIX "C:\\Program Files\\Gnuitar"
#else /* _WIN32 */
#define GNUITAR_INSTALL_PREFIX "/usr/local"
#endif /* _WIN32 */
#endif /* GNUITAR_INSTALL_PREFIX */

#ifndef GNUITAR_DOCS_PATH
#ifdef _WIN32
#define GNUITAR_DOCS_PATH GNUITAR_INSTALL_PREFIX "\\share\\docs"
#else /* _WIN32 */
#define GNUITAR_DOCS_PATH GNUITAR_INSTALL_PREFIX "/share/gnuitar/docs"
#endif /* _WIN32 */
#endif /* GNUITAR_DOCS_PATH */

namespace Gnuitar {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), doc_browser(0)
{
    ui->setupUi(this);

    rack = new Gnuitar::Qt::Rack(ui->central_widget);
    rack->resize(640, 480);

    open_package(GNUITAR_BUILTINS_PACKAGE);

    gnuitar_track_init(&track, "ALSA");
}

MainWindow::~MainWindow(void)
{
    gnuitar_track_done(&track);
    gnuitar_package_done(&package);
    delete ui;
    if (doc_browser != 0)
        delete doc_browser;
}

void
MainWindow::open_documentation(void)
{
    if (doc_browser)
        return;
    doc_browser = new Gnuitar::Qt::DocBrowser();
    if (doc_browser == 0)
        return;


    QStringList docs_search_paths;
    docs_search_paths << GNUITAR_DOCS_PATH;

    const char * docs_path_env = std::getenv("GNUITAR_DOCS_PATH");
    if (docs_path_env != 0)
        docs_search_paths << docs_path_env;

    doc_browser->setSearchPaths(docs_search_paths);
    doc_browser->setSource(QUrl("index.html"));
    doc_browser->resize(640, 480);
    doc_browser->show();
}

void
MainWindow::populate_known_effects(void)
{
    if (!package_open)
        return;

    auto count = gnuitar_package_get_effect_count(&package);
    for (decltype(count) i = 0; i < count; i++) {
        auto effect_name = gnuitar_package_get_effect_name(&package, i);
        if (effect_name == NULL)
            continue;
        auto action = ui->menu_effects->addAction(effect_name);
	connect(action, &QAction::triggered, this, [this, effect_name]{ add_effect(effect_name); });
    }
}

void
MainWindow::open_package(const std::string& package_path)
{
    if (gnuitar_package_open(&package, package_path.c_str()) != 0){
        package_open = false;
    } else {
        package_open = true;
	populate_known_effects();
    }
}

void
MainWindow::add_effect(const QString& effect_name)
{
    GnuitarEffect effect;

    if (gnuitar_package_init_effect(&package, effect_name.toStdString().c_str(), &effect) != 0)
        return;

    if (gnuitar_track_add_effect(&track, &effect) != 0) {
        gnuitar_effect_done(&effect);
	return;
    }

    GnuitarMap effect_map;

    gnuitar_map_init(&effect_map);

    if (gnuitar_effect_get_map(&effect, &effect_map) != 0) {
        gnuitar_map_done(&effect_map);
        return;
    }

    auto effect_widget = new Gnuitar::Qt::Effect(effect_name, ui->central_widget);

    auto parameter_count = gnuitar_map_get_count(&effect_map);
    for (decltype(parameter_count) i = 0; i < parameter_count; i++) {
        auto parameter_name = gnuitar_map_get_name(&effect_map, i);
        if (parameter_name == NULL)
            continue;
        effect_widget->add_parameter(parameter_name);
    }

    gnuitar_map_done(&effect_map);

    rack->add_effect(effect_widget);
}

void
MainWindow::on_quit_triggered(void)
{
    close();
}

void
MainWindow::on_open_package_triggered(void)
{
    auto package_path = QFileDialog::getOpenFileName(this, tr("Open File"), ".", tr("Libraries (*.so)"));

    std::string package_path_utf8 = package_path.toUtf8().constData();

    open_package(package_path_utf8);
}

void
MainWindow::on_report_an_issue_triggered(void)
{
    QDesktopServices::openUrl(QUrl(GNUITAR_ISSUES_LINK));
}

void
MainWindow::on_view_documentation_triggered(void)
{
    open_documentation();
}

} /* namespace Gnuitar */

