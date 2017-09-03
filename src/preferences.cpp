#include <gnu-guitar-qt/preferences.hpp>

#include <gnu-guitar-qt/api-preferences-list.hpp>

#include <QScrollArea>

namespace GnuGuitar {

namespace Qt {

Preferences::Preferences(QWidget *parent) : QDialog(parent) {
  setWindowTitle("Preferences");
  scrollArea = new QScrollArea(this);
  scrollArea->setWidgetResizable(true);
  apiPreferencesList = new ApiPreferencesList(this);
  scrollArea->setWidget(apiPreferencesList);
}

Preferences::~Preferences() {
  if (apiPreferencesList != nullptr) {
    delete apiPreferencesList;
    apiPreferencesList = nullptr;
  }
  if (scrollArea != nullptr) {
    delete scrollArea;
    scrollArea = nullptr;
  }
}

} // namespace Qt

} // namespace GnuGuitar
