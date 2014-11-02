#include <QCoreApplication>
#include <QtCore>
#include <QDebug>

void parser(const QByteArray &text) {
    QStringList systemWords;
    systemWords << "mainpassword" << "login";
    QStringList localWords;
    localWords << "password" << "login" << "email";
    QList<QByteArray> s = text.split('\n');
    bool systemFlag = true;
    bool systemCounter = false;
    bool localFlag = false;
    bool localCounter = false;
    QMap<QString, QString> localMap;
    QMap<QString, QString> systemMap;
    QList< QMap<QString, QString> > systemList;
    QList< QMap<QString, QString> > sitesList;
    int i = 0;
    while(i < s.size() - 1) {
        QList<QByteArray> l;
        l.append(s[i].split(':'));
        QString key = QString(l.at(0));
        QString value = QString(l.at(1));
        if(systemFlag) {
            if(!systemCounter) {
                QString local = "";
                foreach(local, systemWords) {
                    if(local == key && key != QString("system")) systemMap[key] = value;
                }
                if(value == QString("end")) systemCounter = true;
            }
            if( !systemMap.isEmpty() && systemCounter) {
                systemList.append(systemMap);
                systemMap.clear();
                systemCounter = false;
            }
        }
        if (localFlag) {
            if(!localCounter) {
                QString local = "";
                if(value == QString("start")) localMap["site"] = key;
                foreach(local, localWords) {
                    if(local == key && value != QString("end") && value!=QString("start")) {
                        localMap[key] = value;
                    }
                }
                if(value == QString("end")) localCounter = true;
            }
            if(!localMap.isEmpty() && localCounter) {
                sitesList.append(localMap);
                localMap.clear();
                localCounter = false;
            }
        }
        if(value == QString("end") && key == QString("system")) {
            systemFlag = false;
            localFlag = true;
        }
        i++;
    }
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QFile parseFile(qApp->applicationDirPath() + "/parser.txt");
    if (!parseFile.open(QIODevice::ReadWrite)) {
        qDebug() << "File can't open";
        return a.exec();
    }
    QByteArray parseData = parseFile.readAll();
    parseFile.close();
    parser(parseData);
    return a.exec();
}
