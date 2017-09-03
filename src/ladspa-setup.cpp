#include <gnu-guitar-qt/ladspa-setup.hpp>

#include <QComboBox>
#include <QGridLayout>
#include <QPushButton>
#include <QString>

namespace GnuGuitar {

LadspaSetup::LadspaSetup(QWidget *parent) : QDialog(parent) {

  setWindowTitle("Ladspa Setup");

  cancelPushed  = false;

  layout = new QGridLayout(this);

  inputBox = new QComboBox(this);
  auto row = 0;
  auto column = 0;
  layout->addWidget(inputBox, row, column);

  outputBox = new QComboBox(this);
  row = 0;
  column = 1;
  layout->addWidget(outputBox, row, column);

  okayButton = new QPushButton("Okay", this);
  row = 1;
  column = 0;
  layout->addWidget(okayButton, row, column);
  connect(okayButton, &QPushButton::clicked, this, &QDialog::close);

  cancelButton = new QPushButton("Cancel", this);
  row = 1;
  column = 1;
  layout->addWidget(cancelButton, row, column);
  connect(cancelButton, &QPushButton::clicked, this, [&]() {
      cancelPushed = true;
      close();
  });
}

LadspaSetup::~LadspaSetup() {
  delete layout;
  delete inputBox;
  delete outputBox;
  delete okayButton;
  delete cancelButton;
}

void LadspaSetup::addInput(const std::string &name) {
  inputBox->addItem(QString(name.c_str()));
}

void LadspaSetup::addOutput(const std::string &name) {
  outputBox->addItem(QString(name.c_str()));
}

bool LadspaSetup::cancelled(void) const noexcept {
  return cancelPushed;
}

std::string LadspaSetup::getInputName(void) {
  return inputBox->currentText().toStdString();
}

std::string LadspaSetup::getOutputName(void) {
  return outputBox->currentText().toStdString();
}

} // namespace GnuGuitar
