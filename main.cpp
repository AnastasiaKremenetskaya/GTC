#include <QCoreApplication>
#include <iostream>
#include "fileinputmanager.h"
#include "webinputmanager.h"
#include "tableofcontents.h"
#include "outputmanager.h"
#include <QtXml/QDomDocument>

#define qprint qDebug().nospace()

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
    QString input("C:/QtCreator/Projects/example.html");
    QString output("C:/QtCreator/Projects/result.html");
    //temporary

    QString OutputHtml; ///> Строка со сгенерированным html-кодом

    QRegExp UrlRegexp("http(s)?:)"); ///> Регулярное выражение для проверки урла

    try {
        //Определить, откуда получать данные
        InputManager * IM;

        if(QString(argv[1]).contains(UrlRegexp)) {
            IM = new WebInputManager();
        }
        else {
            IM = new FileInputManager();
        }

        //------Получение данных------//
        IM->getData(input, output);

        //Конвертировать html-разметку в xml-разметку
        IM->htmlToXml(output, output);


        //------Генерация оглавления------//
        TableOfContents * TOC = new TableOfContents(input, output);
        TOC->validateXml();
        TOC->writeXmlDoc();
        TOC->deleteThisIsForTest();


        //------Вывод данных------//
        //OutputManager * OM = new OutputManager(); //поменять на статик??
        //OM->xmlToHtml(input, output);

    }

    catch (QString ErrorString) {
        qprint << ErrorString;
    }

    return a.exec();
}
