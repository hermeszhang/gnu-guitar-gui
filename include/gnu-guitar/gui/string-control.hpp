#ifndef GNU_GUITAR_GUI_STRING_CONTROL_HPP
#define GNU_GUITAR_GUI_STRING_CONTROL_HPP

#include <gnu-guitar/gui/control.hpp>

#include <vector>

namespace GnuGuitar::Gui {

class StringControl final : public Control {
  std::vector<std::string> options;
  std::string value;
public:
  StringControl();
  ~StringControl();
  void accept(ControlVisitor &controlVisitor) override;
  void addOption(const std::string &option);
  void getValue(std::string &value) const;
  void setValue(const std::string &value);
  template <typename Callable>
  void visitOptions(Callable &c) const {
    for (const auto &option : options)
      c(option);
  }
};

} // namespace GnuGuitar::Gui

#endif // GNU_GUITAR_GUI_STRING_CONTROL_HPP
