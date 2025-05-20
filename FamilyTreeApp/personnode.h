#ifndef PERSONNODE_H
#define PERSONNODE_H

#include <QGraphicsObject>
#include "Person.h"

class PersonNode : public QGraphicsObject {
    Q_OBJECT
public:
    explicit PersonNode(Person* person, QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    Person* getPerson() const { return m_person; }

signals:
    void nodeClicked(Person* person);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Person* m_person;
    QRectF m_rect;
};

#endif // PERSONNODE_H
