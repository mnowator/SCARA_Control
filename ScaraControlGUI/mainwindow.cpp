#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDomDocument>
#include <QFileDialog>
#include <QTextEdit>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->workspace->hide();

    m_renameSignalMapper    =   new QSignalMapper(this);
    m_saveSignalMapper      =   new QSignalMapper(this);
    m_saveAsSignalMapper    =   new QSignalMapper(this);
    m_setActiveMapper       =   new QSignalMapper(this);
    m_saveAllSignalMapper   =   new QSignalMapper(this);
    m_closeSignalMapper     =   new QSignalMapper(this);
    m_reloadSignalMapper    =   new QSignalMapper(this);
    m_cloneSignalMapper     =   new QSignalMapper(this);
    m_addNewSignalMapper    =   new QSignalMapper(this);
    m_addExistSignalMapper  =   new QSignalMapper(this);
    m_removeSignalMapper    =   new QSignalMapper(this);
    m_runSignalMapper       =   new QSignalMapper(this);
    m_pauseSignalMapper     =   new QSignalMapper(this);
    m_stopSignalMapper      =   new QSignalMapper(this);
    m_restartSignalMapper   =   new QSignalMapper(this);
    m_textChangedMapper     =   new QSignalMapper(this);

    connect(m_renameSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(renameClicked     (QString)));
    connect(m_saveSignalMapper,     SIGNAL(mapped(QString)),this,SLOT(saveClicked       (QString)));
    connect(m_saveAsSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(saveAsClicked     (QString)));
    connect(m_setActiveMapper,      SIGNAL(mapped(QString)),this,SLOT(setActiveClicked  (QString)));
    connect(m_saveAllSignalMapper,  SIGNAL(mapped(QString)),this,SLOT(saveAllClicked    (QString)));
    connect(m_closeSignalMapper,    SIGNAL(mapped(QString)),this,SLOT(closeClicked      (QString)));
    connect(m_reloadSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(reloadClicked     (QString)));
    connect(m_cloneSignalMapper,    SIGNAL(mapped(QString)),this,SLOT(cloneClicked      (QString)));
    connect(m_addNewSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(addNewClicked     (QString)));
    connect(m_addExistSignalMapper, SIGNAL(mapped(QString)),this,SLOT(addExistClicked   (QString)));
    connect(m_removeSignalMapper,   SIGNAL(mapped(QString)),this,SLOT(removeClicked     (QString)));
    connect(m_runSignalMapper,      SIGNAL(mapped(QString)),this,SLOT(runClicked        (QString)));
    connect(m_pauseSignalMapper,    SIGNAL(mapped(QString)),this,SLOT(pauseClicked      (QString)));
    connect(m_stopSignalMapper,     SIGNAL(mapped(QString)),this,SLOT(stopClicked       (QString)));
    connect(m_restartSignalMapper,  SIGNAL(mapped(QString)),this,SLOT(restartClicked    (QString)));
    connect(m_textChangedMapper,    SIGNAL(mapped(QString)),this,SLOT(textChanged       (QString)));

    connect(ui->actionNew_Project,          SIGNAL(triggered(bool)),this,SLOT(newProjectClicked()));
    connect(ui->actionExit,                 SIGNAL(triggered(bool)),this,SLOT(exitAppClicked()));
    connect(ui->actionOpen_Project_or_File, SIGNAL(triggered(bool)),this,SLOT(openProjectProject()));
    connect(ui->actionCloseAll,             SIGNAL(triggered(bool)),this,SLOT(closeAllClicked()));
    connect(ui->actionBack,                 SIGNAL(triggered(bool)),this,SLOT(backClicked()));
    connect(ui->actionForward,              SIGNAL(triggered(bool)),this,SLOT(forwardClicked()));

    connect(ui->projectExplorer, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(projectExplorerDoubleClicked(QTreeWidgetItem*,int)));
    connect(ui->projectExplorer, SIGNAL(customContextMenuRequested(QPoint)),     this,SLOT(projectExplorerContextMenuRequested(QPoint)));

    connect(ui->fileEditor,SIGNAL(tabCloseRequested(int)),this,SLOT(tabCloseClicked(int)));
    connect(ui->fileEditor,SIGNAL(currentChanged(int)),this,SLOT(currentTabChanged(int)));
}

