#include "personview.h"
#include "ui_personview.h"

#include "Person.h"

#include <QListWidget>
#include <QScrollArea>
#include "dialog.h"
#include "treedialog.h"

PersonView::PersonView(QWidget *parent, Tree* tree, Person* person)
    : QDialog(parent)
    , ui(new Ui::PersonView)
{
    ui->setupUi(this);

    setWindowTitle("Person");

    this->tree = tree;
    this->person = person;
    this->accepted = false;

    updateView();

    ui->scrollArea->setWidget(ui->scrollAreaWidgetContents);
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollAreaWidgetContents->setLayout(ui->verticalLayout);

    ui->verticalLayout->setAlignment(Qt::AlignTop);


}

void adjustListWidgetHeight(QListWidget* listWidget) {
    int totalHeight = 0;
    for (int i = 0; i < listWidget->count(); ++i) {
        QListWidgetItem* item = listWidget->item(i);
        totalHeight += listWidget->sizeHintForRow(i);
    }

    // Add space for borders, padding, scrollbar (if needed)
    int frame = 2 * listWidget->frameWidth();
    int spacing = listWidget->spacing() * (listWidget->count() - 1);
    listWidget->setMinimumHeight(totalHeight + frame + spacing);
    listWidget->setMaximumHeight(totalHeight + frame + spacing);
}

void PersonView::updateView() {
    if (!person)
        return;

    ui->personName->setText(QString::fromStdString(person->getName()));

    int id = person->getID();

    // setLayout(ui->verticalLayout);

    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->hide();
            widget->deleteLater();
        }
        delete item;
    }

    if (const auto& family = tree->getBirthFamily(id)) {
        QLabel* label = new QLabel(this);
        label->setText(QString::fromStdString("Birth Family:"));
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        ui->verticalLayout->addWidget(label);

        QListWidget* listWidget = new QListWidget(this);
        listWidget->setSizeAdjustPolicy(QListWidget::AdjustToContents);
        listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        if (const auto& father = family->getFather()) {
            QListWidgetItem* item = new QListWidgetItem(listWidget);
            item->setData(Qt::UserRole, father->getID());
            item->setText(QString::fromStdString("Father: " + father->getRepr()));
        }
        if (const auto& mother = family->getMother()) {
            QListWidgetItem* item = new QListWidgetItem(listWidget);
            item->setData(Qt::UserRole, mother->getID());
            item->setText(QString::fromStdString("Mother: " + mother->getRepr()));
        }
        for (const auto& child : family->getChildren()) {
            if (child != person) {
                QListWidgetItem* item = new QListWidgetItem(listWidget);
                item->setData(Qt::UserRole, child->getID());
                item->setText(QString::fromStdString("Sibling: " + child->getRepr()));
            }
        }

        connect(listWidget, &QListWidget::itemDoubleClicked, listWidget, [&](QListWidgetItem* item) {
            int id = item->data(Qt::UserRole).toInt();
            if (Person* person = tree->getPerson(id)) {
                this->person = person;
                this->updateView();
            }
        });

        adjustListWidgetHeight(listWidget);
        ui->verticalLayout->addWidget(listWidget);
    }

    for (const auto& family : tree->getMarriedFamilies(id)) {
        QLabel* label = new QLabel(this);
        label->setText(QString::fromStdString("Married Family:"));
        label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        ui->verticalLayout->addWidget(label);

        Person* spouse;
        if (person == family->getFather())
            spouse = family->getMother();
        else
            spouse = family->getFather();

        string spouseType = "Spouse";
        if (person->getGender() == Gender::Male)
            spouseType = "Wife";
        else if (person->getGender() == Gender::Female)
            spouseType = "Husband";

        QListWidget* listWidget = new QListWidget(this);
        listWidget->setSizeAdjustPolicy(QListWidget::AdjustToContents);
        listWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

        if (spouse) {
            QListWidgetItem* item = new QListWidgetItem(listWidget);
            item->setData(Qt::UserRole, spouse->getID());
            item->setText(QString::fromStdString(spouseType + ": " + spouse->getRepr()));
        }

        for (const auto& child : family->getChildren()) {
            QListWidgetItem* item = new QListWidgetItem(listWidget);
            item->setData(Qt::UserRole, child->getID());
            item->setText(QString::fromStdString("Child: " + child->getRepr()));
        }

        connect(listWidget, &QListWidget::itemDoubleClicked, listWidget, [&](QListWidgetItem* item) {
            int id = item->data(Qt::UserRole).toInt();
            if (Person* person = tree->getPerson(id)) {
                this->person = person;
                this->updateView();
            }
        });

        adjustListWidgetHeight(listWidget);
        ui->verticalLayout->addWidget(listWidget);
    }

    update();
}

PersonView::~PersonView()
{
    delete ui;
}

void PersonView::on_pushButton_clicked()
{
    Dialog dialog(this, tree, person);
    dialog.show();
    if (dialog.exec() == QDialog::Accepted) {
        tree->updatePerson(person->getID());
        this->accepted = true;
    }
    updateView();
}

bool PersonView::isAccepted() const {
    return accepted;
}

void PersonView::on_pushButton_2_clicked()
{
    TreeDialog* dialog = new TreeDialog(tree, person, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->showMaximized();
}

