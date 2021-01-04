#include <vector>
#include <string>
#include <memory>

#include <QFile>
#include <QTextStream>

#include "enigneinterface.h"

EnigneInterface::EnigneInterface(QObject *parent) : QObject(parent)
{

    QFile dictFile(":/data/valid_word.txt");
    if(!dictFile.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Can't open dictionnary file");
    }
    QTextStream in(&dictFile);

    std::vector<std::string> wordList;
    int count = 0;
    while(!in.atEnd()) {
        QString line = in.readLine();
        wordList.push_back(line.toUtf8().constData());
        if (count++ == 100) {
            break;
        }
    }

    dictFile.close();

    m_engine = std::make_unique<Engine>(std::move(wordList));
};