MainWindow::~MainWindow()
{
    delete m_saveAllSignalMapper;
    delete m_closeSignalMapper;
    delete m_reloadSignalMapper;
    delete m_cloneSignalMapper;
    delete m_removeSignalMapper;
    delete m_runSignalMapper;
    delete m_pauseSignalMapper;
    delete m_stopSignalMapper;
    delete m_restartSignalMapper;

    delete ui;
}

void MainWindow::setActiveProject(const QString &projectName)
{
    if ( !m_activeProject.isEmpty() )
        for ( unsigned index=0; index<ui->projectExplorer->topLevelItemCount(); ++index)
        {
            QTreeWidgetItem* item = ui->projectExplorer->topLevelItem(index);

            if ( item->text(0).length() > item->text(2).length() )
            {
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
    QDomNode filesNode;
    QString errorStr, projectFileContent;
    int errorLine, errorColumn;

    projectFileContent = loadFile(projectPath,projectName);

    if ( projectFileContent.isEmpty() ) return;

    if (!domProject.setContent(projectFileContent, false, &errorStr, &errorLine, &errorColumn))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);
        msgBox.exec();
        return;
    }

    root = domProject.documentElement();

    if ( root.tagName() != "Project")
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'Project' tag in .pro file."));
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

void MainWindow::saveFile(const QString &filePath, const QString &fileName, const QString &fileContent)
{
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
        msgBox.exec();
    }
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
        msgBox.exec();
        return QString();
    }
}

void MainWindow::newProjectClicked()
{
    m_newProjectDialog = new NewProjectDialog(this);
    connect(m_newProjectDialog,SIGNAL(createProjectSignal(QString,QString,QString)),this,SLOT(createProject(QString,QString,QString)));
    m_newProjectDialog->show();
}

void MainWindow::openProjectProject()
{
    QString fullPath, projectFileName, projectName, projectPath;
    QFileDialog folderDialog;
    QDomDocument projectDom;
    QDomElement root;

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
            projectPath = fullPath.left(i);
            projectFileName = fullPath.right(fullPath.length()-i);
            projectName = projectFileName.split('.')[0];
            break;
        }
    }

    if ( m_scaraRobots.contains(projectName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project with given name is already loaded.\n"
                                                                 "Please change name or close another project."));
        msgBox.exec();
        return;
    }

    QFile file(fullPath);

    if ( file.open(QIODevice::ReadWrite))
    {
        QString errorStr;
        int errorLine;
        int errorColumn;

        if (!projectDom.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), errorStr);
            msgBox.exec();
            return;
        }
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot open file."));
        msgBox.exec();
        return;
    }

    root = projectDom.documentElement();
    if ( root.tagName() != "Project")
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("There is no 'Project' tag in .pro file."));
        msgBox.exec();
        return;
    }  

    QTreeWidgetItem* header = new QTreeWidgetItem();
    header->setText(0, tr("Projects"));
    ui->projectExplorer->setHeaderItem(header);

    QTreeWidgetItem* project = new QTreeWidgetItem(ProjectType);
    project->setText(0,projectName);
    project->setText(1,projectPath);
    project->setText(2,projectName + tr(" ( Active )"));
    project->setIcon(0,*(new QIcon(":/new/icons/lc_dbformopen.png")));

    QTreeWidgetItem* projectProFile = new QTreeWidgetItem(ConfigType);
    projectProFile->setText(0,projectName + ".pro");
    projectProFile->setText(1,projectPath);
    projectProFile->setText(2,projectName + ".pro*");
    projectProFile->setIcon(0,*(new QIcon(":/new/icons/lc_adddirect.png")));
    project->addChild(projectProFile);

    ui->projectExplorer->addTopLevelItem(project);

    m_scaraRobots[projectName] = ScaraRobot();

    setActiveProject(projectName);

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
            msgBox.exec();
            continue;
        }

        if ( !path.isNull() )
            fileTreeWidgetItem->setText(1,path.text());
        else
        {
            QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project file is corrupted\n"
                                                                     "'Path' tag is missing in one file."));
            msgBox.exec();
            continue;
        }

        fileTreeWidgetItem->setIcon(0,*(new QIcon(":/new/icons/lc_adddirect.png")));

        project->addChild(fileTreeWidgetItem);
    }

    ui->projectExplorer->addTopLevelItem(project);

    m_scaraRobots[projectName] = ScaraRobot();

    ui->workspace->show();
    ui->actionCloseAll->setEnabled(true);

