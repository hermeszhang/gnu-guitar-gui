// Copyright (C) 2017 Taylor Holberton
//
//  This file is part of GNU Guitar Qt.
//
//  GNU Guitar Qt is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  GNU Guitar Qt is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with GNU Guitar Qt.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GNUITAR_QT_DOC_BROWSER_HPP
#define GNUITAR_QT_DOC_BROWSER_HPP

#include <QWidget>

class QTextBrowser;
class QVBoxLayout;

namespace GnuGuitar {

namespace Qt {

class DocBrowser : public QWidget {
  Q_OBJECT
public:
  DocBrowser(QWidget *parent = nullptr);
  ~DocBrowser(void);
private:
  QVBoxLayout *layout;
  QTextBrowser *textBrowser;
}; /* class DocBrowser */

} /* namespace Qt */

} /* namespace GnuGuitar */

#endif /* GNUITAR_qT_DOC_BROWSER_HPP */
