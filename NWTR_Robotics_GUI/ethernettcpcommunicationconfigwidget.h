#ifndef ETHERNETTCPCOMMUNICATIONCONFIGWIDGET_H
#define ETHERNETTCPCOMMUNICATIONCONFIGWIDGET_H

#include <QWidget>
#include <QDomElement>

namespace Ui {
class EthernetTcpCommunicationConfigWidget;
}

class EthernetTcpCommunicationConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EthernetTcpCommunicationConfigWidget(QWidget *parent = 0);
    ~EthernetTcpCommunicationConfigWidget();

    bool populateFromDomElement(QDomElement const& root);
    void saveChanges(QDomDocument& dom);

private:
    Ui::EthernetTcpCommunicationConfigWidget *ui;

signals:
    void contentChanged();
};

#endif // ETHERNETTCPCOMMUNICATIONCONFIGWIDGET_H
