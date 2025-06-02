#pragma once
#include <algorithm>

namespace stdx::details {

// Шаблонный класс, хранящий C-style строку фиксированной длины
template<typename T>
consteval std::size_t length(const T* str) {
    std::size_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

template<std::size_t N = 0>
struct fixed_string {
    char data[N]{};

    constexpr fixed_string() noexcept = default;
    constexpr fixed_string(const fixed_string&) noexcept = default;
    constexpr fixed_string& operator=(const fixed_string&) noexcept = default;
    constexpr fixed_string(fixed_string&&) noexcept = default;
    constexpr fixed_string& operator=(fixed_string&&) noexcept = default;
    constexpr ~fixed_string() noexcept = default;

    constexpr bool operator==(const fixed_string&) const = default;

    constexpr fixed_string(const char (&str)[N]) {
        std::copy_n(str, N, data);
    }

    constexpr fixed_string(const char* format, const char* format_end) noexcept {
        for (std::size_t i = 0; i < N; ++i) {
            if (format + i != format_end) {
                data[i] = *(format + i);
            } else {
                data[i] = '\0';
                break;
            }
        }
    }

    template<std::size_t format_size> requires(format_size <= N-1)
    constexpr fixed_string(const char (&format)[format_size]) {
        std::copy_n(format, format_size, data);
    }

    constexpr std::size_t size() const noexcept {
        return N;
    }
};
// Шаблонный класс, хранящий fixed_string достаточной длины для хранения ошибки парсинга

// ваш код здесь
struct parse_error : fixed_string<64> {

};

// Шаблонный класс для хранения результатов парсинга

template <typename... Ts>
struct scan_result {
// ваш код здесь
// измените реализацию
    std::tuple<Ts...> ts;

    template <std::size_t I>
    consteval auto values() const {
        return std::get<I>(ts);
    }
};

} // namespace stdx::details
