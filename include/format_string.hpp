#pragma once

#include <array>
#include <expected>
#include <utility>

#include "types.hpp"

namespace stdx::details {

// Шаблонный класс для хранения форматирующей строчки и ее особенностей
// ваш код здесь
template <fixed_string Str = fixed_string<>{}>
class format_string {
public:
    constexpr format_string() noexcept = default;
    static constexpr auto str = Str;

    static consteval std::expected<size_t, parse_error> get_number_placeholders();
    static constexpr auto number_placeholders = get_number_placeholders().value();

    static consteval std::array<std::pair<std::size_t, std::size_t>, number_placeholders> get_placeholder_positions();
    static constexpr auto placeholders = get_placeholder_positions();
};

template<fixed_string Str>
consteval std::expected<size_t, parse_error> format_string<Str>::get_number_placeholders() {
    constexpr size_t N = str.size();
    if (!N)
        return 0;
    size_t placeholder_count = 0;
    size_t pos = 0;
    const size_t size = N - 1;

    while (pos < size) {
        if (str.data[pos] != '{') {
            ++pos;
            continue;
        }

        if (pos + 1 >= size) {
            return std::unexpected(parse_error{"Unclosed last placeholder"});
        }

        ++placeholder_count;
        ++pos;

        if (str.data[pos] == '%') {
            ++pos;
            if (pos >= size) {
                return std::unexpected(parse_error{"Unclosed last placeholder"});
            }


            const char spec = str.data[pos];
            constexpr char valid_specs[] = {'d', 'u', 's'};
            bool valid = false;

            for (const char s : valid_specs) {
                if (spec == s) {
                    valid = true;
                    break;
                }
            }

            if (!valid) {
                return std::unexpected(parse_error{"Invalid specifier."});
            }
            ++pos;
        }

        if (pos >= size || str.data[pos] != '}') {
            return std::unexpected(parse_error{"\'}\' hasn't been found in appropriate place"});
        }
        ++pos;
    }

    return placeholder_count;
}

template<fixed_string Str>
consteval std::array<std::pair<std::size_t, std::size_t>, format_string<Str>::number_placeholders> format_string<Str>::
get_placeholder_positions() {
    std::array<std::pair<std::size_t, std::size_t>, number_placeholders> res{};

    if constexpr (number_placeholders == 0)
        return res;

    constexpr size_t N = str.size()-1;
    std::size_t pos = 0;
    std::size_t array_pos = 0;

    while (pos < N - 1 && array_pos < number_placeholders) {
        if (str.data[pos] != '{') {
            ++pos;
            continue;
        }
        res[array_pos] = { 0, 0 };
        std::get<0>(res[array_pos]) = pos;
        ++pos; // Пропускаем '{'



        if (str.data[pos] == '%') {
            pos += 2; // Пропускаем спецификатор: %d, %u, %s
        }

        // Конечная позиция спецификатора
        std::get<1>(res[array_pos]) = pos;

        ++pos;         // Пропускаем '}'
        ++array_pos;   // Следующий placeholder
    }

    return res;
}


template <fixed_string str>
constexpr format_string<str> operator""_fs() {
    return format_string<str>{};
}



} // namespace stdx::details
