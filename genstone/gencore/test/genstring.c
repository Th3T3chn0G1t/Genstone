// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "genstring"
#include <gentests.h>
#include <genstring.h>
#include <genmemory.h>

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    {
        gen_bool_t equal = gen_false;
        error = gen_string_compare("AAAA", sizeof("AAAA"), "AAAA", sizeof("AAAA"), GEN_STRING_NO_BOUNDS, &equal);
        if(error) return error;

        error = GEN_TESTS_EXPECT(gen_true, equal);
        if(error) return error;

        error = gen_string_compare("AAAA", sizeof("AAAA"), "BBBB", sizeof("BBBB"), GEN_STRING_NO_BOUNDS, &equal);
        if(error) return error;

        error = GEN_TESTS_EXPECT(gen_false, equal);
        if(error) return error;

        error = gen_string_compare("AABB", sizeof("AABB"), "AACC", sizeof("AACC"), 2, &equal);
        if(error) return error;

        error = GEN_TESTS_EXPECT(gen_true, equal);
        if(error) return error;
    }

    {
        const char a[] = "AAAA";
        char b[sizeof(a)] = {0};

        error = gen_string_copy(b, sizeof(b), a, sizeof(a), GEN_STRING_NO_BOUNDS);
        if(error) return error;

        error = GEN_TESTS_EXPECT(a, b);
        if(error) return error;
    }

    {
        const char a[] = "AAAA";
        const char b[] = "BBBB";
        char c[sizeof(a) + sizeof(b) - 1] = {0};

        error = gen_string_append(c, sizeof(c), a, sizeof(a), GEN_STRING_NO_BOUNDS);
        if(error) return error;

        error = gen_string_append(c, sizeof(c), b, sizeof(b), GEN_STRING_NO_BOUNDS);
        if(error) return error;

        error = GEN_TESTS_EXPECT("AAAABBBB", c);
        if(error) return error;
    }

    {
        gen_size_t length = 0;
        error = gen_string_length("AAAA", sizeof("AAAA"), GEN_STRING_NO_BOUNDS, &length);
        if(error) return error;

        error = GEN_TESTS_EXPECT(4, length);
        if(error) return error;

        error = gen_string_length("AAAA", sizeof("AAAA"), 3, &length);
        if(error) return error;

        error = GEN_TESTS_EXPECT(3, length);
        if(error) return error;
    }

    {
        char* dup = GEN_NULL;
        gen_size_t len = 0;
        error = gen_string_duplicate("AAAA", sizeof("AAAA"), GEN_STRING_NO_BOUNDS, &dup, &len);
        if(error) return error;

        error = GEN_TESTS_EXPECT(4, len);
        if(error) return error;

        error = GEN_TESTS_EXPECT("AAAA", dup);
        if(error) return error;

        error = gen_memory_free((void**) &dup);
        if(error) return error;
    }

    {
        gen_size_t found = 0;
        error = gen_string_character_first("ABAB", sizeof("ABAB"), 'B', GEN_STRING_NO_BOUNDS, &found);
        if(error) return error;

        error = GEN_TESTS_EXPECT(1, found);
        if(error) return error;

        error = gen_string_character_first("AAAA", sizeof("AAAA"), 'B', GEN_STRING_NO_BOUNDS, &found);
        if(error) return error;

        error = GEN_TESTS_EXPECT(GEN_STRING_NO_BOUNDS, found);
        if(error) return error;
    }

    {
        gen_size_t found = 0;
        error = gen_string_character_last("ABAB", sizeof("ABAB"), 'B', GEN_STRING_NO_BOUNDS, &found);
        if(error) return error;

        error = GEN_TESTS_EXPECT(3, found);
        if(error) return error;

        error = gen_string_character_last("AAAA", sizeof("AAAA"), 'B', GEN_STRING_NO_BOUNDS, &found);
        if(error) return error;

        error = GEN_TESTS_EXPECT(GEN_STRING_NO_BOUNDS, found);
        if(error) return error;
    }

    {
        gen_size_t number = 0;
        error = gen_string_number("123", sizeof("123"), GEN_STRING_NO_BOUNDS, &number);
        if(error) return error;

        error = GEN_TESTS_EXPECT(123, number);
        if(error) return error;
    }

    {
        gen_size_t length = 0;
        error = gen_string_format(GEN_STRING_NO_BOUNDS, GEN_NULL, &length, "ABC %uz %t %tz", sizeof("ABC %uz %t %tz") - 1, (gen_size_t) 3, "AAAA", "BBXX", (gen_size_t) 2);
        if(error) return error;

        error = GEN_TESTS_EXPECT(13, length);
        if(error) return error;

        char buffer[14] = {0};
        error = gen_string_format(GEN_STRING_NO_BOUNDS, buffer, GEN_NULL, "ABC %uz %t %tz", sizeof("ABC %uz %t %tz") - 1, (gen_size_t) 3, "AAAA", "BBXX", (gen_size_t) 2);
        if(error) return error;

        error = GEN_TESTS_EXPECT("ABC 3 AAAA BB", buffer);
        if(error) return error;
    }

    {
        gen_bool_t contains = gen_false;
        gen_size_t offset = 0;
        error = gen_string_contains("AAAB", sizeof("AAAB"), "AB", sizeof("AB"), GEN_STRING_NO_BOUNDS, &contains, &offset);
        if(error) return error;

        error = GEN_TESTS_EXPECT(gen_true, contains);
        if(error) return error;

        error = GEN_TESTS_EXPECT(2, offset);
        if(error) return error;
        
        error = gen_string_contains("AAAA", sizeof("AAAA"), "AB", sizeof("AB"), GEN_STRING_NO_BOUNDS, &contains, &offset);
        if(error) return error;

        error = GEN_TESTS_EXPECT(gen_false, contains);
        if(error) return error;
    }

    return GEN_NULL;
}
