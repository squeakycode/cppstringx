//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("contains algorithm", "[contains]")
{
    //happy path
    CHECK(cppstringx::contains("Hello World", "ello")); //inner
    CHECK(cppstringx::contains("Hello World", "Hello")); //at begin
    CHECK(cppstringx::contains("Hello World", "World")); //at end
    CHECK(!cppstringx::contains("Hello World", "ella"));
    CHECK(!cppstringx::contains("Hello World", "xxxx"));
    CHECK(!cppstringx::contains("Hello Worl", "World"));
    //affix equal size
    CHECK(cppstringx::contains("Hello", "Hello"));
    CHECK(!cppstringx::contains("Hello", "Hella"));
    //affix larger size
    CHECK(!cppstringx::contains("Hel", "Hello"));
    CHECK(!cppstringx::contains("Hel", "Hella"));
    //affix empty
    CHECK(cppstringx::contains("Hello", ""));
    //text empty affix empty
    CHECK(cppstringx::contains("", ""));
}

TEST_CASE("icontains", "[contains]")
{
    //istarts_with just uses a different equals comparer
    CHECK(cppstringx::icontains("Hello World", "Hello"));
    CHECK(!cppstringx::icontains("Hello World", "Hella"));
}

TEST_CASE("contains universal", "[contains]")
{
    CHECK(cppstringx::contains(" Ifmmp World", "Hello", [](char a, char b) { return a - 1 == b; }));
}

TEST_CASE("contains type combination", "[contains]")
{
    //contains
    CHECK(cppstringx::contains(" Hello World", L"Hello"));
    CHECK(!cppstringx::contains(L" Hello World", "Hella"));

    CHECK(cppstringx::contains(" Hello World", std::string("Hello")));
    CHECK(!cppstringx::contains(std::string(" Hello World"), "Hella"));

    CHECK(cppstringx::contains(" Hello World", std::wstring(L"Hello")));
    CHECK(!cppstringx::contains(std::wstring(L" Hello World"), "Hella"));

    CHECK(cppstringx::contains(" Hello World", std::u16string(u"Hello")));
    CHECK(cppstringx::contains(std::u16string(u" Hello World"), L"Hello"));

    //icontains
    CHECK(cppstringx::icontains(" Hello World", L"Hello"));
    CHECK(!cppstringx::icontains(L" Hello World", "Hella"));

    CHECK(cppstringx::icontains(" Hello World", std::string("Hello")));
    CHECK(!cppstringx::icontains(std::string(" Hello World"), "Hella"));

    CHECK(cppstringx::icontains(" Hello World", std::wstring(L"Hello")));
    CHECK(!cppstringx::icontains(std::wstring(L" Hello World"), "Hella"));

    char buffer[] = { "HHello World" };
    CHECK(cppstringx::contains(" Hello World", L"Hello"));
    CHECK(!cppstringx::contains(buffer, "Hella"));
    CHECK(!cppstringx::contains(&buffer[0], "Hella"));
}

TEST_CASE("contains used with ranges", "[contains]")
{
    char buffer[] = { "Hello World" };
    const char* text = "Hello World";
    std::string text2 = text;

    cppstringx::range<char*> contained_text1(buffer, buffer + 5); //Hello
    std::string contained_text2(contained_text1.begin(), contained_text1.end());
    cppstringx::range<const char*> contained_text3(text + 1, text + 5); //ello
    cppstringx::range<std::string::iterator> text3(text2.begin(), text2.end()); //Hello World

    CHECK(cppstringx::contains(text, contained_text1));
    CHECK(cppstringx::contains(text2, contained_text1));
    CHECK(cppstringx::contains(text3, contained_text1));
    CHECK(cppstringx::contains(text3, contained_text2));
    CHECK(cppstringx::contains(text, contained_text3));
    CHECK(cppstringx::contains(text2, contained_text3));
    CHECK(cppstringx::contains(text3, contained_text3));
}
