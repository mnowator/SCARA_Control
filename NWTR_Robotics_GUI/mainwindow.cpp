#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDomDocument>
#include <QFileDialog>
#include <QClipboard>
#include <QMimeData>
#include <QKeyEvent>
#include <QCoreApplication>
#include <QLineEdit>
#include <QDesktopServices>
#include <QThreadPool>

#include "projectfileeditor.h"
#include "pointsfileeditor.h"
#include "styles.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->projectExplorer->hide();
    ui->fileEditor->hide();
    ui->logo->show();

    m_reloadFilesDialog = NULL;

    m_renameSignalMapper    =   new QSignalMapper(this);
    m_saveSignalMapper      =   new QSignalMapper(this);
    m_saveAsSignalMapper    =   new QSignalMapper(this);
    m_setActiveMapper       =   new QSignalMapper(this);
    m_closeSignalMapper     =   new QSignalMapper(this);
    m_reloadSignalMapper    =   new QSignalMapper(this);
    m_addNewSignalMapper    =   new QSignalMapper(this);
    m_addExistSignalMapper  =   new QSignalMapper(this);
    m_removeSignalMapper    =   new QSignalMapper(this);
    m_runSignalMapper       =   new QSignalMapper(this);
    m_pauseSignalMapper     =   new QSignalMapper(this);
    m_stopSignalMapper      =   new QSignalMapper(this);
    m_restartSignalMapper   =   new QSignalMapper(this);
    m_textChangedMapper     =   new QSignalMapper(this);
    m_saveProjectMapper     =   new QSignalMapper(this);
    m_redoMapper            =   new QSignalMapper(this);
    m_undoMapper            =   new QSignalMapper(this);
    m_tabCloseMapper        =   new QSignalMapper(this);
    m_openUrlMapper         =   new QSignalMapper(this);
    m_reloadSignalMapper    =   new QSignalMapper(this);
    m_determineUndoRedoMapper   =   new QSignalMapper(this);
    m_determineCopyCutMapper    =   new QSignalMapper(this);
    m_openCommandPromptMapper   =   new QSignalMapper(this);
    m_openManualControl     = new QSignalMapper(this);
    m_commandPromptFinishedMapper = new QSignalMapper(this);
    m_threadFinishedMapper = new QSignalMapper(this);

    m_fileSystemWatcher = new QFileSystemWatcher(this);
    connect(m_fileSystemWatcher,SIGNAL(fileChanged(QString)),this,SLOT(fileChanged(QString)));

    m_clipboard = QApplication::clipboard();
    connect((QObject*)m_clipboard,SIGNAL(dataChanged()),this,SLOT(clipboardChange()));
    clipboardChange();

    connect(m_saveSignalMapper,     SIGNAL(mapped(QString)),this,SLOT(saveClicked       (QString)));
    connect(m_saveAsSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(saveAsClicked     (QString)));
    connect(m_setActiveMapper,      SIGNAL(mapped(QString)),this,SLOT(setActiveClicked  (QString)));
    connect(m_closeSignalMapper,    SIGNAL(mapped(QString)),this,SLOT(closeClicked      (QString)));
    connect(m_reloadSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(reloadClicked     (QString)));
    connect(m_addNewSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(addNewClicked     (QString)));
    connect(m_addExistSignalMapper, SIGNAL(mapped(QString)),this,SLOT(addExistClicked   (QString)));
    connect(m_removeSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(removeClicked     (QString)));
    connect(m_runSignalMapper,      SIGNAL(mapped(QString)),this,SLOT(runClicked        (QString)));
    connect(m_pauseSignalMapper,    SIGNAL(mapped(QString)),this,SLOT(pauseClicked      (QString)));
    connect(m_stopSignalMapper,     SIGNAL(mapped(QString)),this,SLOT(stopClicked       (QString)));
    connect(m_restartSignalMapper,  SIGNAL(mapped(QString)),this,SLOT(restartClicked    (QString)));
    connect(m_saveProjectMapper,    SIGNAL(mapped(QString)),this,SLOT(saveProjectClicked(QString)));
    connect(m_renameSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(renameClicked     (QString)));
    connect(m_openUrlMapper,        SIGNAL(mapped(QString)),this,SLOT(openUrlClicked    (QString)));
    connect(m_reloadSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(reloadClicked     (QString)));
    connect(m_openCommandPromptMapper,SIGNAL(mapped(QString)),this,SLOT(openCommandPrompt(QString)));
    connect(m_openManualControl,    SIGNAL(mapped(QString)),this,SLOT(openManualControl(QString)));
    connect(m_commandPromptFinishedMapper, SIGNAL(mapped(QString)),this,SLOT(commandPromptFinished(QString)));
    connect(m_threadFinishedMapper, SIGNAL(mapped(QString)),this,SLOT(threadFinished(QString)));

    connect(m_redoMapper,           SIGNAL(mapped(QWidget*)),this,SLOT(registerRedoStatus(QWidget*)));
    connect(m_undoMapper,           SIGNAL(mapped(QWidget*)),this,SLOT(registerUndoStatus(QWidget*)));
    connect(m_textChangedMapper,    SIGNAL(mapped(QWidget*)),this,SLOT(textChanged       (QWidget*)));
    connect(m_determineUndoRedoMapper,  SIGNAL(mapped(QWidget*)),this,SLOT(determineIfUndoRedoIsAvailable(QWidget*)));
    connect(m_determineCopyCutMapper,   SIGNAL(mapped(QWidget*)),this,SLOT(determineIfCopyCutIsAvailable(QWidget*)));

    connect(m_tabCloseMapper,       SIGNAL(mapped(int)),this,SLOT(tabCloseClicked(int)));

    m_saveSignalMapper->setMapping(ui->actionSave,"current");
    connect(ui->actionSave,SIGNAL(triggered(bool)),m_saveSignalMapper,SLOT(map()));

    m_saveAsSignalMapper->setMapping(ui->actionSave_as,"current");
    connect(ui->actionSave_as,SIGNAL(triggered(bool)),m_saveAsSignalMapper,SLOT(map()));

    m_reloadSignalMapper->setMapping(ui->actionReload,"current");
    connect(ui->actionReload,SIGNAL(triggered(bool)),m_reloadSignalMapper,SLOT(map()));

    m_runSignalMapper->setMapping(ui->actionRun,"current");
    connect(ui->actionRun,SIGNAL(triggered(bool)),m_runSignalMapper,SLOT(map()));

    m_pauseSignalMapper->setMapping(ui->actionPause,"current");
    connect(ui->actionPause,SIGNAL(triggered(bool)),m_pauseSignalMapper,SLOT(map()));

    m_stopSignalMapper->setMapping(ui->actionStop,"current");
    connect(ui->actionRestart,SIGNAL(triggered(bool)),m_restartSignalMapper,SLOT(map()));

    m_restartSignalMapper->setMapping(ui->actionRestart,"current");
    connect(ui->actionRestart,SIGNAL(triggered(bool)),m_restartSignalMapper,SLOT(map()));

    connect(ui->actionNew_Project,          SIGNAL(triggered(bool)),this,SLOT(newProjectClicked()));
    connect(ui->actionExit,                 SIGNAL(triggered(bool)),this,SLOT(exitAppClicked()));
    connect(ui->actionOpen_Project_or_File, SIGNAL(triggered(bool)),this,SLOT(openProjectProjectOrFile()));
    connect(ui->actionCloseAllProjects,     SIGNAL(triggered(bool)),this,SLOT(closeAllProjectsClicked()));
    connect(ui->actionBack,                 SIGNAL(triggered(bool)),this,SLOT(backClicked()));
    connect(ui->actionForward,              SIGNAL(triggered(bool)),this,SLOT(forwardClicked()));
    connect(ui->actionCopy,                 SIGNAL(triggered(bool)),this,SLOT(copyClicked()));
    connect(ui->actionCut,                  SIGNAL(triggered(bool)),this,SLOT(cutClicked()));
    connect(ui->actionPaste,                SIGNAL(triggered(bool)),this,SLOT(pasteClicked()));
    connect(ui->actionCloseAllFiles,        SIGNAL(triggered(bool)),this,SLOT(closeAllFilesClicked()));
    connect(ui->actionSaveAll,              SIGNAL(triggered(bool)),this,SLOT(saveAllClicked()));
    connect(ui->actionRedo,                 SIGNAL(triggered(bool)),this,SLOT(redoClicked()));
    connect(ui->actionUndo,                 SIGNAL(triggered(bool)),this,SLOT(undoClicked()));
    connect(ui->actionSelect_All,           SIGNAL(triggered(bool)),this,SLOT(selectAllClicked()));
    connect(ui->actionClose_File,           SIGNAL(triggered(bool)),this,SLOT(closeFileClicked()));
    connect(ui->actionOptions,              SIGNAL(triggered(bool)),this,SLOT(optionsClicked()));

    connect(ui->projectExplorer, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(projectExplorerDoubleClicked(QTreeWidgetItem*,int)));
    connect(ui->projectExplorer, SIGNAL(customContextMenuRequested(QPoint)),     this,SLOT(projectExplorerContextMenuRequested(QPoint)));

    connect(ui->fileEditor,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseClicked(int)));
    connect(ui->fileEditor,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
    connect(ui->fileEditor,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(fileEditorContextMenuRequested(QPoint)));

    connect(qApp,SIGNAL(focusChanged(QWidget*,QWidget*)),this,SLOT(focusChanged(QWidget*,QWidget*)));

    qApp->setStyleSheet(currentToolTipTheme);

    this->setStyleSheet(currentWindowTheme);
    this->menuBar()->setStyleSheet(currentMenuBarTheme);
    this->ui->fileEditor->setStyleSheet(currentTabWidgetTheme);
    this->ui->projectExplorer->setStyleSheet(currentTreeWigetTheme);
    this->ui->projectExplorer->header()->setStyleSheet(currentHeaderTheme);
    this->ui->logo->setStyleSheet(currentLogoTheme);
}

MainWindow::~MainWindow()
{
    delete m_closeSignalMapper;
    delete m_reloadSignalMapper;
    delete m_removeSignalMapper;
    delete m_runSignalMapper;
    delete m_pauseSignalMapper;
    delete m_stopSignalMapper;
    delete m_restartSignalMapper;

    delete ui;
}

bool MainWindow::lessThenTreeWidgetItem(QTreeWidgetItem *first, QTreeWidgetItem *second)
{
    QString selfCp = first->text(0);
    QString otherCp = second->text(0);
    QString selfExtension;
    QString otherExtension;
    QString selfName;
    QString otherName;

    bool returnValue;

    unsigned selfRating = 0;
    unsigned otherRating = 0;

    while ( selfCp[selfCp.length()-1] == '*' )
        selfCp = selfCp.left(selfCp.length() -1);

    while ( otherCp[otherCp.length()-1] == '*' )
        otherCp = otherCp.left(otherCp.length() -1);

    for ( unsigned i=selfCp.length()-1;i != 0; --i )
        if ( selfCp[i] == '.' )
        {
            selfExtension = selfCp.right(selfCp.length()-i);
            selfName = selfCp.left(i);
        }

    for ( unsigned i=otherCp.length()-1;i != 0; --i )
        if ( otherCp[i] == '.' )
        {
            otherExtension = otherCp.right(otherCp.length()-i);
            otherName = otherCp.left(i);
        }

    if ( selfExtension == ".pro" )
        selfRating = 4;
    else if ( selfExtension == ".js" )
        selfRating = 3;
    else if ( selfExtension == ".pt" )
        selfRating = 2;
    else
        selfRating = 1;

    if ( otherExtension == ".pro" )
        otherRating = 4;
    else if ( otherExtension == ".js" )
        otherRating = 3;
    else if ( otherExtension == ".pt" )
        otherRating = 2;
    else
        otherRating = 1;

    if ( selfRating == otherRating )
        returnValue = selfName > otherName;
    else
        returnValue = selfRating < otherRating;


    if ( returnValue )
    {
        QTreeWidgetItem* clone = first->clone();

        first->setText(0,second->text(0));
        first->setText(1,second->text(1));
        first->setText(2,second->text(2));

        first->setIcon(0,second->icon(0));

        second->setText(0,clone->text(0));
        second->setText(1,clone->text(1));
        second->setText(2,clone->text(2));

        second->setIcon(0,clone->icon(0));
    }

    return returnValue;
}

