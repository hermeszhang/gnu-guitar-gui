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

    QStringList effect_list;

    auto count = gnuitar_package_get_effect_count(&package);
    for (decltype(count) i = 0; i < count; i++) {
        auto effect_name = gnuitar_package_get_effect_name(&package, i);
        if (effect_name == NULL)
            continue;
        effect_list << effect_name;
    }

    auto model = new QStringListModel(this);
    model->setStringList(effect_list);

    ui->known_effects->setModel(model);
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
MainWindow::on_start_button_clicked(void)
{
    if (gnuitar_track_start(&track) != 0) {
        /* error */
    }
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

