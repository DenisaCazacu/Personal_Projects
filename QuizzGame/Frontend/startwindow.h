#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QLabel>

class StartPage : public QWidget {
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);
    void updateMessage(const QString &message);

private:
    QLabel *label;
};

#endif // STARTPAGE_H
