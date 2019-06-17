/*!
\file
\brief Файл с реализацией методов класса для генерации оглавления

Данный файл содержит в себе реализацию методов класса для генерации оглавления
*/

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

        throw QString("Failed to parse file into DOM tree. Error message: ") + ErrorMsg +
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

    QVector<QDomNode> NodeChildren; ///> Cписок детей данного узла

    //Если узел пуст
    if(CurrentNode.isNull())
        //Прекратить обход узла
        return;

    //Если узел имеет дочерние элементы
    if(CurrentNode.hasChildNodes()) {
        //Получить дочерние элементы узла
        getChildren(CurrentNode, NodeChildren);
    }

    //Запомнить элементы, являющиеся заголовочными тегами
    parseHeaders(NodeChildren);
}

void TableOfContents::parseHeaders(QVector<QDomNode> & NodeChildren ) {

    //Для каждого списка детей узла
    for (int i = 0; i < NodeChildren.length(); i++) {

        //Если ребенок является заголовочным тегом
        if (NodeChildren[i].toElement().tagName() == "h1" ||
                NodeChildren[i].toElement().tagName() == "h2" ||
                NodeChildren[i].toElement().tagName() == "h3" ||
                NodeChildren[i].toElement().tagName() == "h4" ||
                NodeChildren[i].toElement().tagName() == "h5" ||
                NodeChildren[i].toElement().tagName() == "h6") {

            //Добавить его в коллекцию заголовчных веб-элементов
            this->HeaderCollection.append(NodeChildren[i].toElement());
        }
    }
}

void TableOfContents::getChildren(QDomNode & Node, QVector<QDomNode> & NodeChildren) {

    //Получить текущего ребенка
    QDomNode ChildNode = Node.firstChild();

    //Пока у узла есть дети
    while(!ChildNode.isNull()) {

        //Если ребенок не является текстом или комментарием
        if(!ChildNode.isText() && !ChildNode.isComment()) {

            //Записать данного ребенка в список детей узла
            NodeChildren.append(ChildNode);
        }

        //Рекурсивно вызывать функцию для каждого ребенка
        if(ChildNode.hasChildNodes()) {
            getChildren(ChildNode, NodeChildren);
        }

        //Перейти к следующему ребенку
        ChildNode = ChildNode.nextSibling();
    }
}

void TableOfContents::getHeaderNestingOrder() {

    QString tags;
    //Занести порядок заголовочного тега в строку
    for (int i = 0; i < HeaderCollection.length(); i++) {

        tags.append(HeaderCollection[i].tagName());
    }

    tags.remove('h');

    for (int i = 0; i < HeaderCollection.length(); i++) {
        this->NestingOrder.append(tags[i].digitValue());
    }
    qDebug() << NestingOrder;
}

void TableOfContents::modifyHeaderCollectionAttributes() {
    //Для каждого элемента коллекции веб-элементов
    for (int i = 0; i < this->HeaderCollection.length(); i++) {
        //Добавить якорную ссылку
        HeaderCollection[i].setAttribute("href", this->InputPathToHtml+"#"+HeaderCollection[i].attributeNode("id").value());
        HeaderCollection[i].setTagName("a");
    }
}

void TableOfContents::createOutputTree() {

    //Создать новое дерево
    QString docType = "Generated TOC";
    QByteArray ba = QByteArray::fromHex(docType.toUtf8());
    OutputDomTree.setContent(ba);

    //Импортировать корневой узел из исходного дерева
    QDomNode root = OutputDomTree.createElement("html");
    QDomNode body = OutputDomTree.createElement("body");
    root.appendChild(body);
    OutputDomTree.appendChild(root);

    //Создать основной список
    QDomNode MainUl = OutputDomTree.createElement("ul");
    body.appendChild(MainUl);

    //Задать вложенность прочим заголовкам

    //Для каждого найденного заголовка
    for (int i = 0; i < HeaderCollection.length() && i < NestingOrder.length(); i++) {
        appendNestingLevels(HeaderCollection[i], NestingOrder[i], MainUl);
    }

    qDebug() <<"Please, work/n" <<OutputDomTree.toString();
}

