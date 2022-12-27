//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("test to_upper_copy", "[to_upper]")
{
    CHECK(cppstringx::to_upper_copy(std::string("AxByCz")) == "AXBYCZ");
    CHECK(cppstringx::to_upper_copy(std::wstring(L"AxByCz")) == L"AXBYCZ");
    CHECK(cppstringx::to_upper_copy(std::string("")) == "");
    CHECK(cppstringx::character_convert_copy(std::string("AxByCz"), [](char a) { if (a == 'y') return 'Y'; return a; }) == "AxBYCz");
    {
        std::string text("AxByCz");
        CHECK(cppstringx::character_convert_copy(text, [](char a) -> std::string { if (a == 'A') return "123"; return std::string(1, a); }) == "123xByCz");
    }

}

TEST_CASE("test to_upper_in_place", "[to_upper]")
{
    {
        std::string text("AxByCz");
        CHECK(cppstringx::to_upper_in_place(text) == "AXBYCZ");
        CHECK(&cppstringx::to_upper_in_place(text) == &text);
    }
    {
        std::string text("AxByCz");
        cppstringx::range<std::string::iterator> r(text.begin(), text.end());
        CHECK(&cppstringx::to_upper_in_place(r) == &r);
        CHECK(std::string(r.begin(), r.end()) == "AXBYCZ");
    }
    {
        char buffer[] = { "AxByCz" };
        cppstringx::range<char*> r(buffer, buffer + 6);
        CHECK(&cppstringx::to_upper_in_place(r) == &r);
        CHECK(std::string(r.begin(), r.end()) == "AXBYCZ");
    }
    {
        std::wstring text(L"AxByCz");
        CHECK(cppstringx::to_upper_in_place(text) == L"AXBYCZ");
        CHECK(&cppstringx::to_upper_in_place(text) == &text);
    }
    {
        std::string text("");
        CHECK(cppstringx::to_upper_in_place(text) == "");
        CHECK(&cppstringx::to_upper_in_place(text) == &text);
    }
    {
        std::string text("AxByCz");
        CHECK(cppstringx::character_convert_in_place(text, [](char a) { if (a == 'y') return 'Y'; return a; }) == "AxBYCz");
        CHECK(&cppstringx::character_convert_in_place(text, [](char a) { if (a == 'y') return 'Y'; return a; }) == &text);
    }
    {
        char buffer[] = { "AxByCz" };
        CHECK(cppstringx::character_convert_in_place(buffer, [](char a) { if (a == 'y') return 'Y'; return a; }) == std::string("AxBYCz"));
        CHECK(cppstringx::character_convert_in_place(buffer, [](char a) { if (a == 'y') return 'Y'; return a; }) == buffer);
    }
}
