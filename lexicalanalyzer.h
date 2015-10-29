/*
 * Это лексический анализатор. Из обычного текста он выделяет лексемы,
 * попадающие под следующее регулярное выражение:
 * (_?[a-zA-Z]\w*)|(\\d*\\.\\d*)|(:=|+|-|:|;)|(case|of|end)
 * и записывает их в таблицу лексем, указывая их тип
 * Другими словами, он ищет ключевые слова "case", "of" и "end",
 * идентификаторы, плюсы, минусы, точки с запятой, двоеточия и числа с
 * плавающей точко.
 *
 * Таблица лексем - это двумерный вектор строк, что несколько
 * неэффективно с точки зрения расхода памяти, зато упрощает отладку программы.
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
    QSet<QChar> arithmeticOperators;
    QSet<QString> keyWords;
    QVector<QVector<QString>> lexicalTable;
    QVector<QString> lexemeTypes;

    void showError() const;
    void showError(QString text) const;

    //эти функции проверяют строку/символ на соответствие
    //типу лексему и если всё хорошо, добавляют строчку в таблицу лексем
    bool isIdentifier(QString & lexeme);
    bool isDigit(QString & lexeme);
    bool isAssign(QChar first, QChar second);
    bool isComprasionSign(QChar symbol);
    bool isArithmeticOperator(QChar symbol);
    bool isColon(QChar symbol);
    bool isSemicolon(QChar symbol);

    //сравнивает указанный тип с типом лексемы в строчке i
    bool compare(int i, Type type) {
        return lexemeTypes[int(type)] == lexicalTable.at(1).at(i);
    }

    void putIntoTable(QString lexeme, Type type);
};

#endif // LEXICALANALYZER_H
