//-----------------------------------------------------------------------------
//  cppstringx tests
//  Copyright (c) 2022 Andreas Gau
//-----------------------------------------------------------------------------

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>
#include <cppstringx/cppstringx.hpp>

//-------------------------------------------------------------------------
// equals_comparer
//-------------------------------------------------------------------------

TEST_CASE("equals_comparer", "[util]")
{
    cppstringx::utility::equals_comparer comparer;
    CHECK(comparer('a', 'a'));
    CHECK(!comparer('a', 'A'));
    CHECK(comparer('a', L'a'));
    CHECK(!comparer('a', L'A'));
    //CHECK(!comparer('a', static_cast<uint16_t>('b')));
}


//-------------------------------------------------------------------------
// equals_comparer_ignoring_case
//-------------------------------------------------------------------------
TEST_CASE("equals_comparer_ignoring_case", "[util]")
{
    cppstringx::utility::equals_comparer_ignoring_case comparer;
    CHECK(comparer('a', 'a'));
    CHECK(comparer('a', 'A'));
    CHECK(comparer('a', L'a'));
    CHECK(comparer('a', L'A'));
    CHECK(!comparer(L'a', 'B'));
    //CHECK(!comparer('a', static_cast<uint16_t>('b')));

// some platforms do not handle setting the locale without throwing an exception
//    cppstringx::utility::equals_comparer_ignoring_case comparer_locale((std::locale("EN")));
//#ifdef _MSC_VER
//#pragma warning( push )
//#pragma warning( disable : 4566 )
//#pragma warning( disable : 4066 )
//#endif
//    CHECK(comparer(L'달', L'달'));
//#ifdef _MSC_VER
//#pragma warning( pop ) 
//#endif

    cppstringx::utility::equals_comparer_ignoring_case comparer_locale((std::locale()));
    CHECK(comparer_locale('a', 'A'));
}


//-------------------------------------------------------------------------
// null_terminated_string_iterator
//-------------------------------------------------------------------------
TEST_CASE("null_terminated_string_iterator", "[util]")
{
    // empty
    {
        cppstringx::utility::null_terminated_string_iterator<char> i1;
        cppstringx::utility::null_terminated_string_iterator<char> i2;
        CHECK(i1 == i2);
        CHECK(!(i1 != i2));
        CHECK((i1 - i2) == 0);
        CHECK(i1.get_position() == nullptr);
        CHECK(i1.get_end() == nullptr);
    }

    // const char*
    {
        const char* text = "Hello World";
        cppstringx::utility::null_terminated_string_iterator<const char> start_position(text);
        cppstringx::utility::null_terminated_string_iterator<const char> it(start_position);
        CHECK(start_position.get_position() == text);
        CHECK(start_position.get_end() == (text + strlen(text)));

        std::string copied; 
        for (; !it.is_end_position(); ++it)
        {
            copied += *it;
        }
        CHECK(copied == text);
        CHECK(copied.size() == static_cast<size_t>(it - start_position));
        start_position++;
        CHECK(*start_position == 'e');
    }
}


//-------------------------------------------------------------------------
// endpos_terminated_string_iterator
//-------------------------------------------------------------------------
TEST_CASE("endpos_terminated_string_iterator", "[util]")
{
    // empty
    {
        cppstringx::utility::endpos_terminated_string_iterator<std::string::iterator> i1;
    }

    // const char*
    {
        std::string text = "Hello World";
        cppstringx::utility::endpos_terminated_string_iterator<std::string::iterator> start_position(text.begin(), text.end());
        cppstringx::utility::endpos_terminated_string_iterator<std::string::iterator> it(start_position);

        std::string copied;
        for (; !it.is_end_position(); ++it)
        {
            copied += *it;
        }
        CHECK(copied == text);
        CHECK(copied.size() == static_cast<size_t>(it - start_position));
        start_position++;
        CHECK(*start_position == 'e');
    }
}

//-------------------------------------------------------------------------
// is_any_of
//-------------------------------------------------------------------------
TEST_CASE("is_any_of", "[util]")
{
    cppstringx::utility::is_any_of<const char*> isanyof("HeloWrd");

    CHECK(isanyof('H'));
    CHECK(isanyof('e'));
    CHECK(isanyof('l'));
    CHECK(isanyof('o'));
    CHECK(isanyof('W'));
    CHECK(isanyof('r'));
    CHECK(isanyof('d'));
    CHECK(!isanyof('x'));
    CHECK(!isanyof('h'));
    CHECK(!isanyof('E'));

    std::string empty;
    cppstringx::utility::is_any_of<std::string> isanyof2(empty);
    CHECK(!isanyof2('E'));
}