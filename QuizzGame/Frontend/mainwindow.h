#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

class StartPage;
class ReadyPage;
class PlayPage;
class GameOverPage;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Q_INVOKABLE void showStartPage(const QString &message);
    Q_INVOKABLE void showReadyPage();
    Q_INVOKABLE void showPlayPage(const QString &question);
    Q_INVOKABLE void showGameOverPage(const QString &message);
    void handleAnswer(const QString &answer);

    void setSocketDescriptor(int descriptor);

private:
    QStackedWidget *stackedWidget;
    StartPage *startPage;
    ReadyPage *readyPage;
    PlayPage *playPage;
    GameOverPage *gameOverPage;

    int socketDescriptor;  
protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif 
