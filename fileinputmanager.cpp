#include "fileinputmanager.h"

FileInputManager::FileInputManager(void)
{
}

FileInputManager::~FileInputManager(void)
{
}

void FileInputManager::getData(QString PathToInputFile, QString PathToOutputFile)
{
    //Привязать имя к файлу
    QFile InputFile(PathToInputFile);

    //Открыть файл для чтения
    InputFile.open(QIODevice::ReadOnly);
    if (InputFile.isOpen())
    {
        //Записать полученные данные в поток вывода
        QTextStream in(&InputFile);

        while (!in.atEnd()) {

            //Сохранить данныe в строку
            InputHtml += in.readLine();
           // qDebug() << InputHtml;
        }

        //Закрыть файл
        InputFile.close();
    }
    else {
        //Сообщить об ошибке
        throw QString ("Wrong input filename");
    }

    // Cохранить html-разметку в результирующий файл
    QFile OutputFile(PathToOutputFile);

    // Если удалось открыть файл для записи
    if(OutputFile.open(QIODevice::WriteOnly)) {

        QTextStream out(&OutputFile);

        out << InputHtml;

        out.flush();

        //Закрыть файл
        OutputFile.close();
    }
    else {
        OutputFile.close();
        throw QString("Unable to open result file");
    }
}
