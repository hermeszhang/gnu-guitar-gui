#include <gnu-guitar/qt/api-form.hpp>

#include <gnu-guitar/qt/labelled-switch.hpp>

namespace GnuGuitar {

namespace Qt {

ApiForm::ApiForm(QWidget *parent) : Form(parent) {

  useSwitch = new LabelledSwitch(this);
  useSwitch->setLabel("Use this API");

  connect(useSwitch, &LabelledSwitch::selected,
          this, &ApiForm::onApiSelected);

  add(useSwitch);
}

ApiForm::~ApiForm() {

}

void ApiForm::getName(QString &name_) const {
  name_ = name;
}

void ApiForm::setName(const QString &name_) {
  name = name_;
}

bool ApiForm::selected() const {
  return isSelected;
}

void ApiForm::select() {
  isSelected = true;
}

void ApiForm::unselect() {
  isSelected = false;
  useSwitch->unselect();
}

void ApiForm::onApiSelected() {
  select();
  emit apiSelected();
}

} // namespace Qt

} // namespace GnuGuitar
