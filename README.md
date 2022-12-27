# C++ String Extensions

## Purpose
The C++ standard library is missing basic functions (see the table below) for handling
standard string operations. You have to implement these yourself. Libraries like Boost String
Algorithms provide the needed functions, but often the libraries that come into question
are quite large and unhandy.

C++ String Extensions provides an easy to use solution to fill this gap.

## Comparison of Common String Handling Support Features
The following table represents only a rough overview.

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

## Design Goals
- Header-only, single header
- KISS principle
- Easy to use and well documented
- Multi-platform support
- No build warnings on common compilers
- High test coverage
- Be able to mix different code unit types in one function

## When to Use
- You need some of the string functions listed above.
- If the C++ Standard Library provides the listed functions, it is recommended to use the the C++ Standard Library instead.
- If you already use boost, it is recommended use Boost String Algorithms Library instead.
- If you already using Qt, it is recommended use the Qt strings instead.

## Sample Code

```
#include <cppstringx/cppstringx.hpp>
#include <iostream>
#include <vector>

int main()
{
    // The basics, see the documentation for additional variants
    std::string hello = "Hello World";
    std::string hello2 = "   Hello World   ";

    std::cout << "a: " << cppstringx::string_length(hello) << std::endl;
    std::cout << "b: " << cppstringx::contains("Hello World", "World") << std::endl;
    std::cout << "c: " << cppstringx::equals(hello, "World") << std::endl;
    std::cout << "d: " << cppstringx::copy<std::string>(hello) << std::endl;
    std::cout << "e: " << cppstringx::replace_all_copy(hello, "World", "Universe") << std::endl;
    std::cout << "f: " << cppstringx::starts_with(hello, "World") << std::endl;
    std::cout << "g: " << cppstringx::ends_with("Hello World", "World") << std::endl;
    std::cout << "h: " << cppstringx::to_lower_copy(hello) << std::endl;
    std::cout << "i: " << cppstringx::to_upper_copy(hello) << std::endl;
    std::cout << "j: " << cppstringx::trim_copy(hello2) << "|" << std::endl;
    std::cout << "k: " << cppstringx::trim_start_copy(hello2) << "|" << std::endl;
    std::cout << "l: " << cppstringx::trim_end_copy(hello2) << "|" << std::endl;
    std::vector<std::string> container;
    cppstringx::split_token(container, hello, "o W");
    cppstringx::join(hello, container, "o - W");
    std::cout << "m: " << hello << std::endl;

    // Case-insensitive variants
    std::cout << "n: " << cppstringx::icontains("Hello world", "World") << std::endl;
    std::cout << "o: " << cppstringx::iequals(hello, "hello world") << std::endl;
    std::cout << "p: " << cppstringx::ireplace_all_copy(hello, "world", "Universe") << std::endl;
    std::cout << "q: " << cppstringx::istarts_with(hello, "world") << std::endl;
    std::cout << "r: " << cppstringx::iends_with("Hello World", "World") << std::endl;
    cppstringx::isplit_token(container, hello, "O - w");
    cppstringx::join(hello, container, "o + W");
    std::cout << "s: " << hello << std::endl;

    // Mixing string types freely
    // --------------------------
    // You can use different string types in all functions
    // The character encoding of the passed strings must be equivalent. (see character encoding section below)
    std::cout << "t: " << cppstringx::icontains("Hello World", L"World") << std::endl;

    // In-place variants
    // -----------------
    // A lot of functions have in-place variants. See the documentation for additional variants.
    std::cout << "u: " << cppstringx::ireplace_all_in_place(hello, "world", "Universe") << std::endl;
    std::cout << "v: " << hello << std::endl;

    // More variants
    // -------------
    // Comparer, predicate, case_converter objects can be used to modify the behavior.
    // See the documentation of additional variants for more information.
    std::cout << "w: " << cppstringx::trim_copy(hello, cppstringx::utility::is_any_of<const char*>("He")) << std::endl;
    // These objects can be replaced with custom lambda expressions for customizing the behavior
    std::cout << "x: " << cppstringx::trim_copy(hello, [](char c) { if (c == 'e') return true; return false; }) << std::endl;
    std::cout << "y: " << cppstringx::contains(" 11.11.2011 ", "dd.dd.dddd", [](char l, char r) { if (r == 'd' && l >= '0' && l <= '9') return true; return r == l; }) << std::endl;

    // More objects
    // ------------
    // Classes like split_iterator, split_token_iterator and range help to work with strings efficiently.
    auto split_it = cppstringx::make_split_chars_iterator(hello, " ");
    while (!split_it.is_end_position())
    {
        std::cout << "z: " << std::string(split_it->begin(), split_it->end()) << std::endl;
        ++split_it;
    }
}
```

## Character Encoding

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
