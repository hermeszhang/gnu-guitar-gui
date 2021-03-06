#ifndef GNU_GUITAR_QT_COMBO_BOX_HPP
#define GNU_GUITAR_QT_COMBO_BOX_HPP

#include <gnu-guitar/qt/form-item.hpp>

#include <QWidget>

class QComboBox;
class QLabel;
class QHBoxLayout;

namespace GnuGuitar {

namespace Qt {

class ComboBox final : public QWidget, public FormItem {
  Q_OBJECT
public:
  ComboBox(QWidget *parent = nullptr);
  ~ComboBox();
  void accept(FormVisitor &formVisitor) const override;
  void addOption(const QString &option);
  QString getLabel() const;
  QString getValue() const;
  void setLabel(const QString &label);
private:
  QLabel *label;
  QComboBox *comboBox;
  QHBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_COMBO_BOX_HPP
