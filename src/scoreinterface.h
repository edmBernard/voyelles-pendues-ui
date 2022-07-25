#pragma once

#include "models.h"

#include <QObject>
#include <QStandardItemModel>
#include <QDateTime>

#include <iostream>
#include <memory>
#include <tuple>
#include <filesystem>

namespace fs = std::filesystem;

class ScoresInterface : public QObject {
  Q_OBJECT
public:
  explicit ScoresInterface(const QString& saveFolder, const QString& name, QObject *parent = nullptr);

  Q_INVOKABLE BestScoreModel *getBestScores() { return &m_bestScoreModel; }
  Q_INVOKABLE void resetBestScores();
  Q_INVOKABLE void addBestScore(uint64_t score);

private:
  std::vector<std::tuple<uint64_t, QDateTime>> m_bestScores;
  BestScoreModel m_bestScoreModel;
  fs::path m_saveFilename;
};
