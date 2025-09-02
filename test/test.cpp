#include <format>
#include <fstream>

#include "../PPPheaders.h"

struct SimpleTable {
    string last_name, first_name, telephone_number, email_addr;
};

struct Point {
    double x, y;
};

istream& operator>>(istream& ist, Point& p) {
    char paren1 = 0, comma = 0, paren2 = 0;

    ist >> paren1 >> p.x >> comma >> p.y >> paren2;

    if (!ist) return ist;

    if (paren1 != '(' || comma != ',' || paren2 != ')')
        error("입력 형식이 잘못되었습니다. 다음과 같이 입력해야합니다. (2, 3)");

    return ist;
}

ostream& operator<<(ostream& ost, const Point& p) { return ost << "(" << p.x << "," << p.y << ")"; }

int main() {
    const int current_year = 2025;
    const int birth_year{2002};

    cout << format("dec: {} \t hex: {:x} \t oct: {:o}\n", birth_year, birth_year, birth_year);
    cout << format("I'm {} years old\n", current_year - birth_year);

    int x = 0, y = 0;

    cout << "oct input: ";
    cin >> oct >> x;

    cout << "hex input: ";
    cin >> hex >> y;

    cout << format("{}, {}\n", x, y);

    const double num = 1234567.89;
    cout << format("{}\n{:f}\n{:e}\n", num, num, num);

    SimpleTable me{"Hwang", "Jinseok", "01086306649", "dudasdaily@gmail.com"};
    SimpleTable eoong{"Joe", "Yeyoung", "01066322788", "baboeoong@gmail.com"};

    // cout << format("{:5} {:7} {:13} {}\n", me.last_name, me.first_name, me.telephone_number, me.email_addr);
    // cout << format("{:5} {:7} {:13} {}\n", eoong.last_name, eoong.first_name, eoong.telephone_number,
    // eoong.email_addr);

    vector<Point> original_points;

    const int vector_capacity = 2;

    int input_success_cnt = 0;

    while (true) {
        if (input_success_cnt == vector_capacity) break;
        cout << format("x,y 좌표를 입력해주세요. ({}/{})\n", input_success_cnt + 1, vector_capacity);

        Point p;
        try {
            cin >> p;
        } catch (exception& e) {
            cerr << e.what() << '\n';
            continue;
        }

        input_success_cnt++;
        original_points.push_back(p);
    }

    for (auto p : original_points) {
        cout << p << '\n';
    }

    ofstream ofs{"./mydata.txt"};

    ofs.exceptions(ofs.exceptions() | ios::badbit);

    for (auto p : original_points) {
        ofs << p << '\n';
    }

    ofs.close();

    ifstream ifs{"./mydata.txt"};
    ifs.exceptions(ifs.exceptions() | ios::badbit);

    vector<Point> processed_points;

    while (ifs) {
        Point p;

        ifs >> p;

        if (ifs.eof()) break;
        processed_points.push_back(p);
    }

    cout << "원본 벡터" << endl;
    for (auto p : original_points) cout << p << '\n';

    cout << "파일에서 읽은 벡터" << endl;
    for (auto p : processed_points) cout << p << '\n';
}