//    element = root.namedItem("Files").toElement();
//    if ( !element.isNull() )
//    {
//    }

    file.close();
}

void MainWindow::exitAppClicked()
{
    close();
}

void MainWindow::closeAllClicked()
{
    m_scaraRobots.clear();
    ui->projectExplorer->clear();
    ui->workspace->hide();
}

void MainWindow::backClicked()
{
    ui->fileEditor->setCurrentIndex(ui->fileEditor->currentIndex()-1);
}

void MainWindow::forwardClicked()
{
    ui->fileEditor->setCurrentIndex(ui->fileEditor->currentIndex()+1);
}

void MainWindow::createProject(QString const& projectName, QString const& communicationType, QString const& projectPath)
{
    QString fileFullPath = projectPath+'/'+projectName+'/'+projectName+".pro";
    QDir dir(projectPath+'/'+projectName);

    if ( m_scaraRobots.contains(projectName))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project with given name is already loaded.\n"
                                                                 "Please change name or close another project."));
        msgBox.exec();
        return;
    }

    if ( !dir.exists())
        dir.mkpath(projectPath+'/'+projectName);


    if ( QFile::exists(fileFullPath))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot create project file because it already exist in given path.\n"
                                                                 "Please choose different localization to project or project name."));

        msgBox.setStyleSheet("QPushButton {"
                                "color: rgb(230, 230, 230);"
                                "border: 2px solid #8f8f91;"
                                "border-radius: 3px;"
                                "height: 30px;"
                                "width: 60;"
                             "}"
                             "QPushButton:hover {"
                                "background-color: rgb(60, 187, 255);"
                             "}"
                             "QMessageBox {"
                                "background-color: rgb(64, 64, 64);"
                             "}"
                             "QLabel {"
                                "color: rgb(230, 230, 230);"
                             "}"
                             );
        msgBox.exec();
        return;
    }

    QDomDocument dom("SCARA_Control_Project_File");
    QDomElement elHigher, elLower, root;

    root = dom.createElement("Project");
    dom.appendChild(root);

//        elHigher = dom.createElement("ProjectName");
//        elHigher.appendChild(dom.createTextNode(projectName));
//        root.appendChild(elHigher);

//        elHigher = dom.createElement("Files");
//        root.appendChild(elHigher);

//        elHigher = dom.createElement("ScaraConfig");
//            elLower = dom.createElement("FirstSegmentLength");
//            elLower.appendChild(dom.createTextNode(""));
//            elHigher.appendChild(elLower);  
            
//            elLower = dom.createElement("SecondSegmentLength");
//            elLower.appendChild(dom.createTextNode(""));
//            elHigher.appendChild(elLower); 
            
//            elLower = dom.createElement("ThirdSegmentLength");
//            elLower.appendChild(dom.createTextNode(""));
//            elHigher.appendChild(elLower);
            
//            elLower = dom.createElement("FirstSegmentBeginLimitAngle");
//            elLower.appendChild(dom.createTextNode(""));
//            elHigher.appendChild(elLower);
        
