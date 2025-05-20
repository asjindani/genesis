// ------ TreeDialog.h ------
#ifndef TREEDIALOG_H
#define TREEDIALOG_H

#include <QDialog>
#include <QGraphicsScene>
#include <QMap>
#include "Tree.h"
#include "Person.h"
#include "personnode.h"

namespace Ui {
class TreeDialog;
}

class TreeDialog : public QDialog {
    Q_OBJECT

public:
    explicit TreeDialog(Tree* tree, Person* rootPerson, QWidget* parent = nullptr);
    ~TreeDialog();

private slots:
    void handleNodeClick(Person* person);

private:
    struct LayoutInfo {
        QPointF position;
        qreal subtreeWidth;
    };

    void visualizeFamilyTree();
    LayoutInfo layoutPerson(Person* person, qreal x, qreal y, int generation);
    LayoutInfo layoutFamily(Family* family, qreal x, qreal y, int generation);
    void drawPartnerConnections(Person* person, const QList<Family*>& families);
    void drawChildConnections(qreal parentX, qreal parentY, qreal childX, qreal childY);
    PersonNode* createNode(Person* person);

    Ui::TreeDialog* ui;
    QGraphicsScene* scene;
    Tree* tree;
    Person* rootPerson;
    QMap<Person*, PersonNode*> nodes;
    QMap<Person*, QPointF> nodePositions;

    // Layout constants
    const qreal NODE_WIDTH = 150;
    const qreal NODE_HEIGHT = 60;
    const qreal HORIZONTAL_SPACING = 80;
    const qreal VERTICAL_SPACING = 120;
    const qreal PARTNER_SPACING = 100;
};

#endif // TREEDIALOG_H
