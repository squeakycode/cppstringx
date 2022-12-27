//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("ends_with algorithm", "[ends_with]")
{
    //happy path
    CHECK(cppstringx::ends_with("Hello World", "World"));
    CHECK(!cppstringx::ends_with("Hello World", "Vorld"));
    //ending equal size
    CHECK(cppstringx::ends_with("World", "World"));
    CHECK(!cppstringx::ends_with("World", "Vorld"));
    //ending larger size
    CHECK(!cppstringx::ends_with("orld", "World"));
    CHECK(!cppstringx::ends_with("orld", "Vorld"));
    //ending empty
    CHECK(cppstringx::ends_with("World", ""));
    //text empty ending empty
    CHECK(cppstringx::ends_with("", ""));
}

TEST_CASE("iends_with", "[ends_with]")
{
    //iends_with just uses a different equals comparer
    CHECK(cppstringx::iends_with("Hello World", "WORLD"));
    CHECK(!cppstringx::iends_with("Hello World", "Vorld"));
}

TEST_CASE("ends_with universal", "[ends_with]")
{
    CHECK(cppstringx::ends_with("Hello Xpsme", "World", [](char a, char b) { return a - 1 == b; }));
}

TEST_CASE("ends_with type combination", "[ends_with]")
{
    //ends_with
    CHECK(cppstringx::ends_with("Hello World", L"World"));
    CHECK(!cppstringx::ends_with(L"Hello World", "Vorld"));

    CHECK(cppstringx::ends_with("Hello World", std::string("World")));
    CHECK(!cppstringx::ends_with(std::string("Hello World"), "Vorld"));

    CHECK(cppstringx::ends_with("Hello World", std::wstring(L"World")));
    CHECK(!cppstringx::ends_with(std::wstring(L"Hello World"), "Vorld"));

    CHECK(cppstringx::ends_with("Hello World", std::u16string(u"World")));
    CHECK(cppstringx::ends_with(std::u16string(u"Hello World"), L"World"));

    //iends_with
    CHECK(cppstringx::iends_with("Hello World", L"World"));
    CHECK(!cppstringx::iends_with(L"Hello World", "Vorld"));

    CHECK(cppstringx::iends_with("Hello World", std::string("World")));
    CHECK(!cppstringx::iends_with(std::string("Hello World"), "Vorld"));

    CHECK(cppstringx::iends_with("Hello World", std::wstring(L"World")));
    CHECK(!cppstringx::iends_with(std::wstring(L"Hello World"), "Vorld"));

    //CHECK(cppstringx::iends_with("Hello World", std::u16string(u"World"))); gcc throws bad_cast
    //CHECK(cppstringx::iends_with(std::u16string(u"Hello World"), L"World")); gcc throws bad_cast
}

TEST_CASE("ends_with used with ranges", "[ends_with]")
{
    char buffer[] = { "Hello World" };
    const char* text = "Hello World";
    std::string text2 = text;

    cppstringx::range<char*> postfix1(buffer + 6, buffer + 11); //World
    std::string postfix2(postfix1.begin(), postfix1.end());
    cppstringx::range<const char*> postfix3(text + 6, text + 10); //Worl
    cppstringx::range<std::string::iterator> text3(text2.begin(), text2.end()); //Hello World

    CHECK(cppstringx::ends_with(text, postfix1));
    CHECK(cppstringx::ends_with(text2, postfix1));
    CHECK(cppstringx::ends_with(text3, postfix1));
    CHECK(cppstringx::ends_with(text3, postfix2));
    CHECK(!cppstringx::ends_with(text, postfix3));
    CHECK(!cppstringx::ends_with(text2, postfix3));
    CHECK(!cppstringx::ends_with(text3, postfix3));
}