void TableOfContents::appendNestingLevels(QDomElement &CurrentElement, int LevelAmount, QDomNode NestingNode) { 
    //------Добавить в список заголовки------//

    //В зависимости от уровня заголовка организовать вложенность
    switch (LevelAmount) {
    case 2:{
        QDomNode ul = OutputDomTree.createElement("ul");
        QDomNode li = OutputDomTree.createElement("li");
        ul.appendChild(li);
        li.appendChild(CurrentElement);
        NestingNode.appendChild(ul);
        break;
    }
    case 3:{
        QDomNode ul1 = OutputDomTree.createElement("ul");
        QDomNode ul = OutputDomTree.createElement("ul");
        QDomNode li = OutputDomTree.createElement("li");
        ul1.appendChild(ul);
        ul.appendChild(li);
        li.appendChild(CurrentElement);
        NestingNode.appendChild(ul1);
        break;
    }
    case 4:{
        QDomNode ul2 = OutputDomTree.createElement("ul");
        QDomNode ul1 = OutputDomTree.createElement("ul");
        QDomNode ul = OutputDomTree.createElement("ul");
        QDomNode li = OutputDomTree.createElement("li");
        ul2.appendChild(ul1);
        ul1.appendChild(ul);
        ul.appendChild(li);
        li.appendChild(CurrentElement);
        NestingNode.appendChild(ul2);
        break;
    }

    case 5: {
        QDomNode ul3 = OutputDomTree.createElement("ul");
        QDomNode ul2 = OutputDomTree.createElement("ul");
        QDomNode ul1 = OutputDomTree.createElement("ul");
        QDomNode ul = OutputDomTree.createElement("ul");
        QDomNode li = OutputDomTree.createElement("li");
        ul3.appendChild(ul2);
        ul2.appendChild(ul1);
        ul1.appendChild(ul);
        ul.appendChild(li);
        li.appendChild(CurrentElement);
        NestingNode.appendChild(ul3);
        break;
    }
    case 6: {
        QDomNode ul4 = OutputDomTree.createElement("ul");
        QDomNode ul3 = OutputDomTree.createElement("ul");
        QDomNode ul2 = OutputDomTree.createElement("ul");
        QDomNode ul1 = OutputDomTree.createElement("ul");
        QDomNode ul = OutputDomTree.createElement("ul");
        QDomNode li = OutputDomTree.createElement("li");
        ul4.appendChild(ul3);
        ul3.appendChild(ul2);
        ul2.appendChild(ul1);
        ul1.appendChild(ul);
        ul.appendChild(li);
        li.appendChild(CurrentElement);
        NestingNode.appendChild(ul4);
        break;
    }
    default: {
        QDomNode li = OutputDomTree.createElement("li");
        li.appendChild(CurrentElement);
        NestingNode.appendChild(li);
        break;
    }
    }
}

QByteArray TableOfContents::getGeneratedHtml() {

    //Выполнить поиск заголовочных тегов в исходном дом-дереве
    generateTOC(this->Root);

    //Проверить документ на наличие заголовочных тегов
    if(this->HeaderCollection.length()==0)
        throw QString("Generation  of Table of contents is impossible: no headers in markup detected");

    //Запомнить порядок вложенности
    getHeaderNestingOrder();

    //Сделать элементы якорными ссылками
    modifyHeaderCollectionAttributes();

    //Создать выходное дерево из необходимых элементов
    createOutputTree();

    //Записать сгенерированное оглавление в байтовый массив
    QByteArray GeneratedHtml;
    GeneratedHtml.append(OutputDomTree.toByteArray());

    return GeneratedHtml;
}
