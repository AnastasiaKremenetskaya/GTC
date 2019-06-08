#include "webinputmanager.h"
#include <QUrl>

WebInputManager::WebInputManager()
{
}

WebInputManager::~WebInputManager()
{
}

void WebInputManager::getData(QString URL, QString PathToOutputFile){

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
    InputHtml = response->readAll();

    // Cохранить html-разметку в результирующий файл
    QFile OutputFile(PathToOutputFile);

    // Если удалось открыть файл для записи
    if(OutputFile.open(QIODevice::WriteOnly)) {

        QTextStream out(&OutputFile);

        out << InputHtml;

        out.flush();
    }
    else {
        OutputFile.close();
        throw QString("Unable to open result file");
    }
}
