#include <memory>
#include <string>
#include <vector>

#include <QFile>
#include <QTextStream>

#include "engineinterface.h"

EngineInterface::EngineInterface(int gridSize, int wordsPerPuzzle, QObject *parent)
    : QObject(parent), m_gridSize(gridSize), m_wordsPerPuzzle(wordsPerPuzzle), m_scoreReserve(gridSize * gridSize) {

  QFile dictFile(":/datas/valid_words.txt");
  if (!dictFile.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Can't open dictionnary file");
  }
  QTextStream in(&dictFile);

  std::vector<std::string> wordList;

  while (!in.atEnd()) {
    QString line = in.readLine();
    wordList.push_back(line.toUtf8().constData());
  }

  dictFile.close();

  m_engine = std::make_unique<vowels::Engine>(gridSize, wordsPerPuzzle, wordList);
  m_numberWords = m_engine->getWordsToFindLength();

  resetGridModel();
  resetWildcardModel();
};

void EngineInterface::previousWord() {
  incrIndex(-1);
  resetWildcardModel();
}

void EngineInterface::nextWord() {
  incrIndex(1);
  resetWildcardModel();
}

void EngineInterface::addLetter(uint64_t index) {
  if (index >= 0 && index < m_engine->getGridSize() * m_engine->getGridSize()) {
    if (fillWildcard(index)) {
      auto item = m_gridModel.item(index);
      item->setData(QString("%0").arg(1), GridModel::selected);
    }
  }
}

void EngineInterface::cleanLetter() {
  for (uint64_t i = 0; i < m_engine->getGridSize() * m_engine->getGridSize(); ++i) {
    auto item = m_gridModel.item(i);
    item->setData(QString("%0").arg(0), GridModel::selected);
  }
  search();
  resetWildcardModel();
}