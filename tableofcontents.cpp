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

    //Рекурсивно вызвать функцию для каждого узла дерева
    for(int i = 0; i < NodeChildren.length(); i++) {
        generateTOC(NodeChildren[i]);
    }
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

        //Если ребенок не является текстом
        if(!ChildNode.isText()) {

            //Записать данного ребенка в список детей узла
            NodeChildren.append(ChildNode);
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

void TableOfContents::setTab(){

}

void TableOfContents::modifyHeaderCollectionAttributes() {
    //Для каждого элемента коллекции веб-элементов
    for (int i = 0; i < this->HeaderCollection.length(); i++) {
        //Добавить якорную ссылку
        HeaderCollection[i].setAttribute("href", this->InputPathToHtml);
        HeaderCollection[i].setTagName("a");
    }
}

void TableOfContents::createOutputTree() {

    //Создать новое дерево
    QString docType = "Generated TOC";
    QByteArray ba = QByteArray::fromHex(docType.toUtf8());
    OutputDomTree.setContent(ba);

    //Импортировать корневой узел из исходного дерева
    QDomNode root = InputDomTree.importNode(this->Root, false);
    OutputDomTree.appendChild(root);

    //Создать основной список
    QDomNode ul = OutputDomTree.createElement("ul");
    OutputDomTree.firstChild().appendChild(ul);

    //------Добавить в список заголовки------//

    //Задать вложенность первому заголовку

    if(NestingOrder[0] > 1) {
        appendNestingLevels(HeaderCollection[0], NestingOrder[0]);
    }
    else {
        //Добавить <li>
        QDomNode li = OutputDomTree.createElement("li");

        ul.appendChild(li);

        //Добавить связь элемента с <li>
        li.appendChild(HeaderCollection[0]);
    }

    //Задать вложенность прочим заголовкам

    //Для каждого найденного заголовка
    for (int i = 1; i < HeaderCollection.length() && i < NestingOrder.length(); i++) {

        //Если уровень родительского элемента выше уровня текущего элемента
        if(NestingOrder[i-1] < NestingOrder[i]) {
            //Задать необходимую вложенность
            appendNestingLevels(HeaderCollection[i],NestingOrder[i] - NestingOrder[i-1]);
        }

        //Иначе, если уровень родительского элемента ниже уровня текущего элемента
        else if(NestingOrder[i-1] > NestingOrder[i]) {
            //Задать необходимую вложенность
            removeNestingLevels(HeaderCollection[i],NestingOrder[i-1] - NestingOrder[i]);
        }

        //Иначе
        else {
            //Добавить <li>
            QDomNode li = OutputDomTree.createElement("li");

            //Добавить связь элемента с <li>
            li.appendChild(HeaderCollection[i]);
        }
    }

    qDebug() <<"Please, work" <<OutputDomTree.toString();
}

void TableOfContents::appendNestingLevels(QDomElement &CurrentElement, int LevelAmount) {

    //Добавить <ul>
    QDomNode ul = this->OutputDomTree.createElement("ul");

    //Добавить <li>
    QDomNode li = this->OutputDomTree.createElement("li");

    this->OutputDomTree.appendChild(ul);

    //Добавить связь <ul> с <li>
    ul.appendChild(li);

    //Добавить связь элемента с <li>
    li.appendChild(CurrentElement);

    //Задать элементу требуемый уровень вложенности
    while (LevelAmount > 1) {

        //Добавить новый <ul>
        QDomNode newUl = this->OutputDomTree.createElement("ul");

        this->OutputDomTree.appendChild(newUl);

        this->OutputDomTree.insertAfter(ul, newUl);

        --LevelAmount;
    }
}

void TableOfContents::removeNestingLevels(QDomElement &CurrentElement, int LevelAmount) {

    //Добавить <li>
    QDomNode li = this->OutputDomTree.createElement("li");

    //Добавить связь элемента с <li>
    li.appendChild(CurrentElement);

    QDomNode InsertingNode = CurrentElement.parentNode();///> Узел для вставки

    //Инсертнуть сразу на требуемый уровень вложенности, значит передать как аргумент узел, куда инсертить

    //Определить в дереве место для вставки
    while (LevelAmount > 1) {

        InsertingNode = InsertingNode.parentNode();

        //InsertingNode = OutputDomTree.previousSibling();

        --LevelAmount;
    }
    //Вставить пункт зоголовок в соответствии с порядком вложенности
    this->OutputDomTree.insertBefore(CurrentElement, InsertingNode);
}

bool TableOfContents::writeXmlDoc() {

    //Выполнить поиск заголовочных тегов в исходном дом-дереве
    generateTOC(this->Root);

    //Проверить документ на наличие заголовочных тегов
    if(this->HeaderCollection.length()==0)
        throw QString("Generation  of Table of contents is impossible: no headers in markup detected");
    //setTab();

    //Запомнить порядок вложенности
    getHeaderNestingOrder();

    //Сделать элементы якорными ссылками
    modifyHeaderCollectionAttributes();

    //Создать выходное дерево из необходимых элементов
    createOutputTree();

    //Записать сгенерированное оглавление в выходной файл
    QFile xml(this->OutputPathToHtml);
    if (!xml.open(QIODevice::WriteOnly))
        throw QString("Unable to write xml data to result file");
    xml.write(OutputDomTree.toByteArray());
    xml.close();

    return true;
}
