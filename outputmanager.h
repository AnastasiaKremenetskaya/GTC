/*!
\file
\brief Заголовочный файл с описанием класса для генерации записи выходных данных
*/

#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <QFile>
#include <QTextStream>

/*!
    \brief Класс, отвечающий за запись выходных данных в файл

    Данный класс записывает сгенерированную html-разметку
    в выходной файл, указанный пользователем
*/

class OutputManager
{
public:
    OutputManager();
    ~OutputManager();

    /*!
       * Заносит сгенерированное оглавление в выходной файл
       *\param [in] GeneratedHtml - Байтовый массив
       *\param [in] OutputPathToHtml - Путь к выходному файлу
       *\return - возвращает true, если запись произошла успешно
       */
    static bool writeHtml(QByteArray GeneratedHtml, const QString OutputPathToHtml);
};

#endif // OUTPUTMANAGER_H
