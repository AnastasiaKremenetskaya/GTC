/*!
\file
\brief Заголовочный файл с описанием абстрактного класса для получения входных данных
*/

#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <QString>
#include <QtXml/QDomDocument>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QEventLoop>
#include <QProcess>
#include <QFile>
#include <QTextCodec>

/*!
    \brief Родительский класс, отвечающий за получение данных

    Данный класс имеет двух наследников, отвечающих либо за загрузку данных из файла,
    либо по URL. Основная задача данного класса - получить и представить исходный
    html-текст в виде фрейма
*/

class InputManager
{
public:
    InputManager(void);
    virtual ~InputManager(void);

    QByteArray InputHtmlAsBa; //!> Байтовый массив с входной html-разметкой
    QString InputHtmlAsString; //!> Строка с входной html-разметкой

    /*!
        * Получает html-текст и представляет его в виде фрейма
        *\param [in] InputPathToHtml - Путь к файлу/URL, где содержится html-текст
        *\param [in] PathToOutputFile - Путь к файлу, куда необходимо сохранить результат
        */
    virtual void getData(QString InputPathToHtml, QString PathToOutputFile) = 0; ///< pure virtual function

    /*!
       * Производит конвертацию html-разметки в xml-разметку средставами xmllint
       * Создает файл, содержащий предупреждения и ошибки в html-разметке, в папке с исполняемым файлом
       *\param [in] InputPathToHtml - Путь к файлу/URL, где содержится html-разметка
       *\param [in] OutputPathToXml - Путь к файлу/URL, где содержится xml-разметка
       *\return - возвращает true, если конвертация произошла успешно
       */
    static bool htmlToXml(const QString InputPathToHtml, const QString OutputPathToXml);
};

#endif // INPUTMANAGER_H
