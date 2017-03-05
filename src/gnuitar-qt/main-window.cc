#include "main-window.h"

namespace Gnuitar {

namespace Qt {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    menu_bar = new MenuBar();

    audio_panel = new AudioPanel();
    audio_panel->resize(640, 120);

    rack = new Rack();

    layout = new QVBoxLayout;
    layout->addWidget(audio_panel);
    layout->addWidget(rack);

    central_widget = new QWidget(this);
    central_widget->setLayout(layout);

    setCentralWidget(central_widget);
    setMenuBar(menu_bar);

    connect(menu_bar, &MenuBar::quit_requested, this, &MainWindow::close);

    gnuitar_track_init(&track);

    gnuitar_package_manager_init(&package_manager);
    auto packages_dir = std::getenv("GNUITAR_PACKAGES_PATH");
    if (packages_dir != nullptr)
        gnuitar_package_manager_set_packages_dir(&package_manager, packages_dir);
    gnuitar_package_manager_refresh_packages(&package_manager);
}

MainWindow::~MainWindow(void)
{
    gnuitar_track_done(&track);
    gnuitar_package_manager_done(&package_manager);
}

} /* namespace Qt */

} /* namespace Gnuitar */

