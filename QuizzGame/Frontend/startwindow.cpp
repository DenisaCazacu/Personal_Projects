#include "startwindow.h"
#include <QVBoxLayout>
#include <QDebug>

StartPage::StartPage(QWidget *parent) : QWidget(parent) {
    //background
    this->resize(750, 550);
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/StartBackground.png"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->resize(this->size());
    backgroundLabel->lower();

    QVBoxLayout *layout = new QVBoxLayout(this);
    label = new QLabel("", this);
    label->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
    layout->addWidget(label,0, Qt::AlignCenter);
}

void StartPage::updateMessage(const QString &message) {
    label->setText(message);
}

