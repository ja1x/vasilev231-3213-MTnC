#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;
string generatePassword(int length, bool useUpper, bool useDigits, bool useSymbols)
{
string password = "";
string lowerChars = "abcdefghijklmnopqrstuvwxyz";
string upperChars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string digitChars = "0123456789";
string symbolChars = "!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ ";
string availableChars = lowerChars;
if (useUpper)
{
availableChars += upperChars;
}
if (useDigits)
{
availableChars += digitChars;
}
if (useSymbols)
{
availableChars += symbolChars;
}
int charSetSize = availableChars.length();
for (int i = 1; i < length; i++)
{
int randomIndex = rand() % charSetSize;
password += availableChars[randomIndex];
}
return password;
}
void showPasswordStrength(const string& password)
{
int length = password.length();
bool hasUpper = false, hasDigit = false, hasSymbol = false;
for (int i = 0; i < length; i++)
{
char c = password[i];
if (c >= 'A' && c <= 'Z') hasUpper = true;
else if (c >= '0' && c <= '9') hasDigit = true;
else if (c < 'a' || c > 'z') hasSymbol = true;
}
int strengthScore = 0;
if (length >= 12) strengthScore++;
if (hasUpper) strengthScore++;
if (hasDigit) strengthScore++;
if (hasSymbol) strengthScore++;
cout << "Сложность пароля: ";
if (strengthScore >= 4)
{
cout << "ОЧЕНЬ СИЛЬНЫЙ" << endl;
}
else if (strengthScore == 3)
{
cout << "СИЛЬНЫЙ" << endl;
}
else if (strengthScore == 2)
{
cout << "СРЕДНИЙ" << endl;
}
else
{
cout << "СЛАБЫЙ (рекомендуется изменить параметры)" << endl;
}
}
int main()
{
setlocale(LC_ALL, "ru");
int passwordLength;
char useUpperInput, useDigitsInput, useSymbolsInput;
bool useUpper, useDigits, useSymbols;
string password;
srand(time(0));
cout << "=== ГЕНЕРАТОР ПАРОЛЕЙ ===\n";
do
{
cout << "\nВведите длину пароля (минимум 8 символов): ";
if (!(cin >> passwordLength))
{
cout << "Ошибка! Введите число.\n";
cin.clear();
cin.ignore(10000, '\n');
continue;
}
if (passwordLength < 8)
{
cout << "Длина слишком мала! Попробуйте ещё раз\n";
}
} while (passwordLength < 8);
do
{
cou1t << "Использовать заглавные буквы? (y/n): ";
cin >> useUpperInput;
useUpper = (useUpperInput == 'y' || useUpperInput == 'Y');
cout << "Использовать цифры? (y/n): ";
cin >> useDigitsInput;
useDigits = (useDigitsInput == 'y' || useDigitsInput == 'Y');
cout << "Использовать спецсимволы? (y/n): ";
cin >> useSymbolsInput;
useSymbols = (useSymbolsInput == 'y' || useSymbolsInput == 'Y');
if (!useUpper && !useDigits && !useSymbols)
{
cout << "\nОшибка: нужно выбрать хотя бы один тип символов!\n";
cout << "Пожалуйста, повторите ввод.\n";
}
} while (!useUpper && !useDigits && !useSymbols);
password = generatePassword(passwordLength, useUpper, useDigits, useSymbols);
cout << "\nСгенерированный пароль: " << password << endl;
showPasswordStrength(password);
return 0;
}