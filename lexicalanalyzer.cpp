#include "lexicalanalyzer.h"
#include <QMessageBox>
#include <QIcon>

LexicalAnalyzer::LexicalAnalyzer(QObject *parent) : QObject(parent)
{
    arithmeticOperators << '+' << '-';
    terminalSymbols << ' ' << '\t' << '\v' << '\f' << '\n' << '\r' << ';' << ':' << '{';
    terminalSymbols.unite(arithmeticOperators);

    keyWords << "case" << "of" << "end";

    identifier.setPattern("^(_?[a-zA-Z]\\w*)");
    digit.setPattern("^(\\d*\\.\\d*)");

    lexicalTable.push_back(QVector<QString>()); //for lexeme
    lexicalTable.push_back(QVector<QString>()); //for type of lexeme
    lexicalTable.push_back(QVector<QString>()); //for labels
    lexicalTable.push_back(QVector<QString>()); //for labels to use

    lexemeTypes << tr("идентификатор") << tr("ключевое слово") << tr("число")
                << tr("арифметический оператор") << tr("присваивание")
                << tr("точка с запятой") << tr("двоеточие");
}

//-------------------------------------------------------------------

LexicalAnalyzer::~LexicalAnalyzer()
{

}

//-------------------------------------------------------------------

void LexicalAnalyzer::analyze(QString source)
{
    for (QVector<QString> & column : lexicalTable)
        column.clear();

    source = source.simplified();
    source += ' '; /*
                    * because cycle below required terminal symbol at the end of source.
                    * this also allows to get rid of the check (i < source.size() - 1)
                    * in comments and assignment conditions.
                    * Also, this cycle is reinventing the wheel; it's better maybe to use
                    * QString::split(), but i'm too lazy to rewriting this code.
                    * It's also better to use QRegularExpression instead of QRegExp,
                    * but... you know. =)
                    */
    QString lexeme;

    for (int i = 0; i < source.size(); i++)
    {
        if (!terminalSymbols.contains(source[i]))
            lexeme += source[i];
        else
        {
            if (source[i] == '{')
            {
                do
                    i++;
                while (i < source.size() && source[i] != '}');
            }

            if(isDigit(lexeme) || isIdentifier(lexeme))
                lexeme.clear();

            if (lexeme.isEmpty())
            {
                isSemicolon(source[i]);
                isColon(source[i]);
                isArithmeticOperator(source[i]);
            }

            if (lexeme.isEmpty() && isAssign(source[i], source[i + 1]) )
                i++;

            if (!lexeme.isEmpty())
            {
                showError();
                return;
            }
        }
    }

    auto & lexemes = lexicalTable.at(0);

    for (int i = 0; i < lexemes.size(); i++)
        if ( compare(i, Type::identifier) && keyWords.contains(lexemes.at(i)))
            lexicalTable[1][i] = lexemeTypes[int(Type::keyword)];

    if (lexemes.isEmpty())
    {
        showError(tr("Тут нет кода!"));
        return;
    }

    emit done();
}

//-------------------------------------------------------------------

void LexicalAnalyzer::showError() const
{
    QMessageBox mb;
    mb.setWindowTitle(tr("Ошибка лексического анализа"));
    mb.setWindowIcon(QIcon("://icon.ico"));
    mb.setText(tr("С лексемой номер %1 что-то не так, плиз проверьте код")
               .arg(lexicalTable.at(0).size() + 1));
    mb.exec();
}

//-------------------------------------------------------------------

void LexicalAnalyzer::showError(QString text) const
{
    QMessageBox mb;
    mb.setWindowTitle(tr("Ошибка лексического анализа"));
    mb.setWindowIcon(QIcon("://icon.ico"));
    mb.setText(text);
    mb.exec();
}

//-------------------------------------------------------------------

bool LexicalAnalyzer::isIdentifier(QString & lexeme)
{
    if(identifier.exactMatch(lexeme))
    {
        putIntoTable(lexeme, Type::identifier);

        return true;
    }
    else
        return false;
}

//-------------------------------------------------------------------

bool LexicalAnalyzer::isDigit(QString & lexeme)
{
    if(digit.exactMatch(lexeme))
    {
        putIntoTable(lexeme, Type::digit);
        return true;
    }
    else
        return false;
}

//-------------------------------------------------------------------

bool LexicalAnalyzer::isAssign(QChar first, QChar second)
{
    if (first == ':' && second == '=')
    {
        putIntoTable(":=", Type::assign);
        return true;
    }
    else
        return false;
}

//-------------------------------------------------------------------

bool LexicalAnalyzer::isArithmeticOperator(QChar symbol)
{
    if (arithmeticOperators.contains(symbol))
    {
        putIntoTable(QString(symbol), Type::arithmeticOperator);
        return true;
    }
    else
        return false;
}

//-------------------------------------------------------------------

bool LexicalAnalyzer::isSemicolon(QChar symbol)
{
    if (symbol == ';')
    {
        putIntoTable(QString(symbol), Type::semicolon);
        return true;
    }
    else
        return false;
}

//-------------------------------------------------------------------

bool LexicalAnalyzer::isColon(QChar symbol)
{
    if (symbol == ':')
    {
        putIntoTable(QString(symbol), Type::colon);
        return true;
    }
    else
        return false;
}

//-------------------------------------------------------------------

void LexicalAnalyzer::putIntoTable(QString lexeme, Type type)
{
    lexicalTable[0].push_back(lexeme);
    lexicalTable[1].push_back(lexemeTypes[int(type)]);
    lexicalTable[2].push_back(QString());
    lexicalTable[3].push_back(QString());
}
