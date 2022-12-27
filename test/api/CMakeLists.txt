add_executable(test_api_runner
            test_contains.cpp
            test_copy.cpp
            test_ends_with.cpp
            test_equals.cpp
            test_join.cpp
            test_range.cpp
            test_replace.cpp
            test_split.cpp
            test_split_token.cpp
            test_starts_with.cpp
            test_string_length.cpp
            test_api.cpp
            test_to_lower.cpp
            test_to_upper.cpp
            test_trim.cpp
            test_trim_end.cpp
            test_trim_start.cpp
        )

target_include_directories(test_api_runner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
${PROJECT_SOURCE_DIR}/include
)

custom_target_use_highest_warning_level(test_api_runner)

add_test(
        NAME test_api
        COMMAND test_api_runner
)
