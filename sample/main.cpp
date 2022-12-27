//-----------------------------------------------------------------------------
//  cppstringx - C++ String Extensions
//-----------------------------------------------------------------------------

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
    // The character encoding of the passed strings must be equivalent,
    // see the character encoding section of the documentation for more information.
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
