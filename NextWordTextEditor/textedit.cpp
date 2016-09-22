/*
 * This file is modified program taken from the examples of the Qt Toolkit under BSD license usage
 *
*/

/*
 * TextEdit Class
 * Implement QTextEdit
*/
#include "textedit.h"

#include <QtWidgets>

#include <QCompleter>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtDebug>
#include <QApplication>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QScrollBar>

QString prevWord = "";

//! [0]
//! main function
//! set text editor format & highlighter
TextEdit::TextEdit(QWidget *parent)
: QTextEdit(parent), c(0)
{
    QFont font;
    font.setFamily("Arial");
    font.setFixedPitch(true);
    font.setPointSize(12);
    this->setFont(font);

    highlighter = new Highlighter(this->document());

}
//! [0]

//! [1]
//! function textEdit
TextEdit::~TextEdit()
{
}
//! [1]

//! [2]
//! function setCompleter(param: QCompleter)
//! set QCompleter class in text editor used to show words suggestion list
void TextEdit::setCompleter(QCompleter *completer)
{
    if (c)
        QObject::disconnect(c, 0, this, 0);

    c = completer;

    if (!c)
        return;

    c->setWidget(this);
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}
//! [2]

//! [3]
//! define completer in text editor
QCompleter *TextEdit::completer() const
{
    return c;
}
//! [3]

//! [4]
//! function insertCompletion(param: string, word under cursor)
//! insert selected suggestion to text editor
void TextEdit::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    c->setModel(modelUpdate(completion));

    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
    prevWord = "";
}
//! [4]

//! [5]
//! function textUnderCursor()
//! get currently typed words, under cursor
QString TextEdit::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
//! [5]

//! [6]
//! function: focusInEvent(param: QFocusEvent)
//! get focused event
void TextEdit::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QTextEdit::focusInEvent(e);
}
//! [6]

//! [7]
//! function keyPressEvent(param: QKeyEvent)
//! called when a key is triggered
void TextEdit::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible()) {
        // The following keys are forwarded by the completer to the widget
       switch (e->key()) {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return; // let the completer do default behavior
       default:
           break;
       }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) & (e->key() == Qt::Key_E)); // CTRL+E
    if (!c || !isShortcut) // do not process the shortcut when we have a completer
        QTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
    if (!c || (ctrlOrShift && e->text().isEmpty()))
        return;

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-="); // end of word
    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
    if (e->text() == " ")
        prevWord = prevWord;
    else
        prevWord = textUnderCursor();

    QString completionPrefix = prevWord;

    if (!isShortcut && ( hasModifier || e->text().isEmpty()|| completionPrefix.length() < 2
                      || eow.contains(e->text().right(1)))) {
        c->popup()->hide();
        return;
    }

    if (completionPrefix != c->completionPrefix()) {
        c->setCompletionPrefix(completionPrefix);
        c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
    }
    QRect cr = cursorRect();
    cr.setWidth(c->popup()->sizeHintForColumn(0)
                + c->popup()->verticalScrollBar()->sizeHint().width());
    c->complete(cr); // popup it up!
}
//! [7]

//! [8]
//! function: modelUpdate(param: string)
//! update suggestion model in QCompleter
//! set used words to higher rank
//! called in insertCompletion
QAbstractItemModel *TextEdit::modelUpdate(const QString& completion)
{

    QStringListModel *mode = (QStringListModel*)(c->model());

    QStringList words;
    words = mode->stringList();
    int row = words.indexOf(completion);
    if (row != 0) {
        words.removeAt(row);
        words.insert(row-1,completion);}

    #ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
    #endif
        return new QStringListModel(words, c);
}
//! [8]
