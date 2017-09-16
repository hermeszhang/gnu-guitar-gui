#ifndef GNU_GUITAR_GUI_CONTROL_VISITOR_HPP
#define GNU_GUITAR_GUI_CONTROL_VISITOR_HPP

namespace GnuGuitar::Gui {

class BinaryControl;
class StringControl;

class ControlVisitor {
public:
  virtual ~ControlVisitor();
  virtual void visit(const BinaryControl &binaryControl) = 0;
  virtual void visit(const StringControl &stringControl) = 0;
};

} // namespace GnuGuitar::Gui

#endif // GNU_GUITAR_GUI_CONTROL_VISITOR_HPP

