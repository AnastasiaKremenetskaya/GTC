/*!
\file
\brief Файл с реализацией методов класса для получения данных из файла

Данный файл содержит в себе реализацию методов класса для получения данных из файла
*/

#include "fileinputmanager.h"

FileInputManager::FileInputManager(void)
{
}

FileInputManager::~FileInputManager(void)
{
}

void FileInputManager::getData(QString PathToInputFile, QString PathToOutputFile)
{
    //Задать необходимую кодировку
    QTextCodec * defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    QTextDecoder * decoder = new QTextDecoder(defaultTextCodec);

    //Привязать имя к файлу
    QFile InputFile(PathToInputFile);

    //Открыть файл для чтения
    InputFile.open(QIODevice::ReadOnly);
    if (InputFile.isOpen())
    {
        //Записать полученные данные в поток вывода
        QTextStream in(&InputFile);

        InputHtmlAsBa = InputFile.readAll();

        InputHtmlAsString = decoder->toUnicode(InputHtmlAsBa);

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

        out << InputHtmlAsString.toUtf8();

        out.flush();

        //Закрыть файл
        OutputFile.close();
    }
    else {
        OutputFile.close();
        throw QString("Unable to open result file");
    }
}
