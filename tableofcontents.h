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
- Находит 'h'-теги;
- Изменяет атрибуты найденных 'h'-тегов для создания якорных ссылок и
их корректного визуального представления в оглавлении;
- На основании вложенности расставляет отступы между заголовками
- Заносит сгенерированное оглавление в байтовый массив
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
    */
    bool writeXmlDoc();

private:

    QVector<QDomElement>HeaderCollection; ///> Заголовочные веб-элементы
    QVector<int>NestingOrder;
    QString ParsedInputXmlStr;
    QDomNode Root; ///> Корневой узел дерева
    QString ErrorMsg; ///> Сообщение об ошибке
    int ErrorLine; ///> Строка, в которой обнаружена ошибка
    int ErrorColumn; ///> Столбец, в котором обнаружена ошибка
    QDomDocument InputDomTree; ///> Исходное дом-дерево html-разметки
    QDomDocument OutputDomTree; ///> Сгенерированное дом-дерево html-разметки
    QString InputPathToHtml; ///> Путь к файлу/URL, где содержится html ( Для организации якорей)
    QString OutputPathToHtml; ///> Путь к файлу/URL, где содержится html ( Для организации якорей)

    /*!
    Генерирует оглавление, используя рекурсивный префиксный обход дерева в глубину
    *\param [in] CurrentNode - Текущий узел
    */
    void generateTOC(QDomNode CurrentNode); //объединение нижних функций

    /*!
    Выполняет поиск в глубину заголовочных тегов в заданном узле
     *\param [in] NodeChildren - Список детей данного узла
    */
    void parseHeaders(QVector<QDomNode> & NodeChildren);

    /*!
     * Получает список детей текущего узла
     *\param [in] Node - Узел дерева
     *\param [out] NodeChildren - Список детей данного узла
     */
    void getChildren(QDomNode & Node, QVector<QDomNode> & NodeChildren);

    /*!
    Изменяет атрибуты заголовочных веб-элементов
    */
    void modifyHeaderCollectionAttributes();

    void getHeaderNestingOrder();

    void createOutputTree();

    /*!
    Определяет отступы по уровню вложенности элементов
    */
    void setTab();

};

#endif // TABLEOFCONTENTS_H
