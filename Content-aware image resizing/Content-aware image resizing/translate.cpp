#include "translate.h"

#include <qapplication.h>
#include <qdebug.h>
#include <qtranslator.h>

constexpr const char* LANGUAGE_EN_US_PATH = "./lang/en_US.qm";
constexpr const char* LANGUAGE_ZH_CN_PATH = "./lang/zh_CN.qm";

static Language CurrentLanguage = LANG_EN_US;

static QString languageFile(Language lang);

Language currentLanguage()
{
    return CurrentLanguage;
}

void setLanguage(Language lang)
{
    CurrentLanguage = lang;

    static QTranslator* qtr = new QTranslator();
    QString langFile = languageFile(lang);
    if (qtr->load(langFile))
    {
        if (!qApp->installTranslator(qtr))
        {
            qDebug() << "Failed to install translator:" << langFile;
            return;
        }
    }
    else
    {
        qDebug() << "Failed to load language file:" << langFile;
        return;
    }
}

QString languageFile(Language lang)
{
    switch (lang)
    {
        case LANG_EN_US:
            return LANGUAGE_EN_US_PATH;
        case LANG_ZH_CN:
            return LANGUAGE_ZH_CN_PATH;
        default:
            return "";
    }
}
