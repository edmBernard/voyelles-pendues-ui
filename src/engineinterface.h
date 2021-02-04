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

  //! Move internal index and update wildcard
  Q_INVOKABLE void previousWord();
  //! Move internal index and update wildcard
  Q_INVOKABLE void nextWord();

  Q_INVOKABLE void addLetter(uint64_t index);
  Q_INVOKABLE void cleanLetter();

  Q_INVOKABLE void generateNewPuzzle();

signals:
  void updateMeta();

  void updateScore(uint64_t score);

  void notify(QString message);

private:
  //! reset grid model with data from engine
  void resetGridModel();

  //! reset wildcard model with data from engine and ask to update meta
  void resetWildcardModel();

  //! Fill empty space of the wildcard with selected letter in the grid
  bool fillWildcard(uint64_t index);

  //! Search for the word filled in wildcard
  void search();

  void incrIndex(int value);

  bool addFoundWord(const QString &word);

  void incrScore(int value);
  void decrScoreReserve();
  void resetScoreReserve();

  int m_gridSize;       //!< Grid size
  int m_wordsPerPuzzle; //!< Maximum Number of word per Puzzle

  int64_t m_currentWordIndex = 0;       //!< Cursor of the current word in ui
  int64_t m_numberWords = 0;            //!< Total number of words to find
  std::vector<uint64_t> m_pressedIndex; //!< List of letter selected by player

  int64_t m_playerScore = 0; //!< Total player score
  uint8_t m_scoreReserve;    //!< Score reserve to add on next match

  std::unique_ptr<vowels::Engine> m_engine; //!< Processing engine for all logic

  GridModel m_gridModel;         //!< Data contained in the letter grid
  WildcardModel m_wildcardModel; //!< Data contained in the woldcard world

  FoundWordModel m_foundwordModel;       //!< List of found word for ui
  std::vector<std::string> m_foundWords; //!< List of found word for processing
};

#endif // ENGINEINTERFACE_H
