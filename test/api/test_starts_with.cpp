//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("starts_with algorithm", "[starts_with]")
{
    //happy path
    CHECK(cppstringx::starts_with("Hello World", "Hello"));
    CHECK(!cppstringx::starts_with("Hello World", "Hella"));
    //prefix equal size
    CHECK(cppstringx::starts_with("Hello", "Hello"));
    CHECK(!cppstringx::starts_with("Hello", "Hella"));
    //prefix larger size
    CHECK(!cppstringx::starts_with("Hel", "Hello"));
    CHECK(!cppstringx::starts_with("Hel", "Hella"));
    //prefix empty
    CHECK(cppstringx::starts_with("Hello", ""));
    //text empty prefix empty
    CHECK(cppstringx::starts_with("", ""));
}

TEST_CASE("istarts_with", "[starts_with]")
{
    //istarts_with just uses a different equals comparer
    CHECK(cppstringx::istarts_with("Hello World", "Hello"));
    CHECK(!cppstringx::istarts_with("Hello World", "Hella"));
}

TEST_CASE("starts_with universal", "[starts_with]")
{
    CHECK(cppstringx::starts_with("Ifmmp World", "Hello", [](char a, char b) { return a - 1 == b; }));
}

TEST_CASE("starts_with type combination", "[starts_with]")
{
    //starts_with
    CHECK(cppstringx::starts_with("Hello World", L"Hello"));
    CHECK(!cppstringx::starts_with(L"Hello World", "Hella"));

    CHECK(cppstringx::starts_with("Hello World", std::string("Hello")));
    CHECK(!cppstringx::starts_with(std::string("Hello World"), "Hella"));

    CHECK(cppstringx::starts_with("Hello World", std::wstring(L"Hello")));
    CHECK(!cppstringx::starts_with(std::wstring(L"Hello World"), "Hella"));

    CHECK(cppstringx::starts_with("Hello World", std::u16string(u"Hello")));
    CHECK(cppstringx::starts_with(std::u16string(u"Hello World"), L"Hello"));

    //istarts_with
    CHECK(cppstringx::istarts_with("Hello World", L"Hello"));
    CHECK(!cppstringx::istarts_with(L"Hello World", "Hella"));

    CHECK(cppstringx::istarts_with("Hello World", std::string("Hello")));
    CHECK(!cppstringx::istarts_with(std::string("Hello World"), "Hella"));

    CHECK(cppstringx::istarts_with("Hello World", std::wstring(L"Hello")));
    CHECK(!cppstringx::istarts_with(std::wstring(L"Hello World"), "Hella"));

    char buffer[] = { "Hello World" };
    CHECK(cppstringx::starts_with("Hello World", L"Hello"));
    CHECK(!cppstringx::starts_with(buffer, "Hella"));
    CHECK(!cppstringx::starts_with(&buffer[0], "Hella"));


    //CHECK(cppstringx::istarts_with("Hello World", std::u16string(u"Hello"))); gcc throws bad_cast
    //CHECK(cppstringx::istarts_with(std::u16string(u"Hello World"), L"Hello")); gcc throws bad_cast
}

TEST_CASE("starts_with used with ranges", "[starts_with]")
{
    char buffer[] = { "Hello World" };
    const char* text = "Hello World";
    std::string text2 = text;

    cppstringx::range<char*> prefix1(buffer, buffer + 5); //Hello
    std::string prefix2(prefix1.begin(), prefix1.end());
    cppstringx::range<const char*> prefix3(text+1, text + 5); //ello
    cppstringx::range<std::string::iterator> text3(text2.begin(), text2.end()); //Hello World

    CHECK(cppstringx::starts_with(text, prefix1));
    CHECK(cppstringx::starts_with(text2, prefix1));
    CHECK(cppstringx::starts_with(text3, prefix1));
    CHECK(cppstringx::starts_with(text3, prefix2));
    CHECK(!cppstringx::starts_with(text, prefix3));
    CHECK(!cppstringx::starts_with(text2, prefix3));
    CHECK(!cppstringx::starts_with(text3, prefix3));
}
