//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <vector>
#include <cppstringx/cppstringx.hpp>

template <typename text_type_tested>
class trim_end_copy_callee
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::trim_end_copy(text));
    }
};

template <typename text_type_tested>
class trim_end_in_place_callee
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::trim_end_in_place(text));
    }
};


template <typename text_type_tested>
class trim_end_copy_range_callee
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        cppstringx::range<typename text_type_tested::iterator> range(text.begin(), text.end());
        return cppstringx::copy<std::string>(cppstringx::trim_end_copy(range));
    }
};

template <typename text_type_tested>
class trim_end_in_place_range_callee
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        cppstringx::range<typename text_type_tested::iterator> range(text.begin(), text.end());
        return cppstringx::copy<std::string>(cppstringx::trim_end_in_place(range));
    }
}; 

template <typename text_type_tested>
class trim_end_in_place_char_range_callee
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        cppstringx::range<const typename text_type_tested::value_type*> range(text.c_str(), text.c_str() + text.size());
        return cppstringx::copy<std::string>(cppstringx::trim_end_in_place(range));
    }
};

template <typename text_type_tested>
class trim_end_in_place_char_callee1
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        std::vector<typename text_type_tested::value_type> text = cppstringx::copy<std::vector<typename text_type_tested::value_type>>(a);
        text.push_back(0);
        typename text_type_tested::value_type* buffer = text.data();
        return cppstringx::copy<std::string>(cppstringx::trim_end_in_place(buffer));
    }
};

template <typename text_type_tested>
class trim_end_in_place_char_callee2
{
public:
    template <typename text_type>
    static inline std::string trim_end(text_type& a)
    {
        std::vector<typename text_type_tested::value_type> text = cppstringx::copy<std::vector<typename text_type_tested::value_type>>(a);
        text.push_back(0);
        return cppstringx::copy<std::string>(cppstringx::trim_end_in_place(text.data()));
    }
};

template <typename T>
static inline void trim_end_algorithm_test(const T& callee)
{
    CHECK(callee.trim_end(" Hello World  ") == " Hello World"); //both possible, trim end
    CHECK(callee.trim_end("   Hello World") == "   Hello World"); //only start, trim nothing
    CHECK(callee.trim_end("Hello World ") == "Hello World"); //only end, trim end
    CHECK(callee.trim_end("Hello World") == "Hello World"); //nothing to trim
    CHECK(callee.trim_end("   ") == ""); //all chars trimmed
    CHECK(callee.trim_end("") == ""); //empty string
}

TEST_CASE("test trim_end algorithm", "[trim_end]")
{
    trim_end_algorithm_test(trim_end_copy_callee<std::string>());
    trim_end_algorithm_test(trim_end_copy_callee<std::wstring>());
    trim_end_algorithm_test(trim_end_in_place_callee<std::string>());
    trim_end_algorithm_test(trim_end_in_place_callee<std::wstring>());
    trim_end_algorithm_test(trim_end_copy_range_callee<std::string>());
    trim_end_algorithm_test(trim_end_copy_range_callee<std::wstring>());
    trim_end_algorithm_test(trim_end_in_place_range_callee<std::string>());
    trim_end_algorithm_test(trim_end_in_place_range_callee<std::wstring>());
    trim_end_algorithm_test(trim_end_in_place_char_range_callee<std::string>());
    trim_end_algorithm_test(trim_end_in_place_char_range_callee<std::wstring>());
    trim_end_algorithm_test(trim_end_in_place_char_callee1<std::string>());
    trim_end_algorithm_test(trim_end_in_place_char_callee1<std::wstring>());
    trim_end_algorithm_test(trim_end_in_place_char_callee2<std::string>());
    trim_end_algorithm_test(trim_end_in_place_char_callee2<std::wstring>());
}

TEST_CASE("test trim_end predicate", "[trim_end]")
{
    CHECK(cppstringx::trim_end_copy<std::string>("Hello Worlddddd", [](char c) {return c == 'd'; }) == "Hello Worl");
    CHECK(cppstringx::trim_end_copy<std::wstring>(L"Hello Worlddddd", [](wchar_t c) {return c == 'd'; }) == L"Hello Worl");
    {
        std::string text("Hello Worlddddd");
        CHECK(cppstringx::trim_end_in_place(text, [](char c) {return c == 'd'; }) == "Hello Worl");
        CHECK(text == "Hello Worl");
    }
}
