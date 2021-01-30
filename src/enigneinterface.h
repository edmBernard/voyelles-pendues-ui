#ifndef ENIGNEINTERFACE_H
#define ENIGNEINTERFACE_H

#pragma once

#include <QObject>

#include <memory>

#include "engine.h"

class EnigneInterface : public QObject
{
    Q_OBJECT
public:
    explicit EnigneInterface(QObject *parent = nullptr);

signals:

public slots:

private:
    std::unique_ptr<vowels::Engine> m_engine;
};

#endif // ENIGNEINTERFACE_H
