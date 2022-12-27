add_executable(test_utility_runner
        test_utility.cpp
        )

target_include_directories(test_utility_runner
PRIVATE
${PROJECT_SOURCE_DIR}/test/include
${PROJECT_SOURCE_DIR}/include
)

custom_target_use_highest_warning_level(test_utility_runner)

add_test(
        NAME test_utility
        COMMAND test_utility_runner
)
