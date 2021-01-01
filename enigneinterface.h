#ifndef ENIGNEINTERFACE_H
#define ENIGNEINTERFACE_H

#include <QObject>

class EnigneInterface : public QObject
{
    Q_OBJECT
public:
    explicit EnigneInterface(QObject *parent = nullptr);

signals:

};

#endif // ENIGNEINTERFACE_H
