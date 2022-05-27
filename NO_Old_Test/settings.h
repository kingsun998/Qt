#ifndef SETTINGS_H
#define SETTINGS_H

#include <qstring.h>
#include <qobject.h>
#include <qsettings.h>
#include <qlist.h>
#include <QAction>
extern class settings settings;

class settings
{
public:
    settings();
    QSettings *configuration;
    void initConfiguration();
    void set(QString qstrNodeName,QString qstrKeyName,QVariant qvarvalue);
    QVariant get(QString qstrNodeName,QString qstrKeyName);

};

#endif // SETTINGS_H
