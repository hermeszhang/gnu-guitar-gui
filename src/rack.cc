#include <gnu-guitar-qt/rack.h>

#include <gnu-guitar-qt/effect-view.hpp>
#include <gnu-guitar-qt/knob.h>

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

Rack::Rack(QWidget *parent) : QWidget(parent) {
  setupColors();
  layout = new QVBoxLayout;
  layout->setAlignment(::Qt::AlignTop);
  setLayout(layout);
}

Rack::~Rack() {

}

void Rack::addEffect(EffectView *effectView) {

  layout->addWidget(effectView);

  auto effectCallback = [this, effectView](const QString &controlName,
                                           int controlValue) {
    auto effectLabel = effectView->getLabel();
    onEffectChanged(effectLabel, controlName, controlValue);
  };
  connect(effectView, &EffectView::controlChanged, this, effectCallback);
}

void Rack::onEffectChanged(const QString &effectName,
                           const QString &controlName,
                           int controlValue) {
  emit effectChanged(effectName, controlName, controlValue);
}

void Rack::setupColors() {
  auto palette_ = palette();
  palette_.setColor(QPalette::Background, QColor(17, 17, 17));
  setAutoFillBackground(true);
  setPalette(palette_);
}

} /* namespace Qt */

} /* namespace Gnuitar */
