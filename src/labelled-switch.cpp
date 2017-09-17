#include <gnu-guitar/qt/labelled-switch.hpp>

#include <gnu-guitar/qt/form-visitor.hpp>

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>

namespace GnuGuitar {

namespace Qt {

LabelledSwitch::LabelledSwitch(QWidget *parent) : QWidget(parent) {

  label = new QLabel(this);

  radioButton = new QRadioButton(this);
  connect(radioButton, &QRadioButton::pressed,
          this, &LabelledSwitch::onSelected);

  layout = new QHBoxLayout(this);
  layout->addWidget(label);
  layout->addWidget(radioButton);
}

LabelledSwitch::~LabelledSwitch() {

}

void LabelledSwitch::accept(FormVisitor &formVisitor) const {
  formVisitor.visit(*this);
}

void LabelledSwitch::getLabel(QString &labelText) const {
  labelText = label->text();
}

void LabelledSwitch::setLabel(const QString &labelText) {
  label->setText(labelText);
}

bool LabelledSwitch::isChecked() const {
  return radioButton->isChecked();
}

void LabelledSwitch::select() {
  radioButton->setChecked(true);
}

void LabelledSwitch::unselect() {
  radioButton->setChecked(false);
}

void LabelledSwitch::onSelected() {
  emit selected();
}

} // namespace Qt

} // namespace GnuGuitar
