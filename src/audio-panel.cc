#include <gnu-guitar-qt/audio-panel.h>

#include <QHBoxLayout>
#include <QPushButton>

namespace {

void setStopped(QPushButton *pushButton) {
  pushButton->setText("Start");
  pushButton->setStyleSheet("QPushButton { background: rgba(0, 255, 0, 0) }");
}

void setStarted(QPushButton *pushButton) {
  pushButton->setText("Stop");
  pushButton->setStyleSheet("QPushButton { background: rgba(255, 0, 0, 0) }");
}

} // namespace

namespace Gnuitar {

namespace Qt {

AudioPanel::AudioPanel(QWidget *parent) : QWidget(parent) {

  on = false;

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout = new QHBoxLayout();
  layout->setAlignment(::Qt::AlignVCenter | ::Qt::AlignLeft);

  powerButton = new QPushButton(this);
  auto width = 75;
  auto height = 50;
  powerButton->setFixedSize(width, height);
  setStopped(powerButton);

  connect(powerButton, &QPushButton::clicked,
          this, &AudioPanel::onPowerButtonClicked);

  layout->addWidget(powerButton);

  setLayout(layout);
}

AudioPanel::~AudioPanel(void) {
  if (powerButton != nullptr) {
    delete powerButton;
    powerButton = nullptr;
  }
  if (layout != nullptr) {
    delete layout;
    layout = nullptr;
  }
}

void AudioPanel::onPowerButtonClicked() {
  if (!on) {
    setStarted(powerButton);
    emit play_triggered();
    on = true;
  } else {
    setStopped(powerButton);
    emit stop_triggered();
    on = false;
  }
}

} /* namespace Qt */

} /* namespace Gnuitar */
