/*
 * Header TextEdit class
 * define functions in TextEdit.cpp
*/

#ifndef TEXTEDIT_H
#define TEXTEDIT_H

//import dependencies
#include <QTextEdit>
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QCompleter;
class QAbstractItemModel;
QT_END_NAMESPACE

//! [0]
class TextEdit : public QTextEdit
{
    Q_OBJECT

    //set public methods &variables
    public:
        TextEdit(QWidget *parent = 0);
        ~TextEdit();

        void setCompleter(QCompleter *c);
        QCompleter *completer() const;

    //set protected methods & variables
    protected:
        void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
        void focusInEvent(QFocusEvent *e) Q_DECL_OVERRIDE;

    //set private slots methods
    private slots:
        void insertCompletion(const QString &completion);

    //set private methods & variables
    private:
        QString textUnderCursor() const;
        QAbstractItemModel *modelUpdate(const QString& completion);
        QCompleter *c;
        Highlighter *highlighter;
};
//! [0]

#endif // TEXTEDITOR_H
