#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#pragma once

#include <QObject>
#include <QStandardItemModel>

#include <iostream>
#include <memory>

#include "engine.h"
#include "models.h"

constexpr int kScoreOnInvalid = 0;
constexpr int kScoreOnReset = -50;

class EngineInterface : public QObject {
  Q_OBJECT
public:
  explicit EngineInterface(int gridSize, int wordsPerPuzzle, QObject *parent = nullptr);

  Q_INVOKABLE GridModel *getGrid() { return &m_gridModel; }
  Q_INVOKABLE WildcardModel *getWord() { return &m_wildcardModel; }
  Q_INVOKABLE FoundWordModel *getFoundWords() { return &m_foundwordModel; }
  Q_INVOKABLE int getGridSize() { return m_gridSize; }
  Q_INVOKABLE uint64_t getFound() { return m_foundWords.size(); }
  Q_INVOKABLE int64_t getTotal() { return m_numberWords; }

  Q_INVOKABLE void previousWord();
  Q_INVOKABLE void nextWord();

  Q_INVOKABLE void addLetter(uint64_t index);
  Q_INVOKABLE void cleanLetter();

  Q_INVOKABLE void generateNewPuzzle();

  void search();

  Q_INVOKABLE void resetGridModel();

  Q_INVOKABLE void resetWildcardModel();

  Q_INVOKABLE bool fillWildcard(uint64_t index);

signals:
  void updateMeta();

  void updateScore(uint64_t score);

  void notify(QString message);

private:
  void incrScore(int value);

  void incrIndex(int value);
  void decrScoreReserve();
  void resetScoreReserve();

  bool addFoundWord(const QString &word);

  int m_gridSize;
  int m_wordsPerPuzzle;

  int64_t m_currentWordIndex = 0;
  std::vector<std::string> m_foundWords;
  int64_t m_numberWords = 0;
  std::vector<uint64_t> m_pressedIndex;

  std::unique_ptr<vowels::Engine> m_engine;
  int64_t m_playerScore = 0;
  uint8_t m_scoreReserve;
  GridModel m_gridModel;
  WildcardModel m_wildcardModel;
  FoundWordModel m_foundwordModel;
};

#endif // ENGINEINTERFACE_H