void MainWindow::sortProjectFiles(QTreeWidgetItem *project)
{
    QList<QTreeWidgetItem*> files;

    if ( project )
        for ( unsigned i=0; i<project->childCount(); ++i )
            files.append(project->child(i));
    else
        return;

    for ( unsigned i=0; i<files.length(); ++i )
    {
        for ( unsigned j=0; j<files.length()-1; ++j )
        {
            lessThenTreeWidgetItem(files.at(j), files.at(j+1));
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    bool prevent = false;

    m_saveChangesDialog = new SaveChangesDialog(this);

    for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
    {
        if ( ui->fileEditor->tabText(i).endsWith('*') )
        {
            CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(i));
            ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(i));
            PointsFileEditor* pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(i));

            if ( codeEditor )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/pythonfile.png"),
                                             ui->fileEditor->tabText(i),
                                             codeEditor->path);

                prevent = true;
            }
            else if ( projectFileEditor )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/profile.png"),
                                             ui->fileEditor->tabText(i),
                                             projectFileEditor->path);

                prevent = true;
            }
            else if ( pointsFileEditor )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/pointsfile.png"),
                                             ui->fileEditor->tabText(i),
                                             pointsFileEditor->path);

                prevent = true;
            }
        }
    }

    if ( !prevent )
        event->accept();
    else
    {
        if ( m_saveChangesDialog->exec() )
        {
            for ( QPair<QString, QString> fileNpath : m_saveChangesDialog->getSelectedFiles() )
                saveClicked(fileNpath.first+imposibleDelimiter+fileNpath.second);

            event->accept();
        }
        else event->ignore();
    }
}

void MainWindow::setActiveProject(const QString &projectName)
{
    if ( !m_activeProject.isEmpty() )
        for ( unsigned index=0; index<ui->projectExplorer->topLevelItemCount(); ++index)
        {
            QTreeWidgetItem* item = ui->projectExplorer->topLevelItem(index);

            if ( item->text(0).length() > item->text(2).length() )
            {
                if ( item->text(0) == projectName )
                    return;

                QString tmpStr = item->text(0);
                QFont font=item->font(0);

                font.setBold(false);
                item->setFont(0,font);
                item->setText(0,item->text(2));
                item->setText(2, tmpStr);
            }
        }

    foreach ( QTreeWidgetItem* project, ui->projectExplorer->findItems(projectName,Qt::MatchExactly,0))
    {
        QFont projectFont = project->font(0);
        QString tempStr = project->text(0);

        m_activeProject = projectName;
        projectFont.setBold(true);
        project->setFont(0,projectFont);
        project->setText(0,project->text(2));
        project->setText(2,tempStr);
        project->font(0);
    }
}

void MainWindow::attachFileToProject(const QString &fileName, const QString &filePath, const QString &projectName, const QString &projectPath)
{
    QDomDocument domProject;
    QDomElement root, filesElement, fileElement, nameElement, pathElement;
    QString errorStr, projectFileContent;
    int errorLine, errorColumn;

    projectFileContent = loadFile(projectPath,projectName);

    if ( projectFileContent.isEmpty() ) return;

    if (!domProject.setContent(projectFileContent, false, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }

    root = domProject.documentElement();

    if ( root.tagName() != "Project")
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'Project' tag in .pro file."));

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }


        filesElement = root.namedItem("Files").toElement();

        if ( filesElement.isNull() )
           filesElement = domProject.createElement("Files");
        else
        {
            for ( QDomElement file = filesElement.firstChildElement("File"); !file.isNull(); file = file.nextSiblingElement("File"))
            {
                if ( file.namedItem("Name").toElement().text() == fileName )
                    return;
            }
        }

            fileElement = domProject.createElement("File");
                nameElement = domProject.createElement("Name");
                nameElement.appendChild(domProject.createTextNode(fileName));

                pathElement = domProject.createElement("Path");
                pathElement.appendChild(domProject.createTextNode(filePath));

            fileElement.appendChild(nameElement);
            fileElement.appendChild(pathElement);

        filesElement.appendChild(fileElement);

    root.appendChild(filesElement);

    saveFile(projectPath,projectName,domProject.toString());
}

void MainWindow::detachFileFromProject(const QString &fileName, const QString &projectName, const QString &projectPath)
{
    QDomDocument domProject;
    QDomElement filesElement, root;
    QString projectFileContent, errorStr;
    int errorLine, errorColumn;

    projectFileContent = loadFile(projectPath,projectName);

    if ( projectFileContent.isEmpty() ) return;

    if (!domProject.setContent(projectFileContent, false, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }

    root = domProject.documentElement();

    if ( root.tagName() != "Project")
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'Project' tag in .pro file."));

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }

    filesElement = root.namedItem("Files").toElement();
    for ( QDomElement file = filesElement.firstChildElement("File"); !file.isNull(); file = file.nextSiblingElement("File") )
    {
        QDomElement name = file.namedItem("Name").toElement();

        if ( name.text() == fileName )
            filesElement.removeChild(file);
    }

    root.appendChild(filesElement);

    saveFile(projectPath,projectName,domProject.toString());
}

void MainWindow::saveFile(const QString &filePath, const QString &fileName, const QString &fileContent)
{
    m_fileSystemWatcher->removePath(filePath+'/'+fileName);

    QFile file(filePath+'/'+fileName);

    if ( file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream ts(&file);
        ts << fileContent;

        file.close();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot save file.\n\n")+
                                                                 tr("Path: ") +filePath+"\n"+
                                                                 tr("Name: ") +fileName);

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
    }
    m_fileSystemWatcher->addPath(filePath+'/'+fileName);
}

QString MainWindow::loadFile(const QString &filePath, const QString &fileName)
{
    QFile file(filePath+fileName);

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QTextStream textStream(&file);

        return textStream.readAll();
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot load file.\n\n")+
                                                                 tr("Path: ") +filePath+"\n"+
                                                                 tr("Name: ") +fileName);

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return QString();
    }
}

void MainWindow::newProjectClicked()
{
    m_newProjectDialog = new NewProjectDialog(this);
    connect(m_newProjectDialog,SIGNAL(createProjectSignal(QString,QString,QString,QString)),this,SLOT(createProject(QString,QString,QString,QString)));
    m_newProjectDialog->show();
}

void MainWindow::openProjectProjectOrFile()
{
    QString fullPath, fileName, pureFileName, filePath;
    QFileDialog folderDialog;
    QDomDocument dom;
    QDomElement root;

    QStringList filters;
    filters << "Openable (*.pro *.js *.pt)"
            << "Project files (*.pro)"
            << "Javascript files (*.js)"
            << "Points files (*.pt)"
            << "Any files (*)";

    folderDialog.setNameFilters(filters);
    folderDialog.setDirectory(QDir::home());

    if ( folderDialog.exec() == QDialog::Accepted )
    {
        fullPath = folderDialog.selectedFiles()[0];
    }
    else return;

    for (unsigned i=fullPath.length();i!=0;--i)
    {
        if ( fullPath[i-1] == '\\' || fullPath[i-1] == '/' )
        {
            filePath = fullPath.left(i);
            fileName = fullPath.right(fullPath.length()-i);
            pureFileName = fileName.split('.')[0];
            break;
        }
    }

    if ( fileName.endsWith(".pro") )
    {
        if ( m_projects.contains(pureFileName))
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project with given name is already loaded.\n"
                                                                     "Please change name or close another project."));

            msgBox.setStyleSheet(currentErrorBoxTheme);

            msgBox.exec();
            return;
        }

        QFile file(fullPath);

        if ( file.open(QIODevice::ReadWrite))
        {
            QString errorStr;
            int errorLine;
            int errorColumn;

            if (!dom.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                return;
            }
        }
        else
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot open file."));

            msgBox.setStyleSheet(currentErrorBoxTheme);

            msgBox.exec();
            return;
        }

        root = dom.documentElement();
        if ( root.tagName() != "Project")
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'Project' tag in .pro file."));

            msgBox.setStyleSheet(currentErrorBoxTheme);

            msgBox.exec();
            return;
        }

        QTreeWidgetItem* project = new QTreeWidgetItem(ProjectType);
        project->setText(0,pureFileName);
        project->setText(1,filePath);
        project->setText(2,pureFileName + tr(" ( Active )"));
        project->setIcon(0,*(new QIcon(":/new/icons/lc_dbformopen.png")));

        QTreeWidgetItem* projectProFile = new QTreeWidgetItem(ConfigType);
        projectProFile->setText(0,fileName);
        projectProFile->setText(1,filePath);
        projectProFile->setText(2,fileName + "*");
        projectProFile->setIcon(0,*(new QIcon(":/new/icons/profile.png")));
        project->addChild(projectProFile);

        QDomElement files = root.namedItem("Files").toElement();
        for ( QDomElement file = files.firstChildElement("File"); !file.isNull(); file = file.nextSiblingElement("File") )
        {
            QTreeWidgetItem* fileTreeWidgetItem = new QTreeWidgetItem(FileType);
            QDomElement name = file.namedItem("Name").toElement();
            QDomElement path = file.namedItem("Path").toElement();

            if ( !name.isNull() )
            {
                fileTreeWidgetItem->setText(0,name.text());
                fileTreeWidgetItem->setText(2,name.text()+'*');
            }
            else
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project file is corrupted\n"
                                                                         "'Name' tag is missing in one file."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                continue;
            }

            if ( !path.isNull() )
                fileTreeWidgetItem->setText(1,path.text());
            else
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project file is corrupted\n"
                                                                         "'Path' tag is missing in one file."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                continue;
            }

            if ( name.text().endsWith(".js") )
                fileTreeWidgetItem->setIcon(0,*(new QIcon(":/new/icons/pythonfile.png")));
            else if ( name.text().endsWith(".pt") )
                fileTreeWidgetItem->setIcon(0,*(new QIcon(":/new/icons/pointsfile.png")));
            else
                fileTreeWidgetItem->setIcon(0,*(new QIcon(":/new/icons/lc_adddirect.png")));


            project->addChild(fileTreeWidgetItem);
        }

        ui->projectExplorer->addTopLevelItem(project);

        m_projects[pureFileName] = new Project();
        m_threads[pureFileName] = new QThread();

        setActiveProject(pureFileName);
        sortProjectFiles(project);

        ui->projectExplorer->show();

        ui->actionCloseAllProjects->setEnabled(true);
        ui->actionRun->setEnabled(true);

        file.close();
    }
    else if ( fileName.endsWith(".js"))
    {
        // If file already is loaded
        for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
        {
            if ( ui->fileEditor->tabText(i) == fileName ||
                 ui->fileEditor->tabText(i) == fileName+'*' )
            {
                CodeEditor* codeEditor = dynamic_cast<CodeEditor*>( ui->fileEditor->widget(i));

                if ( codeEditor->path == filePath )
                {
                    ui->fileEditor->setCurrentWidget(codeEditor);
                    return;
                }
            }
        }

        // If not, lets do it
        QFile file(fullPath);

        // If it is not opened correctly
        if ( !file.open(QIODevice::ReadWrite  | QFile::Text ))
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), "Cannot open this file.");

            msgBox.setStyleSheet(currentErrorBoxTheme);

            msgBox.exec();
            return;
        }
        else
        {
            CodeEditor* codeEditor = new CodeEditor(this);
            QTextStream textStream(&file);

            m_fileSystemWatcher->addPath(fullPath);

            codeEditor->turnOnPythonHighlighting();

            codeEditor->document()->setPlainText(textStream.readAll());
            codeEditor->path = filePath;

            m_textChangedMapper->setMapping(codeEditor,codeEditor);
            connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
            connect(codeEditor,SIGNAL(copyAvailable(bool)),this,SLOT(copyAvailable(bool)));

            m_redoMapper->setMapping(codeEditor,codeEditor);
            connect(codeEditor,SIGNAL(redoAvailable(bool)),m_redoMapper,SLOT(map()));

            m_undoMapper->setMapping(codeEditor,codeEditor);
            connect(codeEditor,SIGNAL(undoAvailable(bool)),m_undoMapper,SLOT(map()));

            connect(codeEditor,SIGNAL(redoAvailable(bool)),this,SLOT(updateRedoStatus(bool)));
            connect(codeEditor,SIGNAL(undoAvailable(bool)),this,SLOT(updateUndoStatus(bool)));

            ui->fileEditor->addTab(codeEditor,QIcon(":/new/icons/pythonfile.png"),fileName);
            ui->fileEditor->setCurrentWidget(codeEditor);

            ui->actionReload->setEnabled(true);
        }
    }
    else if ( fileName.endsWith(".pt"))
    {
        // If file already is loaded
        for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
        {
            if ( ui->fileEditor->tabText(i) == fileName ||
                 ui->fileEditor->tabText(i) == fileName+'*' )
            {
                PointsFileEditor* pointsEditor = dynamic_cast<PointsFileEditor*>( ui->fileEditor->widget(i));

                if ( pointsEditor->path == filePath )
                {
                    ui->fileEditor->setCurrentWidget(pointsEditor);
                    return;
                }
            }
        }

        // If not, lets do it
        QFile file(fullPath);

        // If it is not opened correctly
        if ( !file.open(QIODevice::ReadWrite  | QFile::Text ))
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), "Cannot open this file.");

            msgBox.setStyleSheet(currentErrorBoxTheme);

            msgBox.exec();
            return;
        }
        else
        {
            m_fileSystemWatcher->addPath(fullPath);
            PointsFileEditor* pointsEditor = new PointsFileEditor(this);

            ui->fileEditor->addTab(pointsEditor,QIcon(":/new/icons/pointsfile.png"),fileName);
            ui->fileEditor->setCurrentWidget(pointsEditor);

            ui->actionReload->setEnabled(true);
        }
    }
}

