//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>
#include <vector>

TEST_CASE("test join", "[join]")
{
    std::vector<std::string> v = { "Hello", "World", "hello", "world" };
    std::string text("text");
    CHECK(cppstringx::join(text, v, " ", false) == "textHello World hello world");
    CHECK(cppstringx::join(text, v, " - ") == "Hello - World - hello - world");
    CHECK(&cppstringx::join(text, v, " - ") == &text);
}
