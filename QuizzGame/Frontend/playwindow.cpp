#include "playwindow.h"
#include <QVBoxLayout>
#include <QDebug>

PlayPage::PlayPage(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Background
    this->resize(750, 550);
    QLabel *backgroundLabel = new QLabel(this);
    backgroundLabel->setPixmap(QPixmap(":/QuestionBackground.png"));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->resize(this->size());
    backgroundLabel->lower();

    // Question 
    questionLabel = new QLabel("", this);
    questionLabel->setStyleSheet("color: white; font-size: 12px; font-weight: bold;");
    layout->addWidget(questionLabel, 0, Qt::AlignCenter);

    // Buttons
    this->setStyleSheet(
        "QPushButton {"
        "background-color: #8e44ad;"  
        "color: white;"
        "border-radius: 7px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: #9b59b6;"  
        "}"
    );
    buttonA = new QPushButton("A", this);
    buttonB = new QPushButton("B", this);
    buttonC = new QPushButton("C", this);

    buttonA->setFixedSize(50, 50);
    buttonB->setFixedSize(50, 50);
    buttonC->setFixedSize(50, 50);

    buttonA->move(280, 310);
    buttonB->move(350, 310);  
    buttonC->move(420, 310);  

    connect(buttonA, &QPushButton::clicked, this, &PlayPage::onAnswerClicked);
    connect(buttonB, &QPushButton::clicked, this, &PlayPage::onAnswerClicked);
    connect(buttonC, &QPushButton::clicked, this, &PlayPage::onAnswerClicked);

    // Timer
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, &QTimer::timeout, this, &PlayPage::onTimeout);
}


void PlayPage::updateQuestion(const QString &question) {

    questionLabel->setText(question);
    timer->start(10000);  // 10 seconds
}

void PlayPage::onTimeout() {
    emit answerSelected("not in time");
}

void PlayPage::onAnswerClicked() {

    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        QString answer;
        if (button == buttonA) {
            answer = "a";
        } else if (button == buttonB) {
            answer = "b";
        } else if (button == buttonC) {
            answer = "c";
        }

        timer->stop();
        emit answerSelected(answer);
    }
}
