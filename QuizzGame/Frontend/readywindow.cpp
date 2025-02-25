#include "readywindow.h"
#include <QVBoxLayout>
#include <QLabel>

ReadyPage::ReadyPage(QWidget *parent) : QWidget(parent) {
    this->resize(750, 550);
    this->setStyleSheet(
        "QPushButton {"
        "background-color: #936ef0;"  
        "color: white;"
        "border-radius: 7px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: #ad92f0;"  
        "}"
        "QPushButton:pressed {"
        "background-color: #0af240;"
        "}"
    );
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/ReadyBackground.png"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->resize(this->size());
    backgroundLabel->lower();

    readyButton = new QPushButton("I AM READY", this);
    readyButton->setFixedSize(150, 50);
    readyButton->move(300, 310);

    connect(readyButton, &QPushButton::clicked, this, &ReadyPage::readyClicked);
}
