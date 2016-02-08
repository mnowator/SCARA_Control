#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QTreeWidget>
#include <QHash>
#include <QSignalMapper>
#include <QFileSystemWatcher>

#include "scararobot.h"
#include "newprojectdialog.h"
#include "newfiledialog.h"
#include "savechangesdialog.h"
#include "renamefiledialog.h"
#include "removefiledialog.h"
#include "codeeditor.h"
#include "reloadfilesdialog.h"

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

    QString const imposibleDelimiter = "!@#$%^&*";

    QClipboard* m_clipboard;

    QSignalMapper* m_saveSignalMapper;
    QSignalMapper* m_renameSignalMapper;
    QSignalMapper* m_saveAsSignalMapper;
    QSignalMapper* m_setActiveMapper;
    QSignalMapper* m_closeSignalMapper;
    QSignalMapper* m_reloadSignalMapper;
    QSignalMapper* m_addNewSignalMapper;
    QSignalMapper* m_addExistSignalMapper;
    QSignalMapper* m_removeSignalMapper;
    QSignalMapper* m_runSignalMapper;
    QSignalMapper* m_pauseSignalMapper;
    QSignalMapper* m_stopSignalMapper;
    QSignalMapper* m_restartSignalMapper;
    QSignalMapper* m_textChangedMapper;
    QSignalMapper* m_saveProjectMapper;
    QSignalMapper* m_undoMapper;
    QSignalMapper* m_redoMapper;
    QSignalMapper* m_tabCloseMapper;
    QSignalMapper* m_openUrlMapper;
    QSignalMapper* m_determineUndoRedoMapper;
    QSignalMapper* m_determineCopyCutMapper;

    QFileSystemWatcher* m_fileSystemWatcher;

    Ui::MainWindow *ui;

    CodeEditor* m_undoRedoRegisteredItem;
    QList<CodeEditor*> m_redos;
    QList<CodeEditor*> m_undos;

    QHash<QString, ScaraRobot> m_projects;

    QString m_activeProject;

    NewProjectDialog* m_newProjectDialog;
    NewFileDialog* m_newFileDialog;
    SaveChangesDialog* m_saveChangesDialog;
    RenameFileDialog* m_renameFileDialog;
    RemoveFileDialog* m_removeFileDialog;
    ReloadFilesDialog* m_reloadFilesDialog;

    static bool lessThenTreeWidgetItem(QTreeWidgetItem* first, QTreeWidgetItem* second);

    void sortProjectFiles(QTreeWidgetItem* project);
    void closeEvent(QCloseEvent *event);
    void setActiveProject(QString const& projectName);
    void attachFileToProject(QString const& fileName, QString const& filePath, QString const& projectName, QString const& projectPath);
    void detachFileFromProject(QString const& fileName, QString const& projecName, QString const& projectPath);

    void saveFile(QString const& filePath, QString const& fileName, QString const& fileContent);
    QString loadFile(QString const& filePath, QString const& fileName);

private slots:

    void newProjectClicked();
    void openProjectProjectOrFile();
    void exitAppClicked();
    void closeAllProjectsClicked();
    void closeAllFilesClicked();
    void backClicked();
    void forwardClicked();
    void copyClicked();
    void cutClicked();
    void pasteClicked();
    void clipboardChange();
    void saveAllClicked();
    void selectAllClicked();
    void undoClicked();
    void redoClicked();
    void closeFileClicked();

    void deleteReloadFileDialog();

    void focusChanged(QWidget* old, QWidget* now);

    void createProject(QString const& projectName, QString const& communicationType, QString const& projectPath, QString const& projectType);

    void projectExplorerContextMenuRequested(QPoint const& pos);
    void fileEditorContextMenuRequested(QPoint const& pos);

    void projectExplorerDoubleClicked(QTreeWidgetItem* item, int column);

    void tabCloseClicked(int idx);
    void currentTabChanged(int idx);

    void renameClicked      (QString const& data);
    void saveClicked        (QString const& data);
    void saveAsClicked      (QString const& data);
    void setActiveClicked   (QString const& name);
    void closeClicked       (QString const& name);
    void reloadClicked      (QString const& data);
    void addNewClicked      (QString const& name);
    void addExistClicked    (QString const& name);
    void removeClicked      (QString const& data);
    void runClicked         (QString const& name);
    void pauseClicked       (QString const& name);
    void stopClicked        (QString const& name);
    void restartClicked     (QString const& name);
    void saveProjectClicked (QString const& name);
    void openUrlClicked     (QString const& url);

    void registerRedoStatus (QWidget* widget);
    void registerUndoStatus (QWidget* widget);
    void textChanged        (QWidget* widget);

    void updateRedoStatus   (bool available);
    void updateUndoStatus   (bool available);

    void determineIfUndoRedoIsAvailable(QWidget* widget);
    void determineIfCopyCutIsAvailable(QWidget* widget);

    void fileChanged(QString const& path);
    void reloadFiles(QList<QPair<QString, QString> > listOfFiles);

    void copyAvailable      (bool yes);
};

#endif // MAINWINDOW_H







