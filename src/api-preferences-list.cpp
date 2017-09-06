#include <gnu-guitar-qt/api-preferences-list.hpp>

#include <iostream>

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>

namespace {

class Entry final : public QWidget {
public:
  Entry(QWidget *parent = nullptr);
  ~Entry();
  QString getName() const;
  void setName(const QString &name_);
private:
  QLabel label;
  QPushButton editButton;
  QRadioButton radioButton;
  QHBoxLayout layout;
};

} // namespace

namespace GnuGuitar {

namespace Qt {

ApiPreferencesList::ApiPreferencesList(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
  createApiList();
  setLayout(layout);
}

ApiPreferencesList::~ApiPreferencesList() {
  if (layout != nullptr) {
    delete layout;
    layout = nullptr;
  }
}

QString ApiPreferencesList::getSelectedApi() const {
  return "";
}

void ApiPreferencesList::addApi(const QString &apiName) {
  auto entry = new Entry(this);
  entry->setName(apiName);
  layout->addWidget(entry);
}

void ApiPreferencesList::createApiList() {

}

} // namespace Qt

} // namespace GnuGuitar

namespace {

Entry::Entry(QWidget *parent) : QWidget(parent),
                                label(this),
                                editButton("Edit", this),
                                radioButton(this),
                                layout(this) {
  layout.addWidget(&label);
  layout.addWidget(&radioButton);
  layout.addWidget(&editButton);
  connect(&editButton, &QPushButton::clicked,
          this, [&]() { std::cout << getName().toStdString() << std::endl; });
}

Entry::~Entry() {

}

void Entry::setName(const QString &name_) {
  label.setText(name_);
}

QString Entry::getName() const {
  return label.text();
}

} // namespace

