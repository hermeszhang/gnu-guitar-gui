#ifndef GNU_GUITAR_FORM_ITEM_HPP
#define GNU_GUITAR_FORM_ITEM_HPP

namespace GnuGuitar {

namespace Qt {

class FormVisitor;

class FormItem {
public:
  virtual ~FormItem();
  virtual void accept(FormVisitor &formVisitor) const = 0;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_FORM_ITEM_HPP
