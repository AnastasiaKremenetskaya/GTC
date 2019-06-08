#include "tableofcontents.h"

TableOfContents::TableOfContents(void){};
TableOfContents::~TableOfContents(void){};

TableOfContents::TableOfContents(const QString InputPathToHtml, QDomDocument & DomTree) {

    this->DomTree = DomTree;
    this->InputPathToHtml = InputPathToHtml;
    this->Root = DomTree.elementsByTagName("body").at(0);
    //Если корневой узел отсутствует
    if(Root.isNull()) {
        throw QString("No body element. Termination");
    }
}

void TableOfContents::generateTOC(QDomNode CurrentNode) {

    //Если узел пуст
    if(CurrentNode.isNull())
        //Прекратить обход узла
        return;

    //qprint << CurrentNode.toElement().tagName();

    //Если узел имеет дочерние элементы
    if(CurrentNode.hasChildNodes()) {
        //Получить дочерние элементы узла, являющиеся заголовочными тегами
        getChildren(CurrentNode, NodeChildren);
        //Организовать вложенность заголовочных элементов с помощью маркированного списка
      //  setTab(CurrentNode);
        //Сделать заголовки якорными ссылками
       // modifyHeaderCollectionAttributes(CurrentNode);
    }

    //Рекурсивно вызвать функцию для каждого узла дерева
    for(int i = 0; i < NodeChildren.length(); i++) {
        generateTOC(NodeChildren[i]);
    }
}

bool TableOfContents::validateXml(const QString OutputPathToXml, QDomDocument & DomTree) {

    // Открыть xml для чтения
    QFile xml(OutputPathToXml);
    if (!xml.open(QIODevice::ReadOnly))
        throw QString("Unable to open input xml file");

    // Парсинг xml
    if (!DomTree.setContent(&xml, &ErrorMsg, &ErrorLine, &ErrorColumn)) {
        xml.close();
        throw QString("Failed to parse the file into a DOM tree. Error message: ") + ErrorMsg +
                " from line " + QString::number(ErrorLine) + ", column " + QString::number(ErrorColumn);
    }
    xml.close();

    return true;
}

//QDomNodeList TableOfContents::parseHeaders() {

//    //Получить элементы дерева с заголовочным тегом
//    Headers = DomTree.elementsByTagName("h1,h2,h3,h4,h5,h6");

//    //Если заголовочные теги отсутствуют
//    if( Headers.size() == 0 ) {
//        throw QString("Unable to create table of contents: no header tags in html-markup");
//    }

//    //Проверить, верно ли определены теги
//    for(int i = 0; i < Headers.count(); i++)
//    {
//        QDomNode HeaderNode = Headers.at(i);
//        if(HeaderNode.isElement())
//        {
//            qDebug() << HeaderNode.toElement().tagName()
//                     << " = "
//                     <<  HeaderNode.toElement().text();
//        }
//    }
//    return Headers;
//}

void TableOfContents::getChildren(QDomNode & Node, QVector<QDomNode> & NodeChildren) {

    //Получить текущего ребенка
    QDomNode ChildNode = Node.firstChild(); // текущий ребенок

    //Пока у узла есть дети
    while(!ChildNode.isNull()) {

        //Если ребенок не является текстом
        if(!ChildNode.isText()) {

            //Если ребенок является заголовком
            if(ChildNode.toElement().tagName() == "h1" || ChildNode.toElement().tagName() == "h2"
                    || ChildNode.toElement().tagName() == "h3" || ChildNode.toElement().tagName() == "h4"
                    || ChildNode.toElement().tagName() == "h5" || ChildNode.toElement().tagName() == "h6") {

                //Записать данного ребенка в список детей узла
                NodeChildren.append(ChildNode);
            }
        }
        //Перейти к следующему ребенку
        ChildNode = ChildNode.nextSibling();
    }
}

void TableOfContents::setTab(QDomNode & Node){
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
    QDomNode ul = DomTree.createElement("ul");
    //Добавить пункт списка к первому элементу коллекции

//    for(int i = 0; i < Headers.count(); i++) {
//        Headers.item(i).appendChild()
//                QDomElement bar = elements.at(0).toElement();

//        QDomElement baz = document.createElement( "baz" );
//        baz.setAttribute( "count", QString::number( bar.elementsByTagName( "baz" ).size() + 1 ) );

//        bar.appendChild( baz );
//    }
}

void TableOfContents::modifyHeaderCollectionAttributes(QDomNode & Node) {
//    Алгоритм:
//    1. Для каждого элемента узла
//        1. 1. Заменить тег узла на <a>
//        1. 2. Добавить атрибут, задающий ссылку на первоначальный документ


}

bool TableOfContents::writeXmlDoc(const QString OutputPathToXml) {
    // Открыть xml для записи
    QFile xml(OutputPathToXml);
    if (!xml.open(QIODevice::WriteOnly))
        throw QString("Unable to write xml data to result file");
    xml.write(DomTree.toByteArray());
    xml.close();

    return true;
}
