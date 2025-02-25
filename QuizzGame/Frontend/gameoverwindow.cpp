#include "gameoverwindow.h"
#include <QVBoxLayout>
#include <QDebug>

GameOverPage::GameOverPage(QWidget *parent) : QWidget(parent) {
    //background
    this->resize(750, 550);
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/GameOverBackground.png"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->resize(this->size());
    backgroundLabel->lower();

    QVBoxLayout *layout = new QVBoxLayout(this);
    label = new QLabel("", this);
    label->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    layout->addWidget(label,0, Qt::AlignCenter);
}

void GameOverPage::updateMessage(const QString &message) {
    label->setText(message);
}
