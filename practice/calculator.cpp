#include "PPP_support.h"

using namespace PPP;
using namespace std;

/*
    Chapter6. 의 계산기 프로그램 실습입니다.
*/

const char number = '8';
const char quit = 'q';
const char print = ';';

const string prompt = "> ";
const string result = "= ";

class Token
{
public:
    char kind;
    double value;

    Token() {}
    Token(char ch) : kind(ch), value(0) {}
    Token(char ch, double val) : kind(ch), value(val) {}
};

class Token_stream
{
    bool full;
    Token buffer;

public:
    void putback(Token t);
    Token get();
};

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
        error("Invalid Token");
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

            cout << result << expression() << '\n';
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