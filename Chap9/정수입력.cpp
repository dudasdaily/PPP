#include "../PPPheaders.h"

void skip_to_int() {
    if (cin.fail()) {
        cin.clear();

        for (char ch; cin >> ch;) {
            if (isdigit(ch) || ch == '-') {
                cin.unget();
                return;
            }
        }
    }

    error("no input");
}

int get_int() {
    int n = 0;
    while (true) {
        if (cin >> n) return n;

        cout << "입력 형식이 올바르지 않습니다. 정수를 입력해주세요.\n";

        skip_to_int();
    }
}

int get_int(int low, int high) {
    while (true) {
        int n = get_int();

        if (low <= n && n <= high) return n;

        cout << "입력한 정수의 범위가 [" << low << ", " << high << "] 가 아닙니다.\n"
             << "다시 입력해주세요.\n";
    }
}

int main() {
    cout << get_int(0, 10) << '\n';

    return 0;
}