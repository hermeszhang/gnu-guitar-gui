#ifndef GNU_GUITAR_QT_LABELLED_SWITCH_HPP
#define GNU_GUITAR_QT_LABELLED_SWITCH_HPP

#include <gnu-guitar/qt/form-item.hpp>

#include <QWidget>

class QHBoxLayout;
class QLabel;
class QRadioButton;

namespace GnuGuitar {

namespace Qt {

class LabelledSwitch final : public QWidget, public FormItem {
  Q_OBJECT
public:
  LabelledSwitch(QWidget *parent = nullptr);
  ~LabelledSwitch();
  void accept(FormVisitor &formVisitor) const override;
  void getLabel(QString &labelText) const;
  void setLabel(const QString &labelText);
  void unselect();
  bool isChecked() const;
signals:
  void selected();
private slots:
  void onSelected();
private:
  QLabel *label;
  QRadioButton *radioButton;
  QHBoxLayout *layout;
};

} // namespace Qt

} // namespace GnuGuitar

#endif // GNU_GUITAR_QT_LABELLED_SWITCH_HPP
