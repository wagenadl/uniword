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

  input = new QLineEdit;
  input->setFixedHeight(40);

  output = new OutputWidget;
  output->setReadOnly(true);
  output->setText("");
  output->setAlignment(Qt::AlignLeft | Qt::AlignAbsolute);
  connect(output, &QTextEdit::selectionChanged,
          this, &CenterWindow::selected);
  connect(output, &OutputWidget::hoverChanged,
          this, &CenterWindow::hovered);

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
          [this]() { useInput(input->text(), false); });
  connect(input, &QLineEdit::returnPressed,
          [this]() { useInput(input->text(), true); });
}

CenterWindow::~CenterWindow() {
}

void CenterWindow::useInput(QString const &s, bool definitive) {
  if (QRegExp("[\\s.]*").exactMatch(s)) {
    output->setText("");
    setEmptyComment();
    return;
  }

  setMultiComment();

  QStringList lst = s.split(QRegExp("\\s+"));

  QSet<int> cc = uverse.find(lst);
  if (cc.isEmpty()) {
    output->setText("(none)");
  } else {
    QVector<int> ordered;
    ordered.reserve(cc.size());
    foreach (int c, cc)
      ordered.push_back(c);
    std::sort(ordered.begin(), ordered.end());
    int MAXGLYPHS = definitive ? 500 : 20;
    bool toolong = ordered.size()>MAXGLYPHS;
    if (toolong)
      ordered.resize(MAXGLYPHS);

    output->clear();
    QTextCursor tc(output->textCursor());
    tc.beginEditBlock();
    QString s = "";
    
    bool first = true;
    foreach (int c, ordered) {
      //if (uverse.isCombiner(c))
      //	continue;
      if (!first)
	s += " ";
      first = false;

      if (uverse.isCombiner(c) || uverse.isModifier(c)) {
	tc.insertText(s);
	s = "";
	QTextCharFormat tf = tc.charFormat();
	tf.setForeground(QColor("#aaaaaa"));
	tc.setCharFormat(tf);
	tc.insertText(QChar(0x25fd)); // put combiner over a box
	tf.setForeground(QColor("black"));
	tc.setCharFormat(tf);
      }
      if (uverse.isSpace(c)) {
	tc.insertText(s);
	s = "";
	QTextCharFormat tf = tc.charFormat();
	tf.setFontUnderline(true);
	tf.setUnderlineColor("#aaaaaa");
	tc.setCharFormat(tf);
      }	

      if (c>=65536) {
	s += QChar(QChar::highSurrogate(c));
	s += QChar(QChar::lowSurrogate(c));
      } else {
	s += QChar(c);
      }

      if (uverse.isSpace(c)) {
	tc.insertText(s);
	s = "";
	QTextCharFormat tf = tc.charFormat();
	tf.setFontUnderline(false);
	tc.setCharFormat(tf);
      }	
      
      s += " ";
      s += QChar(0x200e);
    }
    if (toolong)
      s += "(…)";
    
    tc.insertText(s);

    tc.movePosition(QTextCursor::Start);
    tc.endEditBlock();
    
    if (cc.size()==1) {
      s.replace(" ", "");
      s.replace(QChar(0x200e), "");
      if (s[1]==QChar(0x25fd) && s.size()>1)
	s = s.mid(1); // skip combiner and modifier base boxes
      QApplication::clipboard()->setText(s.mid(1));
      setComment(*cc.begin());
    }
  }
}


void CenterWindow::setEmptyComment() {
  comment->setHtml("<i>Type some words to select characters.</i>");
}

void CenterWindow::setMultiComment() {
  comment->setHtml("");//<i>Hover over a character to see description.</i>");
}

void CenterWindow::selected() {
  if (output->toPlainText().isEmpty()) {
    setEmptyComment();
    return;
  }
  QString t = output->textCursor().selectedText();
  int chr = extractChar(t);
  setComment(chr);
  if (chr > 0) {
    QApplication::clipboard()->setText(QString(QChar(chr)));
    QApplication::clipboard()->setText(QString(QChar(chr)),
                                       QClipboard::Selection);
  }
}

int CenterWindow::extractChar(QString t) {
  int chr = -1;
  t.replace(" ", "");
  t.replace(QChar(0x200e), "");
  if (t.length()>1 && t[0]==QChar(0x25fd))
    t=t.mid(1);
  if (t.length()==1) {
    chr = t[0].unicode();
  } else if (t.length()==2) {
    QChar s0 = t[0];
    QChar s1 = t[1];
    if (s0.isHighSurrogate() && s1.isLowSurrogate()) 
      chr = 0x10000
	+ (s0.unicode()-0xd800)*0x400
	+ (s1.unicode()-0xdc00);
  }
  return chr;
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
  output->setFont(f);
}

QFont const &CenterWindow::displayFont() const {
  return output->font();
}

void CenterWindow::hovered(QString s) {
  int chr = extractChar(s);
  setComment(chr);
}
