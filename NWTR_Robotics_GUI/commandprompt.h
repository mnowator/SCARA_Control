#ifndef COMMANDPROMPT_H
#define COMMANDPROMPT_H

#include <QDialog>
#include "project.h"

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
    QString getTitle();

private:
    Ui::CommandPrompt *ui;

    const QString ownTextHTMLFormat = "<font color=\"chartreuse\">";
    const QString dataHTMLFormat = "<font color=\"white\">";
    const QString deviceHTMLFormat = "<font color=\"orange\">";
    const QString infoHTMLFormat = "<font color=\"red\">";

    QString m_title;

signals:
    void sendCommand(QString command);
    void establishConnection();
    void dropConnection();

public slots:
    void receiveCommand(QString command);
    void receiveProjectInfo(QString info);
    void projectChangeStateSlot(ProjectState state);

private slots:
    void sendButtonClicked();
    void connectButtonClicked();
    void disconnectButtonClicked();
};

#endif // COMMANDPROMPT_H
