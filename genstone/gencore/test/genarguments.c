// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#define GEN_TESTS_UNIT "genarguments"
#include <gentests.h>
#include <genarguments.h>

static gen_error_t* gen_main(void) {
    GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_main, GEN_FILE_NAME);
    if(error) return error;

    const char* arguments[] = {
        "--long-arg-test",
        "--long-arg-test-param=param",
        "-s",
        "-pfoo",
        "A",
        "B",
        "C"
    };
    const gen_size_t argument_lengths[] = {
        sizeof("--long-arg-test") - 1,
        sizeof("--long-arg-test-param=param") - 1,
        sizeof("-s") - 1,
        sizeof("-pfoo") - 1,
        sizeof("A") - 1,
        sizeof("B") - 1,
        sizeof("C") - 1
    };
    const gen_size_t arguments_length = sizeof(arguments) / sizeof(arguments[0]);

    const char short_arguments[] = {'s', 'p'};
    const gen_size_t short_arguments_length = sizeof(short_arguments) / sizeof(short_arguments[0]);

    const char* long_arguments[] = {
        "long-arg-test",
        "long-arg-test-param"
    };
    const gen_size_t long_argument_lengths[] = {
        sizeof("long-arg-test") - 1,
        sizeof("long-arg-test-param") - 1
    };
    const gen_size_t long_arguments_length = sizeof(long_arguments) / sizeof(long_arguments[0]);

    gen_arguments_parsed_t parsed = {0};
    error = gen_arguments_parse(arguments, argument_lengths, arguments_length, short_arguments, short_arguments_length, long_arguments, long_argument_lengths, long_arguments_length, &parsed);
    if(error) return error;

    {
        error = GEN_TESTS_EXPECT(2, parsed.short_argument_count);
        if(error) return error;

        error = GEN_TESTS_EXPECT(0, parsed.short_argument_indices[0]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(0, parsed.short_argument_parameter_lengths[0]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(GEN_NULL, parsed.short_argument_parameters[0]);
        if(error) return error;

        error = GEN_TESTS_EXPECT(1, parsed.short_argument_indices[1]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(3, parsed.short_argument_parameter_lengths[1]);
        if(error) return error;
        error = GEN_TESTS_EXPECT("foo", parsed.short_argument_parameters[1]);
        if(error) return error;
    }

    {
        error = GEN_TESTS_EXPECT(2, parsed.long_argument_count);
        if(error) return error;

        error = GEN_TESTS_EXPECT(0, parsed.long_argument_indices[0]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(0, parsed.long_argument_parameter_lengths[0]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(GEN_NULL, parsed.long_argument_parameters[0]);
        if(error) return error;

        error = GEN_TESTS_EXPECT(1, parsed.long_argument_indices[1]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(5, parsed.long_argument_parameter_lengths[1]);
        if(error) return error;
        error = GEN_TESTS_EXPECT("param", parsed.long_argument_parameters[1]);
        if(error) return error;
    }

    {
        error = GEN_TESTS_EXPECT(3, parsed.raw_argument_count);
        if(error) return error;

        error = GEN_TESTS_EXPECT(4, parsed.raw_argument_indices[0]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(5, parsed.raw_argument_indices[1]);
        if(error) return error;
        error = GEN_TESTS_EXPECT(6, parsed.raw_argument_indices[2]);
        if(error) return error;
    }

    return GEN_NULL;
}
