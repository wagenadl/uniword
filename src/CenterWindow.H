// CenterWindow.H

/*
    Uniword: keyword-based unicode character selector
    Copyright (C) 2012  Daniel A. Wagenaar <daw@caltech.edu>

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

#ifndef CENTERWINDOW_H

#define CENTERWINDOW_H

#include <QString>
#include <QWidget>

class CenterWindow: public QWidget {
  Q_OBJECT;
public:
  CenterWindow(class Universe const &uverse,
	       QWidget *parent=0);
  virtual ~CenterWindow();
  QFont const &displayFont() const;
public slots:
  void setDisplayFont(QFont f);
  void copyAvailable(bool s);
  void selected();
  void clipinfo();
private:
  void useInput(QString const &s, bool definitive);
  void setComment(int c);
  void setEmptyComment();
  void setMultiComment();
private:
  class QTextEdit *output;
  class QLineEdit *input;
  class QTextEdit *comment;
  Universe const &uverse;
  bool copyAv;
  QString origclipt, newclipt;
};

#endif
