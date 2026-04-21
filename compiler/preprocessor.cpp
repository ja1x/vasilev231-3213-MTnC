#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>

using namespace std;

// Проверка на незакрытый/неоткрытый многострочный комментарий
bool hasUnclosedComment(const string& code)
{
    int depth = 0;

    for (size_t i = 0; i < code.size(); i++)
    {
        if (code[i] == '/' && i + 1 < code.size() && code[i + 1] == '*')
        {
            depth++;
            i++;
        }
        else if (code[i] == '*' && i + 1 < code.size() && code[i + 1] == '/')
        {
            depth--;
            i++;

            if (depth < 0)
            {
                cerr << "Ошибка: закрывающий комментарий */ без открывающего /*!\n";
                return true;
            }
        }
    }

    if (depth > 0)
    {
        cerr << "Ошибка: незакрытый многострочный комментарий!\n";
        return true;
    }

    return false;
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

// Удаление лишних пробелов между элементами кода
string removeExtraSpaces(const string& code)
{
    stringstream input(code);
    string line, result;

    while (getline(input, line))
    {
        line = regex_replace(line, regex(R"(\s+)"), " ");

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
        return "";
    }

    string result = code;

    result = removeMultiLineComments(result);
    result = removeSingleLineComments(result);
    result = trimSpaces(result);
    result = removeExtraSpaces(result);
    result = removeEmptyLines(result);

    return result;
}