void MainWindow::exitAppClicked()
{
    close();
}

void MainWindow::closeAllFilesClicked()
{
    bool prevent = false;

    m_saveChangesDialog = new SaveChangesDialog(this);

    for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
    {
        if ( ui->fileEditor->tabText(i).endsWith('*') )
        {
            CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(i));
            ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(i));
            PointsFileEditor* pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(i));

            if ( codeEditor )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/pythonfile.png"),
                                             ui->fileEditor->tabText(i),
                                             codeEditor->path);

                prevent = true;
            }
            else if ( projectFileEditor )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/profile.png"),
                                             ui->fileEditor->tabText(i),
                                             projectFileEditor->path);

                prevent = true;
            }
            else if ( pointsFileEditor )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/pointsfile.png"),
                                             ui->fileEditor->tabText(i),
                                             pointsFileEditor->path);

                prevent = true;
            }
        }
    }

    if ( prevent )
    {
        if ( m_saveChangesDialog->exec() )
        {
            for ( QPair<QString, QString> fileNpath : m_saveChangesDialog->getSelectedFiles() )
                saveClicked(fileNpath.first+imposibleDelimiter+fileNpath.second);
        }
        else
            return;
    }

    ui->fileEditor->clear();

    for ( unsigned i=0; i<ui->projectExplorer->topLevelItemCount(); ++i )
    {
        QTreeWidgetItem* topLevelItem = ui->projectExplorer->topLevelItem(i);

        for ( unsigned j=0; j<topLevelItem->childCount(); ++j)
        {
            QTreeWidgetItem* child = topLevelItem->child(j);

            if ( child->text(0).endsWith("*") )
            {
                QString tmpStr = child->text(0);

                child->setText(0,child->text(2));
                child->setText(2,tmpStr);
            }
        }
    }

    ui->actionSaveAll->setEnabled(false);
    ui->actionCloseAllFiles->setEnabled(false);
}

void MainWindow::closeAllProjectsClicked()
{
    ui->projectExplorer->clear();
    ui->projectExplorer->hide();

    ui->actionCloseAllProjects->setEnabled(false);
    ui->actionRun->setEnabled(false);
}

void MainWindow::backClicked()
{
    ui->fileEditor->setCurrentIndex(ui->fileEditor->currentIndex()-1);
}

void MainWindow::forwardClicked()
{
    ui->fileEditor->setCurrentIndex(ui->fileEditor->currentIndex()+1);
}

void MainWindow::copyClicked()
{
    QWidget* focused = QApplication::focusWidget();
    if( focused != 0 )
    {
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyPress,
                                                Qt::Key_C,
                                                Qt::ControlModifier ));
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyRelease,
                                                Qt::Key_C,
                                                Qt::ControlModifier ));
    }
}

void MainWindow::cutClicked()
{
    QWidget* focused = QApplication::focusWidget();

    if( focused != 0 )
    {
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyPress,
                                                Qt::Key_X,
                                                Qt::ControlModifier ));
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyRelease,
                                                Qt::Key_X,
                                                Qt::ControlModifier ));
    }
}

void MainWindow::pasteClicked()
{
    QWidget* focused = QApplication::focusWidget();

    if( focused != 0 )
    {
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyPress,
                                                Qt::Key_V,
                                                Qt::ControlModifier ));
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyRelease,
                                                Qt::Key_V,
                                                Qt::ControlModifier ));
    }
}

void MainWindow::clipboardChange()
{
    if ( m_clipboard->text().isEmpty() )
        ui->actionPaste->setEnabled(false);
}

void MainWindow::saveAllClicked()
{
    for ( unsigned i=0; i< ui->fileEditor->count(); ++i)
    {
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(i));

        if ( codeEditor )
        {
            saveClicked(ui->fileEditor->tabText(i)+imposibleDelimiter+codeEditor->path);
        }
    }

    ui->actionSaveAll->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
}

void MainWindow::selectAllClicked()
{
    QWidget* focused = QApplication::focusWidget();

    if( focused != 0 )
    {
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyPress,
                                                Qt::Key_A,
                                                Qt::ControlModifier ));
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyRelease,
                                                Qt::Key_A,
                                                Qt::ControlModifier ));
    }
}

void MainWindow::undoClicked()
{
    QWidget* focused = QApplication::focusWidget();

    if( focused != 0 )
    {
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyPress,
                                                Qt::Key_Z,
                                                Qt::ControlModifier ));
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyRelease,
                                                Qt::Key_Z,
                                                Qt::ControlModifier ));
    }
}

void MainWindow::redoClicked()
{
    QWidget* focused = QApplication::focusWidget();

    if( focused != 0 )
    {
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyPress,
                                                Qt::Key_Y,
                                                Qt::ControlModifier ));
        QApplication::postEvent( focused,
                                 new QKeyEvent( QEvent::KeyRelease,
                                                Qt::Key_Y,
                                                Qt::ControlModifier ));
    }
}

void MainWindow::focusChanged(QWidget *old, QWidget *now)
{
    CodeEditor* codeEditor;
    QLineEdit* nowLineEdit;
    QLineEdit* oldLineEdit;

    codeEditor = dynamic_cast<CodeEditor*>(now);
    nowLineEdit = dynamic_cast<QLineEdit*>(now);
    oldLineEdit = dynamic_cast<QLineEdit*>(old);

    if ( codeEditor )
    {
        if ( !codeEditor->toPlainText().isEmpty() )
            ui->actionSelect_All->setEnabled(true);
        else
            ui->actionSelect_All->setEnabled(false);

        if ( codeEditor->canPaste() )
        {
            if ( !m_clipboard->text().isEmpty() )
                ui->actionPaste->setEnabled(true);
            else
                ui->actionPaste->setEnabled(false);
        }
        else
            ui->actionPaste->setEnabled(false);

        if ( m_redos.contains(codeEditor))
            ui->actionRedo->setEnabled(true);
        else
            ui->actionRedo->setEnabled(false);

        if ( m_undos.contains(codeEditor))
            ui->actionUndo->setEnabled(true);
        else
            ui->actionUndo->setEnabled(false);
    }
    else if ( nowLineEdit )
    {
        m_determineUndoRedoMapper->setMapping(nowLineEdit,nowLineEdit);
        m_determineCopyCutMapper->setMapping(nowLineEdit,nowLineEdit);
        connect(nowLineEdit,SIGNAL(selectionChanged()),m_determineCopyCutMapper,SLOT(map()));
        connect(nowLineEdit,SIGNAL(textChanged(QString)),m_determineUndoRedoMapper,SLOT(map()));


        if ( !nowLineEdit->text().isEmpty() )
            ui->actionSelect_All->setEnabled(true);
        else
            ui->actionSelect_All->setEnabled(true);

        if ( !m_clipboard->text().isEmpty() )
            ui->actionPaste->setEnabled(true);
        else
            ui->actionPaste->setEnabled(false);

        if ( nowLineEdit->isUndoAvailable() )
            ui->actionUndo->setEnabled(true);
        else
            ui->actionUndo->setEnabled(false);

        if ( nowLineEdit->isRedoAvailable() )
            ui->actionRedo->setEnabled(true);
        else
            ui->actionRedo->setEnabled(false);
    }
    else
    {
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionPaste->setEnabled(false);
        ui->actionSelect_All->setEnabled(false);
    }

    if ( oldLineEdit )
    {
        m_determineUndoRedoMapper->removeMappings(oldLineEdit);
        m_determineCopyCutMapper->removeMappings(oldLineEdit);
        disconnect(oldLineEdit,SIGNAL(textChanged(QString)),m_determineUndoRedoMapper,SLOT(map()));
        disconnect(oldLineEdit,SIGNAL(selectionChanged()),m_determineCopyCutMapper,SLOT(map()));
    }
}

