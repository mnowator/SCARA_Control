#ifndef SCARASC1CONFIGWIDGET_H
#define SCARASC1CONFIGWIDGET_H

#include <QWidget>
#include <QDomElement>

namespace Ui {
class ScaraSC1ConfigWidget;
}

class ScaraSC1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ScaraSC1ConfigWidget(QWidget *parent = 0);
    ~ScaraSC1ConfigWidget();

    bool populateFromDomElement(QDomElement const& root);
    void saveChanges(QDomDocument& dom);

private:
    Ui::ScaraSC1ConfigWidget *ui;

private slots:
    void firstSegmentBeginOnCCWClicked();
    void firstSegmentBeginOnCWClicked();
    void secondSegmentBeginOnCCWClicked();
    void secondSegmentBeginOnCWClicked();
    void increasingZonCWClicked();
    void increasingZonCCWClicked();
    void thirdSegmentBeginOnCWClicked();
    void thirdSegmentBeginOnCCWClicked();

signals:
    void contentChanged();
};

#endif // SCARASC1CONFIGWIDGET_H
