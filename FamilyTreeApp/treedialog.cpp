// ------ TreeDialog.cpp ------
#include "TreeDialog.h"
#include "ui_TreeDialog.h"
#include <QGraphicsLineItem>
#include <QMessageBox>
#include <QScrollBar>

TreeDialog::TreeDialog(Tree* tree, Person* rootPerson, QWidget* parent)
    : QDialog(parent),
    ui(new Ui::TreeDialog),
    scene(new QGraphicsScene(this)),
    tree(tree),
    rootPerson(rootPerson) {

    ui->setupUi(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setDragMode(QGraphicsView::ScrollHandDrag);

    setLayout(ui->verticalLayout);

    if(tree && rootPerson) {
        visualizeFamilyTree();
    }
}

TreeDialog::~TreeDialog() {
    delete ui;
}

void TreeDialog::visualizeFamilyTree() {
    scene->clear();
    nodes.clear();
    nodePositions.clear();

    LayoutInfo rootLayout = layoutPerson(rootPerson, 0, 0, 0);
    ui->graphicsView->centerOn(nodes[rootPerson]);
}

TreeDialog::LayoutInfo TreeDialog::layoutPerson(Person* person, qreal x, qreal y, int generation) {
    if(nodePositions.contains(person)) {
        return {nodePositions[person], 0};
    }

    PersonNode* node = createNode(person);
    node->setPos(x, y);
    nodePositions[person] = QPointF(x, y);

    qreal totalWidth = NODE_WIDTH;
    qreal currentX = x;

    for(Family* family : tree->getMarriedFamilies(person->getID())) {
        LayoutInfo familyLayout = layoutFamily(family, currentX, y, generation);
        totalWidth += familyLayout.subtreeWidth + HORIZONTAL_SPACING;
        currentX += familyLayout.subtreeWidth + HORIZONTAL_SPACING;
    }

    return {QPointF(x, y), totalWidth};
}

TreeDialog::LayoutInfo TreeDialog::layoutFamily(Family* family, qreal x, qreal y, int generation) {
    Person* partner = (family->getFather() == rootPerson) ? family->getMother() : family->getFather();
    qreal familyX = x;

    if(partner && !nodePositions.contains(partner)) {
        PersonNode* partnerNode = createNode(partner);
        partnerNode->setPos(x + NODE_WIDTH + PARTNER_SPACING, y);
        nodePositions[partner] = partnerNode->pos();
        drawPartnerConnections(rootPerson, QList<Family*>{family});
        familyX += NODE_WIDTH + PARTNER_SPACING;
    }

    qreal childY = y + VERTICAL_SPACING + NODE_HEIGHT;
    qreal childX = x;
    qreal maxChildWidth = 0;

    for(Person* child : family->getChildren()) {
        LayoutInfo childLayout = layoutPerson(child, childX, childY, generation + 1);
        drawChildConnections(x + NODE_WIDTH/2, y + NODE_HEIGHT, childLayout.position.x() + NODE_WIDTH/2, childY);
        childX += childLayout.subtreeWidth + HORIZONTAL_SPACING;
        maxChildWidth = qMax(maxChildWidth, childLayout.subtreeWidth);
    }

    return {QPointF(x, y), maxChildWidth};
}

void TreeDialog::drawPartnerConnections(Person* person, const QList<Family*>& families) {
    QPointF personPos = nodePositions[person];

    for(Family* family : families) {
        Person* partner = (family->getFather() == person) ? family->getMother() : family->getFather();
        if(!partner || !nodePositions.contains(partner)) continue;

        QPointF partnerPos = nodePositions[partner];
        QLineF line(personPos.x() + NODE_WIDTH, personPos.y() + NODE_HEIGHT/2, partnerPos.x(), partnerPos.y() + NODE_HEIGHT/2);
        scene->addLine(line, QPen(Qt::darkGray, 2));
    }
}

void TreeDialog::drawChildConnections(qreal parentX, qreal parentY, qreal childX, qreal childY) {
    // Vertical line from parent to child level
    QLineF verticalLine(parentX, parentY, parentX, childY - 20);
    scene->addLine(verticalLine, QPen(Qt::darkGray, 1));

    // Horizontal line to child
    QLineF horizontalLine(parentX, childY - 20, childX, childY - 20);
    scene->addLine(horizontalLine, QPen(Qt::darkGray, 1));

    // Vertical line to child node
    QLineF childLine(childX, childY - 20, childX, childY);
    scene->addLine(childLine, QPen(Qt::darkGray, 1));
}

PersonNode* TreeDialog::createNode(Person* person) {
    if(!nodes.contains(person)) {
        PersonNode* node = new PersonNode(person);
        nodes[person] = node;
        scene->addItem(node);
        connect(node, &PersonNode::nodeClicked, this, &TreeDialog::handleNodeClick);
    }
    return nodes[person];
}

void TreeDialog::handleNodeClick(Person* person) {
    QString info = QString("<b>%1</b><br>Gender: %2<br>Born: %3")
        .arg(QString::fromStdString(person->getName()))
        .arg(GenderCast::toString(person->getGender()))
        .arg(person->getBirthDate().toString("yyyy-MM-dd"));

    QMessageBox::information(this, "Person Details", info);
}