void MainWindow::createProject(QString const& projectName, QString const& communicationType, QString const& projectPath, const QString &projectType)
{
    QString fileFullPath = projectPath+'/'+projectName+'/'+projectName+".pro";
    QDir dir(projectPath+'/'+projectName);

    if ( m_projects.contains(projectName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project with given name is already loaded.\n"
                                                                 "Please change name or close another project."));

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }

    if ( !dir.exists())
        dir.mkpath(projectPath+'/'+projectName);


    if ( QFile::exists(fileFullPath))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot create project file because it already exist in given path.\n"
                                                                 "Please choose different localization to project or project name."));

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }

    QDomDocument dom("NWTR_Robotics");
    QDomElement elHigher, elLower, root;

    root = dom.createElement("Project");
    dom.appendChild(root);

    if ( projectType == "SCARA - SC1" )
    {
        root.setAttribute("project_type", projectType);

        elHigher = dom.createElement("MainProgramFile");
        root.appendChild(elHigher);

        elHigher = dom.createElement("MainPointsFile");
        root.appendChild(elHigher);

        elHigher = dom.createElement("SyncFreq");
        elHigher.appendChild(dom.createTextNode("30"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("LengthOfFirstSegment");
        elHigher.appendChild(dom.createTextNode("30"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("LengthOfSecondSegment");
        elHigher.appendChild(dom.createTextNode("30"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("CorrectionValue");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("FirstSegmentAngleOnCw");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("FirstSegmentAngleOnCCW");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("NumberOfStepsForFirstSegment");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("SecondSegmentAngleOnCw");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("SecondSegmentAngleOnCCW");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("NumberOfStepsForSecondSegment");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("FirstSegmentBeginOn");
        elHigher.appendChild(dom.createTextNode("CW"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("SecondSegmentBeginOn");
        elHigher.appendChild(dom.createTextNode("CW"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("DistanceBetweenTwoLimtsOnZ");
        elHigher.appendChild(dom.createTextNode("30"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("LengthOfThirdSegment");
        elHigher.appendChild(dom.createTextNode("31"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("NumberOfStepsBetweenLimitsOnZ");
        elHigher.appendChild(dom.createTextNode("0"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("IncreasingZOn");
        elHigher.appendChild(dom.createTextNode("CW"));
        root.appendChild(elHigher);

        elHigher = dom.createElement("ThirdSegmentBeginOn");
        elHigher.appendChild(dom.createTextNode("CW"));
        root.appendChild(elHigher);
    }


    elHigher = dom.createElement("CommunicationConfig");

    if ( communicationType == "Serial Communication ( COM )")
    {
        elHigher.setAttribute("communication_type", communicationType);

        elLower = dom.createElement("SerialLine");
        elLower.appendChild(dom.createTextNode("COM3"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("BaudRate");
        elLower.appendChild(dom.createTextNode("9600"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("DataBits");
        elLower.appendChild(dom.createTextNode("8"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("StopBits");
        elLower.appendChild(dom.createTextNode("1"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("Parity");
        elLower.appendChild(dom.createTextNode("None"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("FlowControl");
        elLower.appendChild(dom.createTextNode("None"));

        elHigher.appendChild(elLower);

        root.appendChild(elHigher);
    }
    else if ( communicationType == "Ethernet Communication (TCP/IP)")
    {
        elHigher.setAttribute("communication_type", communicationType);

        elLower = dom.createElement("AddressIP");
        elLower.appendChild(dom.createTextNode("192.168.0.2"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("Port");
        elLower.appendChild(dom.createTextNode("2000"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("WastedBytes");
        elLower.appendChild(dom.createTextNode("2"));

        elHigher.appendChild(elLower);

        elLower = dom.createElement("BytesOnCommand");
        elLower.appendChild(dom.createTextNode("16"));

        elHigher.appendChild(elLower);

        root.appendChild(elHigher);
    }

    saveFile(projectPath+projectName,projectName+".pro",dom.toString());

    QTreeWidgetItem* header = new QTreeWidgetItem();
    header->setText(0, tr("Projects"));
    ui->projectExplorer->setHeaderItem(header);

    QTreeWidgetItem* project = new QTreeWidgetItem(ProjectType);
    project->setText(0,projectName);
    project->setText(1,projectPath + projectName + '/');
    project->setText(2,projectName + tr(" ( Active )"));
    project->setIcon(0,*(new QIcon(":/new/icons/lc_dbformopen.png")));

    QTreeWidgetItem* projectProFile = new QTreeWidgetItem(ConfigType);
    projectProFile->setText(0,projectName + ".pro");
    projectProFile->setText(1,projectPath + projectName + '/');
    projectProFile->setText(2,projectName + ".pro*");
    projectProFile->setIcon(0,*(new QIcon(":/new/icons/profile.png")));
    project->addChild(projectProFile);

    ui->projectExplorer->addTopLevelItem(project);
    ui->actionCloseAllProjects->setEnabled(true);

    m_projects[projectName] = new Project();
    m_threads[projectName] = new QThread();

    setActiveProject(projectName);

    ui->projectExplorer->show();

    m_newProjectDialog->close();
}

void MainWindow::projectExplorerContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem* item = ui->projectExplorer->itemAt(pos);
    QMenu menu(this);

    if (item != NULL)
    {
        switch(item->type())
        {
        case ProjectType:
        {
            QAction* setActive  = new QAction(tr("Set as Active"),      this);
            QAction* addNew     = new QAction(tr("Add New"),            this);
            QAction* addExist   = new QAction(tr("Add Existing File"),  this);
            QAction* saveProject= new QAction(tr("Save Project"),       this);
            QAction* run        = new QAction(tr("Run"),                this);
            QAction* pause      = new QAction(tr("Pause"),              this);
            QAction* stop       = new QAction(tr("Stop"),               this);
            QAction* restart    = new QAction(tr("Restart"),            this);
            QAction* reload     = new QAction(tr("Reload"),             this);
            QAction* remove     = new QAction(tr("Remove"),             this);
            QAction* close      = new QAction(tr("Close"),              this);
            QAction* openInExplorer = new QAction(tr("Open in explorer"), this);
            QAction* openCommandPrompt = new QAction(tr("Open Command Prompt"), this);
            QAction* openManualControl = new QAction(tr("Manual Control"), this);

            saveProject ->setIcon(QIcon(":/new/icons/lc_save.png"));
            run         ->setIcon(QIcon(":/new/icons/avl02049.png"));
            pause       ->setIcon(QIcon(":/new/icons/avl02050.png"));
            stop        ->setIcon(QIcon(":/new/icons/avl02051.png"));
            restart     ->setIcon(QIcon(":/new/icons/avl02052.png"));

            menu.addAction(addNew);
            menu.addAction(addExist);
            menu.addSeparator();
            menu.addAction(setActive);
            menu.addSeparator();
            menu.addAction(openInExplorer);
            menu.addSeparator();
            menu.addAction(saveProject);
            menu.addAction(openCommandPrompt);
            menu.addAction(openManualControl);
            menu.addSeparator();
            menu.addAction(run);
            menu.addAction(pause);
            menu.addAction(stop);
            menu.addAction(restart);
            menu.addSeparator();
            menu.addAction(reload);
            menu.addAction(remove);
            menu.addAction(close);

            m_openManualControl->setMapping(openManualControl,item->text(0));
            m_openUrlMapper         ->  setMapping(openInExplorer,item->text(1));
            m_setActiveMapper       ->  setMapping(setActive,   item->text(0));
            m_reloadSignalMapper    ->  setMapping(reload,      item->text(0));
            m_addNewSignalMapper    ->  setMapping(addNew,      item->text(0));
            m_addExistSignalMapper  ->  setMapping(addExist,    item->text(0));
            m_removeSignalMapper    ->  setMapping(remove,      item->text(0));
            m_runSignalMapper       ->  setMapping(run,         item->text(0));
            m_pauseSignalMapper     ->  setMapping(pause,       item->text(0));
            m_stopSignalMapper      ->  setMapping(stop,        item->text(0));
            m_restartSignalMapper   ->  setMapping(restart,     item->text(0));
            m_closeSignalMapper     ->  setMapping(close,       item->text(0));
            m_saveProjectMapper     ->  setMapping(saveProject, item->text(0));
            m_openCommandPromptMapper->setMapping(openCommandPrompt, item->text(0));

            connect(openManualControl,SIGNAL(triggered(bool)),m_openManualControl,SLOT(map()));
            connect(openCommandPrompt,SIGNAL(triggered(bool)),m_openCommandPromptMapper,SLOT(map()));
            connect(openInExplorer, SIGNAL(triggered(bool)),m_openUrlMapper,    SLOT(map()));
            connect(setActive,  SIGNAL(triggered(bool)),m_setActiveMapper,      SLOT(map()));
            connect(reload,     SIGNAL(triggered(bool)),m_reloadSignalMapper,   SLOT(map()));
            connect(addNew,     SIGNAL(triggered(bool)),m_addNewSignalMapper,   SLOT(map()));
            connect(addExist,   SIGNAL(triggered(bool)),m_addExistSignalMapper, SLOT(map()));
            connect(remove,     SIGNAL(triggered(bool)),m_removeSignalMapper,   SLOT(map()));
            connect(run,        SIGNAL(triggered(bool)),m_runSignalMapper,      SLOT(map()));
            connect(pause,      SIGNAL(triggered(bool)),m_pauseSignalMapper,    SLOT(map()));
            connect(stop,       SIGNAL(triggered(bool)),m_stopSignalMapper,     SLOT(map()));
            connect(restart,    SIGNAL(triggered(bool)),m_restartSignalMapper,  SLOT(map()));
            connect(close,      SIGNAL(triggered(bool)),m_closeSignalMapper,    SLOT(map()));
            connect(saveProject,SIGNAL(triggered(bool)),m_saveProjectMapper,    SLOT(map()));

            break;
        }
        case ConfigType:
        {
            QAction* save   = new QAction(tr("Save"),        this);
            QAction* saveAs = new QAction(tr("Save As..."),  this);
            QAction* rename = new QAction(tr("Rename"),      this);
            QAction* openInExplorer = new QAction(tr("Open in explorer"), this);

            save    ->setIcon(QIcon(":/new/icons/lc_save.png"));
            saveAs  ->setIcon(QIcon(":/new/icons/lc_saveas.png"));

            menu.addAction(save);
            menu.addAction(saveAs);
            menu.addSeparator();
            menu.addAction(rename);
            menu.addSeparator();
            menu.addAction(openInExplorer);

            if ( !item->text(0).endsWith('*') )
                save->setEnabled(false);

            m_openUrlMapper         ->  setMapping(openInExplorer,item->text(1));
            m_saveSignalMapper      ->  setMapping(save,        item->text(0)+imposibleDelimiter+item->text(1));
            m_saveAsSignalMapper    ->  setMapping(saveAs,      item->text(0)+imposibleDelimiter+item->text(1));
            m_renameSignalMapper    ->  setMapping(rename,      item->text(0)+imposibleDelimiter+item->text(1));

            connect(openInExplorer, SIGNAL(triggered(bool)),m_openUrlMapper,    SLOT(map()));
            connect(save,       SIGNAL(triggered(bool)),m_saveSignalMapper,     SLOT(map()));
            connect(saveAs,     SIGNAL(triggered(bool)),m_saveAsSignalMapper,   SLOT(map()));
            connect(rename,     SIGNAL(triggered(bool)),m_renameSignalMapper,   SLOT(map()));

            break;
        }
        case FileType:
        {
            QTreeWidgetItem* project;
            QAction* remove = new QAction(tr("Remove file"), this);
            QAction* rename = new QAction(tr("Rename"),      this);
            QAction* save   = new QAction(tr("Save"),        this);
            QAction* saveAs = new QAction(tr("Save As..."),  this);
            QAction* openInExplorer = new QAction(tr("Open in explorer"), this);

            save    ->setIcon(QIcon(":/new/icons/lc_save.png"));
            saveAs  ->setIcon(QIcon(":/new/icons/lc_saveas.png"));

            menu.addAction(save);
            menu.addAction(saveAs);
            menu.addSeparator();
            menu.addAction(rename);
            menu.addSeparator();
            menu.addAction(openInExplorer);


            if ( !item->text(0).endsWith('*') )
                save->setEnabled(false);

            m_openUrlMapper         ->  setMapping(openInExplorer,item->text(1));
            m_saveSignalMapper      ->  setMapping(save,        item->text(0)+imposibleDelimiter+item->text(1));
            m_saveAsSignalMapper    ->  setMapping(saveAs,      item->text(0)+imposibleDelimiter+item->text(1));
            m_renameSignalMapper    ->  setMapping(rename,      item->text(0)+imposibleDelimiter+item->text(1));

            connect(openInExplorer, SIGNAL(triggered(bool)),m_openUrlMapper,    SLOT(map()));
            connect(rename,     SIGNAL(triggered(bool)),m_renameSignalMapper,   SLOT(map()));
            connect(save,       SIGNAL(triggered(bool)),m_saveSignalMapper,     SLOT(map()));
            connect(saveAs,     SIGNAL(triggered(bool)),m_saveAsSignalMapper,   SLOT(map()));

            project = item->parent();

            if ( project )
            {
                QString projectName = project->text(0)<project->text(2)?project->text(0):project->text(2);
                QString projectPath = project->text(1);

                menu.addSeparator();
                menu.addAction(remove);
                m_removeSignalMapper->setMapping(remove, item->text(0)+imposibleDelimiter+item->text(1)+
                                                 imposibleDelimiter+projectName+imposibleDelimiter+projectPath);
                connect(remove,     SIGNAL(triggered(bool)),m_removeSignalMapper,   SLOT(map()));
            }

            break;
        }
        default:
        {
            break;
        }
        }
    }

    menu.setStyleSheet(currentMenuBarTheme);
    menu.exec( ui->projectExplorer->mapToGlobal(pos));
}

void MainWindow::fileEditorContextMenuRequested(const QPoint &pos)
{
    QMenu menu(this);
    QString path;
    QString tabName;

    bool canSave = false;

    int clickedTabIndex = -1;

    for ( unsigned i=0; i< ui->fileEditor->count(); ++i )
    {
        if ( ui->fileEditor->tabBar()->tabRect(i).contains(pos) )
        {
            clickedTabIndex = i;
            break;
        }
    }

    CodeEditor* codeEditor = dynamic_cast<CodeEditor*>( ui->fileEditor->widget(clickedTabIndex));
    ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>( ui->fileEditor->widget(clickedTabIndex));
    PointsFileEditor* pointsFileEditor = dynamic_cast<PointsFileEditor*>( ui->fileEditor->widget(clickedTabIndex));

    tabName = ui->fileEditor->tabText(clickedTabIndex);

    if (tabName.endsWith('*'))
        canSave = true;

    if (codeEditor)
    {
        path = codeEditor->path;
    }
    else if ( projectFileEditor )
    {
        path = projectFileEditor->path;
    }
    else if ( pointsFileEditor )
    {
        path = pointsFileEditor->path;
    }
    else
        return;

    QAction* save = new QAction(QIcon(":/new/icons/lc_save.png"),"Save",this);
    m_saveSignalMapper->setMapping(save,tabName+imposibleDelimiter+path);
    connect(save,SIGNAL(triggered(bool)),m_saveSignalMapper,SLOT(map()));

    QAction* saveAs = new QAction(QIcon(":/new/icons/lc_saveas.png"),"Save as",this);
    m_saveAsSignalMapper->setMapping(saveAs,tabName+imposibleDelimiter+path);
    connect(saveAs,SIGNAL(triggered(bool)),m_saveAsSignalMapper,SLOT(map()));

    QAction* rename = new QAction("Rename",this);
    m_renameSignalMapper->setMapping(rename,tabName+imposibleDelimiter+path);
    connect(rename,SIGNAL(triggered(bool)),m_renameSignalMapper,SLOT(map()));

    QAction* closeTab = new QAction("Close Tab",this);
    m_tabCloseMapper->setMapping(closeTab,clickedTabIndex);
    connect(closeTab,SIGNAL(triggered(bool)),m_tabCloseMapper,SLOT(map()));

    QAction* closeAllTabs = new QAction("Close all Tabs",this);
    connect(closeAllTabs,SIGNAL(triggered(bool)),this,SLOT(closeAllFilesClicked()));

    QAction* openInExplorer = new QAction("Open in explorer",this);
    m_openUrlMapper->setMapping(openInExplorer,path);
    connect(openInExplorer,SIGNAL(triggered(bool)),m_openUrlMapper,SLOT(map()));

    QAction* reload = new QAction(QIcon(":/new/icons/lc040.png"),"Reload",this);
    m_reloadSignalMapper->setMapping(reload,tabName+imposibleDelimiter+path);
    connect(reload,SIGNAL(triggered(bool)),m_reloadSignalMapper,SLOT(map()));

    if ( !canSave )
    {
        save->setEnabled(false);
        reload->setEnabled(true);
    }
    else
        reload->setEnabled(false);

    menu.addAction(save);
    menu.addAction(saveAs);
    menu.addAction(reload);
    menu.addSeparator();
    menu.addAction(rename);
    menu.addSeparator();
    menu.addAction(openInExplorer);
    menu.addSeparator();
    menu.addAction(closeAllTabs);
    menu.addAction(closeTab);

    menu.setStyleSheet(currentMenuBarTheme);
    menu.exec(ui->fileEditor->tabBar()->mapToGlobal(pos));
}


void MainWindow::renameClicked(QString const& data)
{
    QStringList strList;
    QString name;
    QString path;
    QString projectName;
    QString projectPath;
    QString fileName;
    QString newName;
    bool unsavedChanges = false;

    strList = data.split(imposibleDelimiter);

    if ( strList.length() != 2 )
        return;

    name = strList[0];
    path = strList[1];

    if ( name.length() != 0 )
    {
        if ( name[name.length()-1] == '*' )
        {
            unsavedChanges = true;
            fileName = name.left(name.length()-1);
        }
        else
            fileName = name;
    }
    else
        return;

    m_renameFileDialog = new RenameFileDialog(this);
    m_renameFileDialog->setFileName(fileName);

    if ( m_renameFileDialog->exec() )
        newName = m_renameFileDialog->getFileName();
    else
        return;

    if ( newName == fileName )
        return;

    if ( !QFile::rename(path+fileName,path+newName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot rename file"));

        msgBox.setStyleSheet(currentErrorBoxTheme);

        msgBox.exec();
        return;
    }

    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly | Qt::MatchRecursive,0) )
    {
        if ( item->text(0) == name && item->text(1) == path )
        {
            QTreeWidgetItem* parent = item->parent();

            if ( parent )
            {
                projectName = parent->text(0)<parent->text(2)?parent->text(0):parent->text(2);
                projectPath = parent->text(1);
            }
            else
                return;

            if ( unsavedChanges )
            {
                item->setText(0,newName + '*');
                item->setText(2,newName);
            }
            else
            {
                item->setText(0,newName);
                item->setText(2,newName+'*');
            }

            if ( !name.endsWith(".pro") && !name.endsWith(".pro*") )
            {
                detachFileFromProject(fileName,projectName+".pro",projectPath);
                attachFileToProject(newName,path,projectName+".pro",projectPath);

                sortProjectFiles(parent);
            }
        }
    }

    for ( unsigned i=0;i<ui->fileEditor->count(); ++i )
    {
        if ( ui->fileEditor->tabText(i) == name )
        {
            CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(i));
            ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(i));

            if ( codeEditor )
            {
                if ( codeEditor->path == path )
                {
                    if ( unsavedChanges )
                        ui->fileEditor->setTabText(i,newName+'*');
                    else
                        ui->fileEditor->setTabText(i,newName);
                }
            }
            else if ( projectFileEditor )
            {
                if ( projectFileEditor->path == path )
                {
                    if ( unsavedChanges )
                        ui->fileEditor->setTabText(i,newName);
                    else
                        ui->fileEditor->setTabText(i,newName);
                }
            }
        }
    }
}

void MainWindow::saveClicked(const QString &data)
{
    QStringList strList;
    CodeEditor* codeEditor;
    ProjectFileEditor* projectFileEditor;
    PointsFileEditor* pointsFileEditor;
    QString name;
    QString path;
    bool futherSavingPossible = false;

    if ( data == "current" )
    {
        unsigned currentIdx = ui->fileEditor->currentIndex();

        name = ui->fileEditor->tabText(currentIdx);
        codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->currentWidget());
        projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->currentWidget());
        pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->currentWidget());

        if ( codeEditor )
            path = codeEditor->path;
        else if ( projectFileEditor )
        {
            path = projectFileEditor->path;
            m_saveSignalMapper->removeMappings(projectFileEditor);
            disconnect(projectFileEditor,SIGNAL(saveRequested()),m_saveSignalMapper,SLOT(map()));
        }
        else if ( pointsFileEditor)
        {
            path = pointsFileEditor->path;
        }

        ui->actionReload->setEnabled(true);
        ui->actionSave_as->setEnabled(false);
        ui->actionSave->setEnabled(false);
    }
    else
    {
        strList = data.split(imposibleDelimiter);

        if ( strList.length() != 2 )
            return;

        name = strList[0];
        path = strList[1];
    }

    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly | Qt::MatchRecursive,0) )
    {
        if ( item->text(0) < item->text(2)) continue; // just in case if saved item could call this slot
        else if ( item->text(1) == path )
        {
            QString tmpStr = item->text(0);

            item->setText(0,item->text(2));
            item->setText(2,tmpStr);
        }
    }

    // I hope its good synchronized with project explorer, god bless me
    for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
    {
        if ( ui->fileEditor->tabText(i) == name )
        {
            codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(i));
            projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(i));
            pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(i));

            if ( codeEditor )
            {
                if ( codeEditor->path == path)
                {
                    QString newName = name.left(name.length()-1);

                    saveFile(path,newName,codeEditor->toPlainText());
                    ui->fileEditor->setTabText(i,newName);

                    m_textChangedMapper->setMapping(codeEditor,codeEditor);
                    connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));

                    if ( ui->fileEditor->currentIndex() == i )
                    {
                        ui->actionSave->setEnabled(false);
                        ui->actionReload->setEnabled(true);
                        ui->actionSave_as->setEnabled(false);
                    }
                }
            }
            else if ( projectFileEditor )
            {
                if ( projectFileEditor->path == path )
                {
                    QString newName = name.left(name.length()-1);

                    saveFile(path,newName,projectFileEditor->toStr());
                    ui->fileEditor->setTabText(i,newName);

                    m_textChangedMapper->setMapping(projectFileEditor,projectFileEditor);
                    connect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

                    if ( ui->fileEditor->currentIndex() == i )
                    {
                        ui->actionSave->setEnabled(false);
                        ui->actionReload->setEnabled(true);
                        ui->actionSave_as->setEnabled(false);
                    }
                }
            }
            else if ( pointsFileEditor )
            {
                QString newName = name.left(name.length()-1);

                saveFile(path,newName,pointsFileEditor->toStr());
                ui->fileEditor->setTabText(i,newName);

                m_textChangedMapper->setMapping(pointsFileEditor,pointsFileEditor);
                connect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

                if ( pointsFileEditor->path == path )
                {
                    if ( ui->fileEditor->currentIndex() == i )
                    {
                        ui->actionSave->setEnabled(false);
                        ui->actionReload->setEnabled(true);
                        ui->actionSave_as->setEnabled(false);
                    }
                }
            }
        }
        else if ( ui->fileEditor->tabText(i)[ui->fileEditor->tabText(i).length()-1] == '*' )
            // Now i hope that i never let file have empty tab text
            futherSavingPossible = true;
    }

    if ( futherSavingPossible )
        ui->actionSaveAll->setEnabled(true);
    else
        ui->actionSaveAll->setEnabled(false);
}

void MainWindow::saveAsClicked(const QString &data)
{
    QStringList strList;
    QFileDialog fileDialog;
    CodeEditor* codeEditor;
    ProjectFileEditor* projectFileEditor;
    PointsFileEditor* pointsFileEditor;
    QString name;
    QString path;
    QString nameWithoutStar;

    if ( data == "current" )
    {
        unsigned currentIdx = ui->fileEditor->currentIndex();

        name = ui->fileEditor->tabText(currentIdx);
        codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(currentIdx));
        projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(currentIdx));
        pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(currentIdx));

        if ( codeEditor )
            path = codeEditor->path;
        else if ( projectFileEditor )
            path = projectFileEditor->path;
        else if ( pointsFileEditor )
            path = pointsFileEditor->path;
    }
    else
    {
        strList = data.split(imposibleDelimiter);

        if ( strList.length() != 2 )
            return;

        name = strList[0];
        path = strList[1];
    }

    if ( name[name.length()-1] == '*' )
        nameWithoutStar = name.left(name.length()-1);
    else
        nameWithoutStar = name;

    fileDialog.setDirectory(path);
    fileDialog.selectFile(nameWithoutStar);
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);

    QStringList filters;
    filters << "Openable (*.pro *.js *.pt)"
            << "Project files (*.pro)"
            << "Javascript files (*.js)"
            << "Points files (*.pt)"
            << "Any files (*)";

    fileDialog.setNameFilters(filters);

    if ( name.endsWith(".js") || name.endsWith(".js*") )
        fileDialog.setDefaultSuffix(".js");
    else if ( name.endsWith(".pro") || name.endsWith(".pro*"))
        fileDialog.setDefaultSuffix(".pro");

    if ( fileDialog.exec() )
    {
        QString newFullPath = fileDialog.selectedFiles()[0];
        QString newPath;
        QString newName;

        for ( unsigned i=newFullPath.length(); i!=0; --i)
        {
            if ( newFullPath[i-1] == '\\' || newFullPath[i-1] == '/' )
            {
                newPath = newFullPath.left(i);
                newName = newFullPath.right(newFullPath.length()-i);
                newName = newName.left(newName.length());

                break;
            }
        }

        if ( newName == nameWithoutStar && newPath == path )
        {
            if ( name.endsWith('*') )
                saveClicked(name+imposibleDelimiter+path);

            return;
        }

        for ( unsigned i=0; i<ui->fileEditor->count(); ++i)
        {
            if ( ui->fileEditor->tabText(i) == name )
            {
                codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(i));
                projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(i));
                pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(i));

                if ( codeEditor )
                {
                    if ( codeEditor->path == path )
                    {
                        CodeEditor* newCodeEditor= new CodeEditor(this);

                        newCodeEditor->turnOnPythonHighlighting();

                        newCodeEditor->document()->setPlainText(codeEditor->toPlainText());
                        newCodeEditor->path = newPath;

                        saveFile(newPath,newName,newCodeEditor->toPlainText());

                        m_textChangedMapper->setMapping(newCodeEditor,newCodeEditor);
                        connect(newCodeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
                        connect(newCodeEditor,SIGNAL(copyAvailable(bool)),this,SLOT(copyAvailable(bool)));

                        ui->fileEditor->addTab(newCodeEditor,QIcon(":/new/icons/pythonfile.png"),newName);
                        ui->fileEditor->setCurrentWidget(newCodeEditor);

                        return;
                    }
                }
                else if ( projectFileEditor )
                {
                    if ( projectFileEditor->path == path )
                    {
                        ProjectFileEditor* newProjectFileEditor = new ProjectFileEditor(this);
                        
                        newProjectFileEditor->populateFromString(projectFileEditor->toStr());

                        saveFile(newPath,newName,newProjectFileEditor->toStr());
                        
                        m_textChangedMapper->setMapping(newProjectFileEditor,newProjectFileEditor);
                        connect(newProjectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
                        
                        ui->fileEditor->addTab(newProjectFileEditor,QIcon(":/new/icons/profile.png"),newName);
                        ui->fileEditor->setCurrentWidget(newProjectFileEditor);
                        
                        return;
                    }
                }
                else if ( pointsFileEditor )
                {
                    PointsFileEditor* newPointsFileEditor = new PointsFileEditor(this);

                    newPointsFileEditor->populateFromString(pointsFileEditor->toStr());

                    saveFile(newPath,newName,newPointsFileEditor->toStr());

                    m_textChangedMapper->setMapping(newPointsFileEditor,newPointsFileEditor);
                    connect(newPointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

                    ui->fileEditor->addTab(newPointsFileEditor,QIcon(":/new/icons/pointsfile.png"),newName);
                    ui->fileEditor->setCurrentWidget(newPointsFileEditor);

                    return;
                }
            }
        }

        // If file is not opened, we have to load it
        if ( name.endsWith(".js") )
        {
            QString content = loadFile(path,name);
    
            codeEditor = new CodeEditor(this);
    
            codeEditor->document()->setPlainText(content);
            codeEditor->path = newPath;
    
            m_textChangedMapper->setMapping(codeEditor,codeEditor);
            connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
            connect(codeEditor,SIGNAL(copyAvailable(bool)),this,SLOT(copyAvailable(bool)));
    
            saveFile(newPath,newName,codeEditor->toPlainText());
    
            ui->fileEditor->addTab(codeEditor,QIcon(":/new/icons/pythonfile.png"),newName);
            ui->fileEditor->setCurrentWidget(codeEditor);
        }
        else if ( name.endsWith(".pro") )
        {
            QString content = loadFile(path,name);
            
            projectFileEditor = new ProjectFileEditor(this);

            projectFileEditor->path = newPath;
            projectFileEditor->populateFromString(content);

            m_textChangedMapper->setMapping(projectFileEditor,projectFileEditor);
            connect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

            saveFile(newPath,newName,projectFileEditor->toStr());

            ui->fileEditor->addTab(projectFileEditor,QIcon(":/new/icons/profile.png"),newName);
            ui->fileEditor->setCurrentWidget(projectFileEditor);
        }
        else if ( name.endsWith(".pt") )
        {
            QString content = loadFile(path,name);

            pointsFileEditor = new PointsFileEditor(this);

            pointsFileEditor->path = newPath;
            pointsFileEditor->populateFromString(content);

            m_textChangedMapper->setMapping(pointsFileEditor,pointsFileEditor);
            connect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

            saveFile(newPath,newName,pointsFileEditor->toStr());

            ui->fileEditor->addTab(pointsFileEditor,QIcon(":/new/icons/pointsfile.png"),newName);
            ui->fileEditor->setCurrentWidget(pointsFileEditor);
        }
    }
}

void MainWindow::closeFileClicked()
{
    tabCloseClicked(ui->fileEditor->currentIndex());
}

void MainWindow::optionsClicked()
{
    m_optionsDialog = new OptionsDialog(this);
    m_optionsDialog->show();
}

void MainWindow::commandPromptFinished(const QString &name)
{
    m_projects.take(name);
    m_projects[name] = new Project();
    m_projects[name]->setProjectState(Idle);
}

void MainWindow::threadFinished(const QString &name)
{
    qDebug() << "thread finished";
    qDebug() << name;

    m_projects.take(name);
    m_projects[name] = new Project();

    m_threads.take(name);
    m_threads[name] = new QThread();

    foreach ( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchContains,0) )
        // must be active
    {
        project->setIcon(0,QIcon(":/new/icons/lc_dbformopen.png"));

        if ( project->text(0) > project->text(2))
        {
            ui->actionRun->setEnabled(true);
            ui->actionPause->setEnabled(false);
            ui->actionStop->setEnabled(false);
            ui->actionRestart->setEnabled(false);
        }
    }
}

void MainWindow::deleteReloadFileDialog()
{
    delete m_reloadFilesDialog;
    m_reloadFilesDialog = NULL;
}

void MainWindow::setActiveClicked(const QString &name)
{
    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
        setActiveProject(name);
}

void MainWindow::closeClicked(const QString &name)
{
    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
    {
        QString projectName = item->text(0)<item->text(2)?item->text(0):item->text(2);

        if ( m_projects.contains(projectName))
        {
            if ( m_projects[projectName]->projectState() != Idle )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot close project because it is under control."));

                msgBox.setStyleSheet(currentErrorBoxTheme);
                msgBox.exec();

                break;
            }
        }

        delete item;
        m_projects.take(projectName);
        m_threads.take(projectName);
    }

    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,2) )
    {
        QString projectName = item->text(0)<item->text(2)?item->text(0):item->text(2);

        if (  m_projects.contains(projectName))
        {
            if ( m_projects[projectName]->projectState() != Idle )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot close project because it is under control."));

                msgBox.setStyleSheet(currentErrorBoxTheme);
                msgBox.exec();

                break;
            }
        }

        delete item;
        m_projects.take(projectName);
        m_threads.take(projectName);
    }

    if ( ui->projectExplorer->topLevelItemCount() == 0 )
    {
        ui->projectExplorer->hide();
        ui->actionCloseAllProjects->setEnabled(false);
    }
}


void MainWindow::reloadClicked(const QString &data)
{
    CodeEditor* codeEditor;
    ProjectFileEditor* projectFileEditor;
    PointsFileEditor* pointsFileEditor;
    QString name;
    QString path;

    if ( data == "current" )
    {
        unsigned currentIdx = ui->fileEditor->currentIndex();

        name = ui->fileEditor->tabText(currentIdx);
        codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(currentIdx));
        projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(currentIdx));
        pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(currentIdx));

        if ( codeEditor )
        {
            path = codeEditor->path;

            QFile file(path+name);

            if ( file.open(QIODevice::ReadWrite | QFile::Text))
            {
                QTextStream textStream(&file);

                disconnect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
                codeEditor->document()->clear();
                codeEditor->document()->setPlainText(textStream.readAll());
                connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
            }
        }
        else if ( projectFileEditor )
        {
            path = projectFileEditor->path;

            QFile file(path+name);

            if ( file.open(QIODevice::ReadWrite | QFile::Text))
            {
                QTextStream textStream(&file);

                disconnect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
                projectFileEditor->populateFromString(textStream.readAll());
                connect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
            }
        }
        else if ( pointsFileEditor )
        {
            path = pointsFileEditor->path;

            QFile file(path+name);

            if ( file.open(QIODevice::ReadWrite | QFile::Text))
            {
                QTextStream textStream(&file);

                disconnect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
                pointsFileEditor->populateFromString(textStream.readAll());
                connect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
            }
        }

    }
    // at this momment i do not let situation where file that isnt current, can call this function
}


void MainWindow::addNewClicked(const QString &name)
{
    bool bOrNot2b = true;
    QString fileName, fileType;

    m_newFileDialog = new NewFileDialog(this);

    while(bOrNot2b)
    {
        bOrNot2b = false;

        if ( m_newFileDialog->exec() )
        {
            QStringList data = m_newFileDialog->retrieveData();
            fileName = data[0];
            fileType = data[1];
        }
        else return;

        if ( fileType == "Program ( Javascript File )")
            fileName += ".js";
        else if ( fileType == "Points ( .pt )")
            fileName += ".pt";

        // check if there is a file in a project with the same name
        foreach( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchExactly,0))
        {
            for ( unsigned i=0; i<project->childCount(); ++i)
            {
                QTreeWidgetItem* child = project->child(i);

                if ( child )
                {
                    if ( child->text(0) == fileName ||
                         child->text(2) == fileName )
                    {
                        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is a file with the same name in a project.\n"
                                                                                 "Please change name."));

                        msgBox.setStyleSheet(currentErrorBoxTheme);

                        msgBox.exec();
                        bOrNot2b = true;
                        break;
                    }
                }
            }
        }
    }

    foreach( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchExactly,0))
    {
        QTreeWidgetItem* file = new QTreeWidgetItem(FileType);
        QString projectName = project->text(0)<project->text(2)?project->text(0):project->text(2);
        QString projectPath = project->text(1);

        file->setText(0,fileName);
        file->setText(1,projectPath);
        file->setText(2,fileName+'*');

        if ( fileName.endsWith(".js") )
            file->setIcon(0,*(new QIcon(":/new/icons/pythonfile.png")));
        else if ( fileName.endsWith(".pt") )
            file->setIcon(0,*(new QIcon(":/new/icons/pointsfile.png")));

        project->addChild(file);

        projectExplorerDoubleClicked(file,0);

        attachFileToProject(fileName,projectPath,projectName+".pro",projectPath);

        sortProjectFiles(project);
    }
}