//        el.setAttribute("first_segment_begin_limit_angle", "0");
//        el.setAttribute("first_segment_end_limit_angle", "0");
//        el.setAttribute("second_segment_begin_limit_angle", "0");
//        el.setAttribute("second_segment_end_limit_angle", "0");
//        el.setAttribute("third_segment_begin_limit_angle", "0");
//        el.setAttribute("third_segment_end_limit_angle", "0");
//        el.setAttribute("sync_freq", "");
//        el.setAttribute("motor_1_max_freq", "");
//        el.setAttribute("motor_2_max_freq", "");
//        el.setAttribute("motor_3_max_freq", "");
//        el.setAttribute("motor_1_max_steps", "");
//        el.setAttribute("motor_2_max_steps", "");
//        el.setAttribute("motor_3_max_steps", "");
//        root.appendChild(elHigher);

    elHigher = dom.createElement("CommunicationConfig");

    if ( communicationType == "Serial Communication ( COM )")
        elHigher.setAttribute("communication_type", communicationType);
//        el.setAttribute("com_name", "");
//        el.setAttribute("baud_rate", "");
//        el.setAttribute("data_bits", "");
//        el.setAttribute("stop_bits", "");
//        el.setAttribute("parity", "");
//        el.setAttribute("flow_control", "");
    root.appendChild(elHigher);

    m_scaraRobots[projectName] = ScaraRobot();

    saveFile(projectPath+'/'+projectName,projectName+".pro",dom.toString());

    QTreeWidgetItem* header = new QTreeWidgetItem();
    header->setText(0, tr("Projects"));
    ui->projectExplorer->setHeaderItem(header);

    QTreeWidgetItem* project = new QTreeWidgetItem(ProjectType);
    project->setText(0,projectName);
    project->setText(1,projectPath + '/' + projectName);
    project->setText(2,projectName + tr(" ( Active )"));
    project->setIcon(0,*(new QIcon(":/new/icons/lc_dbformopen.png")));

    QTreeWidgetItem* projectProFile = new QTreeWidgetItem(ConfigType);
    projectProFile->setText(0,projectName + ".pro");
    projectProFile->setText(1,projectPath  + '/' + projectName);
    projectProFile->setIcon(0,*(new QIcon(":/new/icons/lc_adddirect.png")));
    project->addChild(projectProFile);

    ui->projectExplorer->addTopLevelItem(project);
    ui->actionCloseAll->setEnabled(true);

    setActiveProject(projectName);

    ui->workspace->show();

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
            QAction* saveAll    = new QAction(tr("Save Project"),       this);
            QAction* run        = new QAction(tr("Run"),                this);
            QAction* pause      = new QAction(tr("Pause"),              this);
            QAction* stop       = new QAction(tr("Stop"),               this);
            QAction* restart    = new QAction(tr("Restart"),            this);
            QAction* clone      = new QAction(tr("Clone"),              this);
            QAction* reload     = new QAction(tr("Reload"),             this);
            QAction* remove     = new QAction(tr("Remove"),             this);
            QAction* close      = new QAction(tr("Close"),              this);

            saveAll ->setIcon(QIcon(":/new/icons/lc_save.png"));
            run     ->setIcon(QIcon(":/new/icons/avl02049.png"));
            pause   ->setIcon(QIcon(":/new/icons/avl02050.png"));
            stop    ->setIcon(QIcon(":/new/icons/avl02051.png"));
            restart ->setIcon(QIcon(":/new/icons/avl02052.png"));

            menu.addAction(addNew);
            menu.addAction(addExist);
            menu.addSeparator();
            menu.addAction(setActive);
            menu.addSeparator();
            menu.addAction(saveAll);
            menu.addSeparator();
            menu.addAction(run);
            menu.addAction(pause);
            menu.addAction(stop);
            menu.addAction(restart);
            menu.addSeparator();
            menu.addAction(clone);
            menu.addAction(reload);
            menu.addAction(remove);
            menu.addAction(close);

            m_setActiveMapper       ->  setMapping(setActive,   item->text(0));
            m_saveAllSignalMapper   ->  setMapping(saveAll,     item->text(0));
            m_reloadSignalMapper    ->  setMapping(reload,      item->text(0));
            m_cloneSignalMapper     ->  setMapping(clone,       item->text(0));
            m_addNewSignalMapper    ->  setMapping(addNew,      item->text(0));
            m_addExistSignalMapper  ->  setMapping(addExist,    item->text(0));
            m_removeSignalMapper    ->  setMapping(remove,      item->text(0));
            m_runSignalMapper       ->  setMapping(run,         item->text(0));
            m_pauseSignalMapper     ->  setMapping(pause,       item->text(0));
            m_stopSignalMapper      ->  setMapping(stop,        item->text(0));
            m_restartSignalMapper   ->  setMapping(restart,     item->text(0));
            m_closeSignalMapper     ->  setMapping(close,       item->text(0));

            connect(setActive,  SIGNAL(triggered(bool)),m_setActiveMapper,      SLOT(map()));
            connect(saveAll,    SIGNAL(triggered(bool)),m_saveAllSignalMapper,  SLOT(map()));
            connect(reload,     SIGNAL(triggered(bool)),m_reloadSignalMapper,   SLOT(map()));
            connect(clone,      SIGNAL(triggered(bool)),m_cloneSignalMapper,    SLOT(map()));
            connect(addNew,     SIGNAL(triggered(bool)),m_addNewSignalMapper,   SLOT(map()));
            connect(addExist,   SIGNAL(triggered(bool)),m_addExistSignalMapper, SLOT(map()));
            connect(remove,     SIGNAL(triggered(bool)),m_removeSignalMapper,   SLOT(map()));
            connect(run,        SIGNAL(triggered(bool)),m_runSignalMapper,      SLOT(map()));
            connect(pause,      SIGNAL(triggered(bool)),m_pauseSignalMapper,    SLOT(map()));
            connect(stop,       SIGNAL(triggered(bool)),m_stopSignalMapper,     SLOT(map()));
            connect(restart,    SIGNAL(triggered(bool)),m_restartSignalMapper,  SLOT(map()));
            connect(close,      SIGNAL(triggered(bool)),m_closeSignalMapper,    SLOT(map()));

            break;
        }
        case ConfigType:
        {
            QAction* remove = new QAction(tr("Remove file"), this);
            QAction* rename = new QAction(tr("Rename"),      this);
            QAction* save   = new QAction(tr("Save"),        this);
            QAction* saveAs = new QAction(tr("Save As..."),  this);

            save    ->setIcon(QIcon(":/new/icons/lc_save.png"));
            saveAs  ->setIcon(QIcon(":/new/icons/lc_saveas.png"));

            menu.addAction(save);
            menu.addAction(saveAs);
            menu.addSeparator();
            menu.addAction(rename);
            menu.addSeparator();
            menu.addAction(remove);

            m_removeSignalMapper    ->  setMapping(remove,      item->text(0));
            m_renameSignalMapper    ->  setMapping(rename,      item->text(0));
            m_saveSignalMapper      ->  setMapping(save,        item->text(0));
            m_saveAsSignalMapper    ->  setMapping(saveAs,      item->text(0));

            connect(remove,     SIGNAL(triggered(bool)),m_removeSignalMapper,   SLOT(map()));
            connect(rename,     SIGNAL(triggered(bool)),m_renameSignalMapper,   SLOT(map()));
            connect(save,       SIGNAL(triggered(bool)),m_saveSignalMapper,     SLOT(map()));
            connect(saveAs,     SIGNAL(triggered(bool)),m_saveAsSignalMapper,   SLOT(map()));

            break;
        }
        case FileType:
        {
            QAction* remove = new QAction(tr("Remove file"), this);
            QAction* rename = new QAction(tr("Rename"),      this);
            QAction* save   = new QAction(tr("Save"),        this);
            QAction* saveAs = new QAction(tr("Save As..."),  this);

            save    ->setIcon(QIcon(":/new/icons/lc_save.png"));
            saveAs  ->setIcon(QIcon(":/new/icons/lc_saveas.png"));

            menu.addAction(save);
            menu.addAction(saveAs);
            menu.addSeparator();
            menu.addAction(rename);
            menu.addSeparator();
            menu.addAction(remove);

            m_removeSignalMapper    ->  setMapping(remove,      item->text(0));
            m_renameSignalMapper    ->  setMapping(rename,      item->text(0));
            m_saveSignalMapper      ->  setMapping(save,        item->text(0));
            m_saveAsSignalMapper    ->  setMapping(saveAs,      item->text(0));

            connect(remove,     SIGNAL(triggered(bool)),m_removeSignalMapper,   SLOT(map()));
            connect(rename,     SIGNAL(triggered(bool)),m_renameSignalMapper,   SLOT(map()));
            connect(save,       SIGNAL(triggered(bool)),m_saveSignalMapper,     SLOT(map()));
            connect(saveAs,     SIGNAL(triggered(bool)),m_saveAsSignalMapper,   SLOT(map()));

            break;
        }
        default:
        {
            break;
        }
        }
    }

    menu.exec( ui->projectExplorer->mapToGlobal(pos));
}

