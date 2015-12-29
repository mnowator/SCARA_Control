#ifndef SERIALCOMMUNICATIONCONFIGWIDGET_H
#define SERIALCOMMUNICATIONCONFIGWIDGET_H

#include <QWidget>
#include <QDomElement>

namespace Ui {
class SerialCommunicationConfigWidget;
}

class SerialCommunicationConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SerialCommunicationConfigWidget(QWidget *parent = 0);
    ~SerialCommunicationConfigWidget();

    bool populateFromDomElement(QDomElement const& root);

private:
    Ui::SerialCommunicationConfigWidget *ui;
};

#endif // SERIALCOMMUNICATIONCONFIGWIDGET_H
