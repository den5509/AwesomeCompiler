#include "ui.h"
#include "ui_ui.h"

#include <QMessageBox>

UI::UI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::UI)
{
    ui->setupUi(this);
    ui->LexTable->setColumnWidth(0,121);
    ui->LexTable->setColumnWidth(1,118);
    ui->LexTable->setColumnWidth(2,110);
    ui->rulesTextEdit->setReadOnly(true);
    ui->objectCodeTextEdit->setReadOnly(true);
    ui->optimizedCodeTextEdit->setReadOnly(true);

    connect(ui->aboutAction, SIGNAL(triggered()),
            this, SLOT(showAboutBox()));
    signalMapper = new QSignalMapper(this);
    connect(ui->lexTableAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    connect(ui->ruleAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    connect(ui->objectCodeAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));
    connect(ui->optimizedCodeAction, SIGNAL(triggered()),
            signalMapper, SLOT(map()));



    signalMapper->setMapping(ui->lexTableAction, 0);
    signalMapper->setMapping(ui->ruleAction, 1);
    signalMapper->setMapping(ui->objectCodeAction, 2);
    signalMapper->setMapping(ui->optimizedCodeAction, 3);

    connect (signalMapper, SIGNAL(mapped(int)),
             ui->stackedWidget, SLOT(setCurrentIndex(int)));

    la = new LexicalAnalyzer();
    connect(la, SIGNAL(done()),
            this, SLOT(setLexicalTable()));

    eggSound = new QSound("://fusrodah.wav");
}
//-------------------------------------------------------------------

UI::~UI()
{
    delete ui;
    delete la;
    delete eggSound;
}

//-------------------------------------------------------------------

void UI::on_compileButton_clicked()
{
    if (ui->inputTextEdit->toPlainText().simplified() == "FUS RO DAH")
        showAboutBox(true);

    la->analyze(ui->inputTextEdit->toPlainText());
}

//-------------------------------------------------------------------

void UI::setLexicalTable()
{
    auto *lt = ui->LexTable;

    lt->clearContents();
    lt->setRowCount(0);

    for (QString & data : la->getLexTable()[0])
    {
        lt->setRowCount(lt->rowCount() + 1);
        lt->setItem(lt->rowCount()-1, 0, new QTableWidgetItem(data));

    }

    QStringList headers;

    // I combine labels and labels to use because it all is
    // uninteresting and rare information
    for (int i = 0; i < la->getLexTable()[0].count(); i++)
    {
        lt->setItem(i, 1, new QTableWidgetItem(la->getLexTable()[1][i]));
        lt->setItem(i, 2, new QTableWidgetItem(la->getLexTable()[2][i] +
                                               la->getLexTable()[3][i]));
        headers << QString("%1").arg(i);
    }

    lt->setVerticalHeaderLabels(headers);
}

//-------------------------------------------------------------------

void UI::showAboutBox(bool egg)
{
    QMessageBox mb;
    mb.setWindowTitle(tr("О программе"));
    mb.setWindowIcon(QIcon("://icon.ico"));
    QString text = tr("<CENTER>Эта компилятор!</CENTER>"
                  "<CENTER>Автор программы - Денис Горелик</CENTER><br>"
                  "Большое спасибо <a href=http://sbeaumont.deviantart.com>sbeaumont</a> за "
                  "<a href=http://sbeaumont.deviantart.com/art/FUS-RO-DAH-ico-file-268629125>офигенную иконку</a>! =)");
    if (egg)
    {
        mb.setIconPixmap(QPixmap("://icon.ico"));
        text += "<br><br><CENTER><B><BIG>FUS RO DAH!</BIG></B></CENTER>";
        eggSound->play();
        connect (&mb, SIGNAL(destroyed()),
                 eggSound, SLOT(stop()));
        mb.setStyleSheet("QMessageBox { background-image: url(://FUSRODAH.png) }");
    }

    mb.setText(text);
    mb.exec();
}
