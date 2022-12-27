//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <vector>
#include <cppstringx/cppstringx.hpp>

template <typename text_type_tested>
class trim_start_copy_callee
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::trim_start_copy(text));
    }
};

template <typename text_type_tested>
class trim_start_in_place_callee
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::trim_start_in_place(text));
    }
};


template <typename text_type_tested>
class trim_start_copy_range_callee
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        cppstringx::range<typename text_type_tested::iterator> range(text.begin(), text.end());
        return cppstringx::copy<std::string>(cppstringx::trim_start_copy(range));
    }
};

template <typename text_type_tested>
class trim_start_in_place_range_callee
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        cppstringx::range<typename text_type_tested::iterator> range(text.begin(), text.end());
        return cppstringx::copy<std::string>(cppstringx::trim_start_in_place(range));
    }
}; 

template <typename text_type_tested>
class trim_start_in_place_char_range_callee
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        cppstringx::range<const typename text_type_tested::value_type*> range(text.c_str(), text.c_str() + text.size());
        return cppstringx::copy<std::string>(cppstringx::trim_start_in_place(range));
    }
};

template <typename text_type_tested>
class trim_start_in_place_char_callee1
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        std::vector<typename text_type_tested::value_type> text = cppstringx::copy<std::vector<typename text_type_tested::value_type>>(a);
        text.push_back(0);
        typename text_type_tested::value_type* buffer = text.data();
        return cppstringx::copy<std::string>(cppstringx::trim_start_in_place(buffer));
    }
};

template <typename text_type_tested>
class trim_start_in_place_char_callee2
{
public:
    template <typename text_type>
    static inline std::string trim_start(text_type& a)
    {
        std::vector<typename text_type_tested::value_type> text = cppstringx::copy<std::vector<typename text_type_tested::value_type>>(a);
        text.push_back(0);
        return cppstringx::copy<std::string>(cppstringx::trim_start_in_place(text.data()));
    }
};

template <typename T>
static inline void trim_start_algorithm_test(const T& callee)
{
    CHECK(callee.trim_start(" Hello World  ") == "Hello World  "); //both possible, trim start
    CHECK(callee.trim_start("   Hello World") == "Hello World"); //only start, trim start
    CHECK(callee.trim_start("Hello World ") == "Hello World "); //only end, trim nothing
    CHECK(callee.trim_start("Hello World") == "Hello World"); //nothing to trim
    CHECK(callee.trim_start("   ") == ""); //all chars trimmed
    CHECK(callee.trim_start("") == ""); //empty string
}

TEST_CASE("test trim_start algorithm", "[trim_start]")
{
    trim_start_algorithm_test(trim_start_copy_callee<std::string>());
    trim_start_algorithm_test(trim_start_copy_callee<std::wstring>());
    trim_start_algorithm_test(trim_start_in_place_callee<std::string>());
    trim_start_algorithm_test(trim_start_in_place_callee<std::wstring>());
    trim_start_algorithm_test(trim_start_copy_range_callee<std::string>());
    trim_start_algorithm_test(trim_start_copy_range_callee<std::wstring>());
    trim_start_algorithm_test(trim_start_in_place_range_callee<std::string>());
    trim_start_algorithm_test(trim_start_in_place_range_callee<std::wstring>());
    trim_start_algorithm_test(trim_start_in_place_char_range_callee<std::string>());
    trim_start_algorithm_test(trim_start_in_place_char_range_callee<std::wstring>());
    trim_start_algorithm_test(trim_start_in_place_char_callee1<std::string>());
    trim_start_algorithm_test(trim_start_in_place_char_callee1<std::wstring>());
    trim_start_algorithm_test(trim_start_in_place_char_callee2<std::string>());
    trim_start_algorithm_test(trim_start_in_place_char_callee2<std::wstring>());
}

TEST_CASE("test trim_start predicate", "[trim_start]")
{
    CHECK(cppstringx::trim_start_copy<std::string>("HHHHHello World", [](char c) {return c == 'H'; }) == "ello World");
    CHECK(cppstringx::trim_start_copy<std::wstring>(L"HHHHello World", [](wchar_t c) {return c == 'H'; }) == L"ello World");
    {
        std::string text("HHHHello World");
        CHECK(cppstringx::trim_start_in_place(text, [](char c) {return c == 'H'; }) == "ello World");
        CHECK(text == "ello World");
    }
}
