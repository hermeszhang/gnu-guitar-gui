#ifndef GNUITAR_MAIN_WINDOW_H
#define GNUITAR_MAIN_WINDOW_H

#include <QMainWindow>

#include "../gnuitar.h"

namespace Ui {

class MainWindow;

} /* namespace Ui */

namespace Gnuitar {

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow(void);
private slots:
    void on_startButton_clicked(void);
private:
    Ui::MainWindow *ui;
    GnuitarTrack *track;
}; /* class MainWindow */

} /* namespace Gnuitar */

#endif /* GNUITAR_MAIN_WINDOW_H */

