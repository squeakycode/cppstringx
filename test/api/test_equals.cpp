//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("equals algorithm", "[equals]")
{
    //happy path
    CHECK(cppstringx::equals("Hello", "Hello"));
    CHECK(!cppstringx::equals("Hello World", "Hella"));
    CHECK(!cppstringx::equals("Yello", "Hello"));
    CHECK(!cppstringx::equals("Hollo", "Hello"));
    //contained text larger size
    CHECK(!cppstringx::equals("Hel", "Hello"));
    CHECK(!cppstringx::equals("Hel", "Hella"));
    //contained text empty
    CHECK(!cppstringx::equals("Hello", ""));
    //text empty contained text empty
    CHECK(cppstringx::equals("", ""));
}

TEST_CASE("iequals", "[equals]")
{
    //istarts_with just uses a different equals comparer
    CHECK(cppstringx::iequals("Hello", "Hello"));
    CHECK(!cppstringx::iequals("Hello", "Hella"));
}

TEST_CASE("equals universal", "[equals]")
{
    CHECK(cppstringx::equals("Ifmmp", "Hello", [](char a, char b) { return a - 1 == b; }));
}

TEST_CASE("equals type combination", "[equals]")
{
    //equals
    CHECK(cppstringx::equals("Hello", L"Hello"));
    CHECK(!cppstringx::equals(L"Hello", "Hella"));

    CHECK(cppstringx::equals("Hello", std::string("Hello")));
    CHECK(!cppstringx::equals(std::string("Hello"), "Hella"));

    CHECK(cppstringx::equals("Hello", std::wstring(L"Hello")));
    CHECK(!cppstringx::equals(std::wstring(L"Hello"), "Hella"));

    CHECK(cppstringx::equals("Hello", std::u16string(u"Hello")));
    CHECK(cppstringx::equals(std::u16string(u"Hello"), L"Hello"));

    //istarts_with
    CHECK(cppstringx::istarts_with("Hello", L"Hello"));
    CHECK(!cppstringx::istarts_with(L"Hello", "Hella"));

    CHECK(cppstringx::istarts_with("Hello", std::string("Hello")));
    CHECK(!cppstringx::istarts_with(std::string("Hello"), "Hella"));

    CHECK(cppstringx::istarts_with("Hello", std::wstring(L"Hello")));
    CHECK(!cppstringx::istarts_with(std::wstring(L"Hello"), "Hella"));

    char buffer[] = { "Hello World" };
    CHECK(cppstringx::equals("Hello", L"Hello"));
    CHECK(!cppstringx::equals(buffer, "Hella"));
    CHECK(!cppstringx::equals(&buffer[0], "Hella"));
}

TEST_CASE("equals used with ranges", "[equals]")
{
    char buffer[] = { "Hello World" };
    const char* text = "Hello World";
    std::string text2 = text;

    cppstringx::range<char*> another_text1(buffer, buffer + 5); //Hello
    std::string another_text2(another_text1.begin(), another_text1.end());
    cppstringx::range<const char*> another_text3(text + 1, text + 5); //ello
    cppstringx::range<std::string::iterator> text3(text2.begin(), text2.end()); //Hello World

    CHECK(!cppstringx::equals(text, another_text1));
    CHECK(!cppstringx::equals(text2, another_text1));
    CHECK(!cppstringx::equals(text3, another_text1));
    CHECK(!cppstringx::equals(text3, another_text2));
    CHECK(!cppstringx::equals(text, another_text3));
    CHECK(!cppstringx::equals(text2, another_text3));
    CHECK(cppstringx::equals(text3, text));
}
