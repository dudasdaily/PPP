#include "PPP_support.h"

using namespace PPP;
using namespace std;

/*
    Chapter6. 의 계산기 프로그램 실습입니다.
*/

const char number = '8';
const char quit = 'q';
const char print = ';';

class Token
{
public:
    char kind;
    double value;
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
// cin에서 문자 한개를 읽음
{
    char ch;
    cin >> ch;
}

int main()
{
}