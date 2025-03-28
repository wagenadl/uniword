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

#include <QGraphicsView>
#include <QList>

class OutputWidget: public QGraphicsView {
  Q_OBJECT;
public:
  OutputWidget(class Universe const &uverse, QWidget *parent=0);
  virtual ~OutputWidget();
  void resizeEvent(QResizeEvent *) override;
  QFont const &font() const { return font_; }
  void mousePressEvent(QMouseEvent *) override;
  void mouseReleaseEvent(QMouseEvent *) override;
  void mouseMoveEvent(QMouseEvent *) override;
public slots:
  void clear() { setTiles(QList<int>()); }
  void setTiles(QList<int>, bool ellipses=false);
  void setFont(QFont);
signals:
  void hovered(int index);
  void selected(QList<int> indices);
private:
  void setTileText(int index, int character);
  void relayout();
  int indexAt(QPoint);
private:
  class QGraphicsScene *scene;
  QList<class QGraphicsTextItem *> tiles;
  class QGraphicsRectItem *marker;
  QSize tilesize;
  Universe const &uverse;
  QFont font_;
  int pressidx;
};


#endif
