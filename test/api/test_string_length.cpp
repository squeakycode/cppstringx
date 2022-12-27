//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

TEST_CASE("test string_length", "[string_length]")
{
    //a non-empty string
    {
        CHECK(cppstringx::string_length("Hello World") == 11);
        CHECK(cppstringx::string_length(L"Hello World") == 11);
        const uint16_t array1[] = { 1,2,3,0 };
        CHECK(cppstringx::string_length(array1) == 3);
        CHECK(cppstringx::string_length(&array1[0]) == 3);
        uint16_t array2[] = { 1,2,3,0 };
        CHECK(cppstringx::string_length(array2) == 3);
        CHECK(cppstringx::string_length(&array2[0]) == 3);
        std::string text("Hello World");
        CHECK(cppstringx::string_length(text) == 11);
        const char array3[] = { 1,2,3,0 };
        CHECK(cppstringx::string_length(array3) == 3);
        char array4[] = { 1,2,3,0 };
        CHECK(cppstringx::string_length(array4) == 3);
    }
    //empty string
    {
        CHECK(cppstringx::string_length("") == 0);
        CHECK(cppstringx::string_length(L"") == 0);
        const uint16_t array1[] = { 0 };
        CHECK(cppstringx::string_length(array1) == 0);
        CHECK(cppstringx::string_length(&array1[0]) == 0);
        uint16_t array2[] = { 0 };
        CHECK(cppstringx::string_length(array2) == 0);
        CHECK(cppstringx::string_length(&array2[0]) == 0);
        std::string text;
        CHECK(cppstringx::string_length(text) == 0);
    }
}
