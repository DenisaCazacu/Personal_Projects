#ifndef PLAYPAGE_H
#define PLAYPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

class PlayPage : public QWidget {
    Q_OBJECT

public:
    explicit PlayPage(QWidget *parent = nullptr);
    void updateQuestion(const QString &question);

signals:
    void answerSelected(const QString &answer);

private slots:
    void onTimeout();
    void onAnswerClicked();

private:
    QLabel *questionLabel;
    QPushButton *buttonA;
    QPushButton *buttonB;
    QPushButton *buttonC;
    QTimer *timer;
};

#endif // PLAYPAGE_H

