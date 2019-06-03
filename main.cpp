#include <QCoreApplication>
#include <iostream>
#include "fileinputmanager.h"
#include "webinputmanager.h"
#include "tableofcontents.h"
#include "outputmanager.h"

#define qprint qDebug().nospace().noquote()

/*!
\mainpage
\brief Программа для автоматической генерации оглавления в виде якорных ссылок
*\author Кременецкая А.Д.

Программа выполняет следующие функции:
<ol>
<li>ищет в коде заголовочные теги;</li>
<li>на основании тегов организует вложенность;</li>
<li>сообщает об ошибке в случае невалидной html-разметки;</li>
</ol>
<p>Программа принимает на вход URL/путь к файлу, содержащему html-код</p>
<p>Программа возвращает html-разметку со сгенерированным оглавлением с соблюдением вложенности</p>
*/


/*!Главная функция проекта
*\param [in] argc - количество переданных параметров
*\param [in] argv - переданные параметры(пути к файлам)
*\return - 0 при завершении работы программы

*\brief Главная функция проекта
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //temporary
    QString input("file://mac/Home/Desktop/example.html");
    QString output("file://mac/Home/Desktop/result.html");
    //temporary

    QString OutputHtml; ///> Строка со сгенерированным html-кодом

    QDomDocument DomTree; ///> Дом-дерево html-разметки

    QRegExp UrlRegexp;//("(http(s)?:\/\/.)?(www\.)?[-a-zA-Z0-9@:%.\+~#=]{2,256}\.[a-z]{2,6}\b([-a-zA-Z0-9@:%\+.~#?&//=]*)"); ///> Регулярное выражение для проверки урла

    //Определить, откуда получать данные
    InputManager * IM;

    if(QString(argv[1]).contains(UrlRegexp)) {
        IM = new WebInputManager;
    }
    else {
        IM = new FileInputManager;
    }

    //Получить данные
    IM->getData(input, output);

    //Конвертировать html-разметку в xml-разметку
    IM->htmlToXml(output, output);

    //Сгенерировать оглавление
    TableOfContents * TOC = new TableOfContents(input, DomTree);
    TOC->writeXmlDoc(output);

    //Записать сгенерированное оглавление в файл
    OutputManager * OM = new OutputManager; //поменять на статик??
    OM->xmlToHtml(input, output);

    return a.exec();
}
