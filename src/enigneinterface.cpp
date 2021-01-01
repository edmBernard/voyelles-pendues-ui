#include "enigneinterface.h"

EnigneInterface::EnigneInterface(QObject *parent) : QObject(parent)
{
    m_engine = std::make_unique<Engine>(std::vector<std::string>{"azerty", "toto"});
}
