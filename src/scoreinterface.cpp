#include "scoreinterface.h"

#include <QFile>
#include <QTextStream>

#include <spdlog/spdlog.h>

#include <memory>
#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

ScoresInterface::ScoresInterface(const QString& saveFolder, const QString& name, QObject *parent)
  : QObject(parent) {
  fs::path folder = saveFolder.toStdString();
  if (!fs::exists(folder)) {
    spdlog::info("Save folder don't exist creating it : {}", folder.string());
    fs::create_directory(saveFolder.toStdString());
  }

  m_saveFilename = folder / (name.toStdString() + ".txt");

  if (!fs::exists(m_saveFilename)) {
    spdlog::info("No previous save found : {}", m_saveFilename.string());
    return;
  }

  QFile dictFile(QString::fromStdString(m_saveFilename.string()));
  if (!dictFile.open(QIODevice::ReadOnly)) {
    throw std::runtime_error("Can't open best score file : " + m_saveFilename.string());
  }

  spdlog::debug("Score filename {}", m_saveFilename.string());
  QTextStream in(&dictFile);

  while (!in.atEnd()) {
    QString line = in.readLine();
    QStringList splitted = line.split(',');
    int score = splitted[0].toInt();
    QDateTime date = QDateTime::fromString(splitted[1], Qt::RFC2822Date);
    m_bestScores.emplace_back(score, date);
  }

  dictFile.close();

  std::sort(m_bestScores.begin(), m_bestScores.end(), [](auto elem1, auto elem2){return std::get<0>(elem1) > std::get<0>(elem2); });

  for (const auto &[score, date] : m_bestScores) {
    QStandardItem *it = new QStandardItem();
    it->setData(score, BestScoreModel::score);
    it->setData(date, BestScoreModel::date);
    it->setData(15, BestScoreModel::gridType);
    m_bestScoreModel.appendRow(it);
  }
};

void ScoresInterface::resetBestScores() {
  m_bestScores.clear();
  m_bestScoreModel.clear();
  fs::remove(m_saveFilename);
}

void ScoresInterface::addBestScore(int score) {
  m_bestScores.emplace_back(score, QDateTime::currentDateTime());

  std::sort(m_bestScores.begin(), m_bestScores.end(), [](auto elem1, auto elem2){return std::get<0>(elem1) > std::get<0>(elem2); });

  if (m_bestScores.size() > 10)
    m_bestScores.resize(10);

  m_bestScoreModel.clear();

  QFile dictFile(QString::fromStdString(m_saveFilename.string()));
  if (!dictFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
    throw std::runtime_error("Can't open best score file : " + m_saveFilename.string());
  }

  QTextStream out(&dictFile);

  for (auto &[score, date] : m_bestScores) {
    spdlog::debug("Inserted date : {}", date.toString(Qt::RFC2822Date).toStdString());
    QStandardItem *it = new QStandardItem();
    it->setData(score, BestScoreModel::score);
    it->setData(date.toString(Qt::RFC2822Date), BestScoreModel::date);
    it->setData(15, BestScoreModel::gridType);
    m_bestScoreModel.appendRow(it);
    out << score << "," << date.toString(Qt::RFC2822Date) << "\n";
  }

  dictFile.close();
}
