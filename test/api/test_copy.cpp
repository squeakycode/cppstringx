//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("test copy", "[copy]")
{
    CHECK(cppstringx::copy<std::string>("Hello World") == "Hello World");
    CHECK(cppstringx::copy<std::string>("") == "");
    CHECK(cppstringx::copy<std::wstring>("Hello World") == L"Hello World");
    CHECK(cppstringx::copy<std::u32string>("Hello World") == U"Hello World");

    std::string text("text");
    CHECK(cppstringx::copy(text, "Hello World", false) == "textHello World");
    CHECK(cppstringx::copy(text, "Hello World") == "Hello World");
    CHECK(&cppstringx::copy(text, "Hello World") == &text);
    CHECK(cppstringx::copy(text, U"Hello World") == "Hello World");
}
