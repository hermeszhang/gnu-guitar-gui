#include "audio-panel.h"

namespace Gnuitar {

namespace Qt {

AudioPanel::AudioPanel(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout();

    play_button = new Button("play.png");
    stop_button = new Button("stop.png");

    connect(play_button, &Button::clicked, this, &AudioPanel::on_play_triggered);
    connect(stop_button, &Button::clicked, this, &AudioPanel::on_stop_triggered);

    layout->addWidget(play_button);
    layout->addWidget(stop_button);

    setLayout(layout);
}

AudioPanel::~AudioPanel(void)
{

}

void AudioPanel::add_control(const char *name)
{
    (void) name;
}

void AudioPanel::on_play_triggered(void)
{
    emit play_triggered();
}

void AudioPanel::on_stop_triggered(void)
{
    emit stop_triggered();
}

} /* namespace Qt */

} /* namespace Gnuitar */

