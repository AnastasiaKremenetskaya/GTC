/*!
\file
\brief Файл с реализацией методов класса для получения данных по URL

Данный файл содержит в себе реализацию методов класса для получения данных по URL
*/

#include "webinputmanager.h"
#include <QUrl>

WebInputManager::WebInputManager()
{
}

WebInputManager::~WebInputManager()
{
}

void WebInputManager::getData(QString URL, QString PathToOutputFile){
    //Задать необходимую кодировку
    QTextCodec * defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder * decoder = new QTextDecoder(defaultTextCodec);

    //Послать запрос
    response = manager.get(QNetworkRequest(QUrl(URL)));

    //Дождаться загрузки
    QObject::connect(response,SIGNAL(finished()),&event,SLOT(quit()));
    event.exec();

    // Если не удалось загрузить html
    if(response->error() != QNetworkReply::NoError) {
        throw QString("Not valid URL");
    }

    // Сохранить полученные данные в строку
    InputHtmlAsBa = response->readAll();

    InputHtmlAsString = decoder->toUnicode(InputHtmlAsBa);


    // Cохранить html-разметку в результирующий файл
    QFile OutputFile(PathToOutputFile);

    // Если удалось открыть файл для записи
    if(OutputFile.open(QIODevice::WriteOnly)) {

        QTextStream out(&OutputFile);

        out << InputHtmlAsString.toUtf8();

        out.flush();
    }
    else {
        OutputFile.close();
        throw QString("Unable to open result file");
    }
}
