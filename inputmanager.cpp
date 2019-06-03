#include "inputmanager.h"

InputManager::InputManager()
{
}

InputManager::~InputManager(void)
{
}

bool InputManager::htmlToXml(const QString InputPathToHtml, const QString OutputPathToXml) {
    QStringList params; // параметры для запуска xmllint

    params << "-html" << "-xmlout" << InputPathToHtml << "-output" << OutputPathToXml;

    if(QProcess::execute("xmllint", params) != QProcess::NormalExit) {
        throw QString("Unable to create process xmllint. Termination");
    }

    return true;
}
