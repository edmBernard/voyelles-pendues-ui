#include <memory>
#include <string>
#include <vector>

#include <QFile>
#include <QTextStream>

#include "engineinterface.h"

EngineInterface::EngineInterface(int gridSize, int wordsPerPuzzle, QObject *parent)
  : QObject(parent), m_gridSize(gridSize), m_wordsPerPuzzle(wordsPerPuzzle), m_maxGainPerWord(gridSize * gridSize), m_scoreReserve(m_maxGainPerWord) {

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

void EngineInterface::generateNewPuzzle() {
  if (remainingWord() != 0) {
    incrScore(- m_maxGainPerWord * 10); // when the player manually reset
  }
  m_foundWords.clear();
  m_foundwordModel.clear();
  m_currentWordIndex = 0;
  m_engine->generateNewPuzzle();
  m_numberWords = m_engine->getWordsToFindLength();
  m_numberExtraWordFound = 0;
  m_playerScore = 0;

  resetGridModel();
  resetWildcardModel();
  emit updateMeta();
  emit updateScore(m_playerScore);
}

void EngineInterface::getHint() {
  if (m_playerScore < m_maxGainPerWord * 2) {
    return;
  }
  incrScore(- m_maxGainPerWord * 2); // Hint penalty
  const QString word = m_engine->getWord(m_currentWordIndex).word.data();
  auto it_word = word.begin();
  for (auto i = 0; i < m_wildcardModel.rowCount(); ++i, ++it_word) {
    QStandardItem *standardItem = m_wildcardModel.item(i);
    const QString isEmpty = standardItem->data(WildcardModel::empty).toString();
    const QString isHint = standardItem->data(WildcardModel::hint).toString();
    if (isEmpty == "1" && isHint == "0") {
      standardItem->setData(QString("1"), WildcardModel::hint);
      standardItem->setData(QString("%0").arg(*it_word), WildcardModel::letter);
      return;
    }
  }
}

void EngineInterface::search() {
  // Search was trigger when we release touch screen
  // create word from wildcard model
  QString word;
  for (int i = 0; i < m_wildcardModel.rowCount(); ++i) {
    auto standardItem = m_wildcardModel.item(i);
    if (standardItem->data(WildcardModel::empty) == "1") {
      // return without doing anything if incomplete word
      return;
    }
    word.append(standardItem->data(WildcardModel::letter).toString());
  }

  // search word in engine list
  auto result = m_engine->search(word.toUtf8().constData());

  switch (result) {

  case vowels::SearchReturnCode::kWordInList:
    addFoundWord(word, true);
    incrScore(m_scoreReserve);
    resetScoreReserve();
    if (m_currentWordIndex >= remainingWord()) {
      incrIndex(-1);
    }
    emit notify("Mot valide trouvé", "#e4ffe4");
    if (remainingWord() <= 0) {
      emit updateBestScore(m_playerScore);
      generateNewPuzzle();
      return;
    }
    resetGridModel(); // to update bloom filter
    break;

  case vowels::SearchReturnCode::kWordExist:
    if (addFoundWord(word, false)) {
      incrScore(m_maxGainPerWord);
    }
    emit notify("Presque", "#e3f9ff");
    break;

  default:
    emit notify("Essayes encore", "#ffaaaa");
    decrScoreReserve();
  }
}

void EngineInterface::resetGridModel() {
  const auto grid = m_engine->getGrid();
  const auto bloom = m_engine->getBloom();
  m_gridModel.clear();
  for (uint64_t i = 0; i < grid.size(); ++i) {
    QStandardItem *it = new QStandardItem();
    it->setData(QString("%0").arg(grid[i]), GridModel::letter);
    it->setData(QString("%0").arg(bloom[i]), GridModel::bloom);
    it->setData(QString("%0").arg(0), GridModel::selected);
    m_gridModel.appendRow(it);
  }
}

void EngineInterface::resetWildcardModel() {
  const std::string wildcard = m_engine->getWord(m_currentWordIndex).wildCard;
  const QString word(wildcard.data());
  m_wildcardModel.clear();
  m_pressedIndex.clear();
  for (auto it = word.begin(); it != word.end(); ++it) {
    QStandardItem *standardItem = new QStandardItem();
    standardItem->setData(QString("%0").arg(*it), WildcardModel::letter);
    standardItem->setData(QString("%0").arg(*it == '*'), WildcardModel::empty);
    standardItem->setData(QString("0"), WildcardModel::hint);
    m_wildcardModel.appendRow(standardItem);
  }

  emit updateMeta();
}

bool EngineInterface::fillWildcard(uint64_t index) {
  if (index >= m_engine->getGrid().size()) {
    return false;
  }
  if (m_pressedIndex.size() != 0) {
    // Check if index already pressed
    auto found = std::find(m_pressedIndex.begin(), m_pressedIndex.end(), index);
    if (found != m_pressedIndex.end()) {
      return false;
    }
  }
  m_pressedIndex.push_back(index);
  for (int i = 0; i < m_wildcardModel.rowCount(); ++i) {
    auto standardItem = m_wildcardModel.item(i);
    if (standardItem->data(WildcardModel::empty) == "1") {
      standardItem->setData("0", WildcardModel::empty);
      standardItem->setData(QString("%0").arg(m_engine->getGrid()[index]), WildcardModel::letter);
      return true;
    }
  }
  return false;
}

void EngineInterface::incrScore(int value) {
  m_playerScore += value;
  if (m_playerScore < 0) {
    m_playerScore = 0;
  }
  emit updateScore(m_playerScore);
}

void EngineInterface::incrIndex(int value) {
  m_currentWordIndex += value;
  if (m_currentWordIndex < 0) {
    m_currentWordIndex = remainingWord() - 1;
    return;
  }
  if (m_currentWordIndex >= remainingWord()) {
    m_currentWordIndex = 0;
    return;
  }
}
void EngineInterface::decrScoreReserve() {
  if (m_scoreReserve > 0) {
    --m_scoreReserve;
  }
}
void EngineInterface::resetScoreReserve() {
  m_scoreReserve = m_maxGainPerWord;
}

bool EngineInterface::addFoundWord(const QString &word, bool wasInList) {
  const std::string wordStr = word.toUtf8().constData();
  auto result = std::find(m_foundWords.begin(), m_foundWords.end(), wordStr);
  if (result != m_foundWords.end()) {
    return false;
  }
  m_foundWords.push_back(wordStr);
  if (!wasInList)
    m_numberExtraWordFound++;

  QStandardItem *it = new QStandardItem();
  it->setData(word, FoundWordModel::word);
  it->setData(wasInList, FoundWordModel::wasInList);
  m_foundwordModel.insertRow(0, it);
  return true;
}
