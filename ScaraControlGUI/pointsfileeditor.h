#ifndef POINTSFILEEDITOR_H
#define POINTSFILEEDITOR_H

#include <QWidget>
#include <QSignalMapper>

namespace Ui {
class PointsFileEditor;
}

class PointsFileEditor : public QWidget
{
    Q_OBJECT

public:
    explicit PointsFileEditor(QWidget *parent = 0);
    ~PointsFileEditor();

    QString path;

private:
    Ui::PointsFileEditor *ui;

    QSignalMapper* removeRowMapper;

signals:
    void contentChanged();

private slots:

    void addButtonClicked();
};

#endif // POINTSFILEEDITOR_H
