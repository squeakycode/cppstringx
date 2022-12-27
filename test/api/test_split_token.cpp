//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>
#include <vector>

TEST_CASE("split_token_iterator algorithm happy path", "[split]")
{
    std::string text = "Hello World";
    const char* separator = " ";
    cppstringx::split_token_iterator<std::string, const char*, cppstringx::utility::equals_comparer> split_it(text, separator, cppstringx::split_mode::all, cppstringx::utility::equals_comparer());
    CHECK(!split_it.is_end_position());
    CHECK(std::string(split_it->begin(), split_it->end()) == "Hello");
    CHECK(!split_it.is_end_position());
    split_it++;
    CHECK(std::string(split_it->begin(), split_it->end()) == "World");
    *split_it->begin() = 'V';
    CHECK(std::string(split_it->begin(), split_it->end()) == "Vorld");
    CHECK(!split_it.is_end_position());
    split_it++;
    CHECK(split_it.is_end_position());
    CHECK(std::string(split_it->begin(), split_it->end()) == "");
    split_it++;
    CHECK(split_it.is_end_position());
    CHECK(std::string(split_it->begin(), split_it->end()) == "");

    typedef cppstringx::split_token_iterator<std::string, const char*, cppstringx::utility::equals_comparer> split_token_iterator_type;
    CHECK_THROWS_AS(split_token_iterator_type(text, "", cppstringx::split_mode::all, cppstringx::utility::equals_comparer()), std::invalid_argument);
}

template <typename text_type, typename text_type_separator>
void test_split_token_iterator(const text_type& text_to_iterate_over, const text_type_separator& separator_token, cppstringx::split_mode mode, std::vector<const char*> expected)
{
    auto split_it = cppstringx::make_split_token_iterator(text_to_iterate_over, separator_token, mode, cppstringx::utility::equals_comparer());

    size_t count = 0;
    while (!split_it.is_end_position())
    {
        REQUIRE(count < expected.size());
        CHECK(std::string(split_it->begin(), split_it->end()) == expected[count]);
        ++count;
        ++split_it;
    }
    CHECK(expected.size() == count);
}

TEST_CASE("split_token_iterator algorithm", "[split]")
{
    test_split_token_iterator("Hello World", " ", cppstringx::split_mode::all, { "Hello", "World" }); //single char split
    test_split_token_iterator("Hello World", "ll", cppstringx::split_mode::all, { "He", "o World" });  //multi char split
    test_split_token_iterator("Hello World", "l", cppstringx::split_mode::all, { "He", "", "o Wor","d"}); //split with empty string, adjacent separator
    test_split_token_iterator("Hello World", "Hello", cppstringx::split_mode::all, { "", " World" }); //separator at begin
    test_split_token_iterator("Hello World", "x", cppstringx::split_mode::all, { "Hello World" }); //not found
    test_split_token_iterator("", "x", cppstringx::split_mode::all, { "" }); //empty
    test_split_token_iterator("Hello World", "ld", cppstringx::split_mode::all, { "Hello Wor", "" }); //separator at end
    test_split_token_iterator("xHelloxWorldx", "x", cppstringx::split_mode::all, { "", "Hello", "World", "" }); //separator at end and begin

    test_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty, { "He", "o Wor","d" }); //split with empty string skipped
    test_split_token_iterator("Hello World", "Hello", cppstringx::split_mode::skip_empty, { " World" }); //separator at begin
    test_split_token_iterator("Hello World", "ld", cppstringx::split_mode::skip_empty, { "Hello Wor" }); //separator at end
    test_split_token_iterator("", "x", cppstringx::split_mode::skip_empty, { }); //empty
    test_split_token_iterator("xHelloxWorldx", "x", cppstringx::split_mode::skip_empty, { "Hello", "World" }); //separator at end and begin
}

template <typename text_type, typename text_type_separator>
void test_split_token_iterator_types(text_type& text_to_iterate_over, const text_type_separator& separator_token, cppstringx::split_mode mode, std::vector<const char*> expected)
{
    auto split_it = cppstringx::make_split_token_iterator(text_to_iterate_over, separator_token, mode, cppstringx::utility::equals_comparer());

    size_t count = 0;
    while (!split_it.is_end_position())
    {
        REQUIRE(count < expected.size());
        CHECK(cppstringx::copy<std::string>(*split_it) == expected[count]);
        ++count;
        ++split_it;
    }
    CHECK(expected.size() == count);
 }