void MainWindow::renameClicked(const QString &name)
{

}

void MainWindow::saveClicked(const QString &name)
{
    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly | Qt::MatchRecursive,0) )
    {
        QString tmpStr = item->text(0);

        if ( item->text(0) < item->text(2)) continue;
        else if ( item->type() == FileType )
        {
            QTextEdit* textEdit;
            QTreeWidgetItem* parent = item->parent();
            QString projectName = parent->text(0).length()<parent->text(2).length() ? parent->text(0) : parent->text(2);

            for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
                if ( ui->fileEditor->tabText(i) == name )
                {                    
                    textEdit = dynamic_cast<QTextEdit*>( ui->fileEditor->widget(i) );
                    ui->fileEditor->setTabText(i,item->text(2));
                    break;
                }

            attachFileToProject(item->text(2),item->text(1),projectName+".pro",parent->text(1));

            saveFile(item->text(1),item->text(2),textEdit->toPlainText());

            item->setText(0,item->text(2));
            item->setText(2,tmpStr);
        }
    }
}

void MainWindow::saveAsClicked(const QString &name)
{

}

void MainWindow::setActiveClicked(const QString &name)
{
    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
        setActiveProject(name);
}

void MainWindow::saveAllClicked(const QString &name)
{

}


void MainWindow::closeClicked(const QString &name)
{
    bool prevent = false;

    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
    {
        QList<QPair<QString, QString> > selectedFiles;
        QList<QString> preventedFiles;

        m_saveChangesDialog = new SaveChangesDialog(this);

        for ( unsigned i=0; i<item->childCount(); ++i )
        {
            QTreeWidgetItem* child = item->child(i);

            if ( child->text(0) > child->text(2) )
            {
                m_saveChangesDialog->addFile(QIcon(":/new/icons/lc_adddirect.png"),child->text(0),child->text(1));
                preventedFiles.append(child->text(0));

                prevent = true;
            }
        }

        if ( !prevent )
        {
            if ( item->text(0) < item->text(2) )
                m_scaraRobots.remove(item->text(0));
            else m_scaraRobots.remove(item->text(2));

            for ( unsigned i=0; i<item->childCount(); ++i )
            {
                QTreeWidgetItem* child = item->child(i);

                for ( unsigned j=0; j<ui->fileEditor->count(); ++j )
                    if (child->text(0) == ui->fileEditor->tabText(j))
                        ui->fileEditor->removeTab(j);
            }

            delete item;
        }
        else
        {
            if ( m_saveChangesDialog->exec() )
            {
                for ( QString& file : m_saveChangesDialog->getSelectedFiles() )
                    saveClicked(file);

                if ( item->text(0) < item->text(2) )
                    m_scaraRobots.remove(item->text(0));
                else m_scaraRobots.remove(item->text(2));

                for ( unsigned i=0; i<item->childCount(); ++i )
                {
                    QTreeWidgetItem* child = item->child(i);

                    for ( unsigned j=0; j<ui->fileEditor->count(); ++j )
                        if (child->text(0) == ui->fileEditor->tabText(j))
                            ui->fileEditor->removeTab(j);
                }

                delete item;
            }
        }

        delete m_saveChangesDialog;
    }

    if ( ui->projectExplorer->topLevelItemCount() == 0)
        ui->workspace->hide();
}


