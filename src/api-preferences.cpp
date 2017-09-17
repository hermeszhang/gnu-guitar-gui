#include <gnu-guitar-qt/api-preferences.hpp>

#include <gnu-guitar/gui/api-settings.hpp>
#include <gnu-guitar/gui/binary-control.hpp>
#include <gnu-guitar/gui/control-visitor.hpp>
#include <gnu-guitar/gui/string-control.hpp>

#include <gnu-guitar/qt/api-form.hpp>
#include <gnu-guitar/qt/combo-box.hpp>
#include <gnu-guitar/qt/form-visitor.hpp>
#include <gnu-guitar/qt/labelled-switch.hpp>

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

class ApiFormReader final : public GnuGuitar::Qt::FormVisitor {
  GnuGuitar::Gui::ApiSettings &apiSettings;
  bool foundApi;
public:
  ApiFormReader(GnuGuitar::Gui::ApiSettings &apiSettings_) : apiSettings(apiSettings_) {
    foundApi = false;
  }
  ~ApiFormReader() {
  }
  bool apiFound() const {
    return foundApi;
  }
  void visit(const GnuGuitar::Qt::ComboBox &comboBox) override {

    auto label = comboBox.getLabel();
    auto value = comboBox.getValue();

    GnuGuitar::Gui::StringControl stringControl;
    stringControl.setName(label.toStdString());
    stringControl.setValue(value.toStdString());
    apiSettings.addControl(stringControl);
  }
  void visit(const GnuGuitar::Qt::LabelledSwitch &labelledSwitch) override {
    QString label;
    labelledSwitch.getLabel(label);
    if (label == "Use this API") {
      foundApi = true;
      return;
    }

    GnuGuitar::Gui::BinaryControl binaryControl;
    binaryControl.setName(label.toStdString());
    if (labelledSwitch.isChecked())
      binaryControl.setOn();
    else
      binaryControl.setOff();
  }
};

} // namespace

namespace GnuGuitar {

namespace Qt {

ApiPreferences::ApiPreferences(QWidget *parent) : QAccordion(parent) {

}

ApiPreferences::~ApiPreferences() {

}

void ApiPreferences::getSelectedApi(Gui::ApiSettings& apiSettings) const {
  ApiFormReader apiFormReader(apiSettings);
  for (auto form : forms) {

    if (!form->selected())
      continue;

    QString apiName;
    form->getName(apiName);

    apiSettings.setApiName(apiName.toStdString());

    form->accept(apiFormReader);
    break;
  }
}

void ApiPreferences::addApi(const Gui::ApiSettings &apiSettings) {

  std::string apiName;
  apiSettings.getApiName(apiName);

  auto form = new ApiForm(this);

  ::FormBuilder formBuilder(form);
  apiSettings.accept(formBuilder);

  addApi(apiName.c_str(), form);
}

void ApiPreferences::addApi(const QString &apiName, ApiForm *apiForm) {

  auto apiSelectedFunctor = [&, apiForm]() {
    onApiSelected(apiForm);
  };

  connect(apiForm, &ApiForm::apiSelected,
          this, apiSelectedFunctor);

  apiForm->setName(apiName);

  auto contentPane = new ContentPane(apiName, this);
  contentPane->setContentFrame(apiForm);

  if (forms.size() == 0) {
    // The first API to be added
    // will be the default API.
    apiForm->select();
  }

  forms.push_back(apiForm);
  addContentPane(contentPane);
}

void ApiPreferences::onApiSelected(ApiForm *apiForm) {
  for (auto form : forms) {
    if (form == apiForm)
      form->select();
    else
      form->unselect();
  }
}

} // namespace Qt

} // namespace GnuGuitar

