#include <iostream>
#include <string>
#include <stack>
#include <set>
#include <map>
#include <sstream>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::string;

string CharToString(char ch){
    string result;
    result += ch;
    return result;
}

bool IsNumber (string& word){
    int CountOfPoints = 0;
    for (char ch : word){
        if(ch == '.'){
            CountOfPoints++;
            continue;
        }
        if(!(CountOfPoints <= 1 && ch >= 48 && ch <= 57)){
            return false;
        }
    }
    return true;
}

double ExtractFromStack (std::stack<string>& MyStack){
    double a;
    a = stod(MyStack.top()); // второй элемент
    MyStack.pop();
    return a;
}

int main() {
    const double pi = 3.1415926535897932384626433832795;
    const double e = 2.7182818284590452353602874713527;
    const std::set<string> SetOfPrefOperation = {"sin", "cos", "ctg", "exp", "tg", "sqrt"};
    const std::set<string> SetOfOperation = {"+", "-", "*", "/", "^"};
    std::map<string , int> Priority = {{"^", 4},
                                          {"*",3},
                                          {"/",3},
                                          {"+",2},
                                          {"-",2},
                                          {"(",1},
                                          {")",1}};
    string input, result, CopyOfInput;
    bool flag = false;
    std::stack<string> MyStack;
    cin >> input;
    if (input.find("--") != -1){
        cout << "Input error" << endl;
    } else {
        for (int i = 0; i < input.length(); ++i) {  //предварительная замена унарной операции -х на бинарную операцию 0-х
            if (input[i] == '-' && (i - 1 < 0 || !(input[i - 1] >= 48 && input[i - 1] <= 57 || input[i - 1] == ')'))) {
                input.insert(i, "0");
            }
            if ((input[i] >= 48 && input[i] <= 57 && input[i+1] == '(') ||
                    (input[i] == ')' && input[i+1] >= 48 && input[i+1] <= 57) ||
                    (input[i] == ')' && input[i+1] == '(')){
                input.insert(i+1, "*");
            }
        }
        CopyOfInput = input; //создаю копию введёной строки для проверки корректности ввода
        for (int i = 0; i < CopyOfInput.length(); ++i) {
            if (SetOfOperation.count(CharToString(CopyOfInput[i])) != 0 ||
                CopyOfInput[i] == '(' || CopyOfInput[i+1] == ')') {
                CopyOfInput.insert(i, " ");
                CopyOfInput.insert(i + 2, " ");
                i += 2;
            } /*else if (CopyOfInput.substr(i, 3) ==
                       "exp") { //делаю отдельную проверку для функции exp так как она не требудет скобок при записи
                CopyOfInput.insert(i, " ");
                CopyOfInput.insert(i + 4, " ");
                i += 4;
            }*/ //я оставил этот код закоментированны на всякий случай, потому что изначально считал, что функция exp работает так:
                // 2exp3 = 2*10^3 По карайней мере так работает эта функция в инженерном калькуляторе на Windows 10
        }
        std::stringstream ss;
        string symbol;
        int CountOfUnarOperations = 0;
        int CountOfBenarOperations = 0;
        int CountOfBrackets = 0;
        int CountOfNumbers = 0;
        //cout << CopyOfInput << endl;
        ss << CopyOfInput;
        while (ss >> symbol) { //провверка на корректность ввода
            if (CountOfBrackets < 0){
                break;
            }else if (SetOfOperation.count(symbol) != 0 /*|| symbol == "exp"*/) {
                CountOfBenarOperations++;
            } else if (SetOfPrefOperation.count(symbol) != 0) {
                CountOfUnarOperations++;
                CountOfNumbers++;
            } else if (symbol == "(") {
                CountOfBrackets++;
            } else if (symbol == ")") {
                CountOfBrackets--;
            } else if (IsNumber(symbol) || symbol == "pi" || symbol == "e") {
                CountOfNumbers++;
            } else {
                cout << "Enter the value of the variable " << symbol << endl;
                string number;
                cin >> number;
                while (!IsNumber(number)) {
                    cout << number << " not a number. Enter it again " << endl;
                    cin >> number;
                }
                input.replace(input.find(symbol), symbol.length(), number);
                CountOfNumbers++;
            }
        }
        if (!(CountOfBrackets == 0 && CountOfBenarOperations + 1 + CountOfUnarOperations <= CountOfNumbers)) {
            cout << "Input error" << endl;
        } else {
            for (int i = 0; i < input.length(); ++i) {
                if ((input[i] >= 48 && input[i] <= 57 || input[i] == '.' || input.substr(i, 2) == "pi" ||
                    input[i] == 'e') && input.substr(i, 3) != "exp") {
                    if (input.substr(i, 2) == "pi") {
                        result += "pi";
                        i += 1;
                    } else {
                        result += input[i];
                    }
                    flag = true;
                } else {
                    if (flag) {
                        result += ' ';
                        flag = false;
                    }
                    if (SetOfPrefOperation.count(input.substr(i, 3)) != 0) { //обавление в стек префиксно операции
                        MyStack.push(input.substr(i,
                                                  3)); // т к длина записи операций tg и sqrt не равна 3, то для них я описываю отдельные случаи
                        i += 2;
                    }
                    if (input.substr(i, 2) == "tg") {
                        MyStack.push(input.substr(i, 2));
                        i += 1;
                    }
                    if (input.substr(i, 4) == "sqrt") {
                        MyStack.push(input.substr(i, 4));
                        i += 3;
                    }
                    if (input[i] == '(') {
                        MyStack.push("(");
                    }
                    if (input[i] == ')') {
                        while (MyStack.top() != "(") {
                            result += MyStack.top() + ' ';
                            MyStack.pop();
                        }
                        MyStack.pop();
                    }
                    if (SetOfOperation.count(CharToString(input[i]))) {
                        while (!MyStack.empty() && (SetOfPrefOperation.count(MyStack.top()) != 0 ||
                                                    Priority[MyStack.top()] >= Priority[CharToString(input[i])])) {
                            result += MyStack.top() + ' ';
                            MyStack.pop();
                        }
                        MyStack.push(CharToString(input[i]));
                    }
                }
            }
            result += ' ';
            while (!MyStack.empty()) { //вытаскиваю всё их сдека и записываю в строчку итоговую польскую запись
                result += MyStack.top() + ' ';
                MyStack.pop();
            }
            //cout << result << endl;
            std::stringstream ss2;
            ss2 << result;
            while (ss2 >> symbol) { //переношу все числа в стекб потом выполняю операцию над ними
                if (SetOfPrefOperation.count(symbol) == 0 && SetOfOperation.count(symbol) == 0 || symbol == "pi" ||
                    symbol == "e") {
                    if (symbol == "pi") {
                        MyStack.push(std::to_string(pi));
                    } else if (symbol == "e") {
                        MyStack.push(std::to_string(e));
                    } else
                        MyStack.push(symbol);
                } else {
                    double a, b;
                    if (symbol == "+") {
                        a = ExtractFromStack(MyStack);
                        b = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(b + a));
                    } else if (symbol == "-") {
                        a = ExtractFromStack(MyStack);
                        b = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(b - a));
                    } else if (symbol == "*") {
                        a = ExtractFromStack(MyStack);
                        b = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(b * a));
                    } else if (symbol == "/") {
                        a = ExtractFromStack(MyStack);
                        b = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(b / a));
                    } else if (symbol == "^") {
                        a = ExtractFromStack(MyStack);
                        b = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(pow(b, a)));
                    } else if (symbol == "sin") {
                        a = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(sin(a)));
                    } else if (symbol == "cos") {
                        a = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(cos(a)));
                    } else if (symbol == "tg") {
                        a = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(tan(a)));
                    } else if (symbol == "ctg") {
                        a = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(1 / tan(a)));
                    } else /*if (symbol == "exp") {
                        a = ExtractFromStack(MyStack);
                        b = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(b * pow(10, a)));
                    }*/if(symbol == "exp"){
                        a = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(pow(e, a)));
                    } else if (symbol == "sqrt") {
                        a = ExtractFromStack(MyStack);
                        MyStack.push(std::to_string(sqrt(a)));
                    }
                }
            }
            cout << MyStack.top();
            MyStack.pop();
        }
    }
    return 0;
}