void MainWindow::reloadClicked(const QString &name)
{

}

void MainWindow::cloneClicked(const QString &name)
{

}

void MainWindow::addNewClicked(const QString &name)
{
    QString fileName, fileType;
    QTextEdit* textEdit;

    m_newFileDialog = new NewFileDialog(this);

    if ( m_newFileDialog->exec() )
    {
        QStringList data = m_newFileDialog->retrieveData();
        fileName = data[0];
        fileType = data[1];
    }
    else return;

    if ( fileType == "Program ( Python File )")
        fileName += ".py";

    foreach( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,0))
    {
        QTreeWidgetItem* file = new QTreeWidgetItem(FileType);
        file->setText(0,fileName+'*');
        file->setText(1,item->text(1));
        file->setText(2,fileName);
        file->setIcon(0,*(new QIcon(":/new/icons/lc_adddirect.png")));

        item->addChild(file);
    }

    textEdit = new QTextEdit(fileName,this);
    m_textChangedMapper->setMapping(textEdit,fileName);
    connect(textEdit,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));

    ui->fileEditor->addTab(textEdit,fileName+ '*');
    ui->fileEditor->setCurrentWidget(textEdit);

    textEdit->clear();

//    for ( unsigned i=0; ui->fileEditor->count(); ++i )
//        if ( ui->fileEditor->tabText(i) == fileName+'*')
//            ui->fileEditor->setCurrentIndex(i);

    ui->fileEditor->currentWidget()->setFocus();
}

