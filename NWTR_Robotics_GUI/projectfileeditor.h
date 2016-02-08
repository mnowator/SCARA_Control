#ifndef PROJECTFILEEDITOR_H
#define PROJECTFILEEDITOR_H

#include <QWidget>
#include "serialcommunicationconfigwidget.h"
#include "scarasc1configwidget.h"

namespace Ui {
class ProjectFileEditor;
}

class ProjectFileEditor : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectFileEditor(QWidget *parent = 0);
    ~ProjectFileEditor();

    QString path;

    bool populateFromString(QString const& data);
    QString toStr();

signals:
    void contentChanged();
    void saveRequested();

private:
    Ui::ProjectFileEditor *ui;

    QDomDocument dom;

    SerialCommunicationConfigWidget* serialCommunicationConfigWidget;
    ScaraSC1ConfigWidget* scaraSC1ConfigWidget;
};

#endif // PROJECTFILEEDITOR_H
