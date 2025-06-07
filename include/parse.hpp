#pragma once

#include <charconv>
#include <concepts>
#include <expected>
#include <limits>
#include <system_error>

#include "format_string.hpp"
#include "types.hpp"

namespace stdx::details {

// Шаблонная функция, возвращающая пару позиций в строке с исходными данными, соотвествующих I-ому плейсхолдеру
// Функция закомментирована, так как еще не реализованы классы, которые она использует

template<int I, format_string fmt, fixed_string source>
consteval auto get_current_source_for_parsing() {
    static_assert(I >= 0 && I < fmt.number_placeholders, "Invalid placeholder index");

    constexpr auto to_sv = [](const auto& fs) {
        return std::string_view(fs.data, fs.size() - 1);
    };

    constexpr auto fmt_sv = to_sv(fmt.str);
    constexpr auto src_sv = to_sv(source);
    constexpr auto& positions = fmt.placeholders;

    // Получаем границы текущего плейсхолдера в формате
    constexpr auto pos_i = positions[I];
    constexpr size_t fmt_start = pos_i.first, fmt_end = pos_i.second;

    // Находим начало в исходной строке
    constexpr auto src_start = [&]{
        if constexpr (I == 0) {
            return fmt_start;
        } else {
            // Находим конец предыдущего плейсхолдера в исходной строке
            constexpr auto prev_bounds = get_current_source_for_parsing<I-1, fmt, source>();
            const auto prev_end = prev_bounds.second;

            // Получаем разделитель между текущим и предыдущим плейсхолдерами
            constexpr auto prev_fmt_end = positions[I-1].second;
            constexpr auto sep = fmt_sv.substr(prev_fmt_end + 1, fmt_start - (prev_fmt_end + 1));

            // Ищем разделитель после предыдущего значения
            auto pos = src_sv.find(sep, prev_end);
            return pos != std::string_view::npos ? pos + sep.size() : src_sv.size();
        }
    }();

    // Находим конец в исходной строке
    constexpr auto src_end = [&]{
        // Получаем разделитель после текущего плейсхолдера
        if constexpr(fmt_end == (fmt_sv.size() - 1)) {
            return src_sv.size();
        }
        constexpr auto sep = fmt_sv.substr(fmt_end + 1,
            (I < fmt.number_placeholders - 1)
                ? positions[I+1].first - (fmt_end + 1)
                : fmt_sv.size() - (fmt_end + 1));
        // Ищем разделитель после текущего значения
        constexpr auto pos = src_sv.find(sep, src_start);
        return pos != std::string_view::npos ? pos : src_sv.size();
    }();
    return std::pair{src_start, src_end};
}

template<typename T>
concept signed_int = std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept unsigned_int = std::is_integral_v<T> && not std::is_signed_v<T>;

template<typename T>
concept string = std::is_same_v<T, std::string_view>;

template<typename T>
concept allowed_types = string<T> or signed_int<T> or unsigned_int<T>;


template<fixed_string str, std::integral T>
consteval std::expected<T, parse_error> parse_value() {
    T res{0};
    bool negative = false;
    T limit = std::numeric_limits<T>::max();
    std::size_t pos{0};
    if constexpr (str.size() > 0 and str.data[0] == '-') {
        negative = true;
        pos = 1;
        limit = std::numeric_limits<T>::min();
    }
    auto update_result = [&](T digit) -> std::expected<T, parse_error> {
        if (negative) {
            if (res < limit / 10 || res * 10 < limit + digit) {
                return std::unexpected(parse_error{"Negative integer overflow"});
            }
            res = res * 10 - digit;
        } else {
            if (res > limit / 10 || res * 10 > limit - digit) {
                return std::unexpected(parse_error{"Positive integer overflow"});
            }
            res = res * 10 + digit;
        }
        return res;
    };

    while (pos < str.size()) {
        if (str.data[pos] == '\0')
            break;

        if (str.data[pos] < '0' || str.data[pos] > '9') {
            return std::unexpected(parse_error{"Invalid character in input"});
        }

        auto result = update_result(static_cast<T>(str.data[pos] - '0'));
        if (not result)
            return result;
        pos++;
    }
    return res;
}

template<fixed_string str, typename T = std::string_view>
consteval std::expected<std::string_view, parse_error> parse_value() {
    return std::string_view{str.data, str.size()};
}

template<std::size_t I,format_string fmt, fixed_string source, allowed_types T>
consteval T parse_input() {
    constexpr auto pos = fmt.placeholders[I];
    constexpr auto start = pos.first;
    constexpr auto end = pos.second;

    if constexpr(constexpr auto spec = fixed_string<end - start>{fmt.str.data + start, fmt.str.data + end }; spec.size() > 2) {
        if constexpr(spec.data[2] == 'd') {
            static_assert(signed_int<T>, "Type meets the ‘%d’ specifier");
        }
        else if constexpr(spec.data[2] == 'u') {
            static_assert(unsigned_int<T>, "Type meets the ‘%u’ specifier");
        }
        else if constexpr(spec.data[2] == 's') {
            static_assert(string<T>, "Type meets the ‘%s’ specifier");
        }
    }

    constexpr auto src = get_current_source_for_parsing<I, fmt, source>();
    constexpr auto source_for_parsing_start = source.data + src.first;
    constexpr auto source_for_parsing_end = source.data + src.second;

    return parse_value<fixed_string<src.second - src.first>{source_for_parsing_start, source_for_parsing_end}, T>().value();
}

} // namespace stdx::details
