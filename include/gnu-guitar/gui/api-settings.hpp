#ifndef GNU_GUITAR_GUI_API_SETTINGS_HPP
#define GNU_GUITAR_GUI_API_SETTINGS_HPP

#include <string>

namespace GnuGuitar::Gui {

class ControlList;
class ControlVisitor;
class StringControl;

class ApiSettings final {
  std::string apiName;
  ControlList *controlList;
public:
  ApiSettings();
  ApiSettings(const ApiSettings &other) = delete;
  ApiSettings(ApiSettings &&other);
  ~ApiSettings();
  void addControl(const StringControl &stringControl);
  void getApiName(std::string &apiName) const;
  void setApiName(const std::string &apiName);
  void visitControls(ControlVisitor &controlVisitor) const;
};

} // namespace GnuGuitar::Gui

#endif // GNU_GUITAR_GUI_API_SETTINGS_HPP
