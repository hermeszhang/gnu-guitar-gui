#include <gnu-guitar-qt/effect-view.hpp>

#include <gnu-guitar-qt/knob.h>

#include <QGridLayout>
#include <QSizePolicy>

namespace GnuGuitar {

namespace Qt {

EffectView::EffectView(QWidget *parent) : QWidget(parent) {

  columnIndex = 1;
  rowIndex = 1;

  setupColors();

  label = new QLabel;
  label->setAlignment(::Qt::AlignCenter);
  label->setStyleSheet("QLabel { font-weight: bold; color: rgb(255,63,0); }");

  layout = new QGridLayout;
  layout->addWidget(label, 1, 1, 1, 4, ::Qt::AlignCenter);
  setLayout(layout);
}

EffectView::~EffectView(void) {

}

void EffectView::addControl(const QString &controlName) {

  auto controlKnob = new Knob(controlName, this);

  auto controlCallback = [this, controlKnob] {
    onKnobChanged(*controlKnob);
  };

  connect(controlKnob, &Knob::released, this, controlCallback);

  layout->addWidget(controlKnob, rowIndex + 1, columnIndex);
  if (columnIndex >= 4) {
    columnIndex = 1;
    rowIndex++;
  } else {
    columnIndex++;
  }
}

QString EffectView::getLabel(void) const {
  return label->text();
}

void EffectView::setLabel(const QString &labelText) {
  label->setText(labelText);
}

void EffectView::onKnobChanged(const Knob &knob) {
  auto text = knob.get_label();
  auto value = knob.get_value();
  emit controlChanged(text, value);
}

void EffectView::setupColors() {
  auto palette_ = palette();
  palette_.setColor(QPalette::Background, QColor(20, 20, 20));
  setPalette(palette_);
  setAutoFillBackground(true);
}

} /* namespace Qt */

} /* namespace Gnuitar */
