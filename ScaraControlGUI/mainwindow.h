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
#include "savechangesdialog.h"

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

    QClipboard* m_clipboard;

    QSignalMapper* m_saveSignalMapper;
    QSignalMapper* m_renameSignalMapper;
    QSignalMapper* m_saveAsSignalMapper;
    QSignalMapper* m_setActiveMapper;
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
    QSignalMapper* m_textChangedMapper;
    QSignalMapper* m_saveProjectMapper;

    Ui::MainWindow *ui;

    QHash<QString, ScaraRobot> m_scaraRobots;

    QString m_activeProject;

    NewProjectDialog* m_newProjectDialog;
    NewFileDialog* m_newFileDialog;
    SaveChangesDialog* m_saveChangesDialog;

    void closeEvent(QCloseEvent *event);

    void setActiveProject(QString const& projectName);
    void attachFileToProject(QString const& fileName,QString const& filePath, QString const& projectName, QString const& projectPath);

    void saveFile(QString const& filePath, QString const& fileName, QString const& fileContent);
    QString loadFile(QString const& filePath, QString const& fileName);

private slots:

    void newProjectClicked();
    void openProjectProject();
    void exitAppClicked();
    void closeAllClicked();
    void backClicked();
    void forwardClicked();
    void copyClicked();
    void cutClicked();
    void pasteClicked();
    void clipboardChange();
    void saveAllClicked();

    void createProject(QString const& projectName, QString const& communicationType, QString const& projectPath);
    void projectExplorerContextMenuRequested(QPoint const& pos);
    void projectExplorerDoubleClicked(QTreeWidgetItem* item, int column);

    void tabCloseClicked(int idx);
    void currentTabChanged(int idx);

    void renameClicked      (QString const& name);
    void saveClicked        (QString const& _name);
    void saveAsClicked      (QString const& name="current");
    void setActiveClicked   (QString const& name);
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
    void saveProjectClicked (QString const& name);

    void textChanged        (QString const& name);
    void copyAvailable      (bool yes);
};

#endif // MAINWINDOW_H
