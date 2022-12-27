//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>
#include <vector>

TEST_CASE("split_iterator algorithm happy path", "[split]")
{
    std::string text = "Hello World";
    cppstringx::split_iterator<std::string, cppstringx::utility::is_any_of<const char*>> split_it(text, cppstringx::utility::is_any_of<const char*>(" "), cppstringx::split_mode::all);
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
}

template <typename text_type, typename text_type_separator>
void test_split_token_iterator(const text_type& text_to_iterate_over, const text_type_separator& separator_token, cppstringx::split_mode mode, std::vector<const char*> expected)
{
    auto split_it = cppstringx::make_split_iterator(text_to_iterate_over, cppstringx::utility::is_any_of<text_type_separator>(separator_token), mode);

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

TEST_CASE("split_iterator algorithm", "[split]")
{
    test_split_token_iterator("Hello World", " ", cppstringx::split_mode::all, { "Hello", "World" }); //single char split
    test_split_token_iterator("Hello World", "l", cppstringx::split_mode::all, { "He", "", "o Wor","d"}); //split with empty string, adjacent separator
    test_split_token_iterator("Hello World", "H", cppstringx::split_mode::all, { "", "ello World" }); //separator at begin
    test_split_token_iterator("Hello World", "x", cppstringx::split_mode::all, { "Hello World" }); //not found
    test_split_token_iterator("", "x", cppstringx::split_mode::all, { "" }); //empty
    test_split_token_iterator("Hello World", "d", cppstringx::split_mode::all, { "Hello Worl", "" }); //separator at end
    test_split_token_iterator("xHelloxWorldx", "x", cppstringx::split_mode::all, { "", "Hello", "World", "" }); //separator at end and begin

    test_split_token_iterator("Hello World", "l", cppstringx::split_mode::skip_empty, { "He", "o Wor","d" }); //split with empty string skipped
    test_split_token_iterator("Hello World", "H", cppstringx::split_mode::skip_empty, { "ello World" }); //separator at begin
    test_split_token_iterator("Hello World", "d", cppstringx::split_mode::skip_empty, { "Hello Worl" }); //separator at end
    test_split_token_iterator("", "x", cppstringx::split_mode::skip_empty, { }); //empty
    test_split_token_iterator("xHelloxWorldx", "x", cppstringx::split_mode::skip_empty, { "Hello", "World" }); //separator at end and begin
}

template <typename text_type>
void test_split_iterator_types(text_type& text_to_iterate_over,  std::vector<const char*> expected)
{
    auto split_it = cppstringx::make_split_iterator(text_to_iterate_over, cppstringx::utility::is_any_of<const char*>(" "));

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

TEST_CASE("split_iterator types", "[split]")
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

    test_split_iterator_types(L"Hello World", { "Hello", "World" });
    test_split_iterator_types(text1, { "Hello", "World" });
    test_split_iterator_types(text2, { "Hello", "World" });
    test_split_iterator_types(text3, { "Hello", "World" });
    test_split_iterator_types(text4, { "Hello", "World" });
    test_split_iterator_types(text5, { "Hello", "World" });
    test_split_iterator_types(text6, { "Hello", "World" });
    test_split_iterator_types(text7, { "Hello", "World" });
    test_split_iterator_types(text8, { "Hello", "World" });
    test_split_iterator_types(text9, { "Hello", "World" });
    test_split_iterator_types(text10, { "Hello", "World" });
    test_split_iterator_types(text11, { "Hello", "World" });
    test_split_iterator_types(text12, { "Hello", "World" });
    test_split_iterator_types(text13, { "Hello", "World" });
}

TEST_CASE("split_char_iterator", "[split]")
{
    auto split_it = cppstringx::make_split_chars_iterator("Hello World", " ");
    CHECK(std::string(split_it->begin(), split_it->end()) == "Hello");
    split_it = cppstringx::make_split_chars_iterator("Hello World", "H");
    CHECK(std::string(split_it->begin(), split_it->end()) == "");
    split_it = cppstringx::make_split_chars_iterator("Hello World", "H", cppstringx::split_mode::skip_empty);
    CHECK(std::string(split_it->begin(), split_it->end()) == "ello World");
}

TEST_CASE("split_chars container test", "[split]")
{
    {
        std::vector<std::string> result;
        result.push_back("Text");
        std::vector<std::string> expected = { "Hello", "World" };
        cppstringx::split(result, "Hello World", [](char a) {return a == ' '; });
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        result.push_back("Text");
        std::vector<std::string> expected = { "Text", "Hello", "World" };
        cppstringx::split(result, "Hello World", [](char a) {return a == ' '; }, cppstringx::split_mode::all, false /* clear_container */);
        CHECK(result == expected);
    }
    {
        std::vector<cppstringx::range<const char*>> result;
        cppstringx::split_chars(result, "Hello World", " ");
        REQUIRE(result.size() == 2);
        CHECK(std::string(result[0].begin(), result[0].end()) == "Hello");
        CHECK(std::string(result[1].begin(), result[1].end()) == "World");
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "", "o Wor","d" };
        cppstringx::split_chars(result, "Hello World", "l");
        CHECK(result == expected);
    }
    {
        std::vector<std::string> result;
        std::vector<std::string> expected = { "He", "o Wor","d" };
        cppstringx::split_chars(result, "Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(result == expected);
    }
}

TEST_CASE("split_iterator advance test", "[split]")
{
    // advance_to_last
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l"); //last is not empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
        CHECK(split_it.advance_to_last()); //do it twice
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "d"); //last is empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "x"); //no separator
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello World");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("", "d"); //empty string
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }

    // advance
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l");
        CHECK(split_it.advance(0));
        CHECK(std::string(split_it->begin(), split_it->end()) == "He");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l");
        CHECK(split_it.advance(1));
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l");
        CHECK(split_it.advance(2));
        CHECK(std::string(split_it->begin(), split_it->end()) == "o Wor");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l");
        CHECK(split_it.advance(3));
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l");
        CHECK(!split_it.advance(4)); //end reached
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }

    // advance_to_last skip empty
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l", cppstringx::split_mode::skip_empty); //last is not empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
        CHECK(split_it.advance_to_last()); //do it twice
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "d", cppstringx::split_mode::skip_empty); //last is empty
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello Worl");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "x", cppstringx::split_mode::skip_empty); //no separator
        CHECK(split_it.advance_to_last());
        CHECK(std::string(split_it->begin(), split_it->end()) == "Hello World");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("", "ld", cppstringx::split_mode::skip_empty); //empty string
        CHECK(!split_it.advance_to_last()); //no last, end position reached
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }

    // advance skip
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(split_it.advance(0));
        CHECK(std::string(split_it->begin(), split_it->end()) == "He");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(split_it.advance(1));
        CHECK(std::string(split_it->begin(), split_it->end()) == "o Wor");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(split_it.advance(2));
        CHECK(std::string(split_it->begin(), split_it->end()) == "d");
    }
    {
        auto split_it = cppstringx::make_split_chars_iterator("Hello World", "l", cppstringx::split_mode::skip_empty);
        CHECK(!split_it.advance(3)); //end reached
        CHECK(std::string(split_it->begin(), split_it->end()) == "");
    }
}