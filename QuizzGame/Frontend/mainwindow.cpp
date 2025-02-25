#include "mainwindow.h"
#include "startwindow.h"
#include "readywindow.h"
#include "playwindow.h"
#include "gameoverwindow.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <QVBoxLayout>
#include <QDebug>
#include <QCloseEvent>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), socketDescriptor(-1) {

    this->resize(750, 550);

    //make a stack of widgets ( our pages )
    stackedWidget = new QStackedWidget(this);

    startPage = new StartPage(this);
    readyPage = new ReadyPage(this);
    playPage = new PlayPage(this);
    gameOverPage = new GameOverPage(this);

    stackedWidget->addWidget(startPage);
    stackedWidget->addWidget(readyPage);
    stackedWidget->addWidget(playPage);
    stackedWidget->addWidget(gameOverPage);

    setCentralWidget(stackedWidget);

    connect(readyPage, &ReadyPage::readyClicked, this, [this]() {
        if (socketDescriptor != -1) {
            ssize_t bytesWritten = write(socketDescriptor, "y", 1);
            if (bytesWritten == -1) {
                qDebug() << "Error writing to socket:" << strerror(errno);
        }
        }
    });

    connect(playPage, &PlayPage::answerSelected, this, [this](const QString &answer) {
        handleAnswer(answer);
    });

}

MainWindow::~MainWindow() {
    if (socketDescriptor != -1) {
        ::close(socketDescriptor);  
         qDebug() << "Socket closed";
    }
}

void MainWindow::setSocketDescriptor(int descriptor) {
    socketDescriptor = descriptor;
}

void MainWindow::showStartPage(const QString &message) {
    startPage->updateMessage(message);
    stackedWidget->setCurrentWidget(startPage);
}

void MainWindow::showReadyPage() {
    stackedWidget->setCurrentWidget(readyPage);
}

void MainWindow::showPlayPage(const QString &question) {
    playPage->updateQuestion(question);
    stackedWidget->setCurrentWidget(playPage);
}

void MainWindow::showGameOverPage(const QString &message) {
    gameOverPage->updateMessage(message);
    stackedWidget->setCurrentWidget(gameOverPage);
}

void MainWindow::handleAnswer(const QString &answer) {
    if (socketDescriptor != -1) {
        std::string msg = answer.toStdString();
        ssize_t bytesWritten = write(socketDescriptor, msg.c_str(), msg.size());
        if (bytesWritten == -1) {
            qDebug() << "Error writing to socket:" << strerror(errno);
        } 
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (socketDescriptor != -1) {
        ::close(socketDescriptor);  
         qDebug() << "Socket closed";
    }

    QApplication::quit();

    event->accept();
}
