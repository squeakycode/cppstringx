//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("test range", "[range]")
{
    //const char*
    {
        const char* text = "Hello World";
        cppstringx::range<const char*> range(text, text + 5);
        {
            std::string copied;
            for (char c : range)
            {
                copied += c;
            }
            CHECK(copied == "Hello");
        }
        CHECK(range.end() - range.begin() == 5);
    }
    //char* with modify
    {
        char buffer[] = { "Hello World" };
        char* text = buffer;
        cppstringx::range<char*> range(text, text + 5);
        {
            *(range.begin() + 1) = 'a';
            std::string copied;
            for (char c : range)
            {
                copied += c;
            }
            CHECK(copied == "Hallo");
        }
        CHECK(range.end() - range.begin() == 5);
    }
    //std::string::const_iterator
    {
        std::string text("Hello World");
        cppstringx::range<std::string::const_iterator> range(text.begin(), text.begin() + 5);
        {
            std::string copied;
            for (char c : range)
            {
                copied += c;
            }
            CHECK(copied == "Hello");
        }
        CHECK(range.end() - range.begin() == 5);
    }
    //std::string::iterator
    {
        std::string text("Hello World");
        cppstringx::range<std::string::iterator> range(text.begin(), text.begin() + 5);
        {
            *(range.begin() + 1) = 'a';
            std::string copied;
            for (char c : range)
            {
                copied += c;
            }
            CHECK(copied == "Hallo");
        }
        CHECK(range.end() - range.begin() == 5);
    }
    //const char* empty range
    {
        cppstringx::range<const char*> range;
        CHECK(range.end() - range.begin() == 0);
        CHECK(range.begin() == range.end());

    }

    //std::string::iterator empty range
    {
        cppstringx::range<std::string::iterator> range;
    }
}
