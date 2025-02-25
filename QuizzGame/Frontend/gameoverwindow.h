#ifndef GAMEOVERPAGE_H
#define GAMEOVERPAGE_H

#include <QWidget>
#include <QLabel>

class GameOverPage : public QWidget {
    Q_OBJECT

public:
    explicit GameOverPage(QWidget *parent = nullptr);
    void updateMessage(const QString &message);

private:
    QLabel *label;
};

#endif