#ifndef SCARASC1CONFIGWIDGET_H
#define SCARASC1CONFIGWIDGET_H

#include <QWidget>

namespace Ui {
class ScaraSC1ConfigWidget;
}

class ScaraSC1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScaraSC1ConfigWidget(QWidget *parent = 0);
    ~ScaraSC1ConfigWidget();

private:
    Ui::ScaraSC1ConfigWidget *ui;
};

#endif // SCARASC1CONFIGWIDGET_H