void MainWindow::addExistClicked(const QString &name)
{
    QFileDialog folderDialog;
    QString fullPath, fullFileName, filePath;

    QStringList filters;
    filters << "Openable (*.js *.pt)"
            << "Javascript files (*.js)"
            << "Points files (*.pt)"
            << "Any files (*)";

    folderDialog.setNameFilters(filters);
    folderDialog.setDirectory(QDir::home());

    if ( folderDialog.exec() == QDialog::Accepted )
    {
        fullPath = folderDialog.selectedFiles()[0];
    }
    else return;

    for (unsigned i=fullPath.length();i!=0;--i)
    {
        if ( fullPath[i-1] == '\\' || fullPath[i-1] == '/' )
        {
            filePath = fullPath.left(i);
            fullFileName = fullPath.right(fullPath.length()-i);
            break;
        }
    }

    foreach( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchExactly,0 ) )
    {
        QString projectName = project->text(0) < project->text(2) ? project->text(0) : project->text(2);
        QTreeWidgetItem* file;

        for ( unsigned i=0; i<project->childCount(); ++i )
        {
            QTreeWidgetItem* child = project->child(i);

            if ( child->text(0) == fullFileName ||
                 child->text(2) == fullFileName )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is a file with the same name in a project.\n"
                                                                         "Please change name."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                return;
            }
        }

        file = new QTreeWidgetItem(FileType);

        file->setText(0,fullFileName);
        file->setText(1,filePath);
        file->setText(2,fullFileName+'*');
        file->setIcon(0,*(new QIcon(":/new/icons/pythonfile.png")));

        attachFileToProject(fullFileName,filePath,projectName+".pro",project->text(1));

        project->addChild(file);
        projectExplorerDoubleClicked(file,0);

        sortProjectFiles(project);
    }
}

