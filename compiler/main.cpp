#include "preprocessor.h"
#include "lexer.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    setlocale(LC_ALL, "ru");

    // Препроцессор
    ifstream in("test.c");
    stringstream buf;
    buf << in.rdbuf();
    string raw = buf.str();
    in.close();

    string cleaned = preprocess(raw);
    if (cleaned.empty()) return 1;

    ofstream out("cleaned.c");
    out << cleaned;
    out.close();
    cout << "=== Препроцессор завершён ===\n\n";

    // Лексический анализатор
    Lexer lexer(cleaned);
    lexer.tokenize();

    printTable(lexer.getTokens());
    printSequence(lexer.getTokens());

    cout << "\nТокенов: " << lexer.getTokens().size() << "\n";
    if (lexer.getErrors().empty())
        cout << "Ошибок не найдено.\n";
    else
        printErrors(lexer.getErrors());

    return 0;
}