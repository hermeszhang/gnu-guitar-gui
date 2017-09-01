#include <gnu-guitar-qt/rack.h>

#include <gnu-guitar-qt/knob.h>

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>

namespace Gnuitar {

namespace Qt {

Rack::Rack(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout;
  setLayout(layout);
}

Rack::~Rack(void) {}

void Rack::add_effect(EffectView *effect_view) {
  layout->addWidget(effect_view);

  auto effect_callback = [this, effect_view](const QString &control_name,
                                             int control_value) {
    auto effect_label = effect_view->get_label();
    on_effect_changed(effect_label, control_name, control_value);
  };
  connect(effect_view, &EffectView::control_changed, this, effect_callback);
}

void Rack::on_effect_changed(const QString &effect_name,
                             const QString &control_name, int control_value) {
  emit effect_changed(effect_name, control_name, control_value);
}

} /* namespace Qt */

} /* namespace Gnuitar */
