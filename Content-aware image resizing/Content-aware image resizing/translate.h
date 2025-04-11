#pragma once

#include <qstring.h>

enum Language
{
    LANG_EN_US = 0,
    LANG_ZH_CN = 1
};

Language currentLanguage();

void setLanguage(Language lang);
