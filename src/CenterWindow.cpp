// CenterWindow.cpp

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

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QVBoxLayout>
#include <QMimeData>
#include <QTextBlock>
#include <QTextBlockFormat>
#include <algorithm>

#include "CenterWindow.h"
#include "Universe.h"
#include "OutputWidget.h"

#define fg QColor("#000000")
#define bg QColor("#eeeeee")

CenterWindow::CenterWindow(Universe const &uverse,
			   QWidget *parent):
  QWidget(parent),
  uverse(uverse) {
  resize(300,280);

  includemissing = false;
  allowmerging = false;
  input = new QLineEdit;
  input->setFixedHeight(40);

  output = new OutputWidget(uverse);
  connect(output, &OutputWidget::selected,
          this, &CenterWindow::select);
  connect(output, &OutputWidget::hovered,
          this, &CenterWindow::hover);
  connect(output, &OutputWidget::quit,
          this, []() { qDebug() << "??"; QApplication::quit(); });
  connect(output, &OutputWidget::resized,
          this, &CenterWindow::recount);

  comment = new QTextEdit;
  comment->setFixedHeight(80);
  comment->setReadOnly(true);
  setEmptyComment();
  
  QPalette p = palette();
  p.setColor(QPalette::Button, bg); // this is what our children use
  p.setColor(QPalette::Window, bg); // this is what our children use
  p.setColor(QPalette::ButtonText, fg);
  p.setColor(QPalette::WindowText, fg);
  setPalette(p);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(input);
  layout->addWidget(output);
  layout->addWidget(comment);
  this->setLayout(layout);

  connect(input, &QLineEdit::textChanged,
          [this]() { definitive=false; useInput(input->text()); });
  connect(input, &QLineEdit::returnPressed,
          [this]() { definitive=true; useInput(input->text()); });
}

CenterWindow::~CenterWindow() {
}

void CenterWindow::useInput(QString const &s) {
  options.clear();  
  if (QRegExp("[\\s.]*").exactMatch(s)) {
    output->clear();
    setEmptyComment();
    return;
  }
  setMultiComment();

  QStringList lst = s.split(QRegExp("\\s+"));
  QSet<int> cc = uverse.find(lst);
  if (cc.isEmpty()) {
    output->clear();
    return;
  }

  QFontMetrics fm(displayFont());

  int MAXGLYPHS = definitive ? 500 : output->fittableGlyphs() - 2;
  QList<int> chars = cc.values();
  std::sort(chars.begin(), chars.end());
  for (int c: chars) 
    if (options.size() <= MAXGLYPHS && (includemissing || fm.inFontUcs4(c)))
      options << c;

  bool toolong = options.size()>MAXGLYPHS;
  if (toolong) 
    while (options.size() > MAXGLYPHS)
      options.takeLast();
  
  output->setTiles(options, toolong);
  if (options.size() == 1) 
    select(QList<int>{0});
}


void CenterWindow::setEmptyComment() {
  comment->setHtml("<i>Type some words to select characters.</i>");
}

void CenterWindow::setMultiComment() {
  comment->setHtml("");//<i>Hover over a character to see description.</i>");
}

void CenterWindow::recount() {
  if (!definitive)
    useInput(input->text());    
}  

void CenterWindow::select(QList<int> indices) {
  if (indices.isEmpty()) {
    // setEmptyComment();
    return;
  }
  QString text = "";
  bool one = indices.size() == 1;
  qDebug() << one << indices[0] << options.size();
  if (one && indices[0]==options.size()) {
    // ellipsis clicked
    definitive = true;
    useInput(input->text());    
    return;
  }
  for (int idx: indices) {
    if (idx >=0 && idx < options.size()) {
      int c = options[idx];
      if (one)
        setComment(c);
      if (c>=65536) {
        text += QChar(QChar::highSurrogate(c));
        text += QChar(QChar::lowSurrogate(c));
      } else {
        text += QChar(c);
      }
    }
  }
  QApplication::clipboard()->setText(text);
  QApplication::clipboard()->setText(text, QClipboard::Selection);
}
  
void CenterWindow::setComment(int chr) {
  if (chr < 0) {
    setMultiComment();
    return;
  }
  QString d = uverse.describe(chr);
  QString g = uverse.getgroups(chr);
  QString c = d;

  if (!d.isEmpty())
	c += " ";
  c += "(0x"+QString::number(chr, 16) + ")";

  if (!g.isEmpty())
    c+= "<br><i>(" + g + ")</i>";

  comment->setHtml(c);
}

void CenterWindow::setDisplayFont(QFont f) {
  f.setStyleStrategy(QFont::StyleStrategy(QFont::PreferDefault
                                          | (allowmerging ? 0 : QFont::NoFontMerging)
                                          | QFont::PreferQuality
                                          | QFont::PreferNoShaping));
  output->setFont(f);
}

QFont const &CenterWindow::displayFont() const {
  return output->font();
}

void CenterWindow::hover(int index) {
  if (index >=0 && index < options.size())
    setComment(options[index]);
  else
    setMultiComment();
}

void CenterWindow::setExclude(bool x) {
  bool incl = !x;
  if (incl != includemissing) {
    includemissing = incl;
    useInput(input->text());
  }
}


void CenterWindow::setMerging(bool x) {
  if (x != allowmerging) {
    allowmerging = x;
    setDisplayFont(displayFont());
    useInput(input->text());
  }
}
