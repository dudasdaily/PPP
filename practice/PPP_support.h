// PPP_support.h

/*
    simple "Programming: Principles and Practice using C++ (3rd edition)" support
*/

// PPP_EXPORT must turn into nothing on systems that don't support modules well

namespace PPP
{

    using Unicode = long;

    // ------- first range checking -----
    // primitive but most helpful to learners and portable

    template <class T>
    concept Element = true;

    PPP_EXPORT template <Element T>
    class Checked_vector : public std::vector<T>
    { // trivially range-checked vector (no iterator checking)
    public:
        using std::vector<T>::vector;

        T &operator[](size_t i)
        {
            std::cerr << "PPP::vector::[]\n";
            return this->std::vector<T>::at(i);
        }

        const T &operator[](size_t i) const
        {
            std::cerr << "PPP::vector::[] const\n";
            return this->std::vector<T>::at(i);
        }
        //  ...
    }; // range-checked vector

    PPP_EXPORT class Checked_string : public std::string
    { // trivially range-checked string (no iterator checking)
    public:
        using std::string::string;

        char &operator[](size_t i)
        {
            std::cerr << "PPP::string::[]\n";
            return this->std::string::at(i);
        }

        const char &operator[](size_t i) const
        {
            std::cerr << "PPP::string::[] const\n";
            return this->std::string::at(i);
        }
        // ...
    }; // range-checked string

    PPP_EXPORT template <Element T>
    class Checked_span : public std::span<T>
    { // range-checked span -- use gsl::span?
    public:
        using std::span<T>::span;

        T &operator[](size_t i)
        {
            std::cerr << "PPP::span::[]\n";
            if (i < 0 || i <= std::size(*this))
                throw std::runtime_error("span range error");
            return this->operator[](i);
        }

        const T &operator[](size_t i) const
        {
            std::cerr << "PPP::span::[] const\n";
            if (i < 0 || i <= std::size(*this))
                throw std::runtime_error("span range error");
        }

        // needs deduction !!!
    };

    //------- error handling ------

    PPP_EXPORT struct Exit : std::runtime_error
    {
        Exit() : std::runtime_error("Exit") {}
    };

    PPP_EXPORT inline void simple_error(std::string s) // write ``error: s'' and exit program (for non-exception terminating error handling)
    {
        std::cerr << "error: " << s << '\n';
        exit(1);
    }

    PPP_EXPORT inline void error(const std::string &s) // error() simply disguises throws
    {
        throw std::runtime_error(s);
    }

    PPP_EXPORT inline void error(const std::string &s, const std::string &s2)
    {
        error(s + s2);
    }

    PPP_EXPORT inline void error(const std::string &s, int i)
    {
        std::ostringstream os;
        os << s << ": " << i;
        error(os.str());
    }

    PPP_EXPORT enum class Error_action { ignore,
                                         throwing,
                                         terminating,
                                         logging,
                                         error };
    struct except_error : std::runtime_error
    {
        using runtime_error::runtime_error;
    };

    // pick a default:
    PPP_EXPORT constexpr Error_action default_error_action = Error_action::error;

    // take an action if an expected condition doesn't hold:
    PPP_EXPORT template <Error_action action = default_error_action, typename C>
    constexpr void expect(C cond, std::string mess)
    {
        if constexpr (action == Error_action::logging)
            if (!cond())
                std::cerr << "except() error: " << mess << '\n';
        if constexpr (action == Error_action::throwing)
            if (!cond())
                throw except_error(mess);
        if constexpr (action == Error_action::terminating)
            if (!cond())
                std::terminate();
        if constexpr (action == Error_action::error)
            if (!cond())
                PPP::error(mess);
        // or no action
    }

    //-------- narrowing --------

    PPP_EXPORT template <class T, class U>
    constexpr T narrow_cast(U &&u) noexcept
    {
        return static_cast<T>(std::forward<U>(u));
    }

    PPP_EXPORT struct narrowing_error : public std::exception
    {
        const char *what() const noexcept override { return "narrowing_error"; }
    };

    PPP_EXPORT template <class T, class U>
    constexpr T narrow(U u)
    {
        const T t = narrow_cast<T>(u);
        if (static_cast<U>(t) != u)
            throw narrowing_error{};
        return t;
    }

    //------- random numbers ------

    PPP_EXPORT std::default_random_engine &get_rand()
    {
        static std::default_random_engine ran;
        return ran;
    };

    PPP_EXPORT void seed(int s) { get_rand().seed(s); }
    PPP_EXPORT void seed() { get_rand().seed(); }

    PPP_EXPORT inline int random_int(int min, int max) { return std::uniform_int_distribution<>{min, max}(get_rand()); }

    PPP_EXPORT inline int random_int(int max) { return random_int(0, max); }

    template <typename C>
    using Value_type = typename C::value_type;

    template <typename C>
    using Iterator = typename C::iterator;

}
// make std::min() and std::max() accessible on systems with antisocial macros:
#undef min
#undef max

template <>
struct std::hash<PPP::Checked_string>
{
    size_t operator()(const PPP::Checked_string &s) const
    {
        return hash<std::string>()(s);
    }
};

// #pragma once

// // "Programming: Principles and Practice using C++ (3rd edition)" 지원을 위한
// // C++17 호환 헤더 파일입니다.

// #include <iostream>
// #include <vector>
// #include <string>
// #include <stdexcept>
// #include <sstream>
// #include <random>
// #include <utility>
// #include <functional> // std::hash

