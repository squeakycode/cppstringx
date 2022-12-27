// cppstringx - C++ String Extensions
// Link: https://github.com/squeakycode/cppstringx
// Version: 1.0.0
// Minimum required C++ Standard: C++11
// License: BSD 3-Clause License
// 
// Copyright (c) 2022, Andreas Gau
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
\file
\brief Contains cppstringx basic string operation functions extending the C++ Standard Library.
\mainpage
C++ String Extensions {#pageTitle}
=====================

##Purpose##
The C++ standard library is missing basic functions (see the table below) for handling
standard string operations. You have to implement these yourself. Libraries like Boost String
Algorithms provide the needed functions, but often the libraries that come into question
are quite large and unhandy.

C++ String Extensions provides an easy to use solution to fill this gap.

##Comparison of Common String Handling Support Features##

\note The following table represents only a rough overview.

|C++ Standard Library | boost       | .NET String   | python String | Java String | cppstringx  |
|---------------------|-------------|---------------|---------------|-------------|-------------|
| contains (C++23)    | contains    | Contains      | if … in       | contains    | contains    |
| starts_with (C++20) | starts_with | StartsWith    | startswith    | startsWith  | starts_with |
| ends_with (C++20)   | ends_with   | EndsWith      | endswith      | endsWith    | ends_with   |
| -                   | to_lower    | ToUpper       | upper         | toUpperCase | to_lower    |
| -                   | to_upper    | ToLower       | lower         | toLowerCase | to_upper    |
| -                   | trim        | Trim          | strip         | trim        | trim        |
| -                   | trim_right  | TrimEnd       | rstrip        | -           | trim_end    |
| -                   | trim_left   | TrimStart     | lstrip        | -           | trim_start  |
| -                   | replace_all | Replace       | replace       | replace     | replace_all |
| -                   | split       | Split         | split         | split       | split       |
| -                   | join        | Join          | join          | -           | join        |

##Design Goals##
- Header-only, single header
- KISS principle
- Easy to use and well documented
- Multi-platform support
- No build warnings on common compilers
- High test coverage
- Be able to mix different code unit types in one function

##When to Use##
- You need some of the string functions listed above.
- If the C++ Standard Library provides the listed functions, it is recommended to use the the C++ Standard Library instead.
- If you already use boost, it is recommended use Boost String Algorithms Library instead.
- If you already using Qt, it is recommended use the Qt strings instead.

##Character Encoding##  {#character_encoding}

A quick run-down on character encoding, see e.g. Wikipedia for more detailed information:
- Character encoding is the process of assigning numbers to graphical characters.
- A code point is a numerical value that maps to a specific character.
- The ASCII character encoding encodes 128 specified characters into seven-bit integers (Range 0x00 to 0x7F).
- A code page is a character encoding that is typically a superset of ASCII and stored in a single byte (Range 0x00 to 0xFF).
- The range from 0x80 to 0xFF in a code page is typically used to encode characters used by various languages and graphical elements.
- The Latin 1 (ISO/IEC 8859-1) encoding represents the alphabets of Western European language and is stored in a single byte (Range 0x00 to 0xFF).
- The Latin 1 character encoding is a superset of the ASCII character encoding.
- The Unicode Standard is a character encoding that supports a maximum of 1,112,064 code points (Range 0x00 to 0x10FFFF).
- The Unicode character encoding is a superset of the Latin 1 character encoding.
- UTF-8 is a variable-width character encoding for Unicode code points stored in bytes.
- UTF-8 stores the ASCII range in one byte. All other characters are encoded using multiple bytes.
- A code unit is one is the basic component used by a character encoding system, e.g. for UTF-8 a code unit is a byte.
- UTF-16 is a variable-width character encoding for Unicode code points are stored in 16-bit integer values.
- The Unicode ranges U+D800 to 0xDBFF and 0xDC00 to 0xDFFF are no valid characters, they are used for UTF-16 encoding using two code units.
- UTF-32 is a character encoding for Unicode code points are stored in 32-bit integer values, 21 bits are actually used.

For applications it is recommended to use a single character encoding and convert to and from the internally used encoding on in- and output.
Nonetheless cppstringx allows you to compare strings stored in different character types, e.g. char16_t and char. If you compare
strings the used encoding must be equivalent. If this is not the case, it is safe to compare:
- A string that is UTF-8 encoded and another string that is ASCII encoded.
- A string that is UTF-16 encoded and another string that is Latin 1 encoded.
- A string that is UTF-16 encoded and another string that is UTF-32 encoded if one of the strings uses only the encoding range 0x0000 to 0xD7FF.

Typically you know the encoding range for constant string literals in your application that you compare other strings with.
*/

#pragma once

//Contains std::string, std::wstring, std::u8string (C++20), std::u16string (C++11), std::u32string (C++11)
#include <string>
//Encapsulates culture-specific information, needed for string operations.
#include <locale>
//Includes commonly used C string handling functions.
#include <cstring>
//Include commonly used C assert handling.
#include <cassert>




/// Provides basic string operation functions extending the C++ Standard Library.
namespace cppstringx
{
    //-------------------------------------------------------------------------
    // string_length
    //-------------------------------------------------------------------------

    /**
        \brief Determines the string length of a null-terminated string of value type char (C string).
        \param[in] p    A pointer to the null-terminated string. \c p must not be a nullptr.

        \note If the string uses a multi-byte character encoding e.g., UTF-8, the number of printed characters.
        can be less than the number of characters returned by this function.

        \return The number of character values used to store the string without terminating null.
    */
    inline size_t string_length(const char* p)
    {
        //undefined behavior when p == nullptr, omitted any check here for release builds on purpose for speed
        assert(p);
        //use the std method assuming it is more optimized than a for loop
        size_t result = ::strlen(p);
        return result;
    }

    /**
        \brief Determines the string length of a null-terminated string of value type wchar_t.
        \param[in] p    A pointer to the null-terminated string. \c p must not be a nullptr.

        \return The number of character values used to store the string without terminating null.
    */
    inline size_t string_length(const wchar_t* p)
    {
        //undefined behavior when p == nullptr, omitted any check here for release builds on purpose for speed
        assert(p);
        //use the std method assuming it is more optimized than a for loop
        size_t result = ::wcslen(p);
        return result;
    }

    /**
        \brief Determines the string length of a null-terminated string of any value type.
        \param[in] p    A pointer to the null-terminated string. \c p must not be a nullptr.

        \return The number of character values used to store the string without terminating null.
    */
    template <typename char_type>
    size_t string_length(const char_type* p)
    {
        //undefined behavior when p == nullptr, omitted any check here for release builds on purpose for speed
        assert(p);
        //for all other types use a simple loop to find the end of the string
        const char_type* p_end = p;
        for (; *p_end; ++p_end)
        {
        }
        size_t result = p_end - p;
        return result;
    }

    /**
        \brief Determines the string length of a null-terminated string of any value type.
        \param[in] p    A pointer to the null-terminated string. \c p must not be a nullptr.

        \return The number of character values used to store the string without terminating null.
    */
    template <typename char_type>
    size_t string_length(char_type* p)
    {
        size_t result = string_length(const_cast<const char_type*>(p));
        return result;
    }

    /**
        \brief Determines the string length of string or string range object.
        \param[in] text    A string or range object.

        \note This function is intended to be used only if you want to
        handle null-terminated strings and other string objects alike.

        \return The number of character values used to store the string.
    */
    template <typename text_type>
    size_t string_length(const text_type& text)
    {
        size_t result = text.size();
        return result;
    }

    //-------------------------------------------------------------------------
    // implementation
    //-------------------------------------------------------------------------

    // Provides implementation detail. This namespace should not be used directly.
    // Note that namespace implementation appears multiple times in this header file.
    // This is needed to get the declarations in the right order.
    namespace implementation
    {
        // Sometimes we need to deduce types from iterator types.
        // This little helper gets the correct reference to return for an
        // iterators reference operator and the value_type (aka char_type below).
        // Examples:
        // typename iterator_traits_resolver<const char*>::value_type is char
        // typename iterator_traits_resolver<const char*>::reference const is char&
        // typename iterator_traits_resolver<std::string::iterator>::value_type is char
        // typename iterator_traits_resolver<std::string::iterator>::reference is char&
        // typename iterator_traits_resolver<std::string::const_iterator>::value_type is char
        // typename iterator_traits_resolver<std::string::const_iterator>::reference is const char&
        template <typename T> //matches iterators (actually when not a pointer)
        struct iterator_traits_resolver
        {
            typedef typename T::reference reference;   // The char reference type, e.g. const char&
            typedef typename T::value_type value_type; // The char type, e.g. char
        };
        template <typename T> //matches pointers
        struct iterator_traits_resolver<T*>
        {
            typedef T& reference;
            typedef T value_type;
        };
        template <typename T> //matches const pointers
        struct iterator_traits_resolver<const T*>
        {
            typedef const T& reference;
            typedef T value_type;
        };
    }

    //-------------------------------------------------------------------------
    // range
    //-------------------------------------------------------------------------

    /**
    \brief A trivial range class used for defining a range as a view on an existing string.
    The range is defined by a begin and an end position. As per convention the range class is
    not expected to be used with reverse iterators.
    You can use this template class with pointers to strings or string iterators.
    Examples:
    \code
    const char* text = "Hello World";
    cppstringx::range<const char*> range(text, text + 5); //Hello
    ...
    char buffer[] = { "Hello World" };
    char* text = buffer;
    cppstringx::range<char*> range(text, text + 5); //Hello
    ...
    std::string text("Hello World");
    cppstringx::range<std::string::const_iterator> range1(text.begin(), text.begin() + 5);  //Hello
    cppstringx::range<std::string::iterator> range2(text.begin() + 3, text.begin() + 5);  //lo
    \endcode
    */
    template <typename char_pointer_or_iterator_type>
    class range
    {
    public:
        /**
            \brief Constructs an empty range.
        */
        range()
            : it_begin()
            , it_end()
        {
        }

        /**
            \brief Constructs a range.
            \param[in] start_position    The start position in a string.
            \param[in] end_position      The end position in a string. One character behind the last character of the range.
        */
        range(const char_pointer_or_iterator_type& start_position, const char_pointer_or_iterator_type& end_position)
            : it_begin(start_position)
            , it_end(end_position)
        {
        }

        /**
            \brief The start position of the range.
            \return The start position of the range.
        */
        char_pointer_or_iterator_type begin() const
        {
            return it_begin;
        }

        /**
            \brief The end position of the range.
            \return The end position of the range. One character behind the last character of the range.
        */
        char_pointer_or_iterator_type end() const
        {
            return it_end;
        }

        typedef char_pointer_or_iterator_type iterator_type; //!< The iterator type used by the range.
    private:
        char_pointer_or_iterator_type it_begin;
        char_pointer_or_iterator_type it_end;
    };


    //-------------------------------------------------------------------------
    // utility
    //-------------------------------------------------------------------------
    /// Provides trivial string handling helper classes.
    /// These classes are needed to be able to handle different string types the same way
    /// and are optimized away in release builds.
    /// Note that namespace utility appears multiple times in this header file.
    /// This is needed to get the declarations in the right order.
    namespace utility
    {
        //-------------------------------------------------------------------------
        // null_terminated_string_iterator
        //-------------------------------------------------------------------------

        /**
            \brief Used for reading a string from front to back without knowing before-hand where it ends.
                   This optimizes operations like find and starts_with, because the comparison will in a lot of cases never
                   reach the end of the string.
                   The counterpart of null_terminated_string_iterator is the endpos_terminated_string_iterator.
            \note This class is intended to be used only when you want to
            handle null-terminated strings and other string objects alike.
        */
        template <typename char_type>
        class null_terminated_string_iterator
        {
        public:
            /**
                \brief Constructs an empty null-terminated string iterator.
            */
            null_terminated_string_iterator()
                : p()
            {
            }

            /**
                \brief Constructs a null-terminated string iterator pointing to a position in a string.
                \param[in] position    The position in a string.
            */
            null_terminated_string_iterator(char_type* position)
                : p(position)
            {
            }

            /**
                \brief Compares if two iterator positions are equal.
                \param[in] rhs    The right-hand side object to compare to.
                \returns Returns true if the position of the iterators is the same.
            */
            bool operator==(const null_terminated_string_iterator<char_type>& rhs) const
            {
                return p == rhs.p;
            }

            /**
                \brief Compares if two iterator positions are not equal.
                \param[in] rhs    The right-hand side object to compare to.
                \returns Returns true if the position of the iterators is different.
            */
            bool operator!=(const null_terminated_string_iterator<char_type>& rhs) const
            {
                return p != rhs.p;
            }

            /**
                \brief Prefix increment operator.
                \return Advances the iterator to the next position and returns a reference to itself.
            */
            null_terminated_string_iterator<char_type>& operator++ ()
            {
                ++p;
                return *this;
            }

            /**
                \brief Postfix increment operator.
                \return Returns an iterator to the next position.
            */
            null_terminated_string_iterator<char_type>  operator++ (int)
            {
                null_terminated_string_iterator<char_type> result(p);
                ++p;
                return result;
            }

            /**
                \brief Checks whether the end position has been reached.
                \return Returns true if the end position has been reached.
            */
            bool is_end_position() const
            {
                return *p == 0;
            }

            /**
                \brief Reference operator.
                \return Returns a reference to the value at the current position the iterator points to.
            */
            char_type& operator*() const
            {
                return *p;
            }

            /**
                \brief Minus operator.
                \param[in] rhs    The right-hand side object used to compute the distance to.
                \return Returns   The number of character values used to store the string range between the iterator and \c rhs.
            */
            std::ptrdiff_t operator-(const null_terminated_string_iterator<char_type>& rhs) const
            {
                return p - rhs.p;
            }

            /**
                \brief Get the wrapped current position.
                \return Returns the wrapped current position.
            */
            char_type* get_position() const
            {
                return p;
            }

            /**
                \brief Determine and return the end position.
                \return Returns the determined end position.
            */
            char_type* get_end() const
            {
                char_type* result = p;
                if (p && *p) // There is a valid pointer and we are currently not yet positioned at the terminating null.
                {
                    result += string_length(p); // Advance to the end position one character past the last character.
                }
                return result;
            }

            typedef char_type value_type; //!< The type of the character values.
            typedef char_type& reference; //!< The type returned by the reference operator.
            typedef char_type* iterator_type; //!< The type internally used as iterator.
        private:
            char_type* p;
        };

        //-------------------------------------------------------------------------
        // endpos_terminated_string_iterator
        //-------------------------------------------------------------------------

        /**
            \brief Treats an string with iterators like a null-terminated string or you can use two pointers to iterate over a range.
                   The counterpart of null_terminated_string_iterator is the endpos_terminated_string_iterator.
            \note This class is intended to be used only when you want to
            handle null-terminated strings and other string objects alike.
        */
        template <
            typename char_pointer_or_iterator_type, // The character pointer or iterator.
            //The type returned by the reference operator. It is deduced automatically.
            //This template parameter is only added here to be able to make char_type_reference const.
            typename char_type_reference = typename implementation::iterator_traits_resolver<char_pointer_or_iterator_type>::reference
        >
        class endpos_terminated_string_iterator
        {
        public:
            /**
                \brief Constructs an empty null-terminated string iterator.
            */
            endpos_terminated_string_iterator()
                : it_position()
                , it_end()
            {
            }

            /**
                \brief Constructs a null-terminated string iterator pointing to a position in a string.
                \param[in] start_position    The start position in a string.
                \param[in] end_position      The end position in a string. One character behind the last character of the range.

            */
            endpos_terminated_string_iterator(const char_pointer_or_iterator_type& start_position, const char_pointer_or_iterator_type& end_position)
                : it_position(start_position)
                , it_end(end_position)
            {
            }

            /**
                \brief Compares if two iterator positions are equal.
                \param[in] rhs    The right-hand side object to compare to.
                \returns Returns true if the position of the iterators is the same.
            */
            bool operator==(const endpos_terminated_string_iterator<char_pointer_or_iterator_type>& rhs) const
            {
                return it_position == rhs.it_position;
            }

            /**
                \brief Compares if two iterator positions are not equal.
                \param[in] rhs    The right-hand side object to compare to.
                \returns Returns true if the position of the iterators is different.
            */
            bool operator!=(const endpos_terminated_string_iterator<char_pointer_or_iterator_type>& rhs) const
            {
                return it_position != rhs.it_position;
            }

            /**
                \brief Prefix increment operator.
                \return Advances the iterator to the next position and returns a reference to itself.
            */
            endpos_terminated_string_iterator<char_pointer_or_iterator_type>& operator++ ()
            {
                ++it_position;
                return *this;
            }

            /**
                \brief Postfix increment operator.
                \return Returns an iterator to the next position.
            */
            endpos_terminated_string_iterator<char_pointer_or_iterator_type>  operator++ (int)
            {
                endpos_terminated_string_iterator<char_pointer_or_iterator_type> result(it_position, it_end);
                ++it_position;
                return result;
            }

            /**
                \brief Checks whether the end position has been reached.
                \return Returns true if the end position has been reached.
            */
            bool is_end_position() const
            {
                return it_position == it_end;
            }

            /**
                \brief Reference operator.
                \return Returns a reference to the value at the current position the iterator points to.
            */
            char_type_reference operator*() const
            {
                return *it_position;
            }

            /**
                \brief Minus operator.
                \param[in] rhs    The right-hand side object used to compute the distance to.
                \return Returns   The number of character values used to store the string range between the iterator and \c rhs.
            */
            std::ptrdiff_t operator-(const endpos_terminated_string_iterator<char_pointer_or_iterator_type>& rhs) const
            {
                return it_position - rhs.it_position;
            }

            /**
                \brief Get the wrapped current position.
                \return Returns the wrapped current position.
            */
            const char_pointer_or_iterator_type& get_position() const
            {
                return it_position;
            }

            /**
                \brief Get the wrapped end position.
                \return Returns the wrapped end position.
            */
            const char_pointer_or_iterator_type& get_end() const
            {
                return it_end;
            }

            typedef char_pointer_or_iterator_type iterator_type; //!< The type internally used as iterator.

        private:
            char_pointer_or_iterator_type it_position;
            char_pointer_or_iterator_type it_end;
        };

        //-------------------------------------------------------------------------
        // equals_comparer
        //-------------------------------------------------------------------------

        /**
            \brief Compares two character values for equality.
            The comparer classes are used to be able to apply different modes of comparison
            e.g., for ignoring the character casing.
            You can use a two parameter lambda expression as comparer alternatively.
            Character encoding is not handled by comparer classes.
        */
        class equals_comparer
        {
        public:

            /**
                \brief Compares two character values.
                \param[in] value_lhs    The left-hand side value.
                \param[in] value_rhs    The right-hand side value.
                \return Returns true if the character values are equal.
                \note Left-hand side or right-hand side are defined by the order of the parameters
                      of the called cppstringx function.
            */
            template <typename char_type_a, typename char_type_b>
            bool operator()(char_type_a value_lhs, char_type_b value_rhs) const
            {
                // Note: If you get a compile error here the character value types are not directly comparable.
                // You can extend this comparer here or use an own one to work around the problem.
                bool result = (value_lhs == value_rhs);
                return result;
            }
        };

        //-------------------------------------------------------------------------
        // equals_comparer_ignoring_case
        //-------------------------------------------------------------------------

        /**
            \brief Compares two character values for equality  ignoring character casing.
            The comparer classes are used to be able to apply different modes of comparison
            e.g., for ignoring the character casing.
            You can use a two parameter lambda expression as comparer alternatively.
            Character encoding is not handled by comparer classes.
        */
        class equals_comparer_ignoring_case
        {
        public:
            /**
                \brief Constructs a comparer using the default locale.
            */
            equals_comparer_ignoring_case()
                : locale_object()
            {
            }

            /**
                \brief Constructs a comparer using a non default locale.
                \param[in] non_default_locale_object    You can pass a different locale depending on your requirements, e.g. std::locale("Fr_CH").
            */
            equals_comparer_ignoring_case(const std::locale& non_default_locale_object)
                : locale_object(non_default_locale_object)
            {
            }

            /**
                \brief Compares two character values ignoring character casing.
                \param[in] value_lhs    The left-hand side value.
                \param[in] value_rhs    The right-hand side value.
                \return Returns true if the character values are equal. The character casing is ignored
                \note Left-hand side or right-hand side are defined by the order of the parameters
                      of the called cppstringx function.
            */
            template <typename char_type_a, typename char_type_b>
            bool operator()(char_type_a value_lhs, char_type_b value_rhs) const
            {
                // Note: If you get a compile error here the character value types are not directly comparable.
                // You can extend this comparer here or use an own one to work around the problem.
                auto value_lhs_low = std::tolower(value_lhs, locale_object);
                auto value_rhs_low = std::tolower(value_rhs, locale_object);
                bool result = (value_lhs_low == value_rhs_low);
                return result;
            }
        private:
            std::locale locale_object;
        };

        //-------------------------------------------------------------------------
        // is_space
        //-------------------------------------------------------------------------

        /**
            \brief Checks whether a character is a white space character.
            You can use a two parameter lambda expression alternatively.
        */
        class is_space
        {
        public:
            /**
                \brief Constructs an is_space predicate using the default locale.
            */
            is_space()
                : locale_object()
            {
            }

            /**
                \brief Constructs an is_space predicate using a non default locale.
                \param[in] non_default_locale_object    You can pass a different locale depending on your requirements, e.g. std::locale("Fr_CH").
            */
            is_space(const std::locale& non_default_locale_object)
                : locale_object(non_default_locale_object)
            {
            }

            /**
                \brief Checks whether a character is a white space character.
                \param[in] value    The character to check.
                \return Returns true if the character is a white space character.
            */
            template <typename char_type>
            bool operator()(char_type value) const
            {
                bool result = std::isspace(value, locale_object);
                return result;
            }
        private:
            std::locale locale_object;
        };

        //-------------------------------------------------------------------------
        // to_lower_case_converter
        //-------------------------------------------------------------------------

        /**
            \brief Used to convert characters to their lower case version.
            The converter classes are used to be able to apply different modes of
            conversion or handling different character encodings.
        */
        class to_lower_case_converter
        {
        public:
            /**
                \brief Constructs a converter using the default locale.
            */
            to_lower_case_converter()
                : locale_object()
            {
            }

            /**
                \brief Constructs a converter using a non default locale.
                \param[in] non_default_locale_object    You can pass a different locale depending on your requirements, e.g. std::locale("Fr_CH").
            */
            to_lower_case_converter(const std::locale& non_default_locale_object)
                : locale_object(non_default_locale_object)
            {
            }

            /**
                \brief Converts a character to lower case if applicable or returns the same value.
                \param[in] value    A character value.
                \return Returns the converted value or the input value if no conversion is needed.
            */
            template <typename char_type_a>
            char_type_a operator()(char_type_a value) const
            {
                auto result = std::tolower(value, locale_object);
                return result;
            }
        private:
            std::locale locale_object;
        };

        //-------------------------------------------------------------------------
        // to_upper_case_converter
        //-------------------------------------------------------------------------

        /**
            \brief Used to convert characters to their upper case version.
            The converter classes are used to be able to apply different modes of
            conversion or handling different character encodings.
        */
        class to_upper_case_converter
        {
        public:
            /**
                \brief Constructs a converter using the default locale.
            */
            to_upper_case_converter()
                : locale_object()
            {
            }

            /**
                \brief Constructs a converter using a non default locale.
                \param[in] non_default_locale_object    You can pass a different locale depending on your requirements, e.g. std::locale("Fr_CH").
            */
            to_upper_case_converter(const std::locale& non_default_locale_object)
                : locale_object(non_default_locale_object)
            {
            }

            /**
                \brief Converts a character to upper case if applicable or returns the same value.
                \param[in] value    A character value.
                \return Returns the converted value or the input value if no conversion is needed.
            */
            template <typename char_type_a>
            char_type_a operator()(char_type_a value) const
            {
                auto result = std::toupper(value, locale_object);
                return result;
            }
        private:
            std::locale locale_object;
        };

    } //utility namespace


    //-------------------------------------------------------------------------
    // implementation
    //-------------------------------------------------------------------------
    // Provides implementation detail. This namespace should not be used directly.
    // Note that namespace implementation appears multiple times in this header file.
    // This is needed to get the declarations in the right order.
    namespace implementation
    {
        // Checks whether the passed prefix matches.
        template <typename terminated_iterator_type_a, typename terminated_iterator_type_b, typename equals_comparer_type>
        inline bool prefix_matches(terminated_iterator_type_a itt_text, terminated_iterator_type_b itt_prefix, const equals_comparer_type& compare)
        {
            // Read both strings.
            for (; !itt_text.is_end_position() && !itt_prefix.is_end_position(); ++itt_text, ++itt_prefix)
            {
                if (!compare(*itt_text, *itt_prefix))
                {
                    // Stop if there is a difference.
                    break;
                }
            }

            // If we managed to read the full prefix, the prefix matches.
            bool result = itt_prefix.is_end_position();
            return result;
        }

        // Checks whether the passed two strings match.
        template <typename terminated_iterator_type_a, typename terminated_iterator_type_b, typename equals_comparer_type>
        inline bool full_match(terminated_iterator_type_a itt_text_lhs, terminated_iterator_type_b itt_text_rhs, const equals_comparer_type& compare)
        {
            // Read both strings.
            for (; !itt_text_lhs.is_end_position() && !itt_text_rhs.is_end_position(); ++itt_text_lhs, ++itt_text_rhs)
            {
                if (!compare(*itt_text_lhs, *itt_text_rhs))
                {
                    // Stop if there is a difference.
                    break;
                }
            }

            // If both strings are at end position the strings fully match.
            bool result = itt_text_lhs.is_end_position() && itt_text_rhs.is_end_position();
            return result;
        }

        // Checks whether the passed infix matches and returns the found range.
        template <typename terminated_iterator_type_a, typename terminated_iterator_type_b, typename equals_comparer_type>
        inline range<terminated_iterator_type_a> find_forward_optimized(terminated_iterator_type_a itt_text, const terminated_iterator_type_b& itt_contained_string, const equals_comparer_type& compare)
        {
            // Read the text a check for the infix at every position.
            // We need to initialize here for the case if text is empty.
            terminated_iterator_type_a itt_text_compare_loop(itt_text);
            terminated_iterator_type_b itt_contained_string_compare_loop(itt_contained_string);
            for (; !itt_text.is_end_position(); ++itt_text, itt_text_compare_loop = itt_text, itt_contained_string_compare_loop = itt_contained_string)
            {
                // Read both strings.
                for (; !itt_text_compare_loop.is_end_position() && !itt_contained_string_compare_loop.is_end_position(); ++itt_text_compare_loop, ++itt_contained_string_compare_loop)
                {
                    if (!compare(*itt_text_compare_loop, *itt_contained_string_compare_loop))
                    {
                        // Stop if there is a difference.
                        break;
                    }
                }
                // Success, if we managed to read the full contained string, the infix matches
                // or failed, we reached the end of the string while checking for the contained string.
                if (itt_contained_string_compare_loop.is_end_position() || itt_text_compare_loop.is_end_position())
                {
                    break;
                }
            }
            range<terminated_iterator_type_a> result;
            if (itt_contained_string_compare_loop.is_end_position())
            {
                // Success, we found the contained string.
                result = range<terminated_iterator_type_a>(itt_text, itt_text_compare_loop);
            }
            else
            {
                // We did not find the contained string, return begin and end iterator at end position.
                assert(itt_text_compare_loop.is_end_position());
                result = range<terminated_iterator_type_a>(itt_text_compare_loop, itt_text_compare_loop);
            }
            return result; //found if range.begin().is_end_position() != true
        }

        //-------------------------------------------------------------------------
        // terminated_iterator_type_resolver
        //-------------------------------------------------------------------------

        // For the terminated_iterator_type_resolver resolver below the compiler needs to be able to evaluate all
        // variants for a type even though it can know that the variant will never be instantiated
        // to work around this we help it with this extra resolver
        class dummy;
        template <typename T> // this variant is actually used for string objects
        struct dummy_terminated_iterator_type_resolver
        {
            typedef typename T::const_iterator const_iterator;
            typedef typename T::iterator iterator;
            typedef typename T::const_reverse_iterator const_reverse_iterator;
            typedef typename T::reverse_iterator reverse_iterator;
        };
        template <typename T> //dummy
        struct dummy_terminated_iterator_type_resolver<range<T>>
        {
            typedef dummy* const_iterator;
            typedef dummy* iterator;
            typedef dummy* const_reverse_iterator;
            typedef dummy* reverse_iterator;
        };
        template <typename T> //dummy
        struct dummy_terminated_iterator_type_resolver<const range<T>>
        {
            typedef dummy* const_iterator;
            typedef dummy* iterator;
            typedef dummy* const_reverse_iterator;
            typedef dummy* reverse_iterator;
        };
        template <typename T, size_t N> //dummy
        struct dummy_terminated_iterator_type_resolver<const T[N]>
        {
            typedef dummy* const_iterator;
            typedef dummy* iterator;
            typedef dummy* const_reverse_iterator;
            typedef dummy* reverse_iterator;
        };
        template <typename T, size_t N>  //dummy
        struct dummy_terminated_iterator_type_resolver<T[N]>
        {
            typedef dummy* const_iterator;
            typedef dummy* iterator;
            typedef dummy* const_reverse_iterator;
            typedef dummy* reverse_iterator;
        };
        template <typename T>  //dummy
        struct dummy_terminated_iterator_type_resolver<const T*>
        {
            typedef dummy* const_iterator;
            typedef dummy* iterator;
            typedef dummy* const_reverse_iterator;
            typedef dummy* reverse_iterator;
        };
        template <typename T>  //dummy
        struct dummy_terminated_iterator_type_resolver<T*>
        {
            typedef dummy* const_iterator;
            typedef dummy* iterator;
            typedef dummy* const_reverse_iterator;
            typedef dummy* reverse_iterator;
        };

        // The following type resolver is used to get the return types of the following functions.
        // make_terminated_iterator_reverse()
        // make_terminated_iterator_forward()
        // make_const_terminated_iterator_reverse()
        // make_const_terminated_iterator_forward()
        template <typename T>
        struct terminated_iterator_type_resolver  // strings
        {
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::const_iterator > const_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::iterator> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::const_reverse_iterator> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::reverse_iterator> reverse_terminated_iterator_type;
        };
        template <typename T>
        struct terminated_iterator_type_resolver<const T> // const string objects
        {
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::const_iterator > const_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::const_iterator> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::const_reverse_iterator> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename dummy_terminated_iterator_type_resolver<T>::const_reverse_iterator> reverse_terminated_iterator_type;
        };
        template <typename T>
        struct terminated_iterator_type_resolver<range<T>> // range object
        {
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type> const_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type> reverse_terminated_iterator_type;
        };
        template <typename T> //dummy
        struct terminated_iterator_type_resolver<const range<T>> //const range object
        {
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type, const typename iterator_traits_resolver<T>::reference> const_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type, const typename iterator_traits_resolver<T>::reference> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type, const typename iterator_traits_resolver<T>::reference> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<typename range<T>::iterator_type, const typename iterator_traits_resolver<T>::reference> reverse_terminated_iterator_type;
        };
        template <typename T>
        struct terminated_iterator_type_resolver<T*> // char_type* null-terminated strings
        {
            typedef utility::null_terminated_string_iterator<const T> const_terminated_iterator_type;
            typedef utility::null_terminated_string_iterator<T> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<const T*>> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<T*>> reverse_terminated_iterator_type;
        };
        template <typename T>
        struct terminated_iterator_type_resolver<const T*> // const char_type* null-terminated strings
        {
            typedef utility::null_terminated_string_iterator<const T> const_terminated_iterator_type;
            typedef utility::null_terminated_string_iterator<const T> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<const T*>> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<const T*>> reverse_terminated_iterator_type;
        };
        template <typename T, size_t N>
        struct terminated_iterator_type_resolver<const T[N]> // const char_type string literals
        {
            typedef utility::null_terminated_string_iterator<const T> const_terminated_iterator_type;
            typedef utility::null_terminated_string_iterator<const T> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<const T*>> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<const T*>> reverse_terminated_iterator_type;
        };
        template <typename T, size_t N>
        struct terminated_iterator_type_resolver<T[N]> // char_type array of null-terminated strings
        {
            typedef utility::null_terminated_string_iterator<const T> const_terminated_iterator_type;
            typedef utility::null_terminated_string_iterator<T> terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<const T*>> const_reverse_terminated_iterator_type;
            typedef utility::endpos_terminated_string_iterator<std::reverse_iterator<T*>> reverse_terminated_iterator_type;
        };

        //-------------------------------------------------------------------------
        // make_const_terminated_iterator_forward
        //-------------------------------------------------------------------------
        // The following functions are needed to be able to handle different string types the same way.

        // make_const_terminated_iterator_forward factory method for range objects, includes some type deduction explained below.
        template <typename char_pointer_or_iterator_type>
        utility::endpos_terminated_string_iterator<char_pointer_or_iterator_type, const typename iterator_traits_resolver<char_pointer_or_iterator_type>::reference> make_const_terminated_iterator_forward(const range<char_pointer_or_iterator_type>& text)
        {
            utility::endpos_terminated_string_iterator< // We use the endpos_terminated_string_iterator here, but we have to make the reference operator const
                char_pointer_or_iterator_type, // We deduced the char_pointer_or_iterator_type from the input range
                const typename iterator_traits_resolver<char_pointer_or_iterator_type>::reference> // No we deduce the type returned by the reference operator and make it const.
                result(text.begin(), text.end()); // This is the range now converted into a const endpos_terminated_string_iterator.
            return result;
        }

        // make_const_terminated_iterator_forward factory method for string objects
        template <typename text_type>
        utility::endpos_terminated_string_iterator<typename text_type::const_iterator> make_const_terminated_iterator_forward(const text_type& text)
        {
            utility::endpos_terminated_string_iterator<typename text_type::const_iterator> result(text.begin(), text.end());
            return result;
        }

        // make_const_terminated_iterator_forward factory method for null-terminated strings
        template <typename char_type>
        inline utility::null_terminated_string_iterator<const char_type> make_const_terminated_iterator_forward(const char_type* p_text)
        {
            utility::null_terminated_string_iterator<const char_type> result(p_text);
            return result;
        }

        // make_const_terminated_iterator_forward factory method for null-terminated string buffer
        template <typename char_type>
        inline utility::null_terminated_string_iterator<const char_type> make_const_terminated_iterator_forward(char_type* p_text)
        {
            utility::null_terminated_string_iterator<const char_type> result = make_const_terminated_iterator_forward(static_cast<const char_type*>(p_text));
            return result;
        }

        //-------------------------------------------------------------------------
        // make_const_terminated_iterator_reverse
        //-------------------------------------------------------------------------
        // The following functions are needed to be able to handle different string types the same way.

        // make_const_terminated_iterator_reverse factory method for range objects, includes some type deduction explained below.
        template <typename char_pointer_or_iterator_type>
        utility::endpos_terminated_string_iterator<std::reverse_iterator<char_pointer_or_iterator_type>, const typename iterator_traits_resolver<char_pointer_or_iterator_type>::reference> make_const_terminated_iterator_reverse(const range<char_pointer_or_iterator_type>& text)
        {
            utility::endpos_terminated_string_iterator< // We use the endpos_terminated_string_iterator here, but we have to make the reference operator const
                std::reverse_iterator<char_pointer_or_iterator_type>, // We deduced the char_pointer_or_iterator_type from the input range
                const typename iterator_traits_resolver<char_pointer_or_iterator_type>::reference> // No we deduce the type returned by the reference operator and make it const.
                result(std::reverse_iterator<char_pointer_or_iterator_type>(text.end()), std::reverse_iterator<char_pointer_or_iterator_type>(text.begin())); // This is the range now converted into a const endpos_terminated_string_iterator.
            return result;
        }

        // make_const_terminated_iterator_reverse factory method for string objects
        template <typename text_type>
        utility::endpos_terminated_string_iterator<typename text_type::const_reverse_iterator> make_const_terminated_iterator_reverse(const text_type& text)
        {
            utility::endpos_terminated_string_iterator<typename text_type::const_reverse_iterator> result(text.crbegin(), text.crend());
            return result;
        }

        // make_const_terminated_iterator_reverse factory method for null-terminated strings
        template <typename char_type>
        inline utility::endpos_terminated_string_iterator<std::reverse_iterator<const char_type*>> make_const_terminated_iterator_reverse(const char_type* p_text)
        {
            utility::endpos_terminated_string_iterator<std::reverse_iterator<const char_type*>> result(
                // Note std::reverse_iterator decrements the passed values by one.
                std::reverse_iterator<const char_type*>(p_text + string_length(p_text)), // Reverse start is the last character of the string before the terminating null.
                std::reverse_iterator<const char_type*>(p_text) // Reverse end is one character before the text start
            );
            return result;
        }

        // make_const_terminated_iterator_reverse factory method for null-terminated strings
        template <typename char_type>
        inline utility::endpos_terminated_string_iterator<std::reverse_iterator<const char_type*>> make_const_terminated_iterator_reverse(char_type* p_text)
        {
            utility::endpos_terminated_string_iterator<std::reverse_iterator<const char_type*>> result = make_const_terminated_iterator_reverse(static_cast<const char_type*>(p_text));
            return result;
        }

        //-------------------------------------------------------------------------
        // make_terminated_iterator_forward
        //-------------------------------------------------------------------------
        // The following functions are needed to be able to handle different string types the same way.

        // make_terminated_iterator_forward factory method for range objects
        template <typename char_pointer_or_iterator_type>
        utility::endpos_terminated_string_iterator<char_pointer_or_iterator_type> make_terminated_iterator_forward(const range<char_pointer_or_iterator_type>& text)
        {
            utility::endpos_terminated_string_iterator<char_pointer_or_iterator_type>result(text.begin(), text.end());
            return result;
        }

        // make_terminated_iterator_forward factory method for string objects
        template <typename text_type>
        typename implementation::terminated_iterator_type_resolver<text_type>::terminated_iterator_type make_terminated_iterator_forward(text_type& text)
        {
            typename implementation::terminated_iterator_type_resolver<text_type>::terminated_iterator_type result(text.begin(), text.end());
            return result;
        }

        // make_terminated_iterator_forward factory method for null-terminated string buffer
        template <typename char_type>
        inline utility::null_terminated_string_iterator<char_type> make_terminated_iterator_forward(char_type* p_text)
        {
            utility::null_terminated_string_iterator<char_type> result(p_text);
            return result;
        }

        //-------------------------------------------------------------------------
        // make_terminated_iterator_reverse
        //-------------------------------------------------------------------------
        // The following functions are needed to be able to handle different string types the same way.

        // make_terminated_iterator_reverse factory method for range objects, includes some type deduction explained below.
        template <typename char_pointer_or_iterator_type>
        utility::endpos_terminated_string_iterator<std::reverse_iterator<char_pointer_or_iterator_type>> make_terminated_iterator_reverse(const range<char_pointer_or_iterator_type>& text)
        {
            utility::endpos_terminated_string_iterator<std::reverse_iterator<char_pointer_or_iterator_type>>  result(
                std::reverse_iterator<char_pointer_or_iterator_type>(text.end()),
                std::reverse_iterator<char_pointer_or_iterator_type>(text.begin())
            );
            return result;
        }

        // make_terminated_iterator_reverse factory method for string objects
        template <typename text_type>
        typename implementation::terminated_iterator_type_resolver<text_type>::terminated_iterator_type make_terminated_iterator_reverse(text_type& text)
        {
            typename implementation::terminated_iterator_type_resolver<text_type>::terminated_iterator_type result(text.rbegin(), text.rend());
            return result;
        }

        // make_terminated_iterator_reverse factory method for null-terminated strings
        template <typename char_type>
        inline utility::endpos_terminated_string_iterator<std::reverse_iterator<char_type*>> make_terminated_iterator_reverse(char_type* p_text)
        {
            utility::endpos_terminated_string_iterator<std::reverse_iterator<char_type*>> result(
                // Note std::reverse_iterator decrements the passed values by one.
                std::reverse_iterator<char_type*>(p_text + string_length(p_text)), // Reverse start is the last character of the string before the terminating null.
                std::reverse_iterator<char_type*>(p_text) // Reverse end is one character before the text start
            );
            return result;
        }

        //-------------------------------------------------------------------------
        // replace
        //-------------------------------------------------------------------------

        // replace copy for string objects
        template <typename text_type_a, typename terminated_iterator_type_a, typename terminated_iterator_type_b, typename terminated_iterator_type_c, typename equals_comparer_type>
        inline void replace_all_copy_forward(
            text_type_a& result, // This object receives the result of the operation. The result is appended.
            terminated_iterator_type_a itt_text,
            const terminated_iterator_type_b& itt_text_to_be_replaced,
            const terminated_iterator_type_c& itt_text_to_replace_with,
            const equals_comparer_type& compare
        )
        {
            // Loop over the text
            while (!itt_text.is_end_position())
            {
                // For every position in the text check if the itt_text_to_be_replaced matches
                terminated_iterator_type_b itt_text_to_be_replaced_inner_loop = itt_text_to_be_replaced;
                terminated_iterator_type_a itt_text_inner_loop = itt_text;
                for (; !itt_text_inner_loop.is_end_position() && !itt_text_to_be_replaced_inner_loop.is_end_position(); ++itt_text_to_be_replaced_inner_loop, ++itt_text_inner_loop)
                {
                    if (!compare(*itt_text_inner_loop, *itt_text_to_be_replaced_inner_loop))
                    {
                        break;
                    }
                }
                // to optimize: append ranges instead of character-wise copy to possibly optimize string memory allocation
                if (itt_text_to_be_replaced_inner_loop.is_end_position()) //itt_text_to_be_replaced matches
                {
                    // Append the text_to_replace_with
                    for (auto itt = itt_text_to_replace_with; !itt.is_end_position(); ++itt)
                    {
                        result += static_cast<typename text_type_a::value_type>(*itt);
                    }
                    itt_text = itt_text_inner_loop; // Advance the iterator of the outer loop
                }
                else // itt_text_to_be_replaced does not match
                {
                    result += *itt_text; // Append the character
                    ++itt_text; // Advance the iterator of the outer loop
                }
            }
        }

        // replace in-place for string objects
        template <typename text_type_a, typename terminated_iterator_type_a, typename terminated_iterator_type_b, typename equals_comparer_type>
        inline void replace_all_in_place_forward(
            text_type_a& text_to_modify_in_place,
            const terminated_iterator_type_a& itt_text_to_be_replaced,
            const terminated_iterator_type_b& itt_text_to_replace_with,
            const equals_comparer_type& compare
        )
        {
            // The text to replace must not be empty because this would lead to inserting text_to_replace_with infinitely
            assert(!itt_text_to_be_replaced.is_end_position());

            auto itt_text = make_const_terminated_iterator_forward(text_to_modify_in_place); // Get a terminated iterator to be able to call find_forward_optimized
            auto first_range_to_be_replaced = find_forward_optimized(itt_text, itt_text_to_be_replaced, compare); // Check if we have anything to do
            if (!first_range_to_be_replaced.begin().is_end_position()) // Found something to replace, now we have to take action
            {
                // to optimize: reduce copying when text_to_be_replaced.size() <= itt_text_to_replace_with.size()
                // this is postponed until the Unicode support feature is added
                text_type_a copied_tail(first_range_to_be_replaced.end().get_position(), itt_text.get_end());

                // Clip the string to the part before the first range found.
                text_to_modify_in_place.resize(first_range_to_be_replaced.begin().get_position() - itt_text.get_position());

                // Append the text_to_replace_with
                for (auto itt = itt_text_to_replace_with; !itt.is_end_position(); ++itt)
                {
                    text_to_modify_in_place += static_cast<typename text_type_a::value_type>(*itt);
                }

                auto itt_text_tail = make_const_terminated_iterator_forward(copied_tail); // Get a terminated iterator to be able to call replace_all_copy_forward

                // Since we do not check yet if text_to_be_replaced.size() <= itt_text_to_replace_with.size() we cannot know if the string grows or shrinks while replacing
                // so that we use the replace_all_copy_forward for now
                replace_all_copy_forward(
                    text_to_modify_in_place,
                    itt_text_tail,
                    itt_text_to_be_replaced,
                    itt_text_to_replace_with,
                    compare
                );
            }
        }

        //-------------------------------------------------------------------------
        // trim
        //-------------------------------------------------------------------------

        // Clip until a character is reached that does not match is_something. is_something is typically is_space
        template <typename terminated_iterator_type, typename predicate_type>
        inline void trim_iterator(terminated_iterator_type& itt, predicate_type is_something)
        {
            for (; !itt.is_end_position() && is_something(*itt); ++itt)
            {
                //nothing to do here; just advance the iterator
            }
        }

        // Trim range or string creating a copy
        template <typename text_type, typename predicate_type>
        text_type trim_copy(const text_type& text, predicate_type is_something, bool trim_start_enable, bool trim_end_enable)
        {
            text_type result;
            auto itt_text_start = make_const_terminated_iterator_forward(text); // Get a terminated iterator for start.
            if (trim_start_enable) // We assume that the compiler optimizes the unneeded code away, if this is never used in a trim variant.
            {
                trim_iterator(itt_text_start, is_something); // Trim start.
            }
            if (!itt_text_start.is_end_position()) // If not result will already be empty
            {
                auto itt_text_end = make_const_terminated_iterator_reverse(text); // Get a terminated iterator for end.
                if (trim_end_enable) // We assume that the compiler optimizes the unneeded code away, if this is never used in a trim variant.
                {
                    trim_iterator(itt_text_end, is_something); //Trim end.
                }
                // Create a copy from front to end using the text objects iterators
                result = text_type(itt_text_start.get_position(), itt_text_end.get_position().base());
            }
            else
            {
                // This is needed for range objects to be updated with the proper iterators.
                result = text_type(itt_text_start.get_position(), itt_text_start.get_end());
            }
            return result;
        }

        // Trim string in-place
        template <typename text_type, typename predicate_type>
        text_type& trim_in_place(text_type& text, predicate_type is_something, bool trim_start_enable, bool trim_end_enable)
        {
            auto itt_text_start = make_const_terminated_iterator_forward(text); // Get a terminated iterator for start.
            if (trim_start_enable) // We assume that the compiler optimizes the unneeded code away, if this is never used in a trim variant.
            {
                trim_iterator(itt_text_start, is_something); // Trim start.
            }
            if (!itt_text_start.is_end_position()) // If not result will already be empty.
            {
                auto itt_text_end = make_const_terminated_iterator_reverse(text); // Get a terminated iterator for end.
                if (trim_end_enable) // We assume that the compiler optimizes the unneeded code away, if this is never used in a trim variant.
                {
                    trim_iterator(itt_text_end, is_something); // Trim end.
                }
                auto it_begin = itt_text_start.get_position(); // The text objects begin iterator.
                auto it_end = itt_text_end.get_position().base(); // The text objects end iterator.
                if (it_begin != it_end) // If not result will already be empty.
                {
                    bool clip = (it_end != text.end()); // If the end changes then clip the text later.
                    if (it_begin != text.begin()) //If the start changes
                    {
                        // Copy the rest of the string to the front
                        auto it_target = text.begin();
                        for (auto it = it_begin; it != it_end; ++it, ++it_target)
                        {
                            *it_target = *it;
                        }
                        clip = true; // If the start changes then clip text later.
                    }
                    if (clip)
                    {
                        // Remove the trimmed characters from the string by making it smaller.
                        text.resize(it_end - it_begin);
                    }
                }
                else
                {
                    text.clear(); // trim_in_place results in an empty string.
                }
            }
            else
            {
                text.clear(); // trim_in_place results in an empty string.
            }
            return text;
        }

        // Trim range in-place, forward iterators only
        template <typename iterator_type, typename predicate_type>
        range<iterator_type>& trim_in_place(range<iterator_type>& range, predicate_type is_something, bool trim_start_enable, bool trim_end_enable)
        {
            // For the range we just have to adjust the iterators of it. This is
            // basically the same operation as trim_copy
            range = implementation::trim_copy(range, is_something, trim_start_enable, trim_end_enable);
            return range;
        }

        // Trim buffer in-place
        template <typename char_type, typename predicate_type>
        char_type* trim_in_place(char_type* p_text, predicate_type is_something, bool trim_start_enable, bool trim_end_enable)
        {
            auto itt_text_start = make_const_terminated_iterator_forward(p_text); // Get a terminated iterator for start.
            if (trim_start_enable) // We assume that the compiler optimizes the unneeded code away, if this is never used in a trim variant.
            {
                trim_iterator(itt_text_start, is_something); // Trim start.
            }
            if (!itt_text_start.is_end_position()) // If not result will already be empty.
            {
                auto itt_text_end = make_terminated_iterator_reverse(p_text); // Get a terminated iterator for end.
                if (trim_end_enable) // We assume that the compiler optimizes the unneeded code away, if this is never used in a trim variant.
                {
                    trim_iterator(itt_text_end, is_something); // Trim end.
                }
                auto it_begin = itt_text_start.get_position(); // The begin iterator.
                auto it_end = itt_text_end.get_position().base(); // The end iterator.
                if (it_begin != it_end) // If not result will already be empty.
                {
                    bool clip = *it_end != 0; // If the end changes then clip the text later. It is 0 if we are still at end position.
                    if (it_begin != p_text) //If the start changes
                    {
                        // Copy the rest of the string to the front
                        // The blocks can overlap so that we must use memmove
                        memmove(p_text, it_begin, (it_end - it_begin) * sizeof(char_type));
                        clip = true; // If the start changes then clip the text later.
                    }
                    if (clip)
                    {
                        // Remove the trimmed characters from the string by setting the null-termination.
                        *(p_text + (it_end - it_begin)) = 0;
                    }
                }
                else
                {
                    *p_text = 0; // trim_in_place results in an empty string.
                }
            }
            else
            {
                *p_text = 0; // trim_in_place results in an empty string.
            }
            return p_text;
        }

        //-------------------------------------------------------------------------
        // case_convert
        //-------------------------------------------------------------------------

        // Helper for adding different types of objects to a string object
        template <typename text_type, typename char_type, class = typename std::enable_if<std::is_integral<char_type>::value>::type>
        inline void  text_appender(text_type& text, char_type c)
        {
            text.push_back(c);
        }

        // Helper for adding different types of objects to a string object
        template <typename text_type_a, typename text_type_b, class = typename std::enable_if<!std::is_integral<text_type_b>::value>::type>
        inline void  text_appender(text_type_a& text, const text_type_b& text_to_append)
        {
            auto itt_text_to_append = make_const_terminated_iterator_forward(text_to_append); // Get a terminated iterator.
            for (; !itt_text_to_append.is_end_position(); ++itt_text_to_append)
            {
                text.push_back(*itt_text_to_append);
            }
        }

        // string object copy
        template <typename text_type, typename char_converter_type>
        inline text_type character_convert_copy(const text_type& text, const char_converter_type& converter)
        {
            text_type result;
            result.reserve(text.size());
            auto itt_text = make_const_terminated_iterator_forward(text); // Get a terminated iterator.
            for (; !itt_text.is_end_position(); ++itt_text)
            {
                text_appender(result, converter(*itt_text)); //the converter could return multiple characters as string or range
            }
            return result;
        }

        // text object in-place
        template <typename text_type, typename char_converter_type>
        inline void character_convert_in_place(text_type& text, const char_converter_type& converter)
        {
            auto itt_text = make_terminated_iterator_forward(text); // Get a terminated iterator.
            for (; !itt_text.is_end_position(); ++itt_text)
            {
                *itt_text = converter(*itt_text);
            }
        }

        // buffer in-place
        template <typename char_type, typename char_converter_type>
        inline void character_convert_in_place(char_type* text, const char_converter_type& converter)
        {
            auto itt_text = make_terminated_iterator_forward(text); // Get a terminated iterator.
            for (; !itt_text.is_end_position(); ++itt_text)
            {
                *itt_text = converter(*itt_text);
            }
        }

    } //implementation namespace

    //-------------------------------------------------------------------------
    // utility
    //-------------------------------------------------------------------------
    /// Provides trivial string handling helper classes.
    /// These classes are needed to be able to handle different string types the same way
    /// and are optimized away in release builds.
    /// Note that namespace utility appears multiple times in this header file.
    /// This is needed to get the declarations in the right order.
    namespace utility
    {
        //-------------------------------------------------------------------------
        // is_any_of
        //-------------------------------------------------------------------------

        /**
            \brief Checks whether a character is contained in a list of characters.
        */
        template <typename text_type>
        class is_any_of
        {
            typedef typename implementation::terminated_iterator_type_resolver<text_type>::const_terminated_iterator_type terminated_iterator_type_text;
        public:
            /**
                \brief Constructs an is_any_of predicate.
            */
            is_any_of()
            {
            }

            /**
                \brief Constructs an is_any_of predicate.
                \param[in] p_text    A text containing a list of characters.
                                     The is_any_of object only stores a reference to \c text.
                                     \c text must not be destroyed or changed while using the is_any_of object.
            */
            template <typename char_type>
            is_any_of(const char_type* p_text)
                : itt(implementation::make_const_terminated_iterator_forward(p_text))
            {
            }

            /**
                \brief Constructs an is_any_of predicate.
                \param[in] text    A text containing a list of characters.
                                   The is_any_of object only stores a reference to \c text.
                                   \c text must not be destroyed or changed while using the is_any_of object.
            */
            is_any_of(text_type& text)
                : itt(implementation::make_const_terminated_iterator_forward(text))
            {
            }

            /**
                \brief Checks whether a character is contained in the list of characters passed when constructing is_any_of.
                \param[in] value    The character to check.
                \return Returns true if the character is contained in the list of characters passed when constructing is_any_of.
            */
            template <typename char_type>
            bool operator()(char_type value) const
            {
                bool result = false;
                for (terminated_iterator_type_text local_itt = itt; !local_itt.is_end_position(); ++local_itt)
                {
                    // The is_any_of object only stores a reference to \c text.
                    // \c text must not be destroyed or changed while using the is_any_of object.
                    if (*local_itt == value)
                    {
                        result = true;
                        break;
                    }
                }
                return result;
            }
        private:
            terminated_iterator_type_text itt;
        };

    } // utility namespace

    //-------------------------------------------------------------------------
    // copy
    //-------------------------------------------------------------------------
    /**
    \brief Copies a string and returns the copy.
    \param[in] text_to_copy    A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must fit the target string, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string copied = cppstringx::copy<std::string>("Hello World");
    \endcode
    \returns Returns a copy of the string.
    */
    template <typename text_type_a, typename text_type_b>
    inline text_type_a copy(const text_type_b& text_to_copy)
    {
        auto itt = implementation::make_const_terminated_iterator_forward(text_to_copy); // Convert the input to terminated iterator.
        text_type_a result;
        for (; !itt.is_end_position(); ++itt) // Copy the source string.
        {
            result.push_back(static_cast<typename text_type_a::value_type>(*itt)); // Force a code unit type conversion. See character encoding infos.
        }
        return result;
    }

    /**
    \brief Copies a string to another string.
    \param[in] target          A string object, e.g. std::string.
    \param[in] text_to_copy    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] clear_target    Selects whether \c target is cleared before copying.
    \note The character encoding of the passed strings must fit the target string, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string target;
        cppstringx::copy(target, "Hello World");
        cppstringx::copy(target, "!", false);
    \endcode
    \returns Returns modified target string.
    */
    template <typename text_type_a, typename text_type_b>
    inline text_type_a& copy(text_type_a& target, const text_type_b& text_to_copy, bool clear_target = true)
    {
        auto itt = implementation::make_const_terminated_iterator_forward(text_to_copy);  // Convert the input to terminated iterator.
        if (clear_target)
        {
            target.clear(); // Clear the target string.
        }
        for (; !itt.is_end_position(); ++itt) // Copy the source string.
        {
            target.push_back(static_cast<typename text_type_a::value_type>(*itt)); // Force a code unit type conversion. See character encoding infos.
        }
        return target;
    }

    //-------------------------------------------------------------------------
    // equals
    //-------------------------------------------------------------------------

    /**
    \brief Checks whether a string equals another string. This is the most universal overload of equals. Typically you can use a variant without comparer.
    \param[in] text_lhs    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_rhs    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] comparer    Compares two character values for equality.
                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::equals(text, "hello world", cppstringx::utility::equals_comparer_ignoring_case()))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text_lhs equals string \c text_rhs.
    */
    template <typename text_type_a, typename text_type_b, typename equals_comparer_type>
    inline bool equals(const text_type_a& text_lhs, const text_type_b& text_rhs, const equals_comparer_type& comparer)
    {
        bool result = implementation::full_match( //check if the texts match
            implementation::make_const_terminated_iterator_forward(text_lhs), // Convert the input to terminated iterator.
            implementation::make_const_terminated_iterator_forward(text_rhs), // Convert the input to terminated iterator.
            comparer // The equals comparer decides on how the string characters are compared.
        );
        return result;
    }

    /**
    \brief Checks whether a string equals another string.
    \param[in] text_lhs    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_rhs    A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::equals(text, "Hello World"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c equals string \c text_rhs.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool equals(const text_type_a& text_lhs, const text_type_b& text_rhs)
    {
        bool result = equals(text_lhs, text_rhs, utility::equals_comparer());
        return result;
    }

    /**
    \brief Checks whether a string equals another string ignoring character casing.
    \param[in] text_lhs    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_rhs    A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::iequals(text, "hello world"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text_lhs equals string \c text_rhs ignoring character casing.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool iequals(const text_type_a& text_lhs, const text_type_b& text_rhs)
    {
        bool result = equals(text_lhs, text_rhs, utility::equals_comparer_ignoring_case());
        return result;
    }

    //-------------------------------------------------------------------------
    // contains
    //-------------------------------------------------------------------------

    /**
    \brief Checks whether a string contains a certain contained string. This is the most universal overload of contains. Typically you can use a variant without comparer.
    \param[in] text                A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] contained_string    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] comparer    Compares two character values for equality.
                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::contains(text, "wo", cppstringx::utility::equals_comparer_ignoring_case()))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text contains string \c contained_string.
    */
    template <typename text_type_a, typename text_type_b, typename equals_comparer_type>
    inline bool contains(const text_type_a& text, const text_type_b& contained_string, const equals_comparer_type& comparer)
    {
        auto itt_contained_string = implementation::make_const_terminated_iterator_forward(contained_string); // Convert the input to terminated iterators.

        bool result = itt_contained_string.is_end_position() || //if the contained_string is empty it is always contained. This is need if text and contained_string are empty().
            !implementation::find_forward_optimized( //check if the contained_string matches
                implementation::make_const_terminated_iterator_forward(text), // Convert the input to terminated iterators.
                implementation::make_const_terminated_iterator_forward(contained_string),
                comparer // The equals comparer decides on how the string characters are compared. You can use a two parameter lambda expression as comparer.
            ).begin().is_end_position(); // Returns the range where the contained string is found in the string text. 
                                         // If the position is at the end of the string the contained_string has not been found.
        return result;
    }

    /**
    \brief Checks whether a string contains a certain contained string.
    \param[in] text                A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] contained_string    A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::contains(text, "Wo"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c contains string \c contained_string.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool contains(const text_type_a& text, const text_type_b& contained_string)
    {
        bool result = contains(text, contained_string, utility::equals_comparer());
        return result;
    }

    /**
    \brief Checks whether a string contains a certain contained string ignoring character casing.
    \param[in] text                A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] contained_string    A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::icontains(text, "wo"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text contains string \c contained_string ignoring character casing.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool icontains(const text_type_a& text, const text_type_b& contained_string)
    {
        bool result = contains(text, contained_string, utility::equals_comparer_ignoring_case());
        return result;
    }

    //-------------------------------------------------------------------------
    // starts_with
    //-------------------------------------------------------------------------

    /**
    \brief Checks whether a string has a certain prefix. This is the most universal overload of starts_with. Typically you can use a variant without comparer.
    \param[in] text        A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] prefix      A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] comparer    Compares two character values for equality.
                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::starts_with(text, "hello", cppstringx::utility::equals_comparer_ignoring_case()))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text starts with string \c prefix.
    */
    template <typename text_type_a, typename text_type_b, typename equals_comparer_type>
    inline bool starts_with(const text_type_a& text, const text_type_b& prefix, const equals_comparer_type& comparer)
    {
        bool result = implementation::prefix_matches( //check if the prefix matches
            implementation::make_const_terminated_iterator_forward(text), // Convert the input to terminated iterators.
            implementation::make_const_terminated_iterator_forward(prefix), // Convert the input to terminated iterators.
            comparer // The equals comparer decides on how the string characters are compared.
        );
        return result;
    }

    /**
    \brief Checks whether a string has a certain prefix.
    \param[in] text        A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] prefix      A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::starts_with(text, "Hello"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text starts with string \c prefix.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool starts_with(const text_type_a& text, const text_type_b& prefix)
    {
        bool result = starts_with(text, prefix, utility::equals_comparer());
        return result;
    }

    /**
    \brief Checks whether a string has a certain prefix ignoring character casing.
    \param[in] text        A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] prefix      A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::istarts_with(text, "hello"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text starts with string \c prefix ignoring character casing.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool istarts_with(const text_type_a& text, const text_type_b& prefix)
    {
        bool result = starts_with(text, prefix, utility::equals_comparer_ignoring_case());
        return result;
    }

    //-------------------------------------------------------------------------
    // ends_with
    //-------------------------------------------------------------------------

    /**
    \brief Checks whether a string has a certain ending. This is the most universal overload of ends_with. Typically you can use a variant without comparer.
    \param[in] text        A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] ending      A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] comparer    Compares two character values for equality.
                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::ends_with(text, "world", cppstringx::utility::equals_comparer_ignoring_case()))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text ends with string \c ending.
    */
    template <typename text_type_a, typename text_type_b, typename equals_comparer_type>
    inline bool ends_with(const text_type_a& text, const text_type_b& ending, const equals_comparer_type& comparer)
    {
        bool result = implementation::prefix_matches( //check if the prefix matches if the string is processed in reversed order
            implementation::make_const_terminated_iterator_reverse(text), // Convert the input to terminated iterators.
            implementation::make_const_terminated_iterator_reverse(ending),
            comparer // The equals comparer decides on how the string characters are compared.
        );
        return result;
    }

    /**
    \brief Checks whether a string has a certain ending.
    \param[in] text        A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] ending      A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::ends_with(text, "World"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text ends with string \c ending.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool ends_with(const text_type_a& text, const text_type_b& ending)
    {
        bool result = ends_with(text, ending, utility::equals_comparer());
        return result;
    }

    /**
    \brief Checks whether a string has a certain ending ignoring character casing.
    \param[in] text        A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] ending      A string object, e.g. std::string, range object, or a null-terminated string.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        if (cppstringx::iends_with(text, "world"))
        {
            //...
        }
    \endcode
    \returns Returns true if the string \c text ends with string \c ending ignoring character casing.
    */
    template <typename text_type_a, typename text_type_b>
    inline bool iends_with(const text_type_a& text, const text_type_b& ending)
    {
        bool result = ends_with(text, ending, utility::equals_comparer_ignoring_case());
        return result;
    }

    //-------------------------------------------------------------------------
    // replace
    //-------------------------------------------------------------------------

    /**
    \brief Replaces all occurrences of a specified string in a text string with another string returning a modified copy.
    \param[in] text                   A string object.
    \param[in] text_to_be_replaced    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_to_replace_with   A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] comparer    Compares two character values for equality.
                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \pre \c text_to_be_replaced must not be empty.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        std::string modifiedCopy = cppstringx::replace_all_copy(text, "world", "Universe", cppstringx::utility::equals_comparer_ignoring_case());
    \endcode
    \returns Returns a modified copy of \c text.
    */
    template <typename text_type_a, typename text_type_b, typename text_type_c, typename equals_comparer_type>
    inline text_type_a replace_all_copy(const text_type_a& text, const text_type_b& text_to_be_replaced, const text_type_c& text_to_replace_with, const equals_comparer_type& comparer)
    {
        auto itt_text_to_be_replaced = implementation::make_const_terminated_iterator_forward(text_to_be_replaced); // Convert the input to terminated iterator.
        if (itt_text_to_be_replaced.is_end_position())
        {
            throw std::invalid_argument("The replace_all_copy input parameter text_to_be_replaced must not be empty.");
        }
        text_type_a result;
        implementation::replace_all_copy_forward(
            result,
            implementation::make_const_terminated_iterator_forward(text), // Convert the input to terminated iterator.
            itt_text_to_be_replaced,
            implementation::make_const_terminated_iterator_forward(text_to_replace_with), // Convert the input to terminated iterator.
            comparer // The equals comparer decides on how the string characters are compared.
        );
        return result;
    }

    /**
    \brief Replaces all occurrences of a specified string in a text string with another string returning a modified copy.
    \param[in] text                   A string object.
    \param[in] text_to_be_replaced    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_to_replace_with   A string object, e.g. std::string, range object, or a null-terminated string.
    \pre \c text_to_be_replaced must not be empty.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        std::string modifiedCopy = cppstringx::replace_all_copy(text, "World", "Universe");
    \endcode
    \returns Returns a modified copy of \c text.
    */
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    inline text_type_a replace_all_copy(const text_type_a& text, const text_type_b& text_to_be_replaced, const text_type_c& text_to_replace_with)
    {
        text_type_a result = replace_all_copy(text, text_to_be_replaced, text_to_replace_with, utility::equals_comparer());
        return result;
    }

    /**
    \brief Replaces all occurrences of a specified string in a text string with another string returning a modified copy ignoring character casing.
    \param[in] text                   A string object.
    \param[in] text_to_be_replaced    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_to_replace_with   A string object, e.g. std::string, range object, or a null-terminated string.
    \pre \c text_to_be_replaced must not be empty.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        std::string modifiedCopy1 = cppstringx::ireplace_all_copy(text, "world", "Universe");
    \endcode
    \returns Returns a modified copy of \c text.
    */
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    inline text_type_a ireplace_all_copy(const text_type_a& text, const text_type_b& text_to_be_replaced, const text_type_c& text_to_replace_with)
    {
        text_type_a result = replace_all_copy(text, text_to_be_replaced, text_to_replace_with, utility::equals_comparer_ignoring_case());
        return result;
    }

    /**
    \brief Replaces all occurrences of a specified string in a text string with another string returning the modified string.
    \param[in] text                   A string object.
    \param[in] text_to_be_replaced    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_to_replace_with   A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] comparer    Compares two character values for equality.
                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \pre \c text_to_be_replaced must not be empty.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        cppstringx::replace_all_in_place(text, "world", "Universe", cppstringx::utility::equals_comparer_ignoring_case());
    \endcode
    \returns Returns the modified \c text.
    */
    template <typename text_type_a, typename text_type_b, typename text_type_c, typename equals_comparer_type>
    inline text_type_a& replace_all_in_place(text_type_a& text, const text_type_b& text_to_be_replaced, const text_type_c& text_to_replace_with, const equals_comparer_type& comparer)
    {
        auto itt_text_to_be_replaced = implementation::make_const_terminated_iterator_forward(text_to_be_replaced); // Convert the input to terminated iterator.
        if (itt_text_to_be_replaced.is_end_position())
        {
            throw std::invalid_argument("The replace_all_in_place input parameter text_to_be_replaced must not be empty.");
        }
        implementation::replace_all_in_place_forward(
            text,
            itt_text_to_be_replaced,
            implementation::make_const_terminated_iterator_forward(text_to_replace_with), // Convert the input to terminated iterator.
            comparer // The equals comparer decides on how the string characters are compared.
        );
        return text;
    }

    /**
    \brief Replaces all occurrences of a specified string in a text string with another string returning the modified string.
    \param[in] text                   A string object.
    \param[in] text_to_be_replaced    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_to_replace_with   A string object, e.g. std::string, range object, or a null-terminated string.
    \pre \c text_to_be_replaced must not be empty.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        cppstringx::replace_all_in_place(text, "World", "Universe");
    \endcode
    \returns Returns the modified \c text.
    */
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    inline text_type_a& replace_all_in_place(text_type_a& text, const text_type_b& text_to_be_replaced, const text_type_c& text_to_replace_with)
    {
        text_type_a& result = replace_all_in_place(text, text_to_be_replaced, text_to_replace_with, utility::equals_comparer());
        return result;
    }

    /**
    \brief Replaces all occurrences of a specified string in a text string with another string returning the modified string ignoring character casing.
    \param[in] text                   A string object.
    \param[in] text_to_be_replaced    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] text_to_replace_with   A string object, e.g. std::string, range object, or a null-terminated string.
    \pre \c text_to_be_replaced must not be empty.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
        std::string text("Hello World");
        cppstringx::ireplace_all_in_place(text, "world", "Universe");
    \endcode
    \returns Returns the modified \c text.
    */
    template <typename text_type_a, typename text_type_b, typename text_type_c>
    inline text_type_a& ireplace_all_in_place(text_type_a& text, const text_type_b& text_to_be_replaced, const text_type_c& text_to_replace_with)
    {
        text_type_a& result = replace_all_in_place(text, text_to_be_replaced, text_to_replace_with, utility::equals_comparer_ignoring_case());
        return result;
    }

    //-------------------------------------------------------------------------
    // trim
    //-------------------------------------------------------------------------

    /**
    \brief Trim start and end of a string or range object creating a copy.
    \param[in] text         A string object, e.g. std::string, or a range object
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as comparer, e.g. [](char a ) { return a == '-'; }

    Example:
    \code
        std::string text(" Hello World ");
        std::string trimmed = cppstringx::trim_copy(text, cppstringx::utility::is_space());
    \endcode
    \returns The trimmed text as copy.
    */
    template <typename text_type, typename predicate_type>
    inline text_type trim_copy(const text_type& text, const predicate_type& predicate)
    {
        text_type result = implementation::trim_copy(text, predicate, true /*trim_start_enable*/, true /*trim_end_enable*/);
        return result;
    }

    /**
    \brief Trim start and end of a string or range object creating a copy.
    \param[in] text    A string object, e.g. std::string, or a range object (forward iterators only).

    Example:
    \code
        std::string text(" Hello World ");
        std::string trimmed = cppstringx::trim_copy(text);
    \endcode
    \returns The trimmed text as copy.
    */
    template <typename text_type>
    inline text_type trim_copy(const text_type& text)
    {
        text_type result = implementation::trim_copy(text, utility::is_space(), true /*trim_start_enable*/, true /*trim_end_enable*/);
        return result;
    }

    /**
    \brief Trim start and end of a string or range object
    \param[in] text         A string object, e.g. std::string, or a range object
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as comparer, e.g. [](char a ) { return a == '-'; }

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::trim_in_place(text, cppstringx::utility::is_space());
    \endcode
    \returns The trimmed text.
    */
    template <typename text_type, typename predicate_type>
    inline text_type& trim_in_place(text_type& text, const predicate_type& predicate)
    {
        implementation::trim_in_place(text, predicate, true /*trim_start_enable*/, true /*trim_end_enable*/);
        return text;
    }

    /**
    \brief Trim start and end of a string or range object
    \param[in] text    A string object, e.g. std::string, or a range object

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::trim_in_place(text);
    \endcode
    \returns The trimmed text.
    */
    template <typename text_type>
    inline text_type& trim_in_place(text_type& text)
    {
        implementation::trim_in_place(text, utility::is_space(), true /*trim_start_enable*/, true /*trim_end_enable*/);
        return text;
    }

    /**
    \brief Trim start and end of a null-terminated string buffer.
    \param[in] p_text       A null-terminated string buffer.
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
    \returns The trimmed text.
    */
    template <typename char_type, typename predicate_type>
    inline char_type* trim_in_place(char_type* p_text, const predicate_type& predicate)
    {
        implementation::trim_in_place(p_text, predicate, true /*trim_start_enable*/, true /*trim_end_enable*/);
        return p_text;
    }

    /**
    \brief Trim start and end of a null-terminated string buffer.
    \param[in] p_text    A null-terminated string buffer.
    \returns The trimmed text.
    */
    template <typename char_type>
    inline char_type* trim_in_place(char_type* p_text)
    {
        implementation::trim_in_place(p_text, utility::is_space(), true /*trim_start_enable*/, true /*trim_end_enable*/);
        return p_text;
    }

    //-------------------------------------------------------------------------
    // trim_start
    //-------------------------------------------------------------------------

    /**
    \brief Trim the start of a string or range object creating a copy.
    \param[in] text         A string object, e.g. std::string, or a range object
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }

    Example:
    \code
        std::string text(" Hello World ");
        std::string trimmed = cppstringx::trim_start_copy(text, cppstringx::utility::is_space());
    \endcode
    \returns The trimmed text as copy.
    */
    template <typename text_type, typename predicate_type>
    inline text_type trim_start_copy(const text_type& text, const predicate_type& predicate)
    {
        text_type result = implementation::trim_copy(text, predicate, true /*trim_start_enable*/, false /*trim_end_enable*/);
        return result;
    }

    /**
    \brief Trim the start and end of a string or range object creating a copy.
    \param[in] text    A string object, e.g. std::string, or a range object (forward iterators only).

    Example:
    \code
        std::string text(" Hello World ");
        std::string trimmed = cppstringx::trim_start_copy(text);
    \endcode
    \returns The trimmed text as copy.
    */
    template <typename text_type>
    inline text_type trim_start_copy(const text_type& text)
    {
        text_type result = implementation::trim_copy(text, utility::is_space(), true /*trim_start_enable*/, false /*trim_end_enable*/);
        return result;
    }

    /**
    \brief Trim the start of a string or range object
    \param[in] text         A string object, e.g. std::string, or a range object
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::trim_start_in_place(text, cppstringx::utility::is_space());
    \endcode
    \returns The trimmed text.
    */
    template <typename text_type, typename predicate_type>
    inline text_type& trim_start_in_place(text_type& text, const predicate_type& predicate)
    {
        implementation::trim_in_place(text, predicate, true /*trim_start_enable*/, false /*trim_end_enable*/);
        return text;
    }

    /**
    \brief Trim the start of a string or range object
    \param[in] text    A string object, e.g. std::string, or a range object

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::trim_start_in_place(text);
    \endcode
    \returns The trimmed text.
    */
    template <typename text_type>
    inline text_type& trim_start_in_place(text_type& text)
    {
        implementation::trim_in_place(text, utility::is_space(), true /*trim_start_enable*/, false /*trim_end_enable*/);
        return text;
    }

    /**
    \brief Trim the start of a null-terminated string buffer.
    \param[in] p_text       A null-terminated string buffer.
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
    \returns The trimmed text.
    */
    template <typename char_type, typename predicate_type>
    inline char_type* trim_start_in_place(char_type* p_text, const predicate_type& predicate)
    {
        implementation::trim_in_place(p_text, predicate, true /*trim_start_enable*/, false /*trim_end_enable*/);
        return p_text;
    }

    /**
    \brief Trim the start of a null-terminated string buffer.
    \param[in] p_text    A null-terminated string buffer.
    \returns The trimmed text.
    */
    template <typename char_type>
    inline char_type* trim_start_in_place(char_type* p_text)
    {
        implementation::trim_in_place(p_text, utility::is_space(), true /*trim_start_enable*/, false /*trim_end_enable*/);
        return p_text;
    }

    //-------------------------------------------------------------------------
    // trim_end
    //-------------------------------------------------------------------------

    /**
    \brief Trim the end of a string or range object creating a copy.
    \param[in] text         A string object, e.g. std::string, or a range object
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }

    Example:
    \code
        std::string text(" Hello World ");
        std::string trimmed = cppstringx::trim_end_copy(text, cppstringx::utility::is_space());
    \endcode
    \returns The trimmed text as copy.
    */
    template <typename text_type, typename predicate_type>
    inline text_type trim_end_copy(const text_type& text, const predicate_type& predicate)
    {
        text_type result = implementation::trim_copy(text, predicate, false /*trim_start_enable*/, true /*trim_end_enable*/);
        return result;
    }

    /**
    \brief Trim the end of a string or range object creating a copy.
    \param[in] text    A string object, e.g. std::string, or a range object (forward iterators only).

    Example:
    \code
        std::string text(" Hello World ");
        std::string trimmed = cppstringx::trim_end_copy(text);
    \endcode
    \returns The trimmed text as copy.
    */
    template <typename text_type>
    inline text_type trim_end_copy(const text_type& text)
    {
        text_type result = implementation::trim_copy(text, utility::is_space(), false /*trim_start_enable*/, true /*trim_end_enable*/);
        return result;
    }

    /**
    \brief Trim the end of a string or range object
    \param[in] text         A string object, e.g. std::string, or a range object
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::trim_end_in_place(text, cppstringx::utility::is_space());
    \endcode
    \returns The trimmed text.
    */
    template <typename text_type, typename predicate_type>
    inline text_type& trim_end_in_place(text_type& text, const predicate_type& predicate)
    {
        implementation::trim_in_place(text, predicate, false /*trim_start_enable*/, true /*trim_end_enable*/);
        return text;
    }

    /**
    \brief Trim the end of a string or range object
    \param[in] text    A string object, e.g. std::string, or a range object

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::trim_end_in_place(text);
    \endcode
    \returns The trimmed text.
    */
    template <typename text_type>
    inline text_type& trim_end_in_place(text_type& text)
    {
        implementation::trim_in_place(text, utility::is_space(), false /*trim_start_enable*/, true /*trim_end_enable*/);
        return text;
    }

    /**
    \brief Trim the end of a null-terminated string buffer.
    \param[in] p_text       A null-terminated string buffer.
    \param[in] predicate    Is used to check whether a character is to be trimmed.
                            The predicate classes are used to be able to trim different types of characters.
                            The is_space predicate can be passed here provided with a different locale if this is needed.
                            Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
    \returns The trimmed text.
    */
    template <typename char_type, typename predicate_type>
    inline char_type* trim_end_in_place(char_type* p_text, const predicate_type& predicate)
    {
        implementation::trim_in_place(p_text, predicate, false /*trim_start_enable*/, true /*trim_end_enable*/);
        return p_text;
    }

    /**
    \brief Trim the end of a null-terminated string buffer.
    \param[in] p_text    A null-terminated string buffer.
    \returns The trimmed text.
    */
    template <typename char_type>
    inline char_type* trim_end_in_place(char_type* p_text)
    {
        implementation::trim_in_place(p_text, utility::is_space(), false /*trim_start_enable*/, true /*trim_end_enable*/);
        return p_text;
    }

    //-------------------------------------------------------------------------
    // case_convert
    //-------------------------------------------------------------------------

    /**
    \brief Converts characters and returns a modified copy.
    \param[in] text              A string object, e.g. std::string.
    \param[in] char_converter    Used to convert characters
                                 The converter classes are used to be able to apply different modes of
                                 conversion or handling different character encodings.
                                 You can use e.g., utility::to_lower_case_converter or utility::to_upper_case_converter
                                 Optionally you can use a lambda expression as converter, e.g. [](char a) { if (a == 'A') return 'a'; return a; }
    \returns Returns the modified string as copy.
    */
    template <typename text_type, typename char_converter_type>
    inline text_type character_convert_copy(const text_type& text, const char_converter_type& char_converter)
    {
        text_type result = implementation::character_convert_copy(text, char_converter);
        return result;
    }

    /**
    \brief Converts characters in-place.
    \param[in] text              A string object, e.g. std::string, or a range object
    \param[in] char_converter    Used to convert characters to the lower case version.
                                 The converter classes are used to be able to apply different modes of
                                 conversion or handling different character encodings.
                                 You can use e.g., utility::to_lower_case_converter or utility::to_upper_case_converter
                                 Optionally you can use a lambda expression as converter, e.g. [](char a) { if (a == 'A') return 'a'; return a; }
    \returns Returns the modified string.
    */
    template <typename text_type, typename char_converter_type>
    inline text_type& character_convert_in_place(text_type& text, const char_converter_type& char_converter)
    {
        implementation::character_convert_in_place(text, char_converter);
        return text;
    }

    /**
    \brief Converts characters in-place for null-terminated buffers
    \param[in] p_text            A null-terminated string buffer.
    \param[in] char_converter    Used to convert characters to the lower case version.
                                 The converter classes are used to be able to apply different modes of
                                 conversion or handling different character encodings.
                                 You can use e.g., utility::to_lower_case_converter or utility::to_upper_case_converter
                                 Optionally you can use a lambda expression as converter, e.g. [](char a) { if (a == 'A') return 'a'; return a; }
    \returns Returns the modified string.
    */
    template <typename char_type, typename char_converter_type>
    inline char_type* character_convert_in_place(char_type* p_text, const char_converter_type& char_converter)
    {
        implementation::character_convert_in_place(p_text, char_converter);
        return p_text;
    }

    //-------------------------------------------------------------------------
    // to_lower
    //-------------------------------------------------------------------------

    /**
    \brief Converts characters to lower case and returns the copy.
    \param[in] text    A string object, e.g. std::string.

    Example:
    \code
        std::string text(" Hello World ");
        std::string lower = cppstringx::to_lower_copy(text);
    \endcode
    \returns Returns the lower case string copy.
    */
    template <typename text_type>
    inline text_type to_lower_copy(const text_type& text)
    {
        text_type result = character_convert_copy(text, utility::to_lower_case_converter());
        return result;
    }

    /**
    \brief Converts characters to lower case in-place.
    \param[in] text    A string object, e.g. std::string, or a range object.

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::to_lower_in_place(text);
    \endcode
    \returns Returns the to lower case converted string or range object.
    */
    template <typename text_type>
    inline text_type& to_lower_in_place(text_type& text)
    {
        character_convert_in_place(text, utility::to_lower_case_converter());
        return text;
    }

    /**
    \brief Converts characters to lower case in-place for a string buffer.
    \param[in] p_text    A null-terminated string buffer.
    \returns Returns the to lower case converted string.
    */
    template <typename char_type>
    inline char_type* to_lower_in_place(char_type* p_text)
    {
        character_convert_in_place(p_text, utility::to_lower_case_converter());
        return p_text;
    }

    //-------------------------------------------------------------------------
    // to_upper
    //-------------------------------------------------------------------------

    /**
    \brief Converts characters to upper case and returns the copy.
    \param[in] text    A string object, e.g. std::string.

    Example:
    \code
        std::string text(" Hello World ");
        std::string upper = cppstringx::to_upper_copy(text);
    \endcode
    \returns Returns the upper case string copy.
    */
    template <typename text_type>
    inline text_type to_upper_copy(const text_type& text)
    {
        text_type result = character_convert_copy(text, utility::to_upper_case_converter());
        return result;
    }

    /**
    \brief Converts characters to upper case in-place.
    \param[in] text    A string object, e.g. std::string, or a range object.

    Example:
    \code
        std::string text(" Hello World ");
        cppstringx::to_upper_in_place(text);
    \endcode
    \returns Returns the to upper case converted string or range object.
    */
    template <typename text_type>
    inline text_type& to_upper_in_place(text_type& text)
    {
        character_convert_in_place(text, utility::to_upper_case_converter());
        return text;
    }

    /**
    \brief Converts characters to upper case in-place for a string buffer.
    \param[in] p_text    A null-terminated string buffer.
    \returns Returns the to upper case converted string.
    */
    template <typename char_type>
    inline char_type* to_upper_in_place(char_type* p_text)
    {
        character_convert_in_place(p_text, utility::to_upper_case_converter());
        return p_text;
    }

    //-------------------------------------------------------------------------
    // split
    //-------------------------------------------------------------------------

    /**
        \brief Used to select different modes when splitting a string.
    */
    enum class split_mode
    {
        all = 0, //!< Iterate over all sections between start, separators, and end including empty strings.
        skip_empty = 1 //!< Skip empty strings when splitting a string. This is the case e.g. when separators are adjacent.
    };

    /**
        \brief Used for iterating over a string splitting it into ranges at separator tokens between start, separators, and end.
    */
    template <typename text_type, typename text_type_separator, typename equals_comparer_type>
    class split_token_iterator
    {
        typedef typename implementation::terminated_iterator_type_resolver<text_type>::terminated_iterator_type terminated_iterator_type_text;
        typedef typename implementation::terminated_iterator_type_resolver<text_type_separator>::const_terminated_iterator_type terminated_iterator_type_separator;
    public:
        typedef typename terminated_iterator_type_text::iterator_type iterator_type; //!< The type of the iterator for the range containing a section of the string.
        typedef split_token_iterator<text_type, text_type_separator, equals_comparer_type> this_type; //!< The type of this class template instance.

        /**
            \brief Constructs an empty split_token_iterator.
        */
        split_token_iterator()
            : used_mode(split_mode::all)
            , is_end(false)
        {
        }

        /**
        \brief Constructs a split_token_iterator for iterating over a string splitting it into ranges between start, separators, and end.
        \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                           The split_token_iterator only stores a reference to \c text_to_iterate_over.
                                           \c text_to_iterate_over must not be destroyed or changed while using the split_token_iterator.
        \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
                                           The split_token_iterator only stores a reference to \c separator_token.
                                           \c separator_token must not be destroyed or changed while using the split_token_iterator.
        \param[in] mode                    Mode whether to skip empty sections.
        \param[in] equals_comparer         Compares two character values for equality.
                                           The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                                           The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                                           Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }

        Example:
        \code
        std::vector<std::string> container;
        std::string text = "Hello - World";
        // Note that there are factory functions that help you use the split_token_iterator, e.g. make_split_token_iterator().
        cppstringx::split_token_iterator<std::string, const char*, cppstringx::utility::equals_comparer> split_it(text, " - ", cppstringx::split_mode::all, cppstringx::utility::equals_comparer());
        while (!split_it.is_end_position())
        {
            container.emplace_back(std::string(split_it->begin(), split_it->end()));
            ++split_it;
        }
        \endcode
        \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.
        */
        split_token_iterator(text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode, const equals_comparer_type& equals_comparer)
            : comparer(equals_comparer)
            , itt_text(implementation::make_terminated_iterator_forward(text_to_iterate_over))
            , itt_separator(implementation::make_const_terminated_iterator_forward(separator_token))
            , current_separator(itt_text, itt_text)
            , used_mode(mode)
            , is_end(false)
        {
            // An empty string cannot be used as separator_token beacuse it would match anywhere.
            if (itt_separator.is_end_position())
            {
                throw std::invalid_argument("The separator_token input parameter for the split_token_iterator must not be empty.");
            }

            // We do not advance if the string is empty, because it would falsely detect an end position
            if (!itt_text.is_end_position() || mode == split_mode::skip_empty)
            {
                advance(); // Advance to the next range between start, separators, and end
            }
            else
            {
                // If the text is empty we feed the range with the end positions. This results in an empty string range.
                current_range = range<iterator_type>(itt_text.get_position(), itt_text.get_position());
            }
        }

        /**
            \brief Prefix increment operator.
            \return Advances the iterator to the next position and returns a reference to itself.
        */
        this_type& operator++ ()
        {
            advance(); // Advance to the next range between start, separators, and end
            return *this;
        }

        /**
            \brief Postfix increment operator.
            \return Returns an iterator to the next position.
        */
        this_type operator++ (int)
        {
            this_type result(*this);
            advance(); // Advance to the next range between start, separators, and end
            return result;
        }

        /**
            \brief Checks whether the end position has been reached.
            \return Returns true if the end position has been reached.
        */
        bool is_end_position() const
        {
            return is_end;
        }

        /**
            \brief Reference operator.
            \return Returns a reference to the current range.
        */
        const range<iterator_type>& operator*() const
        {
            return current_range;
        }

        /**
            \brief Member access operator.
            \return Returns a pointer to the current range.
        */
        const range<iterator_type>* operator->() const
        {
            return &current_range;
        }

        /**
            \brief Advances n positions to a section. This is usable if a fixed pattern can be expected.
            \param[in] count    Number of positions to advance the iterator. This is the same as using the operator++ \c count times.
            \return Returns true if the position has been reached otherwise the end position has been reached.
        */
        bool advance(size_t count)
        {
            for (size_t i = 0; i < count && !is_end; ++i)
            {
                advance();
            }
            return !is_end;
        }

        /**
            \brief Advances to the last section.
            \return Returns true if the position has been reached otherwise the end position has been reached.
        */
        bool advance_to_last()
        {
            if (!is_end)
            {
                if (used_mode == split_mode::all)
                {
                    while (!is_end && !current_separator.begin().is_end_position())
                    {
                        advance();
                    }
                }
                else if (used_mode == split_mode::skip_empty)
                {
                    // advance to the end and then switch back to the previous position
                    this_type previous;
                    while (!is_end)
                    {
                        previous = *this;
                        advance();
                    }
                    *this = previous;
                }
                else
                {
                    assert(false); // split_mode has been extended, need to extend this if clause too
                }
            }

            return !is_end; //false when all sections are empty and split_mode::skip_empty
        }

    private:

        void advance()
        {
            while (!is_end) // Advance until the end has been reached
            {
                is_end = current_separator.begin().is_end_position(); // This happens when the separator has not been found by the last find_forward_optimized call.
                itt_text = current_separator.end(); // Set the current text to the beginning of the next section that start one character after the separator.
                current_separator = implementation::find_forward_optimized(itt_text, itt_separator, comparer); // Find the next separator.
                current_range = range<iterator_type>(itt_text.get_position(), current_separator.begin().get_position()); // Update the current range between start, separators, and end.
                if (used_mode == split_mode::skip_empty && current_separator.begin() == itt_text) // If skip mode and the current section is empty advance again.
                {
                    //auto increment to next position
                }
                else
                {
                    break; // Done.
                }
            }
        }

    private:
        equals_comparer_type comparer; // Compares two character values for equality. The comparer classes are used to be able to apply different modes of comparison.
        terminated_iterator_type_text itt_text; // The text that is searched.
        terminated_iterator_type_separator itt_separator; // The string that is used as separator.
        range<terminated_iterator_type_text> current_separator; // The last found separator.
        range<iterator_type> current_range; // The found range that is reported.
        split_mode used_mode; // Mainly used to skip over empty sections if needed.
        bool is_end; // Used to detect the end position properly. This does not work using the iterators alone if a separator is at the end of text.
                     // The last section needs to be an empty string in this case if split_mode::all.
    };

    /**
    \brief Constructs a split_token_iterator for iterating over a string splitting it into ranges between start, separators, and end.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_token_iterator only stores a reference to \c text_to_iterate_over.
                                       \c text_to_iterate_over must not be destroyed or changed while using the split_token_iterator.
    \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_token_iterator only stores a reference to \c separator_token.
                                       \c separator_token must not be destroyed or changed while using the split_token_iterator.
    \param[in] mode                    Mode whether to skip empty sections.
    \param[in] equals_comparer         Compares two character values for equality.
                                       The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                                       The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                                       Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello - World";
    auto split_it = make_split_token_iterator(text, " - ", cppstringx::split_mode::all, cppstringx::utility::equals_comparer());
    while (!split_it.is_end_position())
    {
        container.emplace_back(std::string(split_it->begin(), split_it->end()));
        ++split_it;
    }
    \endcode
    */
    template <typename text_type, typename text_type_separator, typename equals_comparer_type>
    split_token_iterator<text_type, text_type_separator, equals_comparer_type> make_split_token_iterator(text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode, const equals_comparer_type& equals_comparer)
    {
        split_token_iterator<text_type, text_type_separator, equals_comparer_type> result(text_to_iterate_over, separator_token, mode, equals_comparer);
        return result;
    }

    /**
    \brief Constructs a split_token_iterator for iterating over a string splitting it into ranges between start, separators, and end.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_token_iterator only stores a reference to \c text_to_iterate_over.
                                       \c text_to_iterate_over must not be destroyed or changed while using the split_token_iterator.
    \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_token_iterator only stores a reference to \c separator_token.
                                       \c separator_token must not be destroyed or changed while using the split_token_iterator.
    \param[in] mode                    Mode whether to skip empty sections.
    \return Returns the split_token_iterator object.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello - World";
    auto split_it = make_split_token_iterator(text, " - ");
    while (!split_it.is_end_position())
    {
        container.emplace_back(std::string(split_it->begin(), split_it->end()));
        ++split_it;
    }
    \endcode
    */
    template <typename text_type, typename text_type_separator>
    split_token_iterator<text_type, text_type_separator, cppstringx::utility::equals_comparer> make_split_token_iterator(text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode = split_mode::all)
    {
        split_token_iterator<text_type, text_type_separator, cppstringx::utility::equals_comparer> result(text_to_iterate_over, separator_token, mode, cppstringx::utility::equals_comparer());
        return result;
    }

    /**
    \brief Constructs a split_token_iterator for iterating over a string splitting it into ranges between start, separators, and end.
           Separators are found using a case insensitive comparison.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_token_iterator only stores a reference to \c text_to_iterate_over.
                                       \c text_to_iterate_over must not be destroyed or changed while using the split_token_iterator.
    \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_token_iterator only stores a reference to \c separator_token.
                                       \c separator_token must not be destroyed or changed while using the split_token_iterator.
    \param[in] mode                    Mode whether to skip empty sections.
    \return Returns the split_token_iterator object.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello World";
    auto split_it = make_isplit_token_iterator(text, "wo");
    while (!split_it.is_end_position())
    {
        container.emplace_back(std::string(split_it->begin(), split_it->end()));
        ++split_it;
    }
    \endcode
    */
    template <typename text_type, typename text_type_separator>
    split_token_iterator<text_type, text_type_separator, cppstringx::utility::equals_comparer_ignoring_case> make_isplit_token_iterator(text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode = split_mode::all)
    {
        split_token_iterator<text_type, text_type_separator, cppstringx::utility::equals_comparer_ignoring_case> result(text_to_iterate_over, separator_token, mode, cppstringx::utility::equals_comparer_ignoring_case());
        return result;
    }

    /**
    \brief Splits a string into sections between start, separator tokens, and end and adds the sections to a container.
    \param[out] container              The ranges between start, separators, and end are added to this container.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] mode                    Mode whether to skip empty sections.
    \param[in] equals_comparer         Compares two character values for equality.
                                       The comparer classes are used to be able to apply different modes of comparison, e.g. case insensitive comparison.
                                       The equals_comparer_ignoring_case can be passed here provided with a different locale if this is needed.
                                       Optionally you can use a two parameter lambda expression as comparer, e.g. [](char a, char b) { return a == b; }
    \param[in] clear_container         Selects whether \c container is cleared before adding new elements.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello - World";
    split_token(container, text, " - ", cppstringx::split_mode::all, cppstringx::utility::equals_comparer());
    \endcode
    */
    template <typename container_type, typename text_type, typename text_type_separator, typename equals_comparer_type>
    void split_token(container_type& container, text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode, const equals_comparer_type& equals_comparer, bool clear_container = true)
    {
        if (clear_container)
        {
            container.clear();
        }
        split_token_iterator<text_type, text_type_separator, equals_comparer_type> split_it(text_to_iterate_over, separator_token, mode, equals_comparer);
        while (!split_it.is_end_position())
        {
            container.emplace_back(split_it->begin(), split_it->end());
            ++split_it;
        }
    }

    /**
    \brief Splits a string into sections between start, separator tokens, and end and adds the sections to a container.
    \param[out] container              The ranges between start, separators, and end are added to this container.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] mode                    Mode whether to skip empty sections.
    \param[in] clear_container         Selects whether \c container is cleared before adding new elements.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello - World";
    split_token(container, text, " - ");
    \endcode
    */
    template <typename container_type, typename text_type, typename text_type_separator>
    void split_token(container_type& container, text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode = split_mode::all, bool clear_container = true)
    {
        split_token(container, text_to_iterate_over, separator_token, mode, cppstringx::utility::equals_comparer(), clear_container);
    }

    /**
    \brief Splits a string into sections between start, separator tokens, and end and adds the sections to a container.
           Separators are found using a case insensitive comparison.
    \param[out] container              The ranges between start, separators, and end are added to this container.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] separator_token         A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] mode                    Mode whether to skip empty sections.
    \note The character encoding of the passed strings must be equivalent, see the [character encoding section](@ref character_encoding) for more information.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello World";
    isplit_token(container, text, "wo");
    \endcode
    */
    template <typename container_type, typename text_type, typename text_type_separator>
    void isplit_token(container_type& container, text_type& text_to_iterate_over, const text_type_separator& separator_token, split_mode mode = split_mode::all)
    {
        split_token(container, text_to_iterate_over, separator_token, mode, cppstringx::utility::equals_comparer_ignoring_case());
    }

    /**
        \brief Used for iterating over a string splitting it into ranges at between start, separator characters, and end.
    */
    template <typename text_type, typename predicate_type>
    class split_iterator
    {
        typedef typename implementation::terminated_iterator_type_resolver<text_type>::terminated_iterator_type terminated_iterator_type_text;
    public:
        typedef typename terminated_iterator_type_text::iterator_type iterator_type; //!< The type of the iterator for the range containing a section of the string.
        typedef split_iterator<text_type, predicate_type> this_type; //!< The type of this class template instance.

        /**
            \brief Constructs an empty split_iterator.
        */
        split_iterator()
            : used_mode(split_mode::all)
            , is_start(false)
            , is_end(false)
        {
        }

        /**
        \brief Constructs a split_iterator for iterating over a string splitting it into ranges between start, separator characters, and end.
        \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                           The split_token_iterator only stores a reference to \c text_to_iterate_over.
                                           \c text_to_iterate_over must not be destroyed or changed while using the split_token_iterator.
        \param[in] is_separator_predicate  Is used to check whether a character is used for separating sections of a string.
                                           The predicate classes are used to be able to separate string sections using different types of characters.
                                           You can use utility::is_any_of or utility::is_space as predicate or equivalent functions from the
                                           Standard C++ Library.
                                           Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
        \param[in] mode                    Mode whether to skip empty sections.

        Example:
        \code
        std::vector<std::string> container;
        std::string text = "Hello World";
        // Note that there are factory functions that help you use the split_iterator, e.g. make_split_iterator().
        cppstringx::split_iterator<std::string, cppstringx::utility::is_space> split_it(text, cppstringx::utility::is_space());
        while (!split_it.is_end_position())
        {
            container.emplace_back(std::string(split_it->begin(), split_it->end()));
            ++split_it;
        }
        \endcode
        */
        split_iterator(text_type& text_to_iterate_over, const predicate_type& is_separator_predicate, split_mode mode = split_mode::all)
            : is_separator(is_separator_predicate)
            , itt_text(implementation::make_terminated_iterator_forward(text_to_iterate_over))
            , current_separator(itt_text)
            , used_mode(mode)
            , is_start(true)
            , is_end(false)
        {
            // We do not advance if the string is empty, because it would falsely detect an end position
            if (!itt_text.is_end_position() || mode == split_mode::skip_empty)
            {
                advance(); // Advance to the next range between start, separators, and end
            }
            else
            {
                // If the text is empty we feed the range with the end positions. This results in an empty string range.
                current_range = range<iterator_type>(itt_text.get_position(), itt_text.get_position());
            }
        }

        /**
            \brief Prefix increment operator.
            \return Advances the iterator to the next position and returns a reference to itself.
        */
        this_type& operator++ ()
        {
            advance(); // Advance to the next range between start, separators, and end
            return *this;
        }

        /**
            \brief Postfix increment operator.
            \return Returns an iterator to the next position.
        */
        this_type operator++ (int)
        {
            this_type result(*this);
            advance(); // Advance to the next range between start, separators, and end
            return result;
        }

        /**
            \brief Checks whether the end position has been reached.
            \return Returns true if the end position has been reached.
        */
        bool is_end_position() const
        {
            return is_end;
        }

        /**
            \brief Reference operator.
            \return Returns a reference to the current range.
        */
        const range<iterator_type>& operator*() const
        {
            return current_range;
        }

        /**
            \brief Member access operator.
            \return Returns a pointer to the current range.
        */
        const range<iterator_type>* operator->() const
        {
            return &current_range;
        }

        /**
            \brief Advances n positions to a section. This is usable if a fixed pattern can be expected.
            \param[in] count    Number of positions to advance the iterator. This is the same as using the operator++ \c count times.
            \return Returns true if the position has been reached otherwise the end position has been reached.
        */
        bool advance(size_t count)
        {
            for (size_t i = 0; i < count && !is_end; ++i)
            {
                advance();
            }
            return !is_end;
        }

        /**
            \brief Advances to the last section.
            \return Returns true if the position has been reached otherwise the end position has been reached.
        */
        bool advance_to_last()
        {
            if (!is_end)
            {
                if (used_mode == split_mode::all)
                {
                    while (!is_end && !current_separator.is_end_position())
                    {
                        advance();
                    }
                }
                else if (used_mode == split_mode::skip_empty)
                {
                    // advance to the end and then switch back to the previous position
                    this_type previous;
                    while (!is_end)
                    {
                        previous = *this;
                        advance();
                    }
                    *this = previous;
                }
                else
                {
                    assert(false); // split_mode has been extended, need to extend this if clause too
                }
            }

            return !is_end; //false when all sections are empty and split_mode::skip_empty
        }

    private:

        void advance()
        {
            while (!is_end) // Advance until the end has been reached
            {
                is_end = current_separator.is_end_position();
                if (!is_end && !is_start)
                {
                    ++current_separator; // goto first character of next section
                }
                else
                {
                    is_start = false;
                }
                itt_text = current_separator; // set as start character of next section
                for (; !current_separator.is_end_position(); ++current_separator) // Find the next separator.
                {
                    if (is_separator(*current_separator))
                    {
                        break;
                    }
                }
                current_range = range<iterator_type>(itt_text.get_position(), current_separator.get_position()); // Update the current range between start, separators, and end.
                if (used_mode == split_mode::skip_empty && current_separator == itt_text) // If skip mode and the current section is empty advance again.
                {
                    //auto increment to next position
                }
                else
                {
                    break; // Done.
                }
            }
        }

    private:
        predicate_type is_separator; // Is used to check whether a character is a separator.
        terminated_iterator_type_text itt_text; // The text that is searched.
        terminated_iterator_type_text current_separator; // The last found separator.
        range<iterator_type> current_range; // The found range that is reported.
        split_mode used_mode; // Mainly used to skip over empty sections if needed.
        bool is_start; // Used to handle start section as a special case.
        bool is_end; // Used to detect the end position properly. This does not work using the iterators alone if a separator is at the end of text.
                     // The last section needs to be an empty string in this case if split_mode::all.
    };

    /**
    \brief Constructs a split_iterator for iterating over a string splitting it into ranges between start, separator characters, and end.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_iterator only stores a reference to \c text_to_iterate_over.
                                       \c text_to_iterate_over must not be destroyed or changed while using the split_iterator.
    \param[in] is_separator            Is used to check whether a character is used for separating sections of a string.
                                       The predicate classes are used to be able to separate string sections using different types of characters.
                                       You can use utility::is_any_of or utility::is_space as predicate or equivalent functions from the
                                       Standard C++ Library.
                                       Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
    \param[in] mode                    Mode whether to skip empty sections.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello World";
    auto split_it = make_split_iterator(text, cppstringx::utility::is_space());
    while (!split_it.is_end_position())
    {
        container.emplace_back(std::string(split_it->begin(), split_it->end()));
        ++split_it;
    }
    \endcode
    \return Returns the split_iterator object.
    */
    template <typename text_type, typename predicate_type>
    split_iterator<text_type, predicate_type> make_split_iterator(text_type& text_to_iterate_over, const predicate_type& is_separator, split_mode mode = split_mode::all)
    {
        split_iterator<text_type, predicate_type> result(text_to_iterate_over, is_separator, mode);
        return result;
    }

    /**
    \brief Constructs a split_iterator for iterating over a string splitting it into ranges between start, separator characters, and end.
    \param[in] text_to_iterate_over    A string object, e.g. std::string, range object, or a null-terminated string.
                                       The split_iterator only stores a reference to \c text_to_iterate_over.
                                       \c text_to_iterate_over must not be destroyed or changed while using the split_iterator.
    \param[in] separator_characters    A string object, e.g. std::string, range object, or a null-terminated string containing the list of characters
                                       used for splitting the string \c text_to_iterate_over.
                                       The split_iterator only stores a reference to \c separator_characters.
                                       \c separator_characters must not be destroyed or changed while using the split_iterator.
    \param[in] mode                    Mode whether to skip empty sections.


    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello World";
    auto split_it = make_split_chars_iterator(text, " \t");
    while (!split_it.is_end_position())
    {
        container.emplace_back(std::string(split_it->begin(), split_it->end()));
        ++split_it;
    }
    \endcode
    \return Returns the split_iterator object.
    */
    template <typename text_type, typename separator_characters_text_type>
    split_iterator<text_type, utility::is_any_of<separator_characters_text_type>> make_split_chars_iterator(text_type& text_to_iterate_over, const separator_characters_text_type& separator_characters, split_mode mode = split_mode::all)
    {
        split_iterator<text_type, utility::is_any_of<separator_characters_text_type>> result(text_to_iterate_over, utility::is_any_of<separator_characters_text_type>(separator_characters), mode);
        return result;
    }

    /**
    \brief Splits a string into sections between start, separator characters, and end and adds the sections to a container.
    \param[out] container              The ranges between start, separators, and end are added to this container.
    \param[in] string_to_split         A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] is_separator            Is used to check whether a character is used for separating sections of a string.
                                       The predicate classes are used to be able to separate string sections using different types of characters.
                                       You can use utility::is_any_of or utility::is_space as predicate or equivalent functions from the
                                       Standard C++ Library.
                                       Optionally you can use a lambda expression as predicate, e.g. [](char a ) { return a == '-'; }
    \param[in] mode                    Mode whether to skip empty sections.
    \param[in] clear_container         Selects whether \c container is cleared before adding new elements.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello World";
    split(container, text, cppstringx::utility::is_space());
    \endcode
    */
    template <typename container_type, typename text_type, typename predicate_type>
    void split(container_type& container, text_type& string_to_split, const predicate_type& is_separator, split_mode mode = split_mode::all, bool clear_container = true)
    {
        if (clear_container)
        {
            container.clear();
        }
        split_iterator<text_type, predicate_type> split_it(string_to_split, is_separator, mode);
        while (!split_it.is_end_position())
        {
            container.emplace_back(split_it->begin(), split_it->end());
            ++split_it;
        }
    }

    /**
    \brief Splits a string into sections between start, separator characters, and end and adds the sections to a container.
    \param[out] container              The ranges between start, separators, and end are added to this container.
    \param[in] string_to_split         A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] separator_characters    A string object, e.g. std::string, range object, or a null-terminated string containing the list of characters
                                       used for splitting the string \c string_to_split.
    \param[in] mode                    Mode whether to skip empty sections.
    \param[in] clear_container         Selects whether \c container is cleared before adding new elements.

    Example:
    \code
    std::vector<std::string> container;
    std::string text = "Hello World";
    split_chars(container, text, " \t");
    \endcode
    */
    template <typename container_type, typename text_type, typename separator_characters_text_type>
    void split_chars(container_type& container, text_type& string_to_split, const separator_characters_text_type& separator_characters, split_mode mode = split_mode::all, bool clear_container = true)
    {
        split(container, string_to_split, utility::is_any_of<separator_characters_text_type>(separator_characters), mode, clear_container);
    }

    //-------------------------------------------------------------------------
    // join
    //-------------------------------------------------------------------------

    /**
    \brief Joins multiple strings from a container to one string while inserting separator strings. A separator string can be empty.
    \param[out] target         A string object, e.g. std::string.
    \param[in] container       The ranges between start, separators, and end are added to this container.
    \param[in] separator       A string object, e.g. std::string, range object, or a null-terminated string.
    \param[in] clear_target    Selects whether \c target is cleared before copying.

    Example:
    \code
    std::vector<std::string> container = { "Hello", "World" };
    std::string text;
    join(text, container, " ");
    \endcode
    \return Returns the target text.
    */
    template <typename text_type, typename container_type, typename separator_text_type>
    text_type& join(text_type& target, const container_type& container, const separator_text_type& separator, bool clear_target = true)
    {
        if (clear_target)
        {
            target.clear();  // Clear the target string if needed.
        }
        bool is_first = true;
        for (auto& item : container)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                cppstringx::copy(target, separator, false /*clear_target*/); // Always add the separator except the first time.
            }
            cppstringx::copy(target, item, false /*clear_target*/); // Add the next item from the container
        }
        return target;
    }

} //namespace cppstringx
