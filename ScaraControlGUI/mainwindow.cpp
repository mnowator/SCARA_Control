#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDomDocument>
#include <QFileDialog>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->workspace->hide();

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

    connect(ui->actionNew_Project,          SIGNAL(triggered(bool)),this,SLOT(newProjectClicked()));
    connect(ui->actionExit,                 SIGNAL(triggered(bool)),this,SLOT(exitAppClicked()));
    connect(ui->actionOpen_Project_or_File, SIGNAL(triggered(bool)),this,SLOT(openProjectProject()));
    connect(ui->actionCloseAll,             SIGNAL(triggered(bool)),this,SLOT(closeAllClicked()));

    connect(ui->projectExplorer, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(projectExplorerContextMenuRequested(QPoint)));
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

    delete m_newProjectDialog;
    delete m_newFileDialog;

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

void MainWindow::newProjectClicked()
{
    m_newProjectDialog = new NewProjectDialog(this);
    connect(m_newProjectDialog,SIGNAL(createProjectSignal(QString,QString,QString)),this,SLOT(createProject(QString,QString,QString)));
    m_newProjectDialog->show();
}

void MainWindow::openProjectProject()
{
    QString fullPath, projectFileName, projectName;
    QFileDialog folderDialog;
    QDomDocument projectDom;
    QDomElement root, element;

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
            projectFileName = fullPath.mid(i);
            projectName = projectFileName.split('.')[0];
            break;
        }
    }

    if (!ui->projectExplorer->findItems(projectName,Qt::MatchExactly,0).isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already loaded."));
        msgBox.exec();
        return;
    }
    if (!ui->projectExplorer->findItems(projectName,Qt::MatchExactly,2).isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already loaded."));
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
    project->setText(1,fullPath);
    project->setText(2,projectName + tr(" ( Active )"));
    project->setIcon(0,*(new QIcon(":/new/icons/lc_dbformopen.png")));

    QTreeWidgetItem* projectProFile = new QTreeWidgetItem(ConfigType);
    projectProFile->setText(0,projectName + ".pro");
    projectProFile->setText(1,fullPath + '/' + projectName + ".pro");
    projectProFile->setIcon(0,*(new QIcon(":/new/icons/pro_file.jpg")));
    project->addChild(projectProFile);

    ui->projectExplorer->addTopLevelItem(project);

    setActiveProject(projectName);

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
    ui->projectExplorer->clear();
    ui->workspace->hide();
}

void MainWindow::createProject(QString const& projectName, QString const& communicationType, QString const& projectPath)
{
    QString goodPath = projectPath + "/" + projectName;
    QDir dir(goodPath);

    if (!ui->projectExplorer->findItems(projectName,Qt::MatchExactly,0).isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project with given name is already loaded.\n"
                                                                 "Please change name or close another project."));
        msgBox.exec();
        return;
    }

    if (!ui->projectExplorer->findItems(projectName,Qt::MatchExactly,2).isEmpty())
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Project is already loaded."));
        msgBox.exec();
        return;
    }

    if ( !dir.exists())
        dir.mkpath(goodPath);


    if ( QFile::exists(goodPath + "/project.pro"))
    {
        QMessageBox msgBox(QMessageBox::Warning, tr("Error"), tr("Cannot create project.pro file because it already exist in given path.\n"
                                                                 "Please choose different localization to project."));

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

    QFile projectFile(goodPath + '/' + projectName + ".pro");
    if ( projectFile.open(QFile::WriteOnly | QFile::Text))
    {
        QDomDocument dom("SCARA_Control_Project_File");
        QDomElement elHigher, elLower, root;

        root = dom.createElement("Project");
        //root.setAttribute("project_name", projectName);
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
        root.appendChild(elHigher);

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

        QTextStream ts(&projectFile);
        ts << dom.toString();

        projectFile.close();
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
    projectProFile->setText(1,goodPath);
    projectProFile->setIcon(0,*(new QIcon(":/new/icons/pro_file.jpg")));
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

            m_saveAllSignalMapper   ->  setMapping(saveAll, item->text(0));
            m_reloadSignalMapper    ->  setMapping(reload,  item->text(0));
            m_cloneSignalMapper     ->  setMapping(clone,   item->text(0));
            m_addNewSignalMapper    ->  setMapping(addNew,  item->text(0));
            m_addExistSignalMapper  ->  setMapping(addExist,item->text(0));
            m_removeSignalMapper    ->  setMapping(remove,  item->text(0));
            m_runSignalMapper       ->  setMapping(run,     item->text(0));
            m_pauseSignalMapper     ->  setMapping(pause,   item->text(0));
            m_stopSignalMapper      ->  setMapping(stop,    item->text(0));
            m_restartSignalMapper   ->  setMapping(restart, item->text(0));
            m_closeSignalMapper     ->  setMapping(close,   item->text(0));

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

void MainWindow::saveAllClicked(const QString &name)
{

}


void MainWindow::closeClicked(const QString &name)
{
    foreach ( QTreeWidgetItem* item, ui->projectExplorer->findItems(name,Qt::MatchExactly,0) )
        ui->projectExplorer->takeTopLevelItem(ui->projectExplorer->indexOfTopLevelItem(item));

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
        file->setText(1,item->text(1)+fileName);

        item->addChild(file);
    }

    ui->fileEditor->addTab(new QTextEdit(this),fileName+ '*');
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



