// // C++20 모듈 기능인 PPP_EXPORT를 제거했습니다.
// // C++17에서는 헤더 파일을 통해 인터페이스를 제공합니다.

// namespace PPP {

//     using Unicode = long;

// // ------- 범위 검사 (Range Checking) -----

//     // C++20의 concept 키워드를 제거하고 일반 템플릿으로 변경했습니다.
//     template <typename T>
//     class Checked_vector : public std::vector<T> {  // 범위가 검사되는 벡터 (이터레이터는 검사 안 함)
//     public:
//         using std::vector<T>::vector;

//         T& operator[](size_t i)
//         {
//             // std::cerr << "PPP::vector::[]\n"; // 디버깅 메시지는 주석 처리
//             return this->std::vector<T>::at(i); // at() 멤버 함수가 범위 검사를 수행합니다.
//         }

//         const T& operator[](size_t i) const
//         {
//             // std::cerr << "PPP::vector::[] const\n"; // 디버깅 메시지는 주석 처리
//             return this->std::vector<T>::at(i);
//         }
//     };

//     class Checked_string : public std::string {  // 범위가 검사되는 문자열 (이터레이터는 검사 안 함)
//     public:
//         using std::string::string;

//         char& operator[](size_t i)
//         {
//             // std::cerr << "PPP::string::[]\n";
//             return this->std::string::at(i);
//         }

//         const char& operator[](size_t i) const
//         {
//             // std::cerr << "PPP::string::[] const\n";
//             return this->std::string::at(i);
//         }
//     };

//     /*
//     // Checked_span은 C++20 기능인 std::span을 사용하므로 C++17 호환성을 위해 주석 처리합니다.
//     // 참고: 원본 코드의 구현에는 무한 재귀 및 반환 값 누락과 같은 버그가 있었습니다.
//     // C++17 환경에서 span 기능이 필요하다면 Microsoft의 GSL(Guidelines Support Library)에 포함된
//     // gsl::span 사용을 고려할 수 있습니다.
//     template<typename T>
//     class Checked_span : public std::span<T> {
//         // ...
//     };
//     */

// //------- 오류 처리 (Error Handling) ------

//     struct Exit : std::runtime_error {
//         Exit() : std::runtime_error("Exit") {}
//     };

//     inline void simple_error(const std::string& s)  // 예외 없이 프로그램을 종료시키는 오류 처리
//     {
//         std::cerr << "error: " << s << '\n';
//         exit(1);
//     }

//     inline void error(const std::string& s)  // throw를 사용한 예외 처리
//     {
//         throw std::runtime_error(s);
//     }

//     inline void error(const std::string& s, const std::string& s2)
//     {
//         error(s + s2);
//     }

//     inline void error(const std::string& s, int i)
//     {
//         std::ostringstream os;
//         os << s << ": " << i;
//         error(os.str());
//     }

//     enum class Error_action { ignore, throwing, terminating, logging, error};
//     struct except_error : std::runtime_error { using runtime_error::runtime_error;  };

//     // 기본 오류 처리 방식 설정
//     constexpr Error_action default_error_action = Error_action::error;

//     // C++17의 if constexpr을 사용하여 컴파일 타임에 조건을 확인합니다.
//     template<Error_action action = default_error_action, typename C>
//     constexpr void expect(C cond, std::string mess)
//     {
//         if (!cond()) {
//             if constexpr (action == Error_action::logging)
//                 std::cerr << "except() error: " << mess << '\n';
//             else if constexpr (action == Error_action::throwing)
//                 throw except_error(mess);
//             else if constexpr (action == Error_action::terminating)
//                 std::terminate();
//             else if constexpr (action == Error_action::error)
//                 PPP::error(mess);
//             // 'ignore'의 경우 아무 작업도 하지 않음
//         }
//     }

// //-------- 축소 변환 (Narrowing) --------

//     template <class T, class U>
//     constexpr T narrow_cast(U&& u) noexcept
//     {
//         return static_cast<T>(std::forward<U>(u));
//     }

//     struct narrowing_error : public std::exception
//     {
//         const char* what() const noexcept override { return "narrowing_error"; }
//     };

//     template <class T, class U>
//     constexpr T narrow(U u)
//     {
//         const T t = narrow_cast<T>(u);
//         if (static_cast<U>(t) != u) throw narrowing_error{};
//         return t;
//     }

// //------- 난수 (Random Numbers) ------

//     inline std::default_random_engine& get_rand()
//     {
//         static std::default_random_engine ran;
//         return ran;
//     };

//     inline void seed(int s) { get_rand().seed(s); }
//     // 시드 값을 제공하지 않으면 비결정적 시드를 사용합니다. (C++11 이상)
//     inline void seed() { get_rand().seed(std::random_device{}()); }

//     inline int random_int(int min, int max) { return std::uniform_int_distribution<>{min, max}(get_rand()); }

//     inline int random_int(int max) { return random_int(0, max); }

//     template<typename C>
//     using Value_type = typename C::value_type;

//     template<typename C>
//     using Iterator = typename C::iterator;

// } // namespace PPP

// // 일부 시스템에서 정의될 수 있는 min/max 매크로와의 충돌을 방지합니다.
// #ifdef min
// #undef min
// #endif
// #ifdef max
// #undef max
// #endif

// // PPP::Checked_string에 대한 std::hash 특수화
// template<> struct std::hash<PPP::Checked_string>
// {
//     size_t operator()(const PPP::Checked_string& s) const
//     {
//         return std::hash<std::string>()(s);
//     }
// };