void MainWindow::removeClicked(const QString &data)
{
    QStringList strList;
    QString name;
    QString path;
    QString proName;
    QString proPath;
    QString fileName;
    bool deletePernamently = false;

    strList = data.split(imposibleDelimiter);

    if ( strList.length() != 4 )
        return;

    name = strList[0];
    path = strList[1];
    proName = strList[2];
    proPath = strList[3];

    if ( name[name.length()-1] == '*' )
        fileName = name.left(name.length()-1);
    else
        fileName = name;

    m_removeFileDialog = new RemoveFileDialog(this);
    m_removeFileDialog->setFile(path+fileName);

    if ( m_removeFileDialog->exec() )
        deletePernamently = m_removeFileDialog->deletePermanently();
    else
        return;

    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly | Qt::MatchRecursive,0) )
    {
        if ( item->text(0) == name && item->text(1) == path )
        {
            QString fileName = item->text(0)<item->text(2)?item->text(0):item->text(2);
            QString filePath = item->text(1);
            QTreeWidgetItem* project = item->parent();

            if ( project )
            {
                QString projectName = project->text(0)<project->text(2)?project->text(0):project->text(2);
                QString projectPath = project->text(1);

                if ( projectName == projectName && projectPath == projectPath )
                {
                    if ( deletePernamently )
                    {
                        QFile file(filePath+fileName);

                        if ( !file.remove() )
                        {
                            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot remove file"));

                            msgBox.setStyleSheet(currentErrorBoxTheme);

                            msgBox.exec();
                            return;
                        }
                    }

                    detachFileFromProject(fileName,projectName+".pro",projectPath);

                    delete item;
                }
            }
        }
    }
}

void MainWindow::runClicked(const QString &name)
{
    QString projectName = name == "current" ? m_activeProject : name;

    saveProjectClicked(name);

    foreach ( QTreeWidgetItem* project, ui->projectExplorer->findItems(projectName,Qt::MatchContains,0) )
    {
        QTreeWidgetItem* configFile;
        QString data, configFileName;

        for ( unsigned i=0; project->childCount(); ++i)
        {
            if ( project->child(i)->type() == ConfigType )
            {
                configFile = project->child(i);

                break;
            }
        }

        configFileName = configFile->text(0)<configFile->text(2)?configFile->text(0):configFile->text(2);

        if ( project->text(0) < project->text(2) )
        {
            if ( m_projects[project->text(0)]->getProjectThreadState() == Running )
                return;

            data = loadFile(configFile->text(1),configFileName);

            m_projects[project->text(0)]->populateFromString(data);
            m_projects[project->text(0)]->setupThread(m_threads[project->text(0)]);
            m_projects[project->text(0)]->moveToThread(m_threads[project->text(0)]);
            m_projects[project->text(0)]->setProjectState(ControlledByScript);

            m_threadFinishedMapper->setMapping(m_threads[project->text(0)],project->text(0));
            connect(m_threads[project->text(0)],SIGNAL(finished()),m_threadFinishedMapper,SLOT(map()));

            project->setIcon(0,QIcon(":/new/icons/avl02049.png"));

            m_threads[project->text(0)]->start(QThread::HighestPriority);
        }
        else if ( project->text(0) > project->text(2) ) // then project is active
        {
            if ( m_projects[project->text(2)]->getProjectThreadState() == Running )
                return;

            data = loadFile(configFile->text(1),configFileName);

            m_projects[project->text(2)]->populateFromString(data);
            m_projects[project->text(2)]->setupThread(m_threads[project->text(2)]);
            m_projects[project->text(2)]->moveToThread(m_threads[project->text(2)]);
            m_projects[project->text(2)]->setProjectState(ControlledByScript);

            m_threadFinishedMapper->setMapping(m_threads[project->text(2)],project->text(2));
            connect(m_threads[project->text(2)],SIGNAL(finished()),m_threadFinishedMapper,SLOT(map()));

            project->setIcon(0,QIcon(":/new/icons/avl02049.png"));

            m_threads[project->text(2)]->start(QThread::HighestPriority);

            ui->actionRun->setEnabled(false);
            ui->actionPause->setEnabled(true);
            ui->actionStop->setEnabled(true);
            ui->actionRestart->setEnabled(true);
        }
    }
}

void MainWindow::pauseClicked(const QString &name)
{

}

void MainWindow::stopClicked(const QString &name)
{

}

void MainWindow::restartClicked(const QString &name)
{
    
}

