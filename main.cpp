/*!
\file main.cpp
\brief Файл, содержащий главную функцию программы
*/

/*!
\mainpage
\brief Программа для автоматической генерации оглавления в виде якорных ссылок
*\author Кременецкая А.Д.

Программа выполняет следующие функции:
<ul>
<li>Ищет в коде заголовочные теги;</li>
<li>На основании тегов организует вложенность;</li>
<li>Сообщает об ошибке в случае невалидной html-разметки;</li>
</ul>
<p>Программа принимает на вход URL/путь к файлу, содержащему html-код</p>
<p>Программа возвращает html-разметку со сгенерированным оглавлением с соблюдением вложенности</p>
*/

#include <QCoreApplication>
#include <iostream>
#include "fileinputmanager.h"
#include "webinputmanager.h"
#include "tableofcontents.h"
#include "outputmanager.h"
#include <QtXml/QDomDocument>
#include <QTextCodec>

#define qprint qDebug().nospace()


/*!Главная функция проекта
*\param [in] argc - количество переданных параметров
*\param [in] argv - переданные параметры(пути к файлам)
*\return - 0 при завершении работы программы

*\brief Главная функция проекта
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setlocale(LC_ALL, "Russian");

    QString input(argv[1]);
    QString output(argv[2]);

    QByteArray OutputHtml; //!> Строка со сгенерированным html-кодом

    QRegExp UrlRegexp("http(s)?:"); //!> Признак того, что данные получаются по урлу

    try {
        //Определить, откуда получать данные
        InputManager * IM;

        if(input.contains(UrlRegexp)) {
            IM = new WebInputManager();
        }
        else {
            IM = new FileInputManager();
        }

        //------Получение данных------//
        IM->getData(input, output);

        //Конвертировать html-разметку в xml-разметку
        InputManager::htmlToXml(output, output);

        //------Генерация оглавления------//
        TableOfContents * TOC = new TableOfContents(input, output);
        TOC->validateXml();
        OutputHtml = TOC->getGeneratedHtml();

        //------Вывод данных------//
        if(OutputManager::writeHtml(OutputHtml, output))
            qprint << "Done";

    }

    catch (QString ErrorString) {
        qprint << ErrorString;
    }

    return 0;
}
