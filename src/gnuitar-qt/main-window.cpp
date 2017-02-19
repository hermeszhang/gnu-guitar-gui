#include "main-window.h"

#include "ui_main-window.h"

namespace Gnuitar {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    gnuitar_track_init(&track, "ALSA");
}

MainWindow::~MainWindow(void)
{
    gnuitar_track_done(&track);
    delete ui;
}


void MainWindow::on_startButton_clicked(void)
{
    if (gnuitar_track_start(&track) != 0) {
        /* error */
    }
}

} /* namespace Gnuitar */

