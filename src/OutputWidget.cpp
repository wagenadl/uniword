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

#include "OutputWidget.h"
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
  setScene(scene);
  tilesize = QSize(32, 32);
  marker = new QGraphicsRectItem();
  marker->setBrush(QColor(220, 220, 220));
  marker->setPen(QPen(Qt::NoPen));
  scene->addItem(marker);
  marker->hide();
  relayout();
}

OutputWidget::~OutputWidget() {
  for (auto tile: tiles)
    delete tile;
  delete scene;
}

void OutputWidget::resizeEvent(QResizeEvent *e) {
  QGraphicsView::resizeEvent(e);
  relayout();
}

void OutputWidget::setTiles(QList<int> characters, bool ellipses) {
    marker->hide();
 
  int length = characters.size() + (ellipses ? 1 : 0);
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
  if (ellipses)
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
  int columns = 8;
  for (int k=0; k<tiles.size(); k++) {
    int x = k % columns;
    int y = k / columns;
    x *= tilesize.width();
    y *= tilesize.height();
    x += tilesize.width() / 3 - tiles[k]->boundingRect().width() / 2;
    tiles[k]->setPos(x, y);
  }
  int rows = (tiles.size() + columns - 1) / columns;
  if (rows < 3)
    rows = 3;
  setSceneRect(QRectF(0, 0, columns*tilesize.width(),
                      rows*tilesize.height()));
}

void OutputWidget::setFont(QFont f) {
  font_ = f;
  for (auto tile: tiles)
    tile->setFont(f);
  QFontMetrics fm(f);
  QSize s = fm.boundingRect("[W]").size();
  tilesize = s + QSize(4, 8);
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
  pressidx = indexAt(e->pos());
  if (pressidx >= 0)
    emit selected(QList<int>{pressidx});
}

void OutputWidget::mouseReleaseEvent(QMouseEvent *e) {
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
}

void OutputWidget::mouseMoveEvent(QMouseEvent *e) {
  int idx = indexAt(e->pos());
  if (idx < 0) {
    marker->hide();
  } else {
    marker->setRect(QRectF(QPointF((idx%8)*tilesize.width(),
                                   (idx/8)*tilesize.height()),
                           QSizeF(tilesize.width()*2/3,
                                  tilesize.height())));
    marker->show();
  }
  emit hovered(idx); // even -1
}
