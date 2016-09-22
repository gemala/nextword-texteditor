/*
 * This file is modified program taken from the examples of the Qt Toolkit under BSD license usage
 *
*/

/*
 * Class MainWindow
 * Set up main window widget and app's menu
*/

//import necessary classes & header
#include <QCloseEvent>
#include <QtWidgets>
#include "mainwindow.h"
#include "textedit.h"

//! [0]
//! main function: setting up main window
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), completer(0)
{
    //call function to create menu
    createMenu();

    //setting up text editor window
    completingTextEdit = new TextEdit;

    //setting up QCompleter class
    completer = new QCompleter(this);
    //set completer model from modelFromFile function
    completer->setModel(modelFromFile(":/resources/wordlist.txt"));
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    //set completer to text editor
    completingTextEdit->setCompleter(completer);

    //place text editor in the main window widget
    setCentralWidget(completingTextEdit);
    resize(700, 555);
    setWindowTitle(tr("Next Word Text Editor"));
}
//! [0]

//! [1]
//! function: createMenu()
//! set actions for menu and connect it to trigger
void MainWindow::createMenu()
{
    //define Icon for menu item
    const QIcon saveIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/save.png"));
    const QIcon openIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/open.png"));
    const QIcon newIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/new.png"));

    //define menu action
    QAction *exitAction = new QAction(tr("Exit"), this);
    QAction *aboutAct = new QAction(tr("About"), this);
    QAction *aboutQtAct = new QAction(tr("About Qt"), this);
    QAction *newFileAct = new QAction(newIcon,tr("New File"),this);
    newFileAct->setShortcuts(QKeySequence::New);
    QAction *closeFileAct = new QAction(newIcon,tr("Close File"),this);
    newFileAct->setShortcuts(QKeySequence::Close);
    QAction *openFileAct = new QAction(openIcon,tr("Open File"),this);
    openFileAct->setShortcuts(QKeySequence::Open);
    QAction *saveAsAct = new QAction(saveIcon,tr("Save As"),this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    QAction *saveAct = new QAction(saveIcon,tr("Save"),this);
    saveAct->setShortcuts(QKeySequence::Save);

    //connecting action
    connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(newFileAct,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(closeFileAct,SIGNAL(triggered()),this,SLOT(newFile()));
    connect(openFileAct,SIGNAL(triggered()),this,SLOT(openFile()));
    connect(saveAsAct,SIGNAL(triggered()),this,SLOT(saveAs()));
    connect(saveAct,SIGNAL(triggered()),this,SLOT(save()));

    //add actions to the menu
    QMenu* fileMenu = menuBar()->addMenu(tr("File"));
    fileMenu->addAction(exitAction);
    fileMenu->addAction(newFileAct);
    fileMenu->addAction(openFileAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(closeFileAct);

    QMenu* helpMenu = menuBar()->addMenu(tr("About"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    //add actions to toolbar
    QToolBar *tools = addToolBar(tr("Tools"));
    tools->addAction(newFileAct);
    tools->addAction(openFileAct);
    tools->addAction(saveAct);

}
//! [1]

//! [2]
//! function: modelFromFile()
//! get list of words sugestions from resource file
//! read the file, assign content as QCompleter model
//! Return QStringListModel
//! Parameter: string of file path
QAbstractItemModel *MainWindow::modelFromFile(const QString& fileName)
{
    //open source file
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly))
        return new QStringListModel(completer);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    #endif

    //read & save content as list of string
    QStringList words;
    while (!file.atEnd()) {
        QByteArray line = file.readLine();
        if (!line.isEmpty())
            words << line.trimmed();
    }

    //return list of words as completer model
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
        return new QStringListModel(words, completer);
}
//! [2]

//! [3]
//! function: About()
//! Shows pop up message box
//! Called in aboutAct
void MainWindow::about()
{
    QMessageBox::about(this, tr("About"), tr("The Next Word Text Editor is a text editing application with next word suggestion feature"));
}
//! [3]

//! [4]
//! function: newFile()
//! Reset text in text editor window
//! check if current file is saved
//! called in newFileAct
void MainWindow::newFile()
{
    if (maybeSave()) {
        completingTextEdit->clear();
        setCurrentFile(QString());
    }
}
//! [4]

//! [5]
//! function: openFile()
//! shows open file dialog
//! check if current file is saved
//! called in openFileAct
void MainWindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "Text Files (*.tex *.txt)");
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}
//! [5]

//! [6]
//! function: saveAs()
//! Open dialog to save current text as new file
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return false;
    return saveFile(dialog.selectedFiles().first());
}
//! [6]

//! [7]
//! Function: save()
//! check if current text is an opened file
//! open dialog save as new file if current text is not saved
//! save text as file
bool MainWindow::save()
{
    if (curFile.isEmpty()) {
        return saveAs();
    } else {
        return saveFile(curFile);
    }
}
//! [7]

//! [8]
//! Function:setCurrentFile(param: string file path)
//! called when open a file & after saving a file
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    completingTextEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
    setWindowFilePath(shownName);
}
//! [8]

//! [9]
//! Function: maybeSave()
//! Check if the text is modified
//! Open dialog to save text
//! Called when open file, create new file and close file
bool MainWindow::maybeSave()
{
    if (!completingTextEdit->document()->isModified())
        return true;
    const QMessageBox::StandardButton ret
        = QMessageBox::warning(this, tr("Application"),
                               tr("The document has been modified.\n"
                                  "Do you want to save your changes?"),
                               QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    switch (ret) {
    case QMessageBox::Save:
        return save();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }
    return true;
}
//! [9]

//! [10]
//! Function saveFile(param: string, file path)
//! check if file is writeable
//! save current text to the file
//! called in save & save as function
bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName),
                                  file.errorString()));
        return false;
    }

    QTextStream out(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        out << completingTextEdit->toPlainText();
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
//! [10]

//! [11]
//! Function loadFile(param: string, file path)
//! load existing text file
void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }
    QTextStream in(&file);
    #ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
    #endif
        completingTextEdit->setPlainText(in.readAll());
    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}
//! [11]


//! [12]
//! check if file is saved on close
//! called when user click on x in window
void MainWindow::closeEvent (QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    }
}
//![12]
