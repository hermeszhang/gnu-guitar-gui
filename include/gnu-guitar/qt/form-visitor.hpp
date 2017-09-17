#ifndef GNU_GUITAR_QT_FORM_VISITOR_HPP
#define GNU_GUITAR_QT_FORM_VISITOR_HPP

namespace GnuGuitar {

namespace Qt {

class ComboBox;
class LabelledSwitch;

class FormVisitor {
public:
  virtual ~FormVisitor();
  virtual void visit(const ComboBox &comboBox) = 0;
  virtual void visit(const LabelledSwitch &labelledSwitch) = 0;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_FORM_VISITOR_HPP
