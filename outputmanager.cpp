#include "outputmanager.h"

OutputManager::OutputManager()
{
}


OutputManager::~OutputManager()
{
}

bool OutputManager::xmlToHtml(const QString OutputPathToXml, const QString OutputPathToHtml) {
    QStringList params; // параметры для запуска xmllint

    params << "-html" << "-htmlout" << OutputPathToXml << "-output" << OutputPathToHtml;

    if(QProcess::execute("xmllint", params) != QProcess::NormalExit) {
        throw QString("Unable to create xmllint process. Termination");
    }

    return true;
}
