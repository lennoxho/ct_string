#include <string_view>
#include <type_traits>
#include <iostream>

template <typename char_t, char_t... str>
class basic_ct_string {
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

// http://open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3599.html
template <typename char_t, char_t... str>
constexpr ct_string<str...> operator "" _cts() {
    return {};
}

template <char... num>
constexpr ct_string<num...> operator "" _ct() {
    return {};
}

constexpr auto num_str1 = 123_ct;
constexpr auto num_str2 = 456_ct;

constexpr auto str1 = "abc"_cts;
constexpr auto str2 = "def"_cts;

constexpr auto num_str_concat = num_str1 + num_str2;
constexpr auto str_concat = str1 + str2;

int main() {
    std::cout << str1.view();
    std::cout << str2.view();

    std::cout << str_concat.view();

    std::cout << num_str1.view();
    std::cout << num_str2.view();

    std::cout << num_str_concat.view();

    return 0;
}
