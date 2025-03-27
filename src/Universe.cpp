// Universe.C

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

#include "Universe.h"

#include <QDebug>

Universe::Universe() {}
Universe::~Universe() {}

void Universe::add(int c, QString desc) {
  QStringList bits = desc.split(QRegExp("[ ,;]+"));
  foreach (QString s, bits) {
    words[c].insert(s.toLower());
    if (s=="COMBINING")
      combiners.insert(c);
    else if (s=="MODIFIER")
      modifiers.insert(c);
    else if (s=="SPACE")
      spaces.insert(c);
  }
  addDescription(c, desc);
}

void Universe::addgroup(int c, QString desc) {
  if (!words.contains(c))
    return;

  QStringList bits = desc.split(QRegExp("[ ,;]+"));
  foreach (QString s, bits) {
    words[c].insert("." + s.toLower());
  }
  addGroup(c, desc);
}

void Universe::addGroup(int c, QString desc) {
  if (groups.contains(c)) 
    if (groups[c].contains(desc))
      return;
  groups[c].append(desc);
}

void Universe::addDescription(int c, QString desc) {
  if (descs.contains(c))
    return;
  
  QStringList bits = desc.split(QRegExp(" "));
  QString d;
  QLocale l;
  bool atfirst = true;
  foreach (QString b, bits) {
    if (b.isEmpty())
      continue;
    d += " ";
    if (b.length()==1)
      d += l.toUpper(b);
    else if (atfirst) 
      d += l.toUpper(b.left(1)) + l.toLower(b.mid(1));
    else
      d += l.toLower(b);
    atfirst = false;
  }
  descs[c] = d.mid(1);
}

QString Universe::describe(int c) const {
  return descs.contains(c) ? descs[c] : "";
}

QString Universe::getgroups(int c) const {
  return groups.contains(c) ? groups[c].join("; ") : "";
}

QSet<int> Universe::all() const {
  QSet<int> res;
  foreach (int c, words.keys())
    res.insert(c);
  return res;
}

QSet<int> Universe::find(QString const &selector) const {
  QSet<int> res = all();
  if (selector != "")
    res = refine(res, selector);
  return res;
}

QSet<int> Universe::exclude(QSet<int> const &uverse,
			     QString selector) const {
  if (selector == "")
    return uverse;
  
  selector = selector.toLower();
  QSet<int> res;

  // First, let's exclude all partial matches
  foreach (int c, uverse) {
    QSet<QString> const &ww = words[c];
    bool ok = true;
    foreach (QString const &w, ww) {
      if (w.startsWith(selector)) {
	ok = false;
	break;
      }
    }
    if (ok)
      res.insert(c);      
  }

  if (res.isEmpty()) {
    // If that makes the result nil, exclude only exact matches
    foreach (int c, uverse) {
      QSet<QString> const &ww = words[c];
      if (!ww.contains(selector))
	res.insert(c);
    }
  }
  
  return res;
}
 

QSet<int> Universe::refine(QSet<int> const &uverse,
			     QString selector) const {
  if (selector == "")
    return uverse;
  
  if (selector.startsWith("!"))
    return exclude(uverse, selector.mid(1));
  
  selector = selector.toLower();
  QSet<int> res;

  // First, let's find exact matches
  foreach (int c, uverse) {
    QSet<QString> const &ww = words[c];
    if (ww.contains(selector))
      res.insert(c);
  }
  
  if (res.isEmpty()) {
    // If no exact matches, allow starting matches
    foreach (int c, uverse) {
      QSet<QString> const &ww = words[c];
      foreach (QString const &w, ww) {
	if (w.startsWith(selector)) {
	  res.insert(c);
	  break;
	}
      }
    }
  }
  
  return res;
}

QSet<int> Universe::find(QStringList const &selectors) const {
  QSet<int> res = all();
  foreach (QString const &s, selectors)
    res = refine(res, s);
  return res;
}

bool Universe::isCombiner(int c) const {
  return combiners.contains(c);
}

bool Universe::isModifier(int c) const {
  return modifiers.contains(c);
}

bool Universe::isSpace(int c) const {
  return spaces.contains(c);
}
