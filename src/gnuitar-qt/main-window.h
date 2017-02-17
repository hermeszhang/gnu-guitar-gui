#ifndef GNUITAR_MAIN_WINDOW_H
#define GNUITAR_MAIN_WINDOW_H

#include <QMainWindow>

namespace Ui {

class MainWindow;

} /* namespace Ui */

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
}; /* class MainWindow */

#endif /* GNUITAR_MAIN_WINDOW_H */

