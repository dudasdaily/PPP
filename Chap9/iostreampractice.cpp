#include <fstream>

#include "../PPPheaders.h"

struct Reading {         // 시간 온도를 읽음
    int hour;            // 자정 이후의 시간
    double temperature;  // 화씨
};

void read_file(vector<Reading>& temps) {
    string file_name;  // 열려는 파일 이름

    cout << "입력 파일의 이름을 쳐주세요: ";
    cin >> file_name;

    int hour = -1;
    double temperature = -700;

    ifstream ist{file_name};                     // 파일에 스트림 부착
    if (!ist) error("파일을 열지 못했습니다.");  // 파일 스트림 부착 확인

    while (ist >> hour >> temperature) {
        if (hour < 0 || hour > 23) error("시간이 범위를 벗어났습니다.");

        temps.push_back(Reading{hour, temperature});
    }
}

void write_file(vector<Reading>& temps) {
    string file_name;  // 열려는 파일 이름
    cout << "출력 파일의 이름을 쳐주세요: ";
    cin >> file_name;

    ofstream ost{file_name};
    if (!ost) error("파일을 열지 못했습니다.");

    for (auto p : temps) {
        ost << '(' << p.hour << ", " << p.temperature << ")\n";
    }
}

void fill_vector(istream& ist, vector<int>& v, char terminator) {
    for (int x; ist >> x;) v.push_back(x);

    if (ist.eof()) return;

    // 여기 왔다는건 success, eof, bad 가 아니라는 뜻
    // 즉 ist.fail() 이라는 뜻

    ist.clear();  // ist 플래그를 sucess로 바꿈

    char c = 0;
    ist >> c;  // terminator를 읽기 위해

    if (c != terminator) {        // 종료 문자가 아니였다..
        ist.unget();              // 스트림에 읽은 문자를 다시 넣어두고
        ist.clear(ios::failbit);  // ist 플래그를 fail로 바꿈
    }
}

void fill_vector2(istream& ist, vector<int>& v) {
    // fill_vector에서 리커버리를 안하는 버전
    // 만약 ist가 fail() 이 되버리면 throw
    for (int x; ist >> x;) {
        v.push_back(x);
    }
    ist.exceptions(ist.exceptions() | ios::badbit);
}

int main() {
    // vector<Reading> temps;  // 읽은 것들을 여기 저장

    // read_file(temps);
    // write_file(temps);

    vector<int> v;
    fill_vector2(cin, v);

    return 0;
}