#include "main-window.h"

#include "ui_main-window.h"

namespace Gnuitar {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    track = gnuitar_track_create("ALSA");
}

MainWindow::~MainWindow(void)
{
    if (track != NULL)
        gnuitar_track_destroy(track);
    delete ui;
}


void MainWindow::on_startButton_clicked(void)
{
    if (gnuitar_track_start(track) != 0) {
        /* error */
    }
}

} /* namespace Gnuitar */

