#ifndef GNUITAR_QT_BUTTON_H
#define GNUITAR_QT_BUTTON_H

#include <QLabel>
#include <QPixmap>

namespace Gnuitar
{

namespace Qt
{

class Button : public QLabel
{
    Q_OBJECT
public:
  Button (const QString& image_path, int width = 64, int height = 64, QWidget *parent = nullptr);
  ~Button (void);
signals:
  void clicked (void);
protected:
  void mousePressEvent (QMouseEvent *event);
private:
  QPixmap pixmap;
}; /* class Button */

} /* namespace Qt */

} /* namespace Gnuitar */

#endif /* GNUITAR_QT_BUTTON_H */

