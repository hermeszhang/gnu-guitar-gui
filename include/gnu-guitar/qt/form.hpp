#ifndef GNU_GUITAR_QT_FORM_HPP
#define GNU_GUITAR_QT_FORM_HPP

#include <QFrame>

class QVBoxLayout;

namespace GnuGuitar {

namespace Gui {

class BinaryControl;
class StringControl;

} // namespace Gui

namespace Qt {

class ComboBox;
class FormItem;
class FormVisitor;
class LabelledSwitch;

class Form : public QFrame {
  Q_OBJECT
public:
  Form(QWidget *parent = nullptr);
  virtual ~Form();
  void accept(FormVisitor &formVisitor) const;
  void addControl(const Gui::BinaryControl &binaryControl);
  void addControl(const Gui::StringControl &stringControl);
protected:
  void add(ComboBox *comboBox);
  void add(LabelledSwitch *switch_);
private:
  QList<FormItem*> items;
  QVBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_FORM_HPP

