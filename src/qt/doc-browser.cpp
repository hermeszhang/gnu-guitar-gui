// Copyright (C) 2017 Taylor Holberton
//
//  This file is part of GNU Guitar GUI.
//
//  GNU Guitar GUI is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  GNU Guitar GUI is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with GNU Guitar GUI.  If not, see <http://www.gnu.org/licenses/>.

#include <gnu-guitar/qt/doc-browser.hpp>

#include <QTextBrowser>
#include <QVBoxLayout>

namespace GnuGuitar {

namespace Qt {

DocBrowser::DocBrowser(QWidget *parent) : QWidget(parent) {

  textBrowser = new QTextBrowser(this);
  textBrowser->setOpenLinks(true);
  textBrowser->setOpenExternalLinks(true);
  textBrowser->setSearchPaths(QStringList() << ":/docs");
  textBrowser->setSource(QString("index.html"));

  layout = new QVBoxLayout(this);
  layout->addWidget(textBrowser);

  setLayout(layout);
}

DocBrowser::~DocBrowser(void) {

}

} /* namespace Qt */

} /* namespace Gnuitar */
