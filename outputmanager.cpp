#include "outputmanager.h"

OutputManager::OutputManager()
{
}


OutputManager::~OutputManager()
{
}

bool OutputManager::writeHtml(QByteArray GeneratedHtml, QString OutputPathToHtml) {
    //Записать сгенерированное оглавление в выходной файл
    QFile xml(OutputPathToHtml);

    if (!xml.open(QIODevice::WriteOnly))
        throw QString("Unable to write data to result file");

    xml.write(GeneratedHtml);
    xml.close();

    return true;
}
