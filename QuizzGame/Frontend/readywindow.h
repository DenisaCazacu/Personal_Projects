#ifndef READYPAGE_H
#define READYPAGE_H

#include <QWidget>
#include <QPushButton>

class ReadyPage : public QWidget {
    Q_OBJECT

public:
    explicit ReadyPage(QWidget *parent = nullptr);

signals:
    void readyClicked();

private:
    QPushButton *readyButton;
};

#endif // READYPAGE_H

