#include "../PPPheaders.h"

struct Year {
    int y;
};

enum class Month { jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

class Date {
   public:
    Date() : y{-1}, m{-1}, d{-1} {};
    Date(Year year, Month month, int day);

    int year() { return y; }
    int month() { return m; }
    int day() { return d; }

   private:
    int y;
    int m;
    int d;
};

bool is_leap(int year) { return year % 4 && ((year % 100) || !(year % 400)); }

bool is_date(int year, int month, int day) {
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31) return false;

    int last_day = -1;

    switch (month) {
        case 4:
        case 6:
        case 9:
        case 11:
            last_day = 30;
            break;

        case 2:
            last_day = is_leap(year) ? 29 : 28;

        default:
            last_day = 31;
    }

    if (day > last_day) return false;

    return true;
}

Date::Date(Year year, Month month, int day) : y{year.y}, m{static_cast<int>(month)}, d{day} {
    if (!is_date(y, m, d)) error("Invalid Date");
}

ostream& operator<<(ostream& ost, Date d) { return ost << d.year() << "/" << d.month() << "/" << d.day(); }

istream& operator>>(istream& ist, Date& d) {
    int year, month, day;
    char punc1, punc2;

    ist >> year >> punc1 >> month >> punc2 >> day;

    if (!ist) return ist;

    if (punc1 != ',' && punc2 != ',') {
        ist.clear(ios::failbit);
        return ist;
    }

    d = Date{Year{year}, Month{month}, day};
    return ist;
}

void end_of_loop(istream& ist, char terminator, const string& msg) {
    if (ist.fail()) {
        ist.clear();
        char ch = 0;

        if (ist >> ch && ch == terminator) return;

        error(msg);
    }
}

int main() {
    for (Date d; cin >> d;) {
        if (!is_date(d.year(), d.month(), d.day())) error("Invalid Date");
        cout << d << '\n';
    }

    end_of_loop(cin, ';', "bad terminator of input");

    return 0;
}