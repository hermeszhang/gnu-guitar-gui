#include <gnu-guitar-qt/api-preferences-list.hpp>

#include <rtaudio/rtaudio.hpp>

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
  std::vector<RtAudio::ApiSpecifier> apiList;
  RtAudio::getCompiledApi(apiList);
  for (auto api : apiList) {
    switch (api) {
      case RtAudio::ApiSpecifier::ALSA:
        addApi("ALSA");
        break;
      case RtAudio::ApiSpecifier::Jack:
        addApi("Jack");
        break;
      case RtAudio::ApiSpecifier::Pulse:
        addApi("Pulse");
        break;
      case RtAudio::ApiSpecifier::WASAPI:
        addApi("WASAPI");
        break;
      case RtAudio::ApiSpecifier::ASIO:
        addApi("ASIO");
        break;
      case RtAudio::ApiSpecifier::CoreAudio:
        addApi("Core Audio");
        break;
      case RtAudio::ApiSpecifier::OpenSoundSystem:
        addApi("Open Sound System");
        break;
      case RtAudio::ApiSpecifier::DirectSound:
        addApi("Direct Sound");
        break;
      case RtAudio::ApiSpecifier::Dummy:
        addApi("Dummy");
        break;
      case RtAudio::ApiSpecifier::Any:
        break;
    }
  }
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

