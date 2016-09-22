/*
 * Header class: MainWindow
 * Implement QMainWIndow
*/
#ifndef MAINEDITOR_H
#define MAINEDITOR_H

//import dependencies
#include <QMainWindow>

QT_BEGIN_NAMESPACE
class QAbstractItemModel;
class QComboBox;
class QCompleter;
class QLabel;
class QLineEdit;
class QProgressBar;
QT_END_NAMESPACE
class TextEdit;

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT

//set public methods &variables
    public:
        MainWindow(QWidget *parent = 0);
        void loadFile(const QString &fileName);

//set private slot methods & variables
    private slots:
        void about();
        void newFile();
        void openFile();
        bool saveAs();
        bool save();

//set private methods
    private:
        void createMenu();
        bool maybeSave();
        void setCurrentFile(const QString &fileName);
        bool saveFile(const QString &fileName);
        void closeEvent (QCloseEvent *event);
        QAbstractItemModel *modelFromFile(const QString& fileName);

        QCompleter *completer;
        TextEdit *completingTextEdit;
        QString curFile;
};
//! [0]

#endif // MAINWINDOW_H
