#ifndef OUTPUTMANAGER_H
#define OUTPUTMANAGER_H

#include <QFile>
#include <QTextStream>
#include <QProcess>

class OutputManager
{
public:
    OutputManager();
    ~OutputManager();

    QFile ResultFile; ///< Результирующий файл

    /*!
       * Производит конвертацию xml-разметки в html-разметку средставами xmllint
       * Создает файл, содержащий предупреждения и ошибки в html-разметке, в папке с исполняемым файлом
       *\param [in] OutputPathToXml - Путь к файлу/URL, где содержится xml-разметка
       *\param [in] OutputPathToHtml - Путь к файлу/URL, где содержится html-разметка
       *\return - возвращает true, если конвертация произошла успешно
       */
    bool xmlToHtml(const QString OutputPathToXml, const QString OutputPathToHtml);
};

#endif // OUTPUTMANAGER_H
