#include "../PPPheaders.h"

using namespace PPP;
using namespace std;

/*
    Chapter6. 의 계산기 프로그램 실습입니다.
*/

const char number = '8';
const char quit = 'q';
const char print = ';';
const char let = 'L';
const char var_kind = 'a';

const string prompt = "> ";
const string result = "= ";
const string declkey = "let";

class Token
{
public:
    char kind;
    double value;
    string name; // 계산기에서 사용할 변수의 이름

    Token() {}
    Token(char ch) : kind(ch), value(0) {}
    Token(char ch, double val) : kind(ch), value(val) {}
    Token(char ch, string var) : kind(ch), name(var) {}
};

class Token_stream
{
    bool full;
    Token buffer;

public:
    void putback(Token t);
    Token get();
};

class Variable
{
public:
    string name;
    double value;

    Variable(string name, double value) : name(name), value(value) {}
};

vector<Variable> var_table;

bool is_declared(string name)
{
    for (const Variable &v : var_table)
        if (v.name == name)
            return true;

    return false;
}

double get_value(string name)
{
    for (const Variable &v : var_table)
        if (name == v.name)
            return v.value;

    const string err_msg = "var_table에 " + name + "변수가 없습니다.";
    error(err_msg);
}

void set_value(string name, double val)
{
    for (Variable &v : var_table)
    {
        if (v.name == name)
        {
            v.value = val;
            return;
        }
    }

    const string err_msg = "var_table에 " + name + "변수가 없습니다.";
    error(err_msg);
}

void Token_stream::putback(Token t)
// 인자로 받은 토큰을 Token_stream의 버퍼에 넣음.
{
    if (full)
        error("꽉 찬 버퍼에 토큰을 넣으려고 합니다");

    full = true;
    buffer = t;
}

Token Token_stream::get()
// cin에서 문자 한개를 읽어서 문자 종류에 따른 토큰 생성
{
    if (full)
    {
        full = false;
        return buffer;
    }

    char ch;
    cin >> ch;

    switch (ch)
    {
    case quit:
    case print:
    case let:
    case '=':
    case '+':
    case '-':
    case '*':
    case '/':
    case '%':
    case '(':
    case ')':
        return Token(ch);

    case '.':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
        cin.putback(ch);
        double d;
        cin >> d;

        return Token(number, d);
    }

    default:
    {
        if (isalpha(ch))
        {
            string var;
            var += ch;

            while (cin.get(ch) && (isalpha(ch) || isdigit(ch)))
                var += ch;

            cin.putback(ch);

            if (var == declkey)
                return Token(let, var);

            return Token(var_kind, var);
        }

        error("Invalid Token");
    }
        return -1;
    }
}

Token_stream ts; // 토큰 스트림 전역변수

double expression();

double primary()
{
    Token t = ts.get();

    switch (t.kind)
    {
    case '(':
    {
        double d = expression();
        t = ts.get();

        if (t.kind != ')')
            error("expected ')'");

        return d;
    }

    case number:
        return t.value;

    case '-':
        return -primary();

    case '+':
        return primary();

    case var_kind:
        return get_value(t.name);

    default:
        error("Invalid Token");
        return -1;
    }
}

double term()
{
    double left = primary();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
        case '*':
            left *= primary();
            t = ts.get();
            break;

        case '/':
        {
            double d = primary();

            if (d == 0)
                error("divide by 0");

            left /= d;
            t = ts.get();
            break;
        }

        case '%':
        {
            int i1 = narrow<int>(left);
            int i2 = narrow<int>(primary());

            if (i2 == 0)
                error("Can't do modulor operation on floating-point");

            left = i1 % i2;
            t = ts.get();
            break;
        }

        default:
            ts.putback(t);
            return left;
        }
    }
}

double expression()
{
    double left = term();
    Token t = ts.get();

    while (true)
    {
        switch (t.kind)
        {
        case '+':
            left += term();
            t = ts.get();
            break;

        case '-':
            left -= term();
            t = ts.get();
            break;

        default:
            ts.putback(t);
            return left;
        }
    }
}

double declaration()
{
    Token t = ts.get();

    switch (t.kind)
    {
    case var_kind:
    {
        if (is_declared(t.name))
            error(t.name, "이 var_table에 이미 존재합니다. 같은 변수를 두번 선언 할 수 없습니다!");

        string name = t.name;

        t = ts.get();

        if (t.kind != '=')
            error("expected '='");

        double d = expression();

        var_table.push_back(Variable(name, d));

        return d;
    }

    default:
        error("expected variable");
    }
}

double statement()
{
    Token t = ts.get();

    switch (t.kind)
    {
    case let:
        return declaration();

    default:
        ts.putback(t);
        return expression();
    }
}

void calculate()
{
    try
    {
        while (true)
        {
            cout << prompt;
            Token t = ts.get();

            while (t.kind == print)
                t = ts.get();

            if (t.kind == quit)
                return;

            ts.putback(t);

            cout << result << statement() << '\n';
        }
    }
    catch (exception &e)
    {
        cerr << e.what() << '\n';
    }
    catch (...)
    {
        cerr << "Unknown error\n";
    }
}

int main()
{
    try
    {
        calculate();
    }
    catch (exception &e)
    {
        cerr << e.what() << '\n';
    }
    catch (...)
    {
        cerr << "Unknown error\n";
    }
}