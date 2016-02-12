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

    void setTitle(QString title);

private:
    Ui::CommandPrompt *ui;

    const QString ownTextHTMLFormat = "<font color=\"chartreuse\">";
    const QString dataHTMLFormat = "<font color=\"white\">";
    const QString controllerHTMLFormat = "<font color=\"orange\">";
    const QString infoHTMLFormat = "<font color=\"red\">";

    QString m_title;

signals:
    void sendCommand(QString command);
    void establishConnection();
    void dropConnection();

public slots:
    void receiveCommand(QString command);
    void receiveProjectInfo(QString info);

private slots:
    void sendButtonClicked();
    void connectButtonClicked();
    void disconnectButtonClicked();
};

#endif // COMMANDPROMPT_H
