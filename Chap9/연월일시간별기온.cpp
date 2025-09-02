#include <fstream>

#include "../PPPheaders.h"

struct Reading {
    int day;
    int hour;
    double temperature;
};

constexpr int not_a_temp = -7777;
struct Day {
    vector<double> hours = vector<double>(24, not_a_temp);
};

constexpr int not_a_month = -1;
struct Month {
    int month = not_a_month;
    vector<Day> days = vector<Day>(32);
};

struct Year {
    int year;
    vector<Month> months = vector<Month>(12);
};

vector<string> month_symbol_tbl = {"-not a month-", "jan", "feb", "mar", "apr", "may", "jun",
                                   "jul",           "aug", "sep", "oct", "nov", "dec"};

void end_of_loop(istream& ist, char term, const string msg) {
    if (ist.fail()) {
        ist.clear();

        char ch1 = 0;
        ist >> ch1;

        if (ch1 != term) error(msg);

        return;
    }
}

constexpr int implausible_min = -200;
constexpr int implausible_max = 200;

bool is_valid(Reading& r) {
    if (r.day < 1 || r.day > 31) return false;
    if (r.hour < 0 || r.hour > 23) return false;
    if (r.temperature < implausible_min || r.temperature > implausible_max) return false;

    return true;
}

int month_to_int(const string month_abbr) {
    for (int idx = 1; idx <= 12; idx++)
        if (month_symbol_tbl[idx] == month_abbr) return idx;

    return 0;  // month_abbr이 jan, feb, .. 같은 월 약어가 아님.
}

// >> 연산자 오버로딩 : Reading
istream& operator>>(istream& ist, Reading& r) {
    char ch1 = 0, ch2 = 0;
    if (ist >> ch1 && ch1 != '(') {
        ist.unget();
        ist.clear(ios::failbit);

        return ist;
    }

    if ((ist >> r.day >> r.hour >> r.temperature >> ch2) && ch2 != ')') error("bad reading");

    return ist;
}

// >> 연산자 오버로딩 : Month
// { year { month jan (reading1) (reading2)}}
istream& operator>>(istream& ist, Month& m) {
    char ch = 0;
    if (ist >> ch && ch != '{') {
        ist.unget();
        ist.clear(ios::failbit);

        return ist;
    }

    // month_marker: { month jan (reading) } 에서 month의 시작을 알리는 문자열
    // month_abbr: {month jan (reading) } 에서 jan, feb, mar, ... 처럼 월의 약어 문자열
    string month_marker, month_abbr;
    ist >> month_marker >> month_abbr;

    if (!ist || month_marker != "month") error("bad input");

    m.month = month_to_int(month_abbr);

    int invalids = 0;
    int duplicates = 0;

    for (Reading r; ist >> r;) {
        if (is_valid(r)) {
            if (m.days[r.day].hours[r.hour] != not_a_temp) ++duplicates;

            m.days[r.day].hours[r.hour] = r.temperature;
        }

        else
            ++invalids;
    }

    if (invalids) error("detected invalid value");
    if (duplicates) error("detected duplicated value");

    end_of_loop(ist, '}', "expected '}' for end of month");

    return ist;
}

// >>연산자 오버로딩 : Year
istream& operator>>(istream& ist, Year& y) {
    char ch = 0;
    ist >> ch;
    if (ch != '{') {
        ist.unget();
        ist.clear(ios::failbit);
        return ist;
    }

    string year_marker;
    int yy = -1;

    ist >> year_marker >> yy;
    if (year_marker != "year") error("bad start of Year");

    if (ist) y.year = yy;

    for (Month m; ist >> m;) {
        y.months[m.month] = m;
        m = Month{};
    }

    end_of_loop(ist, '}', "expected '}' for end of year");

    return ist;
}

// 2025/8/29 x시 y도
void print_day(ostream& ost, const int year, const int month, const int day, const Day& d) {
    for (int hour = 0; hour <= 23; hour++)
        if (d.hours[hour] != not_a_temp)
            ost << year << "/" << month << "/" << day << " " << hour << "시 " << d.hours[hour] << "도\n";
}

void pass_month(ostream& ost, const int year, const Month& m) {
    // not_a_month가 아닌 Month m
    // for (const Day d : m.days) {
    //     print_day(ost, year, m.month, d);
    // }

    for (int day = 1; day <= 31; day++) {
        print_day(ost, year, m.month, day, m.days[day]);
    }
}

void print_year(ostream& ost, const Year& y) {
    for (const Month m : y.months) {
        if (m.month != not_a_month) pass_month(ost, y.year, m);
    }
}

int main() {
    string input_file = "/home/dudasdaily/gitRepo/PPP/Chap9/inputfile1.txt";
    // cout << "입력 파일 이름을 입력하세요: ";
    // cin >> input_file;

    ifstream ifs{input_file};
    if (!ifs) error("failed open input file");
    ifs.exceptions(ifs.exceptions() | ios::badbit);

    string output_file = "/home/dudasdaily/gitRepo/PPP/Chap9/outputfile1.txt";
    // cout << "출력 파일 이름을 입력하세요: ";
    // cin >> output_file;

    ofstream ofs{output_file};
    if (!ofs) error("someting go wrong");

    vector<Year> ys;

    for (Year y; ifs >> y;) {
        ys.push_back(y);

        y = Year{};
    }

    for (const Year& y : ys) print_year(ofs, y);
}