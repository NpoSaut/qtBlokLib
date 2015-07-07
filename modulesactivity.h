#ifndef MODULESACTIVITY_H
#define MODULESACTIVITY_H

#include <QByteArray>
#include <QString>

class ModulesActivity
{
public:
    ModulesActivity ()
        : ipd (false), monitor(false), mpAls(false), electronicMap(false), navigation (false),
          breaksBrain (false), radio (false), tskbm (false), controlSystem (false), outputModule (false),
          collected (false)
    {}

//    ModulesActivity (const ModulesActivity& ma);
//    ModulesActivity &operator =(const ModulesActivity& ma);

    bool ipd;
    bool monitor;
    bool mpAls;
    bool electronicMap;
    bool navigation;
    bool breaksBrain;
    bool radio;
    bool tskbm;
    bool controlSystem;
    bool outputModule;
    bool saut;

    bool isCollected () const { return collected; }
    QString toString () const;

    static ModulesActivity loadFromMcoState (const QByteArray& mcoStateMessage);

    bool operator== (const ModulesActivity& b);
    bool operator!= (const ModulesActivity& b) { return !(*this == b);}

private:
    bool collected;

};

#endif // MODULESACTIVITY_H
