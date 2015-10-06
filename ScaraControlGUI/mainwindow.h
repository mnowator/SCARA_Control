#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTreeWidget>
#include <QHash>
#include <QSignalMapper>

#include "scararobot.h"
#include "newprojectdialog.h"
#include "newfiledialog.h"

namespace Ui {
class MainWindow;
}

class Q_DECL_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    static const int ProjectType    = QTreeWidgetItem::UserType + 1;
    static const int FileType       = QTreeWidgetItem::UserType + 2;
    static const int ConfigType     = QTreeWidgetItem::UserType + 3;

    QSignalMapper* m_saveSignalMapper;
    QSignalMapper* m_saveAsSignalMapper;
    QSignalMapper* m_setActiveMapper;
    QSignalMapper* m_saveAllSignalMapper;
    QSignalMapper* m_closeSignalMapper;
    QSignalMapper* m_reloadSignalMapper;
    QSignalMapper* m_cloneSignalMapper;
    QSignalMapper* m_addNewSignalMapper;
    QSignalMapper* m_addExistSignalMapper;
    QSignalMapper* m_removeSignalMapper;
    QSignalMapper* m_runSignalMapper;
    QSignalMapper* m_pauseSignalMapper;
    QSignalMapper* m_stopSignalMapper;
    QSignalMapper* m_restartSignalMapper;

    Ui::MainWindow *ui;

    QHash<QString, ScaraRobot> m_projects;

    QString m_activeProject;

    NewProjectDialog* m_newProjectDialog;
    NewFileDialog* m_newFileDialog;

    void setActiveProject(QString const& projectName);
    void attachFileToProject(QString const& fileName, QString const& projectName);

private slots:

    void newProjectClicked();
    void openProjectProject();
    void exitAppClicked();
    void closeAllClicked();
    void createProject(QString const& projectName, QString const& communicationType, QString const& projectPath);
    void projectExplorerContextMenuRequested(QPoint const& pos);

    void saveClicked        (QString const& name);
    void saveAsClicked      (QString const& name);
    void setActiveClicked   (QString const& name);
    void saveAllClicked     (QString const& name);
    void closeClicked       (QString const& name);
    void reloadClicked      (QString const& name);
    void cloneClicked       (QString const& name);
    void addNewClicked      (QString const& name);
    void addExistClicked    (QString const& name);
    void removeClicked      (QString const& name);
    void runClicked         (QString const& name);
    void pauseClicked       (QString const& name);
    void stopClicked        (QString const& name);
    void restartClicked     (QString const& name);
};

#endif // MAINWINDOW_H
