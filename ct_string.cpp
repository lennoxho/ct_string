#include <string_view>
#include <type_traits>
#include <iostream>

template <typename char_t, char_t... str>
class basic_ct_string {
    // Warning: Not null terminated.
    static constexpr const char_t s_buf[] = { str... };
    static constexpr std::size_t s_buf_size = sizeof...(str);
    
public:
    constexpr std::basic_string_view<char_t> view() const {
        return { s_buf, s_buf_size };
    }
};

template <typename char_t, char... lhs_str, char... rhs_str>
constexpr auto operator+(basic_ct_string<char_t, lhs_str...> lhs, basic_ct_string<char_t, rhs_str...> rhs) {
    return basic_ct_string<char_t, lhs_str..., rhs_str...>{};
}

template <char... str>
using ct_string = basic_ct_string<char, str...>;

constexpr std::size_t ct_strlen(const char* str) {
    std::size_t len = 0;
    while (*str != '\0') {
        ++len;
        ++str;
    }
    return len;
}

namespace detail {
    template <std::size_t length, std::size_t curr, const char* str>
    struct make_ct_string_ {
        constexpr auto operator()() const {
            return ct_string<str[curr]>{} + make_ct_string_<length-1, curr+1, str>{}();
        }
    };

    template <std::size_t curr, const char* str>
    struct make_ct_string_<0, curr, str> {
        constexpr auto operator()() const {
            return ct_string<>{};
        }
    };
}

template <const char* str>
constexpr auto make_ct_string() {
    return detail::make_ct_string_<ct_strlen(str), 0, str>()();
}

template <char... num>
constexpr ct_string<num...> operator "" _ct() {
    return {};
}

#ifndef _MSC_VER
// http://open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3599.html
template <typename char_t, char_t... str>
constexpr ct_string<str...> operator "" _cts() {
    return {};
}
#endif

constexpr auto num_str1 = 123_ct;
constexpr auto num_str2 = 456_ct;
constexpr auto num_str_concat = num_str1 + num_str2;

#ifndef _MSC_VER
// Depends on GCC/Clang extension.
constexpr auto str1 = "abc"_cts;
constexpr auto str2 = "def"_cts;
constexpr auto str_concat = str1 + str2;
#endif

// This is the closest I can get with standard C++.
// MSVC Preview 2018 still craps out on this.
constexpr const char foo[] = "foo";
constexpr auto cpp14_compliant = make_ct_string<foo>();

int main() {
#ifndef _MSC_VER
    std::cout << str1.view();
    std::cout << str2.view();

    std::cout << str_concat.view();
#endif

    std::cout << num_str1.view();
    std::cout << num_str2.view();

    std::cout << num_str_concat.view();
    std::cout << cpp14_compliant.view();

    return 0;
}
