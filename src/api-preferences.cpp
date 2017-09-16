#include <gnu-guitar-qt/api-preferences.hpp>

#include <gnu-guitar/gui/api-settings.hpp>
#include <gnu-guitar/gui/control-visitor.hpp>

#include <gnu-guitar/qt/form.hpp>

namespace {

class FormBuilder final : public GnuGuitar::Gui::ControlVisitor {
  GnuGuitar::Qt::Form *form;
public:
  FormBuilder(GnuGuitar::Qt::Form *form_) : form(form_) {

  }
  ~FormBuilder() {

  }
  void visit(const GnuGuitar::Gui::BinaryControl &binaryControl) {
    form->addControl(binaryControl);
  }
  void visit(const GnuGuitar::Gui::StringControl &stringControl) {
    form->addControl(stringControl);
  }
};

} // namespace

namespace GnuGuitar {

namespace Qt {

ApiPreferences::ApiPreferences(QWidget *parent) : QAccordion(parent) {

}

ApiPreferences::~ApiPreferences() {

}

QString ApiPreferences::getSelectedApi() const {
  return "";
}

void ApiPreferences::addApi(const Gui::ApiSettings &apiSettings) {

  std::string apiName;
  apiSettings.getApiName(apiName);

  auto form = new Form(this);

  ::FormBuilder formBuilder(form);
  apiSettings.visitControls(formBuilder);

  auto contentPane = new ContentPane(apiName.c_str(), this);
  contentPane->setContentFrame(form);

  addContentPane(contentPane);
}

void ApiPreferences::onApiClicked(const QString &apiName) {
#if 0
  for (auto api : apiList) {
    if (api->getName() == apiName)
      continue;
    else
      api->uncheck();
  }
#endif
  emit apiClicked(apiName);
}

} // namespace Qt

} // namespace GnuGuitar

