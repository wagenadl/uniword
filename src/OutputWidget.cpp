// OutputWidget.cpp

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

#include <QDebug>
#include "OutputWidget.h"
#include <QApplication>
#include <QTextDocument>
#include <QTextCursor>
#include <QMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include "Universe.h"

OutputWidget::OutputWidget(Universe const &uverse, QWidget *parent):
  QGraphicsView(parent), uverse(uverse) {
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
  scene = new QGraphicsScene(QRectF(0, 0, 10, 10));
  setAlignment(Qt::AlignLeft | Qt::AlignTop);
  columns = 8;
  minrows = 3;
  setScene(scene);
  setFont(QFont());
  marker = new QGraphicsRectItem();
  marker->setBrush(QColor(220, 220, 220));
  marker->setPen(QPen(Qt::NoPen));
  scene->addItem(marker);
  marker->hide();
  //  resize((columns+0.5)*tilesize.width(), 3.5*tilesize.height());
  relayout();
}

OutputWidget::~OutputWidget() {
  for (auto tile: tiles)
    delete tile;
  delete scene;
}

void OutputWidget::resizeEvent(QResizeEvent *e) {
  QGraphicsView::resizeEvent(e);
  columns = int(viewport()->size().width()/(tilesize.width() + .01) + 0.3);
  if (columns < 4)
    columns = 4;
  relayout();
  emit resized();
}

void OutputWidget::clear() {
   setTiles(QList<int>());
}

void OutputWidget::setTiles(QList<int> characters, bool ellipsis) {
  minrows = 1;
  marker->hide();
 
  int length = characters.size() + (ellipsis ? 1 : 0);
  while (tiles.size() > length)
    delete tiles.takeLast();
  while (tiles.size() < length) {
    QGraphicsTextItem *tile = new QGraphicsTextItem();
    tile->setFont(font_);
    scene->addItem(tile);
    tiles.append(tile);
  }
  for (int k=0; k<characters.size(); k++) 
    setTileText(k, characters[k]);
  if (ellipsis)
    tiles[length-1]->setHtml("(…)");
  relayout();
  emit selected(QList<int>());
  emit hovered(-1);
}

void OutputWidget::setTileText(int index, int character) {
  if (uverse.isSpace(character)) {
    tiles[index]->setHtml("<span style=""background-color:#eeeeee;"">x</span>");

    QTextDocument *doc = tiles[index]->document();
    QTextCursor cursor(doc);
    cursor.insertText(QString(QChar(character)));
    cursor.deleteChar();
    return;
  }
  
  QString html = "";
  if (uverse.isCombiner(character) || uverse.isModifier(character)) {
    html += "<span style=""color:#aaaaaa;"">";
    html += QChar(0x25fd);
    html += "</span>";
  }
  if (character>=65536) {
    html += QChar(QChar::highSurrogate(character));
    html += QChar(QChar::lowSurrogate(character));
  } else {
    html += QChar(character);
  }
  tiles[index]->setHtml(html);
}
  

void OutputWidget::relayout() {
  for (int k=0; k<tiles.size(); k++) {
    int x = k % columns;
    int y = k / columns;
    x *= tilesize.width();
    y *= tilesize.height();
    x += tilesize.width() / 3 - tiles[k]->boundingRect().width() / 2;
    tiles[k]->setPos(x, y);
  }
  int rows = (tiles.size() + columns - 1) / columns;
  if (rows < minrows)
    rows = minrows;
  setSceneRect(QRectF(0, 0, (columns+0.2)*tilesize.width(),
                      (rows+0.2)*tilesize.height()));
  setTransform(QTransform());
}

void OutputWidget::setFont(QFont f) {
  QFontMetrics fm(f);
  QSize s = fm.boundingRect("[W]").size();
  tilesize = QSize(s.width(), 10*s.height()/9);
  font_ = f;
  for (auto tile: tiles)
    tile->setFont(f);
  relayout();
}

int OutputWidget::indexAt(QPoint p) {
  QGraphicsItem *item = scene->itemAt(mapToScene(p), QTransform());
  if (item)
    for (int k=0; k<tiles.size(); k++)
      if (tiles[k]==item)
        return k;
  return -1;
}

void OutputWidget::mousePressEvent(QMouseEvent *e) {
  QGraphicsView::mousePressEvent(e);
  pressidx = indexAt(e->pos());
  if (pressidx >= 0)
    emit selected(QList<int>{pressidx});
  e->accept();
}

void OutputWidget::mouseReleaseEvent(QMouseEvent *e) {
  QGraphicsView::mouseReleaseEvent(e);
  int relidx = indexAt(e->pos());
  if (relidx >=0 && pressidx >=0 && relidx != pressidx) {
    QList<int> indices;
    if (relidx > pressidx) 
      for (int k=pressidx; k<=relidx; k++)
        indices << k;
    else
      for (int k=relidx; k<=pressidx; k++)
        indices << k;
    emit selected(indices);
  }
  e->accept();
}

void OutputWidget::mouseMoveEvent(QMouseEvent *e) {
  int idx = indexAt(e->pos());
  if (idx < 0) {
    marker->hide();
  } else {
    marker->setRect(QRectF(QPointF((idx%columns)*tilesize.width(),
                                   (idx/columns)*tilesize.height()),
                           QSizeF(tilesize.width()*2./3,
                                  tilesize.height())));
    marker->show();
  }
  emit hovered(idx); // even -1
}

void OutputWidget::mouseDoubleClickEvent(QMouseEvent *) {
  qDebug() << "double click";
  emit quit();
}

int OutputWidget::fittableGlyphs() const {
  int rows = int(viewport()->height() / (tilesize.height()+.01) - 0.2);
  if (rows < minrows)
    rows = minrows;
  return rows * columns;
}
