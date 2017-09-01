#include <gnu-guitar-qt/button.h>

namespace Gnuitar {

namespace Qt {

Button::Button(const QString &image_path, int width, int height,
               QWidget *parent)
    : QLabel(parent) {
  if (pixmap.load(image_path) == true) {
    pixmap = pixmap.scaled(width, height, ::Qt::KeepAspectRatio,
                           ::Qt::SmoothTransformation);
    setPixmap(pixmap);
    setScaledContents(false);
  } else
    setText(image_path);
  setAlignment(::Qt::AlignHCenter);
}

Button::~Button(void) {}

void Button::mousePressEvent(QMouseEvent *) { emit clicked(); }

} /* namespace Qt */

} /* namespace Gnuitar */
