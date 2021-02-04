#include <vector>
#include <string>
#include <memory>

#include <QFile>
#include <QTextStream>

#include "engineinterface.h"

EngineInterface::EngineInterface(int gridSize, int wordsPerPuzzle, QObject *parent)
  : QObject(parent)
  , m_gridSize(gridSize)
  , m_wordsPerPuzzle(wordsPerPuzzle)
{
    QFile dictFile(":/datas/valid_words.txt");
    if(!dictFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Can't open dictionnary file");
    }
    QTextStream in(&dictFile);

    std::vector<std::string> wordList;

    while(!in.atEnd()) {
        QString line = in.readLine();
        wordList.push_back(line.toUtf8().constData());
    }

    dictFile.close();

    m_engine = std::make_unique<vowels::Engine>(gridSize, wordsPerPuzzle, wordList);
    m_numberWords = m_engine->getWordsToFindLength();

    resetGridModel();
    resetWildcardModel();
};
