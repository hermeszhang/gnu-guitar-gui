#include <gnu-guitar-qt/audio-panel.h>

#include <gnu-guitar-qt/power-button.hpp>

#include <QHBoxLayout>

namespace GnuGuitar {

namespace Qt {

AudioPanel::AudioPanel(QWidget *parent) : QWidget(parent) {

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  layout = new QHBoxLayout();
  layout->setAlignment(::Qt::AlignVCenter | ::Qt::AlignLeft);

  powerButton = new PowerButton(this);
  connect(powerButton, &PowerButton::activated,
          this, &AudioPanel::onPowerButtonActivated);
  connect(powerButton, &PowerButton::deactivated,
          this, &AudioPanel::onPowerButtonDeactivated);

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

void AudioPanel::onPowerButtonActivated() {
  emit play_triggered();
}

void AudioPanel::onPowerButtonDeactivated() {
  emit stop_triggered();
}

} /* namespace Qt */

} /* namespace Gnuitar */
