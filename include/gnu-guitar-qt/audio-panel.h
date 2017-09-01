#ifndef GNUITAR_AUDIO_PANEL_H
#define GNUITAR_AUDIO_PANEL_H

#include <QWidget>
#include <QHBoxLayout>

#include "button.h"

namespace Gnuitar {

namespace Qt {

class AudioPanel : public QWidget {
    Q_OBJECT
public:
    AudioPanel(QWidget *parent = nullptr);
    ~AudioPanel(void);
    void add_control(const char *name);
public slots:
    void on_play_triggered(void);
    void on_stop_triggered(void);
signals:
    void play_triggered(void);
    void stop_triggered(void);
private:
    QHBoxLayout *layout;
    Button *play_button;
    Button *stop_button;
}; /* class AudioPanel */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_AUDIO_PANEL_H */

