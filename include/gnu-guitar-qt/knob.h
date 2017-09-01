#ifndef GNUITAR_QT_KNOB_H
#define GNUITAR_QT_KNOB_H

#include <QDial>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

namespace Gnuitar {

namespace Qt {

class Knob final : public QWidget {
  Q_OBJECT
public:
  Knob(const QString &label, QWidget *parent = nullptr);
  ~Knob(void);
  QString get_label(void) const;
  int get_value(void) const;
  void set_label(const QString &label);
  void set_value(int value);
signals:
  void released(void);
protected slots:
  void on_released(void);

private:
  QDial *dial;
  QLabel *label;
  QVBoxLayout *layout;
}; /* class Knob */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_KNOB_H */
