//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("test to_lower_copy", "[to_lower]")
{
    CHECK(cppstringx::to_lower_copy(std::string("AxByCz")) == "axbycz");
    CHECK(cppstringx::to_lower_copy(std::wstring(L"AxByCz")) == L"axbycz");
    CHECK(cppstringx::to_lower_copy(std::string("")) == "");
    CHECK(cppstringx::character_convert_copy(std::string("AxByCz"), [](char a) { if (a == 'A') return 'a'; return a; }) == "axByCz");
    {
        std::string text("AxByCz");
        CHECK(cppstringx::character_convert_copy(text, [](char a) -> std::string { if (a == 'A') return "123"; return std::string(1, a); }) == "123xByCz");
    }

}

TEST_CASE("test to_lower_in_place", "[to_lower]")
{
    {
        std::string text("AxByCz");
        CHECK(cppstringx::to_lower_in_place(text) == "axbycz");
        CHECK(&cppstringx::to_lower_in_place(text) == &text);
    }
    {
        std::string text("AxByCz");
        cppstringx::range<std::string::iterator> r(text.begin(), text.end());
        CHECK(&cppstringx::to_lower_in_place(r) == &r);
        CHECK(std::string(r.begin(), r.end()) == "axbycz");
    }
    {
        char buffer[] = { "AxByCz" };
        cppstringx::range<char*> r(buffer, buffer + 6);
        CHECK(&cppstringx::to_lower_in_place(r) == &r);
        CHECK(std::string(r.begin(), r.end()) == "axbycz");
    }
    {
        std::wstring text(L"AxByCz");
        CHECK(cppstringx::to_lower_in_place(text) == L"axbycz");
        CHECK(&cppstringx::to_lower_in_place(text) == &text);
    }
    {
        std::string text("");
        CHECK(cppstringx::to_lower_in_place(text) == "");
        CHECK(&cppstringx::to_lower_in_place(text) == &text);
    }
    {
        std::string text("AxByCz");
        CHECK(cppstringx::character_convert_in_place(text, [](char a) { if (a == 'A') return 'a'; return a; }) == "axByCz");
        CHECK(&cppstringx::character_convert_in_place(text, [](char a) { if (a == 'A') return 'a'; return a; }) == &text);
    }
    {
        char buffer[] = { "AxByCz" };
        CHECK(cppstringx::character_convert_in_place(buffer, [](char a) { if (a == 'A') return 'a'; return a; }) == std::string("axByCz"));
        CHECK(cppstringx::character_convert_in_place(buffer, [](char a) { if (a == 'A') return 'a'; return a; }) == buffer);
    }
}
