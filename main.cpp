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

int main() {
    const double pi = 3.1415926535897932384626433832795;
    const double e = 2.7182818284;
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
    for (int i = 0; i < input.length(); ++i){  //предварительная замена унарной операции -х на бинарную операцию 0-х
        if (input[i] == '-' &&
        (i-1 < 0 || !(input[i-1] >= 48 && input[i-1] <= 57 || input[i-1] == ')'))){
            input.insert(i, "0");
        }
    }
    CopyOfInput = input; //создаю копию введёной строки для проверки корректности ввода
    for(int i = 0; i < CopyOfInput.length(); ++i){
        if(SetOfOperation.count(CharToString(CopyOfInput[i])) != 0 ||
        CopyOfInput[i] == '(' || CopyOfInput[i] == ')'){
            CopyOfInput.insert(i, " ");
            CopyOfInput.insert(i+2, " ");
            i+=2;
        } else if(CopyOfInput.substr(i, 3) == "exp"){ //делаю отдельную проверку для функции exp так как она не требудет скобок при записи
            CopyOfInput.insert(i, " ");
            CopyOfInput.insert(i+4, " ");
            i+=4;
        }
    }
    std::stringstream ss;
    string symbol;
    int CountOfUnarOperations = 0;
    int CountOfBenarOperations = 0;
    int CountOfOpen = 0; //количетсво открывающихся скобочек
    int CountOfClose = 0; //количетсво закрывающихся скобочек
    int CountOfNumbers = 0;
    //cout << CopyOfInput << endl;
    ss << CopyOfInput;
    while (ss >> symbol){
        if(SetOfOperation.count(symbol) != 0 || symbol == "exp"){
            CountOfBenarOperations++;
        } else if(SetOfPrefOperation.count(symbol) != 0){
            CountOfUnarOperations++;
            CountOfNumbers++;
        } else if(symbol == "("){
            CountOfOpen++;
        } else if(symbol == ")"){
            CountOfClose++;
        } else if(IsNumber(symbol) || symbol == "pi" || symbol == "e"){
            CountOfNumbers++;
        } else {
            cout << "Enter the value of the variable " << symbol << endl;
            string number;
            cin >> number;
            while (!IsNumber(number)){
                cout << number << " not a number. Enter it again " << endl;
                cin >> number;
            }
            input.replace(input.find(symbol), symbol.length(), number);
            CountOfNumbers++;
        }
    }
    if(!(CountOfOpen == CountOfClose && CountOfBenarOperations+1+CountOfUnarOperations <= CountOfNumbers)){
        cout << "Input error" << endl;
    } else {
        for (int i = 0; i < input.length(); ++i) {
            if (input[i] >= 48 && input[i] <= 57 || input[i] == '.' || input.substr(i, 2) == "pi" ||
                input.substr(i, 2) == "e") {
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
                                              3)); // т к длина записи операций tg и sqrt не равна 3, то для них я описываю оотдельные случаи
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
        while (!MyStack.empty()) {
            result += MyStack.top() + ' ';
            MyStack.pop();
        }
        //cout << result << endl;
        std::stringstream ss2;
        ss2 << result;
        while (ss2 >> symbol) {
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
                if (!MyStack.empty()) {
                    a = stod(MyStack.top()); // второй элемент
                    MyStack.pop();
                }
                if (!MyStack.empty()) {
                    b = stod(MyStack.top()); // первый элемент
                    MyStack.pop();
                }
                if (symbol == "+") {
                    MyStack.push(std::to_string(b + a));
                }
                if (symbol == "-") {
                    MyStack.push(std::to_string(b - a));
                }
                if (symbol == "*") {
                    MyStack.push(std::to_string(b * a));
                }
                if (symbol == "/") {
                    MyStack.push(std::to_string(b / a));
                }
                if (symbol == "^") {
                    MyStack.push(std::to_string(pow(b, a)));
                }
                if (symbol == "sin") {
                    MyStack.push(std::to_string(sin(a)));
                }
                if (symbol == "cos") {
                    MyStack.push(std::to_string(cos(a)));
                }
                if (symbol == "tg") {
                    MyStack.push(std::to_string(tan(a)));
                }
                if (symbol == "ctg") {
                    MyStack.push(std::to_string(1 / tan(a)));
                }
                if (symbol == "exp") {
                    MyStack.push(std::to_string(b * pow(10, a)));
                }
                if (symbol == "sqrt") {
                    MyStack.push(std::to_string(sqrt(a)));
                }
            }
        }
        cout << MyStack.top();
        MyStack.pop();
    }
    return 0;
}
