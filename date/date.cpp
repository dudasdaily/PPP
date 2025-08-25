#include <string>

#include "../PPPheaders.h"

/*
    간단한 날짜 클래스 구현
*/

namespace Chrono {
struct Year {
    int y;
};

enum class Month { jan = 1, feb, mar, apr, may, jun, jul, aug, sep, oct, nov, dec };

bool is_leap_year(int y) {
    if (y % 100 != 0 && y % 4 == 0) return true;
    if (y % 400 == 0) return true;

    return false;
}

// Helper function
// 클래스 밖에 분리함으로서 representation과 상관 없이 동작
bool is_date(int y, int m, int d) {
    if (y < 0 || d > 31) return false;
    if (m < static_cast<int>(Month::jan) || m > static_cast<int>(Month::dec)) return false;

    int lastday_in_month = -1;

    switch (m) {
        case 4:
        case 6:
        case 9:
        case 11:
            lastday_in_month = 30;
            break;

        case 2:
            lastday_in_month = is_leap_year(y) ? 29 : 28;  // 윤년이면 29일, 윤년이 아니면 28일
            break;

        default:
            lastday_in_month = 31;
            break;
    }

    if (d > lastday_in_month) return false;

    return true;
}

class Date {
   public:
    Date();
    Date(Year y, Month m, int d);

    // non-const 함수
    void add_year(int n);
    void add_month(int n);
    void add_day(int n);

    // const 함수
    int year() const { return y.y; }
    int month() const { return static_cast<int>(m); }
    int day() const { return d; }

   private:
    Year y;   // year
    Month m;  // month
    int d;    // day
};

Date::Date() : y{Year{2001}}, m{Month::jan}, d{1} {}
Date::Date(Year y, Month m, int d) : y{y}, m{m}, d{d} {
    if (!is_date(year(), month(), day())) error("유효한 날짜가 아닙니다!");
}

void Date::add_day(int n) {
    d += n;
    if (!is_date(year(), month(), day())) {
        d -= n;

        auto err_msg = to_string(year()) + "/" + to_string(month()) + "/" + to_string(day() + n);
        error(err_msg, "는 유효한 날짜가 아닙니다!");
    }
}

ostream& operator<<(ostream& os, Date d) { return os << d.year() << '/' << d.month() << '/' << d.day(); }
};  // namespace Chrono

int main() {
    using namespace Chrono;

    Date today{Year{2020}, Month::feb, 2};
    Date tomorrow = today;

    tomorrow.add_day(1);

    try {
        tomorrow.add_day(30);
    } catch (exception& e) {
        cerr << e.what() << '\n';
    }

    cout << today << '\n' << tomorrow << '\n';
}