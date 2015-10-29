#ifndef UI_H
#define UI_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QSound>

#include "lexicalanalyzer.h"

namespace Ui {
class UI;
}

class UI : public QMainWindow
{
    Q_OBJECT

public:
    explicit UI(QWidget *parent = 0);
    ~UI();

private slots:
    void on_compileButton_clicked();
    void setLexicalTable();
    void showAboutBox(bool egg = false);

private:
    Ui::UI *ui;
    LexicalAnalyzer *la;
    QSignalMapper *signalMapper;
    QSound *eggSound;
};

#endif // UI_H
