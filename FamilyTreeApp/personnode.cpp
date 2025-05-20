#include "PersonNode.h"
#include <QPainter>
#include <QDate>

PersonNode::PersonNode(Person* person, QGraphicsItem* parent)
    : QGraphicsObject(parent), m_person(person), m_rect(0, 0, 150, 50) {
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
}

QRectF PersonNode::boundingRect() const {
    return m_rect;
}

void PersonNode::paint(QPainter* painter,
                       const QStyleOptionGraphicsItem* option,
                       QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    // Style based on gender
    painter->setBrush(m_person->getGender() == Male ? QColor(173, 216, 230) :
                          (m_person->getGender() == Female ? QColor(255, 182, 193) : Qt::gray));

    painter->setPen(Qt::black);
    painter->drawRoundedRect(m_rect, 5, 5);

    QString birthYear = m_person->getBirthDate().isValid() ?
                            QString::number(m_person->getBirthDate().year()) : "?";

    painter->drawText(m_rect.adjusted(5, 5, -5, -5),
                      Qt::AlignLeft | Qt::AlignTop,
                      QString::fromStdString(m_person->getName()));
    painter->drawText(m_rect.adjusted(5, 5, -5, -5),
                      Qt::AlignRight | Qt::AlignBottom,
                      birthYear);
}

void PersonNode::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    Q_EMIT nodeClicked(m_person);
    QGraphicsObject::mousePressEvent(event);
}
