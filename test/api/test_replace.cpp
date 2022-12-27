//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

template <typename text_type_tested>
class replace_all_copy_callee
{
public:
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    static inline std::string replace_all(text_type_a& a, text_type_b& b, text_type_c& c)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::replace_all_copy(text, b, c));
    }
};

template <typename text_type_tested>
class replace_all_in_place_callee
{
public:
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    static inline std::string replace_all(text_type_a& a, text_type_b& b, text_type_c& c)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::replace_all_in_place(text, b, c));
    }
};

template <typename text_type_tested>
class ireplace_all_copy_callee
{
public:
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    static inline std::string replace_all(text_type_a& a, text_type_b& b, text_type_c& c)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::ireplace_all_copy(text, b, c));
    }
};

template <typename text_type_tested>
class ireplace_all_in_place_callee
{
public:
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    static inline std::string replace_all(text_type_a& a, text_type_b& b, text_type_c& c)
    {
        text_type_tested text = cppstringx::copy<text_type_tested>(a);
        return cppstringx::copy<std::string>(cppstringx::ireplace_all_in_place(text, b, c));
    }
};


template <typename T>
static inline void replace_all_algorithm_test(const T& callee)
{
    CHECK(callee.replace_all("Hello World", "ello", "12345") == "H12345 World"); //inner, larger replacement
    CHECK(callee.replace_all("Hello World", "ello", "1234") == "H1234 World"); //inner, equal size replacement
    CHECK(callee.replace_all("Hello World", "ello", "123") == "H123 World"); //inner, smaller replacement

    CHECK(callee.replace_all("Hello World", "Hello", "123") == "123 World"); // at begin
    CHECK(callee.replace_all("Hello World", "World", "123") == "Hello 123"); // at end

    CHECK(callee.replace_all("Hello World", "xxxx", "123") == "Hello World"); // no replacement
    CHECK(callee.replace_all("Hello World", "l", "123") == "He123123o Wor123d"); // multiple replacements

    CHECK(callee.replace_all("aaaa aaaa", "aa", U"123") == "123123 123123"); //adjacent repeating multiple ambiguous replacements

    CHECK(callee.replace_all("Hello World", "Hellos", "123") == "Hello World"); // almost matching

    CHECK(callee.replace_all("Hello World", "l", "") == "Heo Word"); // replace with empty string

    CHECK_THROWS_AS(callee.replace_all("Hello World", "", "H"), std::invalid_argument);
}

template <typename T>
static inline void ireplace_all_algorithm_test(const T& callee)
{
    CHECK(callee.replace_all("Hello World", "ELLO", "12345") == "H12345 World"); //inner, larger replacement
    CHECK(callee.replace_all("Hello World", "ELLO", "1234") == "H1234 World"); //inner, equal size replacement
    CHECK(callee.replace_all("Hello World", "ELLO", "123") == "H123 World"); //inner, smaller replacement

    CHECK(callee.replace_all("Hello World", "HELLO", U"123") == "123 World"); // at begin
    CHECK(callee.replace_all("Hello World", "WORLD", "123") == "Hello 123"); // at end

    CHECK(callee.replace_all("Hello World", "xxxx", "123") == "Hello World"); // no replacement
    CHECK(callee.replace_all("Hello World", "L", "123") == "He123123o Wor123d"); // multiple replacements

    CHECK(callee.replace_all("AAAA AAAA", "aa", "123") == "123123 123123"); //adjacent repeating multiple ambiguous replacements

    CHECK(callee.replace_all("Hello World", "HELLOS", "123") == "Hello World"); // almost matching

    CHECK(callee.replace_all("HeLLo WorLd", "l", "") == "Heo Word"); // replace with empty string

    CHECK_THROWS_AS(callee.replace_all("Hello World", "", "H"), std::invalid_argument);
}


TEST_CASE("replace_all_copy algorithm", "[replace_all]")
{
    replace_all_algorithm_test(replace_all_copy_callee<std::string>());
    replace_all_algorithm_test(replace_all_copy_callee<std::wstring>());
}

TEST_CASE("replace_all_in_place algorithm", "[replace_all]")
{
    replace_all_algorithm_test(replace_all_in_place_callee<std::string>());
    replace_all_algorithm_test(replace_all_in_place_callee<std::wstring>());
}

TEST_CASE("ireplace_all_copy algorithm", "[replace_all]")
{
    ireplace_all_algorithm_test(ireplace_all_copy_callee<std::string>());
    ireplace_all_algorithm_test(ireplace_all_copy_callee<std::wstring>());
}

TEST_CASE("ireplace_all_in_place algorithm", "[replace_all]")
{
    ireplace_all_algorithm_test(ireplace_all_in_place_callee<std::string>());
    ireplace_all_algorithm_test(ireplace_all_in_place_callee<std::wstring>());
}

TEST_CASE("replace_all comparer", "[replace_all]")
{
    std::string text("Hello XllX");
    CHECK(cppstringx::replace_all_in_place(text, "?ll?", U"----", [](char a, char b) {
            if (b == '?')
            {
                return true;
            }
            else if (a == b)
            {
                return true;
            }
            return false;
        }
    ) == "H---- ----");

    CHECK(cppstringx::replace_all_copy(std::string("Hello XllX"), "?ll?", "----", [](char a, char b) {
        if (b == '?')
        {
            return true;
        }
        else if (a == b)
        {
            return true;
        }
        return false;
        }
    ) == "H---- ----");
}
