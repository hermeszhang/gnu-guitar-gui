#ifndef GNUITAR_QT_RACK_H
#define GNUITAR_QT_RACK_H

#include <QVBoxLayout>
#include <QWidget>

namespace GnuGuitar {

namespace Qt {

class EffectView;

class Rack final : public QWidget {
  Q_OBJECT
public:
  Rack(QWidget *parent = nullptr);
  ~Rack(void);
  void addEffect(EffectView *effectView);
signals:
  void effectChanged(const QString &effectName,
                     const QString &controlName,
                     int value);
protected slots:
  void onEffectChanged(const QString &effectName,
                       const QString &controlName,
                       int value);
protected:
  void setupColors();
private:
  QVBoxLayout *layout;
}; /* class Rack */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_RACK_H */
