#include <cstdint>
#include <string_view>

#include "scan.hpp"

using namespace stdx::details;

int main() {
    {
        constexpr auto fmt = "Zero unsigned: {%u}"_fs;
        constexpr auto src = fixed_string{"Zero unsigned: 00000"};
        constexpr auto res = stdx::scan<fmt, src, unsigned int>();
        static_assert(res.values<0>() == 0);
    }
    {
        constexpr auto fmt = "Max64: {%d}"_fs;
        constexpr auto src = fixed_string{"Max64: 9223372036854775807"};
        constexpr auto res = stdx::scan<fmt, src, std::int64_t>();
        static_assert(res.values<0>() == 9223372036854775807LL);
    }
    {
        constexpr auto fmt = "Say {%s} {%u} times!"_fs;
        constexpr auto src = fixed_string{"Say Ping 5 times!"};
        constexpr auto res = stdx::scan<fmt, src, std::string_view, unsigned int>();
        static_assert(res.values<0>() == "Ping");
        static_assert(res.values<1>() == 5);
    }
    {
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Integer: 17"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == 17);
    }
    {
        constexpr auto fmt = "Population: {%u}"_fs;
        constexpr auto src = fixed_string{"Population: 8000000000"};
        constexpr auto res = stdx::scan<fmt, src, std::uint64_t>();
        static_assert(res.values<0>() == 8000000000ULL);
    }
    {
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Integer: -00789"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == -789);
    }
    {
        constexpr auto fmt = "My name is {%s} {%s}!"_fs;
        constexpr auto src = fixed_string{"My name is Jane Smith!"};
        constexpr auto res = stdx::scan<fmt, src, std::string_view, std::string_view>();
        static_assert(res.values<0>() == "Jane");
        static_assert(res.values<1>() == "Smith");
    }
    {
        constexpr auto fmt = "Negative integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Negative integer: -56"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == -56);
    }
    {
        constexpr auto fmt = "String view of: {%s}!"_fs;
        constexpr auto src = fixed_string{"String view of: C++ Rocks!"};
        constexpr auto res = stdx::scan<fmt, src, std::string_view>();
        static_assert(res.values<0>() == "C++ Rocks");
    }
    {
        constexpr auto fmt = "Unsigned integer: {%u}"_fs;
        constexpr auto src = fixed_string{"Unsigned integer: 99"};
        constexpr auto res = stdx::scan<fmt, src, unsigned int>();
        static_assert(res.values<0>() == 99);
    }
    {
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Integer: 000123"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == 123);
    }
    {
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Integer: -0000"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == 0);
    }
    {
        constexpr auto fmt = "Max32u: {%u}"_fs;
        constexpr auto src = fixed_string{"Max32u: 4294967295"};
        constexpr auto res = stdx::scan<fmt, src, std::uint32_t>();
        static_assert(res.values<0>() == 4294967295U);
    }
    {
        constexpr auto fmt = "Hex as dec: {%u}"_fs;
        constexpr auto src = fixed_string{"Hex as dec: 3735928559"};
        constexpr auto res = stdx::scan<fmt, src, std::uint32_t>();
        static_assert(res.values<0>() == 3735928559U);
    }
    {
        constexpr auto fmt = "{%u}/{%u} complete"_fs;
        constexpr auto src = fixed_string{"7/10 complete"};
        constexpr auto res = stdx::scan<fmt, src, unsigned int, unsigned int>();
        static_assert(res.values<0>() == 7);
        static_assert(res.values<1>() == 10);
    }
    {
        constexpr auto fmt = "Temp: {%d}C"_fs;
        constexpr auto src = fixed_string{"Temp: -273C"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == -273);
    }
    {
        constexpr auto fmt = "ID: {%d}"_fs;
        constexpr auto src = fixed_string{"ID: 0"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == 0);
    }
    {
        constexpr auto fmt = "{%d} {%d} {%u} {%d} {%u}"_fs;
        constexpr auto src = fixed_string{"-128 127 255 01 18446744073709551614"};
        constexpr auto res = stdx::scan<fmt, src,
                                        std::int8_t, std::int8_t, std::uint8_t,
                                        std::int8_t, std::uint64_t>();
        static_assert(res.values<0>() == -128);
        static_assert(res.values<1>() == 127);
        static_assert(res.values<2>() == 255);
        static_assert(res.values<3>() == 1);
        static_assert(res.values<4>() == 18446744073709551614ULL);
    }
    {
        constexpr auto fmt = "Max Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Max Integer: 2147483647"};
        constexpr auto res = stdx::scan<fmt, src, std::int32_t>();
        static_assert(res.values<0>() == 2147483647);
    }
    {
        constexpr auto fmt = "Min Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Min Integer: -2147483648"};
        constexpr auto res = stdx::scan<fmt, src, std::int32_t>();
        static_assert(res.values<0>() == -2147483648);
    }
    {
        constexpr auto fmt = "Bounds: {%d} {%u}"_fs;
        constexpr auto src = fixed_string{"Bounds: -1 4294967295"};
        constexpr auto res = stdx::scan<fmt, src, int, std::uint32_t>();
        static_assert(res.values<0>() == -1);
        static_assert(res.values<1>() == 4294967295U);
    }
    {
        constexpr auto fmt = "Size: {%u} bytes"_fs;
        constexpr auto src = fixed_string{"Size: 4096 bytes"};
        constexpr auto res = stdx::scan<fmt, src, std::uint32_t>();
        static_assert(res.values<0>() == 4096);
    }
    {
        constexpr auto fmt = "Integer: {%d}"_fs;
        constexpr auto src = fixed_string{"Integer: -1024"};
        constexpr auto res = stdx::scan<fmt, src, int>();
        static_assert(res.values<0>() == -1024);
    }
    {
        constexpr auto fmt = "{%s} {%s} {%s}"_fs;
        constexpr auto src = fixed_string{"alpha beta gamma"};
        constexpr auto res = stdx::scan<fmt, src, std::string_view, std::string_view, std::string_view>();
        static_assert(res.values<0>() == "alpha");
        static_assert(res.values<1>() == "beta");
        static_assert(res.values<2>() == "gamma");
    }
    return 0;
}
