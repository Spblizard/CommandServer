#include "jsonconf.h"

JsonConf::JsonConf(QObject *parent) : QObject(parent), mPath("NULL"), mConfigName("config.json")
{

}

void JsonConf::setPath(const QString &path)
{
    mPath = path;
}

void JsonConf::setNameConfig(const QString &name)
{
    mConfigName = name;
}

QVariant JsonConf::getParametr(const QString &key)
{
    if (mJsonObj.isEmpty())
        return 0;
    else {
        if (mJsonObj.contains(key)) 
            return mJsonObj[key].toVariant();
        else 
            return 0;
    }
}

void JsonConf::openConfig()
{
     QDir dir;
     if (mPath == "NULL") {
        mPath = QCoreApplication::applicationDirPath() + "/";
        dir.setPath(mPath);
     } else {
        dir.setPath(mPath);
        if (mPath.right(1) != "/")
            mPath += "/";
     }
     QStringList files = dir.entryList();
     for (int i = 0; i < files.length(); i++) {
         if (files[i] == mConfigName)
             mPath += mConfigName;
     }
     QFile file(mPath);
     file.open(QIODevice::ReadOnly);

     QJsonDocument jsonDoc = QJsonDocument::fromJson(file.readAll());
     file.close();

     mJsonObj = jsonDoc.object();
}
