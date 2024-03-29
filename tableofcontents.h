/*!
\file
\brief Заголовочный файл с описанием класса для генерации оглавления
*/

#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H
#include <QString>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDebug>

/*!
\brief Класс для генерации оглавления

В процессе генерации оглавления данный класс
выполняет следующие задачи:
<ol>
<li>Находит 'h'-теги;</li>
<li>Изменяет атрибуты найденных 'h'-тегов для создания якорных ссылок и
их корректного визуального представления в оглавлении;</li>
<li>На основании вложенности расставляет отступы между заголовками</li>
<li>Заносит сгенерированное оглавление в байтовый массив</li>
</ol>
*/

class TableOfContents
{
public:

    TableOfContents(void);
    ~TableOfContents(void);

    //Конструктор
    TableOfContents(const QString InputPathToHtml, const QString OutputPathToHtml);

    /*!
       * Проверка корректности xml-разметки
       *\param [in] OutputPathToXml - Путь к файлу/URL, где содержится xml-разметка
       *\return - возвращает true, если разметка валидна
       */
    bool validateXml();

    void deleteThisIsForTest();

    /*!
    Генерирует и заносит оглавление в результирующий файл
    *\return - возвращает байтовый массив со сгенерированным оглавлением
    */
    QByteArray getGeneratedHtml();

    //--------------------Public only for testing--------------------//

    QVector<int>NestingOrder; //!> Вектор, содержащий инфорамацию об уровне вложенности всех найденных заголовков
    QVector<QDomElement>HeaderCollection; //!> Заголовочные веб-элементы

    /*!
     * Получает список детей текущего узла
     *\param [in] Node - Узел дерева
     *\param [out] NodeChildren - Список детей данного узла
     */
    void getChildren(QDomNode & Node, QVector<QDomNode> & NodeChildren);

    /*!
    Выполняет поиск заголовочных тегов в дереве, используя рекурсивный префиксный обход в глубину
    *\param [in] CurrentNode - Текущий узел
    */
    void parseHeadersInTree(QDomNode CurrentNode);

private:

    QDomNode Root; //!> Корневой узел исходного дерева
    QString ErrorMsg; //!> Сообщение об ошибке
    int ErrorLine; //!> Строка, в которой обнаружена ошибка
    int ErrorColumn; //!> Столбец, в котором обнаружена ошибка
    QDomDocument InputDomTree; //!> Исходное дом-дерево html-разметки
    QDomDocument OutputDomTree; //!> Сгенерированное дом-дерево html-разметки
    QString InputPathToHtml; //!> Путь к файлу/URL, где содержится html ( Для организации якорей)
    QString OutputPathToHtml; //!> Путь к файлу/URL, где содержится html ( Для организации якорей)

    /*!
    Выполняет поиск заголовочных тегов в заданном узле
     *\param [in] NodeChildren - Список детей данного узла
    */
    void parseHeadersInNode(QVector<QDomNode> & NodeChildren);

    /*!
    Изменяет атрибуты заголовочных веб-элементов
    */
    void modifyHeaderCollectionAttributes();

    /*!
    Запоминает порядок вложенности заголовочных веб-элементов
    */
    void getHeaderNestingOrder();

    /*!
     * Сдвигает входной элемент вправо
     *\param [in] CurrentElement - Элемент дерева
     *\param [in] LevelAmount - Требуемый уровень вложенности
     * \param [in] NestingNode - Родительский элемент для добавляемых тегов
     */
    void appendNestingLevels(QDomElement &CurrentElement, int LevelAmount, QDomNode NestingNode);

    /*!
    Формирует сгенерированное дерево
    */
    void createOutputTree();
};

#endif // TABLEOFCONTENTS_H
