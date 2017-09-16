#include <gnu-guitar/qt/form.hpp>

#include <gnu-guitar/qt/combo-box.hpp>

#include <gnu-guitar/gui/string-control.hpp>

#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

Form::Form(QWidget *parent) : QFrame(parent) {
  layout = new QVBoxLayout(this);
  setLayout(layout);
}

Form::~Form() {

}

void Form::addControl(const Gui::StringControl &stringControl) {

  std::string controlName;
  stringControl.getName(controlName);

  auto comboBox = new ComboBox(this);
  comboBox->setLabel(controlName.c_str());

  auto optionVisitor = [comboBox](const std::string &option) {
    comboBox->addOption(option.c_str());
  };

  stringControl.visitOptions(optionVisitor);

  layout->addWidget(comboBox);
}

} // namespace Qt

} // namespace GnuGuitar
