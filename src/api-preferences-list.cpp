#include <gnu-guitar-qt/api-preferences-list.hpp>

#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

ApiPreferencesList::ApiPreferencesList(QWidget *parent) : QWidget(parent) {
  layout = new QVBoxLayout(this);
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

} // namespace Qt

} // namespace GnuGuitar
