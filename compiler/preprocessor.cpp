#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

// Проверка на незакрытый многострочный комментарий
bool hasUnclosedComment(const string& code)
{
    size_t openCount = 0, closeCount = 0;

    size_t pos = 0;
    while ((pos = code.find("/*", pos)) != string::npos)
    {
        openCount++;
        pos += 2;
    }

    pos = 0;
    while ((pos = code.find("*/", pos)) != string::npos)
    {
        closeCount++;
        pos += 2;
    }

    return openCount > closeCount;
}

// Удаление многострочных комментариев
string removeMultiLineComments(const string& code)
{
    regex pattern(R"(/\*[\s\S]*?\*/)");
    return regex_replace(code, pattern, "");
}

// Удаление однострочных комментариев
string removeSingleLineComments(const string& code)
{
    regex pattern(R"(//.*)");
    return regex_replace(code, pattern, "");
}

// Удаление лишних пробелов в начале и конце строк
string trimSpaces(const string& code)
{
    stringstream input(code);
    string line, result;

    while (getline(input, line))
    {
        line = regex_replace(line, regex(R"(^\s+)"), "");
        line = regex_replace(line, regex(R"(\s+$)"), "");

        if (!result.empty())
            result += "\n";
        result += line;
    }

    return result;
}

// Удаление пустых строк
string removeEmptyLines(const string& code)
{
    stringstream input(code);
    string line, result;

    while (getline(input, line))
    {
        if (!regex_match(line, regex(R"(^\s*$)")))
        {
            if (!result.empty())
                result += "\n";
            result += line;
        }
    }

    return result;
}

// Основной препроцессор
string preprocess(const string& code)
{
    if (hasUnclosedComment(code))
    {
        cerr << "Ошибка: незакрытый многострочный комментарий!\n";
        return "";
    }

    string result = code;

    result = removeMultiLineComments(result);
    result = removeSingleLineComments(result);
    result = trimSpaces(result);
    result = removeEmptyLines(result);

    return result;
}

int main()
{
    setlocale(LC_ALL, "ru");

    string inputFile = "test.cpp";
    string outputFile = "cleaned.cpp";

    ifstream in(inputFile);
    if (!in)
    {
        cerr << "Ошибка: не удалось открыть входной файл!\n";
        return 1;
    }

    stringstream buffer;
    buffer << in.rdbuf();
    string code = buffer.str();
    in.close();

    cout << "Файл успешно прочитан.\n";

    string cleanedCode = preprocess(code);

    if (cleanedCode.empty())
    {
        cerr << "Обработка прервана из-за ошибки.\n";
        return 1;
    }

    ofstream out(outputFile);
    if (!out)
    {
        cerr << "Ошибка: не удалось создать выходной файл!\n";
        return 1;
    }

    out << cleanedCode;
    out.close();

    cout << "Файл успешно обработан!\n";
    cout << "Результат записан в: " << outputFile << endl;

    return 0;
}