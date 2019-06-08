#include "fileinputmanager.h"

FileInputManager::FileInputManager(void)
{
}

FileInputManager::~FileInputManager(void)
{
}

void FileInputManager::getData(QString FilePath, QString PathToOutputFile)
{
    //Привязать имя к файлу
    InputFile.setFileName(FilePath);

    //Открыть файл для чтения
    InputFile.open(QIODevice::ReadOnly);
    if (InputFile.isOpen())
    {
        //Записать полученные данные в поток вывода
        QTextStream out(&InputHtml);

        //Сохранить данныe в строку
        out << InputHtml;

        //Закрыть файл
        InputFile.close();
    }

    else{
        //Сообщить об ошибке
        throw QString ("Wrong input filename");
    }

    // Cохранить html-разметку в результирующий файл
    QFile OutputFile(PathToOutputFile);

    // Если удалось открыть файл для записи
    if(OutputFile.open(QIODevice::WriteOnly)) {
        QTextStream out(&PathToOutputFile);
        out << InputHtml;
    }
    else {
        OutputFile.close();
        throw QString("Unable to open result file");
    }
}
