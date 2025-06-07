#pragma once

#include <tuple>

#include "parse.hpp"
#include "format_string.hpp"
#include "types.hpp"

namespace stdx {

template <details::format_string fmt, details::fixed_string source, typename... Ts>
consteval details::scan_result<Ts...> scan() {
    return []<std::size_t... I>(std::index_sequence<I...>) consteval {
        return details::scan_result<Ts...> {
            std::make_tuple(details::parse_input<I, fmt, source, Ts>()...)
        };
    }(std::make_index_sequence<sizeof...(Ts)>{});
}

} // namespace stdx