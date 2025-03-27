// OutputWidget.h

/*
    Uniword: keyword-based unicode character selector
    Copyright (C) 2012-2025  Daniel A. Wagenaar <daw@caltech.edu>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OUTPUTWIDGET_H

#define OUTPUTWIDGET_H

#include <QTextEdit>

class OutputWidget: public QTextEdit {
  Q_OBJECT;
public:
  OutputWidget(QWidget *parent=0);
signals:
  void hoverChanged(QString);
public:
  void mouseMoveEvent(QMouseEvent *);
};

#endif
