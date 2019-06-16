#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <QFile>
#include <QTextStream>

class OutputManager
{
public:
    OutputManager();
    ~OutputManager();

    /*!
       * Заносит сгенерированное оглавление в выходной файл
       *\param [in] GeneratedHtml - Байтовый массив
       *\param [in] OutputPathToHtml - Путь к выходному файлу
       *\return - возвращает true, если конвертация произошла успешно
       */
    bool writeHtml(QByteArray GeneratedHtml, const QString OutputPathToHtml);
};

#endif // OUTPUTMANAGER_H
