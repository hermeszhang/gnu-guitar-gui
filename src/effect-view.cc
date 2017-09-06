#include <gnu-guitar-qt/effect-view.h>
#include <gnu-guitar-qt/knob.h>

#include <QSizePolicy>

namespace Gnuitar {

namespace Qt {

EffectView::EffectView(const QString &label_text_, QWidget *parent) : QWidget(parent) {

  setupColors();

  label = new QLabel;
  label->setTextFormat(::Qt::RichText);
  label->setAlignment(::Qt::AlignLeft | ::Qt::AlignVCenter);
  label->setFixedSize(200, 100);
  label->setStyleSheet("QLabel { color: rgb(255,63,0); }");

  layout = new QHBoxLayout;
  layout->setAlignment(::Qt::AlignLeft);
  layout->addWidget(label);
  setLayout(layout);

  set_label(label_text_);
}

EffectView::~EffectView(void) {}

void EffectView::add_control(const QString &control_name) {

  auto control_knob = new Knob(control_name, this);

  auto control_callback = [this, control_knob] {
    EffectView::on_knob_changed(control_knob);
  };

  connect(control_knob, &Knob::released, this, control_callback);

  layout->addWidget(control_knob);
}

const QString &EffectView::get_label(void) const { return label_text; }

void EffectView::set_label(const QString &label_text_) {
  label_text = label_text_;
  label->setText("<i><b>" + label_text + "</i></b>");
}

void EffectView::on_knob_changed(const Knob *knob) {
  auto text = knob->get_label();
  auto value = knob->get_value();
  emit control_changed(text, value);
}

void EffectView::setupColors() {
  auto palette_ = palette();
  palette_.setColor(QPalette::Background, QColor(20, 20, 20));
  setPalette(palette_);
  setAutoFillBackground(true);
}

} /* namespace Qt */

} /* namespace Gnuitar */
