#ifndef TABLEOFCONTENTS_H
#define TABLEOFCONTENTS_H
#include <QString>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QDebug>

#define qprint qDebug().nospace().noquote()

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

    //Конструктор копирования
    TableOfContents(const QString InputPathToHtml, QDomDocument & DomTree);

    QString InputPathToHtml; ///> Путь к файлу/URL, где содержится html ( Для организации якорей)
    QDomNode Root; ///> Корневой узел дерева

    /*!
       * Проверка корректности xml-разметки
       *\param [in] OutputPathToXml - Путь к файлу/URL, где содержится xml-разметка
       *\param [out] treeHtml - Исходное дерево html-разметки
       *\return - возвращает true, если разметка валидна
       */
    bool validateXml(const QString OutputPathToXml, QDomDocument & tree);

    /*!
    Заносит оглавление в результирующий файл
    \param[in] OutputPathToXml - Путь к файлу/URL, где содержится xml-разметка
    */
    bool writeXmlDoc(const QString OutputPathToXml);

private:

    QVector<QDomNode> NodeChildren; ///> Cписок детей узла
    QString ErrorMsg; ///> Сообщение об ошибке
    int ErrorLine; ///> Строка, в которой обнаружена ошибка
    int ErrorColumn; ///> Столбец, в котором обнаружена ошибка
    QDomDocument DomTree; ///> Дом-дерево html-разметки

    /*!
    Генерирует оглавление, используя рекурсивный префиксный обход дерева в глубину
    *\param [in] CurrentNode - Текущий узел
    */
    void generateTOC(QDomNode CurrentNode); //объединение нижних функций

    /*!
    Выполняет поиск заголовочных тегов
    /return вернуть все узлы дерева, являющиеся заголовками;
    */
    //QDomNodeList parseHeaders();

    /*!
    Определяет отступы по уровню вложенности элементов
    *\param [in] Node - Узел дерева
    */
    void setTab(QDomNode & Node);

    /*!
    Изменяет атрибуты заголовочных веб-элементов
    *\param [in] Node - Узел дерева
    */
    void modifyHeaderCollectionAttributes(QDomNode & Node);

    /*!
     * Получает список детей текущего узла
     *\param [in] Node - Узел дерева
     *\param [out] NodeChildren - Список детей данного узла
     */
    void getChildren(QDomNode & Node, QVector<QDomNode> & NodeChildren);
};

#endif // TABLEOFCONTENTS_H
