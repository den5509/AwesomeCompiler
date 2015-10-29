/*
 * It's lexical analyzer. It allocates from plain text lexemes
 * which matches this regular exeption:
 * (_?[a-zA-Z]\w*)|(\\d*\\.\\d*)|(<|>|=|:=|+|-|:|;)|(case|of|end)
 * and puts them into the lexical table with their type.
 * In other words, it searches keywords like "case", "of", "end",
 * identifiers, comprasion signs, plus, minus and float digits.
 *
 * Lexical table here is vector of vectors of qstrings, and it's
 * a bit inefficiently, but better for debugging.
 */

#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <QObject>
#include <QRegExp>
#include <QVector>
#include <QSet>

#include "lexemetype.h"

class LexicalAnalyzer : public QObject
{
    Q_OBJECT
public:

    explicit LexicalAnalyzer(QObject *parent = 0);
    ~LexicalAnalyzer();

    void analyze(QString source);
    QVector<QVector<QString>> & getLexTable() {
        return lexicalTable;
    }

signals:
    void done();

private:
    QRegExp identifier;
    QRegExp digit;
    QSet<QChar> terminalSymbols;
    QSet<QChar> comprasionSigns;
    QSet<QChar> arithmeticOperators;
    QSet<QString> keyWords;
    QVector<QVector<QString>> lexicalTable;
    QVector<QString> lexemeTypes;

    void showError() const;
    void showError(QString text) const;

    //those functions verify the lexeme and
    //put it into the lexical table in case of success
    bool isIdentifier(QString & lexeme);
    bool isDigit(QString & lexeme);
    bool isAssign(QChar first, QChar second);
    bool isComprasionSign(QChar symbol);
    bool isArithmeticOperator(QChar symbol);
    bool isColon(QChar symbol);
    bool isSemicolon(QChar symbol);

    //compare specified type with type of lexeme with index i
    bool compare(int i, Type type) {
        return lexemeTypes[int(type)] == lexicalTable.at(1).at(i);
    }

    void putIntoTable(QString lexeme, Type type);
};

#endif // LEXICALANALYZER_H
