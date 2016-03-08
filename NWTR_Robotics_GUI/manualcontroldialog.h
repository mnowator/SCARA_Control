#ifndef MANUALCONTROLLDIALOG_H
#define MANUALCONTROLLDIALOG_H

#include <QDialog>

namespace Ui {
class ManualControlDialog;
}

class ManualControlDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManualControlDialog(QWidget *parent = 0);
    ~ManualControlDialog();

    void setTitle(QString title);

private slots:
    void on_connectPushButton_clicked();
    void on_disconnectPushButton_clicked();
    void on_moveJogLeftFirstSegmentPushButton_pressed();
    void on_moveJogLeftFirstSegmentPushButton_released();
    void on_moveJogRightFirstSegmentPushButton_pressed();
    void on_moveJogRightFirstSegmentPushButton_released();
    void on_moveJogLeftSecondSegmentPushButton_pressed();
    void on_moveJogLeftSecondSegmentPushButton_released();
    void on_moveJogRightSecondSegmentPushButton_pressed();
    void on_moveJogRightSecondSegmentPushButton_released();
    void on_zPlusPushButton_released();
    void on_zPlusPushButton_pressed();
    void on_zMinusPushButton_pressed();
    void on_zMinusPushButton_released();

    void receiveCommand(QString command);
    void positionHasChanged(double x, double y, double z);
    void firstSegmentHomed();
    void firstSegmentNotHomed();
    void secondSegmentHomed();
    void secondSegmentNotHomed();
    void thirdSegmentHomed();
    void thirdSegmentNotHomed();
    void receiveProjectInfo(QString info);

    void on_sendPushButton_clicked();
    void on_homingFirstSegmentPushButon_clicked();
    void on_homingSecondSegmentPushButton_clicked();
    void on_homingThirdSegmentPushButton_clicked();
    void on_firstSegmentSpinBox_editingFinished();
    void on_secondSegmentSpinBox_editingFinished();
    void on_thirdSegmentSpinBox_editingFinished();

    void on_pickNPlacePushButton_clicked();

    void on_pickNPlacePushButton_pressed();

    void on_pickNPlacePushButton_released();

signals:
    void sendCommand(QString command);
    void establishConnection();
    void dropConnection();

private:
    Ui::ManualControlDialog *ui;

    const QString ownTextHTMLFormat = "<font color=\"chartreuse\">";
    const QString dataHTMLFormat = "<font color=\"white\">";
    const QString deviceHTMLFormat = "<font color=\"orange\">";
    const QString infoHTMLFormat = "<font color=\"red\">";

    QString m_title;

};

#endif // MANUALCONTROLLDIALOG_H
