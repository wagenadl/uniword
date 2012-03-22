// Universe.C

#include "Universe.H"

#include <QDebug>

Universe::Universe() {}
Universe::~Universe() {}

void Universe::add(int c, QString desc) {
  QStringList bits = desc.split(QRegExp("[ ,;]+"));
  foreach (QString s, bits) {
    words[c].insert(s.toLower());
  }
}

void Universe::addperhaps(int c, QString desc) {
  if (!words.contains(c))
    return;

  QStringList bits = desc.split(QRegExp("[ ,;]+"));
  foreach (QString s, bits) {
    words[c].insert("." + s.toLower());
  }
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

QSet<int> Universe::refine(QSet<int> const &uverse,
			     QString selector) const {
  if (selector == "")
    return uverse;
  
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

  if (res.size()<0) {
    qDebug() << "Results:";
    foreach (int const &c, res) {
      qDebug() << "  " << c << ": " << QString::number(c,16);
    }
  }
  
  return res;
}
