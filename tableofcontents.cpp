#include "tableofcontents.h"

TableOfContents::TableOfContents(void){};
TableOfContents::~TableOfContents(void){};

TableOfContents::TableOfContents(const QString InputPathToHtml, const QString OutputPathToHtml) {
    this->OutputPathToHtml = OutputPathToHtml;
    this->InputDomTree.setContent(OutputPathToHtml);
    this->InputPathToHtml = InputPathToHtml;
}

void TableOfContents::deleteThisIsForTest(){
    for (int i = 0; i < this->HeaderCollection.length(); i++) {
        qDebug() << this->HeaderCollection[i].text();

    }
}
bool TableOfContents::validateXml() {

    // Открыть xml для чтения
    QFile XmlFile(this->OutputPathToHtml);
    if (!XmlFile.open(QIODevice::ReadOnly))
        throw QString("Unable to open input xml file. Should be stored at the result file after convertation");

    // Парсинг xml
    if (!InputDomTree.setContent(&XmlFile, &ErrorMsg, &ErrorLine, &ErrorColumn)) {
        XmlFile.close();
        throw QString("Failed to parse the file into a DOM tree. Error message: ") + ErrorMsg +
                " from line " + QString::number(ErrorLine) + ", column " + QString::number(ErrorColumn);
    }
    XmlFile.close();

    this->Root = InputDomTree.documentElement();
    //Если корневой узел отсутствует
    if(Root.isNull()) {
        throw QString("No root element in markup. Termination");
    }

    return true;
}

void TableOfContents::generateTOC(QDomNode CurrentNode) {
    QVector<QDomNode> NodeChildren; ///> Cписок детей узла

    //Если узел пуст
    if(CurrentNode.isNull())
        //Прекратить обход узла
        return;

    //qprint << CurrentNode.toElement().tagName();

    //Если узел имеет дочерние элементы
    if(CurrentNode.hasChildNodes()) {
        //Получить дочерние элементы узла
        getChildren(CurrentNode, NodeChildren);
    }

    //Запомнить элементы, являющиеся заголовочными тегами
    parseHeaders(NodeChildren);

    //Рекурсивно вызвать функцию для каждого узла дерева
    for(int i = 0; i < NodeChildren.length(); i++) {
        generateTOC(NodeChildren[i]);
    }
}

void TableOfContents::parseHeaders(QVector<QDomNode> & NodeChildren ) {
    for (int i = 0; i < NodeChildren.length(); i++) {
        if (NodeChildren[i].toElement().tagName() == "h1" ||
                NodeChildren[i].toElement().tagName() == "h2" ||
                NodeChildren[i].toElement().tagName() == "h3" ||
                NodeChildren[i].toElement().tagName() == "h4" ||
                NodeChildren[i].toElement().tagName() == "h5" ||
                NodeChildren[i].toElement().tagName() == "h6") {
            this->HeaderCollection.append(NodeChildren[i].toElement());
        }
    }
}

void TableOfContents::getChildren(QDomNode & Node, QVector<QDomNode> & NodeChildren) {

    //Получить текущего ребенка
    QDomNode ChildNode = Node.firstChild(); // текущий ребенок

    //Пока у узла есть дети
    while(!ChildNode.isNull()) {

        //Если ребенок не является текстом
        if(!ChildNode.isText()) {

            //Записать данного ребенка в список детей узла
            NodeChildren.append(ChildNode);
        }
        //Перейти к следующему ребенку
        ChildNode = ChildNode.nextSibling();
    }
}

void TableOfContents::setTab(){
    //Алгоритм:
    //1. Объявить <ul>
    //2. Добавить связь узла с <ul>
    //3. Для каждого элемента узла
    //	3. 1. Если уровень родительского элемента выше уровня текущего элемента
    //		3. 1. 1. Добавить новый <ul>
    //		3. 1. 2. Добавить связь узла с <ul>
    //		3. 1. 3. Добавить новый пункт списка <li>
    //		3. 1. 4. Добавить связь узла с <li>
    //	3. 2. Иначе, если уровень родительского тега h ниже уровня текущего тега
    //		3. 2. 1. Добавить связь родителя с тегом <ul>
    //		3. 2. 2. Добавить новый пункт списка <li>
    //	3. 3. Иначе если уровень родительского тега h равен уровню текущего тега
    //		3. 3. 1. Добавить новый пункт списка <li>
    //4. Объявить закрывающий </ul>

    //Добавить в начало список
    QDomNode ul = InputDomTree.createElement("ul");
    //Добавить пункт списка к первому элементу коллекции

    //    for(int i = 0; i < Headers.count(); i++) {
    //        Headers.item(i).appendChild()
    //                QDomElement bar = elements.at(0).toElement();

    //        QDomElement baz = document.createElement( "baz" );
    //        baz.setAttribute( "count", QString::number( bar.elementsByTagName( "baz" ).size() + 1 ) );

    //        bar.appendChild( baz );
    //    }
}

void TableOfContents::modifyHeaderCollectionAttributes() {
    //    Алгоритм:
    //    1. Для каждого элемента узла
    //        1. 1. Заменить тег узла на <a>
    //        1. 2. Добавить атрибут, задающий ссылку на первоначальный документ


}

bool TableOfContents::writeXmlDoc() {

    //Выполнить поиск заголовочных тегов в исходном дом-дереве
    generateTOC(this->Root);
    if(this->HeaderCollection.length()==0)
        throw QString("Generation  of Table of contents is impossible: no headers in markup detected");

    // modifyHeaderCollectionAttributes();
    //setTab();

    // Открыть xml для записи
    QFile xml(this->OutputPathToHtml);
    if (!xml.open(QIODevice::WriteOnly))
        throw QString("Unable to write xml data to result file");
    xml.write(OutputDomTree.toByteArray());
    xml.close();

    return true;
}