TEST_CASE("split_token_iterator types", "[split]")
{
    {
        std::string text1("Hello World");
        std::wstring text2(L"Hello World");
        cppstringx::range<std::string::iterator> text3(text1.begin(), text1.end());
        cppstringx::range<std::string::const_iterator> text4(text1.begin(), text1.end());
        const std::string& text5 = text1;
        const std::wstring& text6 = text2;
        const cppstringx::range<std::string::iterator>& text7 = text3;
        const cppstringx::range<std::string::const_iterator>& text8 = text4;
        char text9[] = { "Hello World" };
        char* text10 = text9;
        const char* text11 = "Hello World";
        cppstringx::range<char*> text12(text9, text9+11);
        cppstringx::range<const char*> text13(text11, text11+11);

        test_split_token_iterator_types(L"Hello World", " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text1, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text2, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text3, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text4, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text5, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text6, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text7, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text8, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text9, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text10, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text11, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text12, " ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types(text13, " ", cppstringx::split_mode::all, { "Hello", "World" });
    }
    {
        std::string text1(" ");
        std::wstring text2(L" ");
        cppstringx::range<std::string::iterator> text3(text1.begin(), text1.end());
        cppstringx::range<std::string::const_iterator> text4(text1.begin(), text1.end());
        const std::string& text5 = text1;
        const std::wstring& text6 = text2;
        const cppstringx::range<std::string::iterator>& text7 = text3;
        const cppstringx::range<std::string::const_iterator>& text8 = text4;
        char text9[] = { ' ', 0 };
        char* text10 = text9;
        const char* text11 = " ";
        cppstringx::range<char*> text12(text9, text9 + 1);
        cppstringx::range<const char*> text13(text11, text11 + 1);

        test_split_token_iterator_types(L"Hello World", L" ", cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text1, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text2, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text3, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text4, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text5, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text6, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text7, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text8, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text9, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text10, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text11, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text12, cppstringx::split_mode::all, { "Hello", "World" });
        test_split_token_iterator_types("Hello World", text13, cppstringx::split_mode::all, { "Hello", "World" });
    }
}

TEST_CASE("split_token_iterator variants", "[split]")
{
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", " ");
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello");
        split_it = cppstringx::make_split_token_iterator("Hello World", "H");
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
        split_it = cppstringx::make_split_token_iterator("Hello World", "H", cppstringx::split_mode::skip_empty);
        CHECK(std::string(split_it->begin(), split_it->end()) == "ello World");
    }
    {
        auto split_it = cppstringx::make_isplit_token_iterator("Hello World", " ");
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello");
        split_it = cppstringx::make_isplit_token_iterator("Hello World", "h");
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
        split_it = cppstringx::make_isplit_token_iterator("Hello World", "h", cppstringx::split_mode::skip_empty);
        CHECK(std::string(split_it->begin(), split_it->end()) == "ello World");
    }
}

TEST_CASE("split_token container test", "[split]")
{
    {
        std::vector<std::string> result;
        result.push_back("Text");
        std::vector<std::string> expected = { "Hello", "World" };
        cppstringx::split_token(result, "Hello World", " ");
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        result.push_back("Text");
        std::vector<std::string> expected = { "Text", "Hello", "World"};
        cppstringx::split_token(result, "Hello World", " ", cppstringx::split_mode::all, false /*clear_container */);
        CHECK(result == expected);
    }
    {
        std::vector<cppstringx::range<const char*>> result;
        cppstringx::split_token(result, "Hello World", " ");
        REQUIRE(result.size() == 2);
        CHECK(std::string(result[0].begin(), result[0].end()) == "Hello");
        CHECK(std::string(result[1].begin(), result[1].end()) == "World");
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "", "o Wor","d" };
        cppstringx::split_token(result, "Hello World", "l");
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "o Wor","d" };
        cppstringx::split_token(result, "Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(result == expected);
    }

    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "Hello ", "orld" };
        cppstringx::isplit_token(result, "Hello World", "w");
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "", "o Wor","d" };
        cppstringx::isplit_token(result, "Hello World", "L");
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "o Wor","d" };
        cppstringx::isplit_token(result, "Hello World", "L", cppstringx::split_mode::skip_empty);
        CHECK(result == expected);
    }

    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "o Wor","d" };
        cppstringx::split_token(result, "Hello World", "L", cppstringx::split_mode::skip_empty, cppstringx::utility::equals_comparer_ignoring_case());
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "o Wor","d" };
        cppstringx::split_token(result, "Hello World", "l", cppstringx::split_mode::skip_empty, [](char a, char b) { return a == b; });
        CHECK(result == expected);
    }
}

TEST_CASE("split_token_iterator advance test", "[split]")
{
    // advance_to_last
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l"); //last is not empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
        CHECK(split_it.advance_to_last()); //do it twice
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "ld"); //last is empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "x"); //no separator
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello World");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("", "ld"); //empty string
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }

    // advance
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l");
        CHECK(split_it.advance(0));
        CHECK(std::string(split_it->begin(), split_it->end()) == "He");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l");
        CHECK(split_it.advance(1));
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l");
        CHECK(split_it.advance(2));
        CHECK(std::string(split_it->begin(), split_it->end()) == "o Wor");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l");
        CHECK(split_it.advance(3));
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l");
        CHECK(!split_it.advance(4)); //end reached
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }

    // advance_to_last skip empty
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty); //last is not empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
        CHECK(split_it.advance_to_last()); //do it twice
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "ld", cppstringx::split_mode::skip_empty); //last is empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello Wor");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "x", cppstringx::split_mode::skip_empty); //no separator
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello World");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("", "ld", cppstringx::split_mode::skip_empty); //empty string
        CHECK(!split_it.advance_to_last()); //no last, end position reached
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }

    // advance skip
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(split_it.advance(0));
        CHECK(std::string(split_it->begin(), split_it->end()) == "He");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(split_it.advance(1));
        CHECK(std::string(split_it->begin(), split_it->end()) == "o Wor");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(split_it.advance(2));
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(!split_it.advance(3)); //end reached
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }
}
