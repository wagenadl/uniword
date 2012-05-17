// MainWindow.C

#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QStringList>
#include <QRegExp>
#include <QDebug>
#include <QApplication>
#include <QClipboard>

#include "MainWindow.H"
#include "Universe.H"

#define FONT "Linux Libertine"
#define fg QColor("#000000")
#define bg QColor("#eeeeee")

MainWindow::MainWindow(Universe const &uverse, QWidget *parent):
  QWidget(parent),
  uverse(uverse) {
  resize(300,280);

  input = new QLineEdit(this);
  input->setGeometry(10,10,280,50);
  input->setFont(QFont(FONT, 12));

  output = new QTextEdit(this);
  // output->setWordWrap(true);
  //  output->setTextInteractionFlags(Qt::TextSelectableByMouse);
  output->setReadOnly(true);
  output->setGeometry(10,80,280,110);
  output->setFont(QFont(FONT, 15));
  output->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  output->setText("");
  copyAv = false;
  connect(output, SIGNAL(copyAvailable(bool)), this, SLOT(copyAvailable(bool)));
  connect(output, SIGNAL(selectionChanged()), this, SLOT(selected()));

  comment = new QTextEdit(this);
  comment->setReadOnly(true);
  comment->setGeometry(10,210,280,60);
  comment->setFont(QFont(FONT, 12));
  comment->setText("(Type some words to select characters)");
  
  QPalette p = palette();
  p.setColor(QPalette::Button,bg); // this is what our children use
  p.setColor(QPalette::Window,bg); // this is what our children use
  p.setColor(QPalette::ButtonText,fg);
  p.setColor(QPalette::WindowText,fg);
  setPalette(p);

  connect(input, SIGNAL(textChanged(QString const &)),
	  SLOT(edited(QString const &)));
}

MainWindow::~MainWindow() {
}

void MainWindow::edited(QString const &s) {
  
  if (QRegExp("[\\s.]*").exactMatch(s)) {
    output->setFont(QFont(FONT, 15));
    output->setText("");
    comment->setText("(Type some words to select characters)");
    return;
  }
  comment->setText("(Select a single character to see description)");

  QStringList lst = s.split(QRegExp("\\s+"));

  QSet<int> cc = uverse.find(lst);
  if (cc.isEmpty()) {
    output->setFont(QFont(FONT, 15));
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
      output->setFont(QFont(FONT, 8));
    else if (cc.size()>40)
      output->setFont(QFont(FONT, 12));
    else if (cc.size()>30)
      output->setFont(QFont(FONT, 14));
    else if (cc.size()>20)
      output->setFont(QFont(FONT, 18));
    else if (cc.size()>10)
      output->setFont(QFont(FONT, 24));
    else
      output->setFont(QFont(FONT, 32));
    if (cc.size()==1) {
      QApplication::clipboard()->setText(s.mid(1));
      comment->setText(uverse.describe(*cc.begin()) + " (0x"+QString::number(*cc.begin(), 16) + ")");
    }
  }
}

void MainWindow::copyAvailable(bool s) {
  copyAv = s;
}

void MainWindow::selected() {
  comment->setText("(Select a single character to see description)");
  if (!copyAv)
    return;
  output->copy();
  QClipboard *clip = QApplication::clipboard();
  QString t = clip->text();
  clip->setText(t); // does this clean it?

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
    comment->setText(uverse.describe(chr) + " (0x" + QString::number(chr, 16) + ")");

}


  