void MainWindow::saveProjectClicked(const QString &name)
{
    foreach ( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
    {
        for ( unsigned i=0; i<project->childCount(); ++i )
        {
            QTreeWidgetItem* child = project->child(i);

            if ( child->text(0) > child->text(2) )
            {
                saveClicked(child->text(0)+imposibleDelimiter+child->text(1));
            }
        }
    }
}

void MainWindow::openCommandPrompt(const QString &name)
{
    foreach ( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
    {
        QTreeWidgetItem* configFile;
        CommandPrompt* commandPrompt = new CommandPrompt();
        QString data, configFileName;

        for ( unsigned i=0; project->childCount(); ++i)
        {
            if ( project->child(i)->type() == ConfigType )
            {
                configFile = project->child(i);

                break;
            }
        }

        configFileName = configFile->text(0)<configFile->text(2)?configFile->text(0):configFile->text(2);

        if ( project->text(0) < project->text(2) )
        {
            if ( m_projects[project->text(0)]->projectState() != Idle )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already under control."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                return;
            }

            data = loadFile(configFile->text(1),configFileName);

            m_projects[project->text(0)]->populateFromString(data);
            m_projects[project->text(0)]->setupConnections();
            m_projects[project->text(0)]->setProjectState(ControlledByCommandPrompt);

            commandPrompt->setTitle(project->text(0));

            connect(commandPrompt,SIGNAL(establishConnection()),m_projects[project->text(0)],SLOT(establishConnectionSlot()));
            connect(commandPrompt,SIGNAL(dropConnection()),m_projects[project->text(0)],SLOT(dropConnectionSlot()));
            connect(commandPrompt,SIGNAL(sendCommand(QString)),m_projects[project->text(0)],SLOT(sendCommandSlot(QString)));

            connect(m_projects[project->text(0)],SIGNAL(sendProjectInfo(QString)),commandPrompt,SLOT(receiveProjectInfo(QString)));
            connect(m_projects[project->text(0)],SIGNAL(receivedCommand(QString)),commandPrompt,SLOT(receiveCommand(QString)));

            m_commandPromptFinishedMapper->setMapping(commandPrompt,project->text(0));
            connect(commandPrompt,SIGNAL(finished(int)),m_commandPromptFinishedMapper,SLOT(map()));
        }
        else if ( project->text(0) > project->text(2) )
        {
            if ( m_projects[project->text(2)]->projectState() != Idle )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already under control."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                return;
            }

            data = loadFile(configFile->text(1),configFileName);

            m_projects[project->text(2)]->populateFromString(data);
            m_projects[project->text(2)]->setupConnections();
            m_projects[project->text(2)]->setProjectState(ControlledByCommandPrompt);

            commandPrompt->setTitle(project->text(2));

            connect(commandPrompt,SIGNAL(establishConnection()),m_projects[project->text(2)],SLOT(establishConnectionSlot()));
            connect(commandPrompt,SIGNAL(dropConnection()),m_projects[project->text(2)],SLOT(dropConnectionSlot()));
            connect(commandPrompt,SIGNAL(sendCommand(QString)),m_projects[project->text(2)],SLOT(sendCommandSlot(QString)));

            connect(m_projects[project->text(2)],SIGNAL(sendProjectInfo(QString)),commandPrompt,SLOT(receiveProjectInfo(QString)));
            connect(m_projects[project->text(2)],SIGNAL(receivedCommand(QString)),commandPrompt,SLOT(receiveCommand(QString)));

            m_commandPromptFinishedMapper->setMapping(commandPrompt,project->text(2));
            connect(commandPrompt,SIGNAL(finished(int)),m_commandPromptFinishedMapper,SLOT(map()));
        }

        commandPrompt->show();
    }
}

void MainWindow::openManualControl(const QString &name)
{
    foreach ( QTreeWidgetItem* project, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
    {
        QTreeWidgetItem* configFile;
        ManualControlDialog* manualControlDialog = new ManualControlDialog();
        QString data, configFileName;

        for ( unsigned i=0; project->childCount(); ++i)
        {
            if ( project->child(i)->type() == ConfigType )
            {
                configFile = project->child(i);

                break;
            }
        }

        configFileName = configFile->text(0)<configFile->text(2)?configFile->text(0):configFile->text(2);

        if ( project->text(0) < project->text(2) )
        {
            if ( m_projects[project->text(0)]->projectState() != Idle )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already under control."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                return;
            }

            data = loadFile(configFile->text(1),configFileName);

            m_projects[project->text(0)]->populateFromString(data);
            m_projects[project->text(0)]->setupConnections();
            m_projects[project->text(0)]->setProjectState(ControlledByManualControl);

            manualControlDialog->setTitle(project->text(0));

            connect(manualControlDialog,SIGNAL(establishConnection()),m_projects[project->text(0)],SLOT(establishConnectionSlot()));
            connect(manualControlDialog,SIGNAL(dropConnection()),m_projects[project->text(0)],SLOT(dropConnectionSlot()));
            connect(manualControlDialog,SIGNAL(sendCommand(QString)),m_projects[project->text(0)],SLOT(sendCommandSlot(QString)));

            connect(m_projects[project->text(0)],SIGNAL(sendProjectInfo(QString)),manualControlDialog,SLOT(receiveProjectInfo(QString)));
            connect(m_projects[project->text(0)],SIGNAL(receivedCommand(QString)),manualControlDialog,SLOT(receiveCommand(QString)));
            connect(m_projects[project->text(0)],SIGNAL(positionHasChanged(double,double,double)),manualControlDialog,SLOT(positionHasChanged(double,double,double)));
            connect(m_projects[project->text(0)],SIGNAL(firstSegmentHomed()),manualControlDialog,SLOT(firstSegmentHomed()));
            connect(m_projects[project->text(0)],SIGNAL(firstSegmentNotHomed()),manualControlDialog,SLOT(firstSegmentNotHomed()));
            connect(m_projects[project->text(0)],SIGNAL(secondSegmentHomed()),manualControlDialog,SLOT(secondSegmentHomed()));
            connect(m_projects[project->text(0)],SIGNAL(secondSegmentNotHomed()),manualControlDialog,SLOT(secondSegmentNotHomed()));
            connect(m_projects[project->text(0)],SIGNAL(thirdSegmentHomed()),manualControlDialog,SLOT(thirdSegmentHomed()));
            connect(m_projects[project->text(0)],SIGNAL(thirdSegmentNotHomed()),manualControlDialog,SLOT(thirdSegmentNotHomed()));

            m_commandPromptFinishedMapper->setMapping(manualControlDialog,project->text(0));
            connect(manualControlDialog,SIGNAL(finished(int)),m_commandPromptFinishedMapper,SLOT(map()));
        }
        else if ( project->text(0) > project->text(2) )
        {
            if ( m_projects[project->text(2)]->projectState() != Idle )
            {
                QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already under control."));

                msgBox.setStyleSheet(currentErrorBoxTheme);

                msgBox.exec();
                return;
            }

            data = loadFile(configFile->text(1),configFileName);

            m_projects[project->text(2)]->populateFromString(data);
            m_projects[project->text(2)]->setupConnections();
            m_projects[project->text(2)]->setProjectState(ControlledByManualControl);

            manualControlDialog->setTitle(project->text(2));

            connect(manualControlDialog,SIGNAL(establishConnection()),m_projects[project->text(2)],SLOT(establishConnectionSlot()));
            connect(manualControlDialog,SIGNAL(dropConnection()),m_projects[project->text(2)],SLOT(dropConnectionSlot()));
            connect(manualControlDialog,SIGNAL(sendCommand(QString)),m_projects[project->text(2)],SLOT(sendCommandSlot(QString)));

            connect(m_projects[project->text(2)],SIGNAL(sendProjectInfo(QString)),manualControlDialog,SLOT(receiveProjectInfo(QString)));
            connect(m_projects[project->text(2)],SIGNAL(receivedCommand(QString)),manualControlDialog,SLOT(receiveCommand(QString)));
            connect(m_projects[project->text(2)],SIGNAL(positionHasChanged(double,double,double)),manualControlDialog,SLOT(positionHasChanged(double,double,double)));
            connect(m_projects[project->text(2)],SIGNAL(firstSegmentHomed()),manualControlDialog,SLOT(firstSegmentHomed()));
            connect(m_projects[project->text(2)],SIGNAL(firstSegmentNotHomed()),manualControlDialog,SLOT(firstSegmentNotHomed()));
            connect(m_projects[project->text(2)],SIGNAL(secondSegmentHomed()),manualControlDialog,SLOT(secondSegmentHomed()));
            connect(m_projects[project->text(2)],SIGNAL(secondSegmentNotHomed()),manualControlDialog,SLOT(secondSegmentNotHomed()));
            connect(m_projects[project->text(2)],SIGNAL(thirdSegmentHomed()),manualControlDialog,SLOT(thirdSegmentHomed()));
            connect(m_projects[project->text(2)],SIGNAL(thirdSegmentNotHomed()),manualControlDialog,SLOT(thirdSegmentNotHomed()));

            m_commandPromptFinishedMapper->setMapping(manualControlDialog,project->text(2));
            connect(manualControlDialog,SIGNAL(finished(int)),m_commandPromptFinishedMapper,SLOT(map()));
        }

        manualControlDialog->show();
    }
}

void MainWindow::openUrlClicked(const QString &url)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(url));
}

void MainWindow::registerRedoStatus(QWidget *widget)
{
    m_undoRedoRegisteredItem = dynamic_cast<CodeEditor*>( widget );
}

void MainWindow::registerUndoStatus(QWidget *widget)
{
    m_undoRedoRegisteredItem = dynamic_cast<CodeEditor*>( widget );
}

void MainWindow::textChanged(QWidget* widget)
{
    QString name = ui->fileEditor->tabText(ui->fileEditor->indexOf(widget));

    CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(widget);
    ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>(widget);
    PointsFileEditor* pointsFileEditor = dynamic_cast<PointsFileEditor*>(widget);

    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly | Qt::MatchRecursive,0) )
    {
        QString path;

        if ( codeEditor ) path = codeEditor->path;
        else if ( projectFileEditor ) path = projectFileEditor->path;
        else if ( pointsFileEditor ) path = pointsFileEditor->path;

        if ( path == item->text(1))
        {
            QString tmpStr = item->text(0);

            item->setText(0,item->text(2));
            item->setText(2,tmpStr);
        }
    }

    if ( codeEditor )
    {
        m_textChangedMapper->removeMappings(codeEditor);
        disconnect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
    }
    else if ( projectFileEditor )
    {
        m_textChangedMapper->removeMappings(projectFileEditor);
        disconnect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

        m_saveSignalMapper->setMapping(projectFileEditor,"current");
        connect(projectFileEditor,SIGNAL(saveRequested()),m_saveSignalMapper,SLOT(map()));
    }
    else if ( pointsFileEditor )
    {
        m_textChangedMapper->removeMappings(pointsFileEditor);
        disconnect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
    }

    ui->fileEditor->setTabText(ui->fileEditor->indexOf(widget),name+'*');
    ui->actionSaveAll->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionReload->setEnabled(false);
}

void MainWindow::updateRedoStatus(bool available)
{
    ui->actionRedo->setEnabled(available);

    if ( available )
    {
        if ( m_redos.contains(m_undoRedoRegisteredItem) )
            return;

        m_redos.append( m_undoRedoRegisteredItem );
    }
    else
    {
        if ( !m_redos.contains(m_undoRedoRegisteredItem))
            return;

        for ( unsigned i=0; i<m_redos.count(); ++i )
            if ( m_redos[i] == m_undoRedoRegisteredItem )
                m_redos.removeAt(i);
    }
}

void MainWindow::updateUndoStatus(bool available)
{
    ui->actionUndo->setEnabled(available);

    if ( available )
    {
        if ( m_undos.contains(m_undoRedoRegisteredItem) )
            return;

        m_undos.append( m_undoRedoRegisteredItem );
    }
    else
    {
        if ( !m_undos.contains(m_undoRedoRegisteredItem))
            return;

        for ( unsigned i=0; i<m_undos.count(); ++i )
            if ( m_undos[i] == m_undoRedoRegisteredItem )
                m_undos.removeAt(i);
    }
}

void MainWindow::determineIfUndoRedoIsAvailable(QWidget *widget)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);

    if ( lineEdit )
    {
        if ( lineEdit->isUndoAvailable() )
            ui->actionUndo->setEnabled(true);
        else
            ui->actionUndo->setEnabled(false);

        if ( lineEdit->isRedoAvailable() )
            ui->actionRedo->setEnabled(true);
        else
            ui->actionRedo->setEnabled(false);

        if ( !lineEdit->text().isEmpty() )
            ui->actionSelect_All->setEnabled(true);
        else
            ui->actionSelect_All->setEnabled(true);

        if ( !m_clipboard->text().isEmpty() )
            ui->actionPaste->setEnabled(true);
        else
            ui->actionPaste->setEnabled(false);
    }
}

void MainWindow::determineIfCopyCutIsAvailable(QWidget *widget)
{
    QLineEdit* lineEdit = dynamic_cast<QLineEdit*>(widget);

    if ( lineEdit )
    {
        if ( lineEdit->hasSelectedText() )
        {
            ui->actionCopy->setEnabled(true);
            ui->actionCut->setEnabled(true);
        }
        else
        {
            ui->actionCopy->setEnabled(false);
            ui->actionCut->setEnabled(false);
        }
    }
}

