#ifndef POINTSEDITOR_H
#define POINTSEDITOR_H

#include <QTableWidget>

class PointsEditor : public QTableWidget
{
public:
    PointsEditor(QWidget* parent = 0);

    QString path;

signals:

public slots:
};

#endif // POINTSEDITOR_H
