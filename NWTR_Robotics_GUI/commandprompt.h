#ifndef COMMANDPROMPT_H
#define COMMANDPROMPT_H

#include <QDialog>

namespace Ui {
class CommandPrompt;
}

class CommandPrompt : public QDialog
{
    Q_OBJECT

public:
    explicit CommandPrompt(QWidget *parent = 0);
    ~CommandPrompt();

private:
    Ui::CommandPrompt *ui;

    const QString ownTextHTMLFormat = "<font color=\"chartreuse\">";
    const QString dataHTMLFormat = "<font color=\"white\">";

signals:
    void sendCommand(QString command);

public slots:
    void receiveCommand(QString command);
    void receiveProjectInfo(QString info);

private slots:
    void sendButtonClicked();
};

#endif // COMMANDPROMPT_H