void MainWindow::fileChanged(const QString &path)
{
    QString fileName, filePath;

    if ( m_reloadFilesDialog == NULL )
    {
        m_reloadFilesDialog = new ReloadFilesDialog(this);
        connect(m_reloadFilesDialog,SIGNAL(reloadFiles(QList<QPair<QString,QString> >)),this,SLOT(reloadFiles(QList<QPair<QString,QString> >)));
        connect(m_reloadFilesDialog,SIGNAL(rejected()),this,SLOT(deleteReloadFileDialog()));
        m_reloadFilesDialog->show();
    }

    for (unsigned i=path.length();i!=0;--i)
    {
        if ( path[i-1] == '\\' || path[i-1] == '/' )
        {
            filePath = path.left(i);
            fileName = path.right(path.length()-i);
            break;
        }
    }

    if ( fileName.endsWith(".pro") || fileName.endsWith(".pro*") )
        m_reloadFilesDialog->addFile(QIcon(":/new/icons/profile.png"), filePath, fileName);
    else if ( fileName.endsWith(".js") || fileName.endsWith(".js*") )
        m_reloadFilesDialog->addFile(QIcon(":/new/icons/pythonfile.png"), filePath, fileName);
    else if ( fileName.endsWith(".pt") || fileName.endsWith(".pt*") )
        m_reloadFilesDialog->addFile(QIcon(":/new/icons/pointsfile.png"), filePath, fileName);
}

void MainWindow::reloadFiles(QList<QPair<QString, QString> > listOfFiles)
{
    for ( QPair<QString, QString> fileNpath : listOfFiles )
    {
        for ( unsigned i=0; i<ui->fileEditor->count(); ++i)
        {
            if ( ui->fileEditor->tabText(i) == fileNpath.first ||
                 ui->fileEditor->tabText(i) == fileNpath.first+"*" )
            {
                ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>( ui->fileEditor->widget(i) );
                CodeEditor* codeEditor = dynamic_cast<CodeEditor*>( ui->fileEditor->widget(i) );
                PointsFileEditor* pointsFileEditor = dynamic_cast<PointsFileEditor*>( ui->fileEditor->widget(i) );
                QFile file(fileNpath.second  +fileNpath.first);

                if ( file.open(QIODevice::ReadWrite | QFile::Text))
                {
                    QTextStream textStream(&file);
                    m_fileSystemWatcher->removePath(fileNpath.second + fileNpath.first);

                    if ( projectFileEditor )
                    {
                        if ( projectFileEditor->path == fileNpath.second )
                        {
                            disconnect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

                            projectFileEditor->populateFromString(textStream.readAll());
                            saveClicked(fileNpath.second+fileNpath.first);

                            connect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
                        }
                    }
                    else if ( codeEditor )
                    {
                        if ( codeEditor->path == fileNpath.second )
                        {
                            disconnect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));

                            codeEditor->document()->setPlainText(textStream.readAll());
                            saveClicked(fileNpath.second+fileNpath.first);

                            connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
                        }
                    }
                    else if ( pointsFileEditor )
                    {
                        if ( pointsFileEditor->path == fileNpath.second )
                        {
                            disconnect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

                            pointsFileEditor->populateFromString(textStream.readAll());
                            saveClicked(fileNpath.second+fileNpath.first);

                            connect(pointsFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));
                        }
                    }

                    m_fileSystemWatcher->addPath(fileNpath.second + fileNpath.first);
                }
            }
        }
    }

    delete m_reloadFilesDialog;
    m_reloadFilesDialog = NULL;
}

void MainWindow::copyAvailable(bool yes)
{
    if ( yes )
    {
        ui->actionCopy->setEnabled(true);
        ui->actionCut->setEnabled(true);
    }
    else
    {
        ui->actionCopy->setEnabled(false);
        ui->actionCut->setEnabled(false);
    }
}

void MainWindow::projectExplorerDoubleClicked(QTreeWidgetItem *item, int column)
{
    switch ( item->type() )
    {
    case FileType:
    {
        QString name = item->text(0) < item->text(2) ? item->text(0) : item->text(2);

        //Check if it is already opened and if it is, set tab as current and retrun
        for ( unsigned idx=0; idx < ui->fileEditor->count(); ++idx )
        {
            if (ui->fileEditor->tabText(idx)==item->text(0) ||
                    ui->fileEditor->tabText(idx)==item->text(2) )
            {
                if ( name.endsWith(".js"))
                {
                    CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(idx));

                    if ( codeEditor->path == item->text(1) )
                    {
                        ui->fileEditor->setCurrentIndex(idx);
                        return;
                    }
                }
                else if ( name.endsWith(".pt"))
                {
                    PointsFileEditor* pointsEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(idx));

                    if ( pointsEditor->path == item->text(1) )
                    {
                        ui->fileEditor->setCurrentIndex(idx);
                        return;
                    }
                }
            }
        }

        // If not, we have to create apropierate widget

        QFile file(item->text(1)+name);

        if ( file.open(QIODevice::ReadWrite | QFile::Text))
        {
            if ( name.endsWith(".js") )
            {
                CodeEditor* codeEditor = new CodeEditor(this);
                QTextStream textStream(&file);

                codeEditor->turnOnPythonHighlighting();

                codeEditor->document()->setPlainText(textStream.readAll());
                codeEditor->path = item->text(1);

                m_textChangedMapper->setMapping(codeEditor,codeEditor);
                connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
                connect(codeEditor,SIGNAL(copyAvailable(bool)),this,SLOT(copyAvailable(bool)));

                m_redoMapper->setMapping(codeEditor,codeEditor);
                connect(codeEditor,SIGNAL(redoAvailable(bool)),m_redoMapper,SLOT(map()));

                m_undoMapper->setMapping(codeEditor,codeEditor);
                connect(codeEditor,SIGNAL(undoAvailable(bool)),m_undoMapper,SLOT(map()));

                connect(codeEditor,SIGNAL(redoAvailable(bool)),this,SLOT(updateRedoStatus(bool)));
                connect(codeEditor,SIGNAL(undoAvailable(bool)),this,SLOT(updateUndoStatus(bool)));

                ui->fileEditor->addTab(codeEditor,QIcon(":/new/icons/pythonfile.png"),item->text(0));
                ui->fileEditor->setCurrentWidget(codeEditor);
            }
            else if ( name.endsWith(".pt") )
            {
                PointsFileEditor* pointsEditor = new PointsFileEditor(this);
                QTextStream textStream(&file);

                pointsEditor->populateFromString(textStream.readAll());
                pointsEditor->path = item->text(1);

                m_textChangedMapper->setMapping(pointsEditor,pointsEditor);
                connect(pointsEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

                ui->fileEditor->addTab(pointsEditor,QIcon(":/new/icons/pointsfile.png"),item->text(0));
                ui->fileEditor->setCurrentWidget(pointsEditor);
            }
            else
            {
                CodeEditor* codeEditor = new CodeEditor(this);
                QTextStream textStream(&file);

                codeEditor->document()->setPlainText(textStream.readAll());
                codeEditor->path = item->text(1);

                m_textChangedMapper->setMapping(codeEditor,codeEditor);
                connect(codeEditor,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));
                connect(codeEditor,SIGNAL(copyAvailable(bool)),this,SLOT(copyAvailable(bool)));

                m_redoMapper->setMapping(codeEditor,codeEditor);
                connect(codeEditor,SIGNAL(redoAvailable(bool)),m_redoMapper,SLOT(map()));

                m_undoMapper->setMapping(codeEditor,codeEditor);
                connect(codeEditor,SIGNAL(undoAvailable(bool)),m_undoMapper,SLOT(map()));

                connect(codeEditor,SIGNAL(redoAvailable(bool)),this,SLOT(updateRedoStatus(bool)));
                connect(codeEditor,SIGNAL(undoAvailable(bool)),this,SLOT(updateUndoStatus(bool)));

                ui->fileEditor->addTab(codeEditor,QIcon(":/new/icons/pythonfile.png"),item->text(0));
                ui->fileEditor->setCurrentWidget(codeEditor);
            }

            ui->fileEditor->currentWidget()->setFocus();

            ui->actionReload->setEnabled(true);

            m_fileSystemWatcher->addPath(item->text(1)+name);
        }

        break;
    }
    case ConfigType:
    {
        QString name = item->text(0) < item->text(2) ? item->text(0) : item->text(2);

        //Check if it is already opened and if it is, set tab as current and retrun
        for ( unsigned idx=0; idx < ui->fileEditor->count(); ++idx )
        {
            if (ui->fileEditor->tabText(idx)==item->text(0) ||
                    ui->fileEditor->tabText(idx)==item->text(2) )
            {
                ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(idx));

                if ( projectFileEditor->path == item->text(1) )
                {
                    ui->fileEditor->setCurrentIndex(idx);
                    return;
                }
            }
        }

        // If not, we have to create apropierate widget
        QFile file(item->text(1)+name);

        if ( file.open(QIODevice::ReadWrite | QFile::Text))
        {
            ProjectFileEditor* projectFileEditor = new ProjectFileEditor(this);
            QTextStream textStream(&file);

            projectFileEditor->populateFromString(textStream.readAll());
            projectFileEditor->path = item->text(1);

            m_textChangedMapper->setMapping(projectFileEditor,projectFileEditor);
            connect(projectFileEditor,SIGNAL(contentChanged()),m_textChangedMapper,SLOT(map()));

            ui->fileEditor->addTab(projectFileEditor,QIcon(":/new/icons/profile.png"),item->text(0));
            ui->fileEditor->setCurrentWidget(projectFileEditor);
        }

        break;
    }
    default:
        break;
    }
}

void MainWindow::tabCloseClicked(int idx)
{
    QString tabName = ui->fileEditor->tabText(idx);

    if ( tabName.endsWith('*') )
    {
        m_saveChangesDialog = new SaveChangesDialog(this);
        CodeEditor* codeEditor = dynamic_cast<CodeEditor*>(ui->fileEditor->widget(idx));
        ProjectFileEditor* projectFileEditor = dynamic_cast<ProjectFileEditor*>(ui->fileEditor->widget(idx));
        PointsFileEditor* pointsFileEditor = dynamic_cast<PointsFileEditor*>(ui->fileEditor->widget(idx));

        if ( codeEditor )
        {
            m_saveChangesDialog->addFile(QIcon(":/new/icons/pythonfile.png"),
                                         tabName,
                                         codeEditor->path);
        }
        else if ( projectFileEditor )
        {
            m_saveChangesDialog->addFile(QIcon(":/new/icons/profile.png"),
                                         tabName,
                                         projectFileEditor->path);
        }
        else if ( pointsFileEditor )
        {
            m_saveChangesDialog->addFile(QIcon(":/new/icons/pointsfile.png"),
                                         tabName,
                                         pointsFileEditor->path);
        }

        if ( m_saveChangesDialog->exec() )
        {
            for ( QPair<QString, QString> fileNpath : m_saveChangesDialog->getSelectedFiles() )
                saveClicked(fileNpath.first+imposibleDelimiter+fileNpath.second);

            foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(tabName,Qt::MatchExactly | Qt::MatchRecursive,0) )
            {
                QString tmp = item->text(0);
                item->setText(0,item->text(2));
                item->setText(2,tmp);
            }

            ui->fileEditor->removeTab(idx);
        }
    }
    else
        ui->fileEditor->removeTab(idx);
}

void MainWindow::currentTabChanged(int idx)
{
    QString name = ui->fileEditor->tabText(idx);
    name = name.endsWith("*") ? name.left(name.length()-1) : name;
    this->setWindowTitle("NWTR Robotics - " + name);

    if ( ui->fileEditor->currentIndex() > 0 )
        ui->actionBack->setEnabled(true);
    else
        ui->actionBack->setEnabled(false);

    if ( ui->fileEditor->currentIndex() < ui->fileEditor->count()-1 )
        ui->actionForward->setEnabled(true);
    else
        ui->actionForward->setEnabled(false);

    if ( idx < 0 )
    {
        ui->actionClose_File->setEnabled(false);
        ui->actionCloseAllFiles->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSave_as->setEnabled(false);
        ui->actionUndo->setEnabled(false);
        ui->actionRedo->setEnabled(false);
        ui->actionPrint->setEnabled(false);
        ui->actionReload->setEnabled(false);

        ui->fileEditor->hide();
        ui->logo->show();
    }
    else if ( idx >= 0 )
    {
        ui->actionCloseAllFiles->setEnabled(true);
        ui->actionClose_File->setEnabled(true);
        ui->actionPrint->setEnabled(true);

        ui->logo->hide();
        ui->fileEditor->show();
    }
}



























