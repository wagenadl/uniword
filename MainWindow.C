// MainWindow.C

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

#include "MainWindow.H"
#include "Universe.H"

#define FONT "Linux Libertine"
#define fg QColor("#000000")
#define bg QColor("#eeeeee")

MainWindow::MainWindow(Universe const &uverse, QString fontname,
		       QWidget *parent):
  QWidget(parent),
  uverse(uverse),
  fontname(fontname) {
  resize(300,280);

  if (fontname.isEmpty())
    fontname = FONT;

  input = new QLineEdit;
  input->setFixedHeight(40);
  input->setFont(QFont(fontname, 12));

  output = new QTextEdit;
  // output->setWordWrap(true);
  //  output->setTextInteractionFlags(Qt::TextSelectableByMouse);
  output->setReadOnly(true);
  output->setFont(QFont(fontname, 15));
  output->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  output->setText("");
  copyAv = false;
  connect(output, SIGNAL(copyAvailable(bool)), this, SLOT(copyAvailable(bool)));
  connect(output, SIGNAL(selectionChanged()), this, SLOT(selected()));

  connect(QApplication::clipboard(), SIGNAL(selectionChanged()), this, SLOT(clipinfo()));

  comment = new QTextEdit;
  comment->setFixedHeight(80);
  comment->setReadOnly(true);
  comment->setFont(QFont(fontname, 12));
  setEmptyComment();
  
  QPalette p = palette();
  p.setColor(QPalette::Button,bg); // this is what our children use
  p.setColor(QPalette::Window,bg); // this is what our children use
  p.setColor(QPalette::ButtonText,fg);
  p.setColor(QPalette::WindowText,fg);
  setPalette(p);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(input);
  layout->addWidget(output);
  layout->addWidget(comment);
  this->setLayout(layout);

  connect(input, SIGNAL(textChanged(QString const &)),
	  SLOT(edited(QString const &)));
}

MainWindow::~MainWindow() {
}

void MainWindow::edited(QString const &s) {
  
  if (QRegExp("[\\s.]*").exactMatch(s)) {
    output->setFont(QFont(fontname, 15));
    output->setText("");
    setEmptyComment();
    return;
  }

  setMultiComment();

  QStringList lst = s.split(QRegExp("\\s+"));

  QSet<int> cc = uverse.find(lst);
  if (cc.isEmpty()) {
    output->setFont(QFont(fontname, 15));
    output->setText("(none)");
  } else {
    QString s = "";
    QList<int> ordered;
    ordered = cc.toList();
    qSort(ordered);
    //    if (ordered.size()>150)
    //      ordered.resize(150);
    foreach (int c, ordered) {
      s += " ";
      if (c>=65536) {
	s += QChar(QChar::highSurrogate(c));
	s += QChar(QChar::lowSurrogate(c));
      } else {
	s += QChar(c);
      }
    }
    output->setText(s.mid(1));
    if (cc.size()>80)
      output->setFont(QFont(fontname, 8));
    else if (cc.size()>40)
      output->setFont(QFont(fontname, 12));
    else if (cc.size()>30)
      output->setFont(QFont(fontname, 14));
    else if (cc.size()>20)
      output->setFont(QFont(fontname, 18));
    else if (cc.size()>10)
      output->setFont(QFont(fontname, 24));
    else
      output->setFont(QFont(fontname, 32));
    if (cc.size()==1) {
      QApplication::clipboard()->setText(s.mid(1));
      setComment(*cc.begin());
    }
  }
}

void MainWindow::copyAvailable(bool s) {
  copyAv = s;
}

void MainWindow::setEmptyComment() {
  comment->setHtml("<i>Type some words to select characters.</i>");
}

void MainWindow::setMultiComment() {
  comment->setHtml("<i>Select a single character to see description.</i>");
}

void MainWindow::selected() {
  if (output->toPlainText().isEmpty()) {
    setEmptyComment();
    return;
  }
  setMultiComment();
  if (!copyAv)
    return;
  output->copy();
  QClipboard *clip = QApplication::clipboard();
  QString t = clip->text();
  clip->setText(t); // this cleans it
  clip->setText(t, QClipboard::Selection); // this doesn't actually affect the selection for double clicks!. I don't know why.
  itsmine = t; // this is used to reselect after doubleclick. ugly!
  int chr = -1;
  
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
  if (chr>=0) 
    setComment(chr);
}

void MainWindow::clipinfo() {
  // Ugly hack to prevent "<!--StartFragment-->"
  QClipboard *clip = QApplication::clipboard();
  QString t = clip->text( QClipboard::Selection);
  if (t==itsmine) {
    itsmine = "";
    clip->setText(t, QClipboard::Selection);
  }
}
  
void MainWindow::setComment(int chr) {
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
