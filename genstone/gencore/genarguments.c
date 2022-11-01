// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2022 Emily "TTG" Banerjee <prs.ttg+genstone@pm.me>

#include "include/genarguments.h"

#include "include/gencommon.h"
#include "include/genmemory.h"
#include "include/genstring.h"

gen_error_t* gen_arguments_parse(const char* const restrict* const restrict arguments, const gen_size_t* const restrict argument_lengths, const gen_size_t argument_count, const char* const restrict short_arguments, const gen_size_t short_argument_count, const char* const restrict* const restrict long_arguments, const gen_size_t* const restrict long_argument_lengths, const gen_size_t long_argument_count, gen_arguments_parsed_t* const restrict out_parsed) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_arguments_parse, GEN_FILE_NAME);
	if(error) return error;

	if(!arguments) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`arguments` was GEN_NULL");
	if(!argument_lengths) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`argument_lengths` was GEN_NULL");
	if(short_argument_count && !short_arguments) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`short_argument_count` was not 0 but `short_arguments` was GEN_NULL");
	if(long_argument_count && !long_arguments) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`long_argument_count` was not 0 but `long_arguments` was GEN_NULL");
	if(long_argument_count && !long_argument_lengths) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "``long_argument_count` was not 0 but long_argument_lengths` was GEN_NULL");
	if(!out_parsed) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`out_parsed` was GEN_NULL");

	for(gen_size_t i = 0; i < argument_count; ++i) {
		// Handle long arguments (`--foo=bar` style arguments)
		if(argument_lengths[i] >= 3 && arguments[i][0] == '-' && arguments[i][1] == '-') {
			const char* argument = arguments[i] + 2 /* Skip `--` */;
			const gen_size_t argument_length = argument_lengths[i] - 2 /* Skipped `--` */;

			for(gen_size_t j = 0; j < long_argument_count; ++j) {
				gen_size_t occurrence = 0;
				error = gen_string_character_first(argument, argument_length + 1, '=', GEN_STRING_NO_BOUNDS, &occurrence);
				if(error) return error;

				const char* parameter = GEN_NULL;
				gen_size_t parameter_length = 0;
				if(occurrence != GEN_SIZE_MAX) {
					parameter = &argument[occurrence + 1];
					parameter_length = argument_length - ((gen_size_t) (parameter - argument));
				}

				gen_bool_t equal = gen_false;
				error = gen_string_compare(argument, argument_length + 1, long_arguments[j], long_argument_lengths[j], occurrence, &equal);
				if(error) return error;

				if(equal) {
					error = gen_memory_reallocate_zeroed((void**) &out_parsed->long_argument_indices, out_parsed->long_argument_count, out_parsed->long_argument_count + 1, sizeof(gen_size_t));
					if(error) return error;
					error = gen_memory_reallocate_zeroed((void**) &out_parsed->long_argument_parameters, out_parsed->long_argument_count, out_parsed->long_argument_count + 1, sizeof(const char*));
					if(error) return error;
					error = gen_memory_reallocate_zeroed((void**) &out_parsed->long_argument_parameter_lengths, out_parsed->long_argument_count, out_parsed->long_argument_count + 1, sizeof(gen_size_t));
					if(error) return error;

					out_parsed->long_argument_indices[out_parsed->long_argument_count] = j;
					out_parsed->long_argument_parameters[out_parsed->long_argument_count] = parameter;
					out_parsed->long_argument_parameter_lengths[out_parsed->long_argument_count] = parameter_length;

					++out_parsed->long_argument_count;

					goto next_argument;
				}
			}

			return gen_error_attach_backtrace_formatted(GEN_ERROR_NO_SUCH_OBJECT, GEN_LINE_NUMBER, "Invalid argument `%tz`", arguments[i], argument_lengths[i]);
		}
		// Handle short arguments (`-fbar` style arguments)
		else if(argument_lengths[i] >= 2 && arguments[i][0] == '-') {
			const char* argument = arguments[i] + 1 /* Skip `-` */;
			const gen_size_t argument_length = argument_lengths[i] - 1 /* Skipped `-` */;

			for(gen_size_t j = 0; j < short_argument_count; ++j) {
				if(argument[0] == short_arguments[j]) {
					const char* parameter = GEN_NULL;
					gen_size_t parameter_length = 0;
					if(argument_length > 1) {
						parameter = argument + 1;
						parameter_length = argument_length - 1;
					}

					error = gen_memory_reallocate_zeroed((void**) &out_parsed->short_argument_indices, out_parsed->short_argument_count, out_parsed->short_argument_count + 1, sizeof(gen_size_t));
					if(error) return error;
					error = gen_memory_reallocate_zeroed((void**) &out_parsed->short_argument_parameters, out_parsed->short_argument_count, out_parsed->short_argument_count + 1, sizeof(const char*));
					if(error) return error;
					error = gen_memory_reallocate_zeroed((void**) &out_parsed->short_argument_parameter_lengths, out_parsed->short_argument_count, out_parsed->short_argument_count + 1, sizeof(gen_size_t));
					if(error) return error;

					out_parsed->short_argument_indices[out_parsed->short_argument_count] = j;
					out_parsed->short_argument_parameters[out_parsed->short_argument_count] = parameter;
					out_parsed->short_argument_parameter_lengths[out_parsed->short_argument_count] = parameter_length;

					++out_parsed->short_argument_count;

					goto next_argument;
				}
			}

			return gen_error_attach_backtrace_formatted(GEN_ERROR_NO_SUCH_OBJECT, GEN_LINE_NUMBER, "Invalid argument `%tz`", arguments[i], argument_lengths[i]);
		}
		else {
			error = gen_memory_reallocate_zeroed((void**) &out_parsed->raw_argument_indices, out_parsed->raw_argument_count, out_parsed->raw_argument_count + 1, sizeof(gen_size_t));
			if(error) return error;

			out_parsed->raw_argument_indices[out_parsed->raw_argument_count] = i;

			++out_parsed->raw_argument_count;
		}

	next_argument:
		continue;
	}

	return GEN_NULL;
}

gen_error_t* gen_arguments_free_parsed(gen_arguments_parsed_t* const restrict parsed) {
	GEN_TOOLING_AUTO gen_error_t* error = gen_tooling_push(GEN_FUNCTION_NAME, (void*) gen_arguments_free_parsed, GEN_FILE_NAME);
	if(error) return error;

	if(!parsed) return gen_error_attach_backtrace(GEN_ERROR_INVALID_PARAMETER, GEN_LINE_NUMBER, "`parsed` was `GEN_NULL`");

	if(parsed->short_argument_indices) {
		error = gen_memory_free((void**) &parsed->short_argument_indices);
		if(error) return error;
	}
	if(parsed->short_argument_parameters) {
		error = gen_memory_free((void**) &parsed->short_argument_parameters);
		if(error) return error;
	}
	if(parsed->short_argument_parameter_lengths) {
		error = gen_memory_free((void**) &parsed->short_argument_parameter_lengths);
		if(error) return error;
	}
	if(parsed->long_argument_indices) {
		error = gen_memory_free((void**) &parsed->long_argument_indices);
		if(error) return error;
	}
	if(parsed->long_argument_parameters) {
		error = gen_memory_free((void**) &parsed->long_argument_parameters);
		if(error) return error;
	}
	if(parsed->long_argument_parameter_lengths) {
		error = gen_memory_free((void**) &parsed->long_argument_parameter_lengths);
		if(error) return error;
	}
	if(parsed->raw_argument_indices) {
		error = gen_memory_free((void**) &parsed->raw_argument_indices);
		if(error) return error;
	}

	return GEN_NULL;
}
