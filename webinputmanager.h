/*!
\file
\brief Заголовочный файл с описанием класса для получения данных по URL
*/

#ifndef WEBINPUTMANAGER_H
#define WEBINPUTMANAGER_H

#include "inputmanager.h"

/*!
    \brief Дочерний класс, цель - загрузка данных по URL

    Дочерний класс, который отнаследован от ранее созданного класса InputManager
*/

class WebInputManager : public InputManager
{
public:
    WebInputManager();
    ~WebInputManager();

    /*!
     * Получает html-текст по URL и представляет его в виде фрейма
     *\param [in] Path - URL страницы, где содержится html-текст
     *\param [in] PathToOutputFile - Путь к файлу, куда необходимо сохранить результат
*/
    virtual void getData(QString URL, QString PathToOutputFile);

private:

    QNetworkAccessManager manager; ///< Объект запроса
    QNetworkReply *response; ///< Запрос
    QEventLoop event; ///> Цикл для загрузки асинхронных элементов
};

#endif // WEBINPUTMANAGER_H
