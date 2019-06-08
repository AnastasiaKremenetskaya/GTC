#ifndef FILEINPUTMANAGER_H
#define FILEINPUTMANAGER_H

#include <QFile>
#include "inputmanager.h"

/*!
    \brief Дочерний класс, цель - загрузка данных из файла

    Дочерний класс, который отнаследован от ранее созданного класса InputManager
*/
class FileInputManager : public InputManager
{
public:
    FileInputManager(void);
    ~FileInputManager(void);

    /*!
     * Получает html-текст по указанному пути и представляет его в виде фрейма
     *\param[in] PathToInputFile Путь к файлу, где содержится html-текст
     *\param [in] PathToOutputFile - Путь к файлу, куда необходимо сохранить результат
*/
    virtual void getData(QString PathToInputFile, QString PathToOutputFile);

private:

};

#endif // FILEINPUTMANAGER_H