void MainWindow::addExistClicked(const QString &name)
{
}

void MainWindow::removeClicked(const QString &name)
{

}

void MainWindow::runClicked(const QString &name)
{

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

void MainWindow::textChanged(const QString &name)
{
    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly | Qt::MatchRecursive,0) )
    {
        QString tmpStr = item->text(0);

        item->setText(0,item->text(2));
        item->setText(2,tmpStr);
    }

    for ( unsigned i=0; i<ui->fileEditor->count(); ++i )
        if ( ui->fileEditor->tabText(i) == name )
            ui->fileEditor->setTabText(i,name+'*');
}

void MainWindow::projectExplorerDoubleClicked(QTreeWidgetItem *item, int column)
{
    for ( unsigned idx=0; idx < ui->fileEditor->count(); ++idx )
    {
        if (ui->fileEditor->tabText(idx)==item->text(0) ||
                ui->fileEditor->tabText(idx)==item->text(2))
        {
            ui->fileEditor->setCurrentIndex(idx);
            return;
        }
    }

    switch ( item->type() )
    {
    case FileType:
    {
        QString name = item->text(0) < item->text(2) ? item->text(0) : item->text(2);
        QFile file(item->text(1)+name);

        if ( file.open(QFile::ReadOnly | QFile::Text))
        {

            QTextStream textStream(&file);
            QTextEdit* textEdit = new QTextEdit(name,this);

            textEdit->setText(textStream.readAll());

            m_textChangedMapper->setMapping(textEdit,name);
            connect(textEdit,SIGNAL(textChanged()),m_textChangedMapper,SLOT(map()));

            ui->fileEditor->addTab(textEdit,name);

            for ( unsigned idx=0; idx < ui->fileEditor->count(); ++idx )
            {
                if (ui->fileEditor->tabText(idx)==item->text(0) ||
                        ui->fileEditor->tabText(idx)==item->text(2))
                {
                    ui->fileEditor->setCurrentIndex(idx);
                }
            }
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

    if ( tabName[tabName.length()-1] == '*' )
    {
        return;
    }

    ui->fileEditor->removeTab(idx);
}

void MainWindow::currentTabChanged(int idx)
{
    if ( ui->fileEditor->currentIndex() > 0 )
        ui->actionBack->setEnabled(true);
    else
        ui->actionBack->setEnabled(false);

    if ( ui->fileEditor->currentIndex() < ui->fileEditor->count()-1 )
        ui->actionForward->setEnabled(true);
    else
        ui->actionForward->setEnabled(false);
}



























