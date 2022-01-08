// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genargs.h"

#include "include/gencommon.h"

gen_error_t gen_parse_args(const int argc, const char* const restrict* restrict const argv, const gen_arg_handler_t handler, const size_t n_short_args, const char* restrict short_args, const size_t n_long_args, const char* const restrict* const restrict long_args, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_parse_args);

	GEN_INTERNAL_BASIC_PARAM_CHECK(argv);
	if(argc < 0) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`argc` was invalid (`argc` < 0)");
	GEN_INTERNAL_BASIC_PARAM_CHECK(handler);
	if(n_short_args && !short_args) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`short_args` was NULL but `n_short_args` > 0");
	if(n_long_args && !long_args) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`long_args` was NULL but `n_long_args` > 0");

	size_t* long_arg_lens = NULL;
	if(n_long_args) {
		gen_error_t error = gzalloc((void**) &long_arg_lens, n_long_args, sizeof(size_t));
		GEN_ERROR_OUT_IF(error, "`gzalloc` failed");
		GEN_FOREACH_PTR(i, len, n_long_args, long_arg_lens) {
			*len = 0;
			while(long_args[i][*len]) ++(*len);
		}
	}

	GEN_FOREACH_PTR(i, arg, argc - 1, argv + 1) {
		gen_arg_type_t type;
		size_t argn = SIZE_MAX;
		const char* value = NULL;

		if((*arg)[0] == '-') {
			if((*arg)[1] == '-' && n_long_args) {
				type = GEN_ARG_LONG;
				GEN_FOREACH_PTR(j, long_arg, n_long_args, long_args) {
					const size_t arg_len = long_arg_lens[j];

					// Calculating inline like this might be very slightly faster
					// given the specific use-case
					GEN_FOREACH_PTR(k, arg_char, arg_len, *long_arg) {
						if(*arg_char != (*arg)[k + 2]) goto long_arg_continue;
					}

					argn = j;
					if((*arg)[arg_len + 2]) value = (*arg) + 2 + arg_len + 1;

				long_arg_continue:
					continue;
				}
			}
			else if(n_short_args) {
				type = GEN_ARG_SHORT;
				GEN_FOREACH_PTR(j, short_arg, n_short_args, short_args) {
					if((*short_arg) != (*arg)[1]) continue;

					argn = j;

					if((*arg)[2]) value = (*arg) + 2;
					break;
				}
			}
			else {
				type = GEN_ARG_RAW;
				argn = 0;
				value = (*arg);
			}
		}
		else {
			type = GEN_ARG_RAW;
			argn = 0;
			value = (*arg);
		}

		if(argn == SIZE_MAX) {
			if(n_long_args) {
				gen_error_t error = gfree(long_arg_lens);
				GEN_ERROR_OUT_IF(error, "`gfree` failed");
			}
			GEN_ERROR_OUT(GEN_NO_SUCH_OBJECT, "An unknown argument was passed");
		}
		handler(type, argn, value, passthrough);
	}

	if(n_long_args) {
		gen_error_t error = gfree(long_arg_lens);
		GEN_ERROR_OUT_IF(error, "`gfree` failed");
	}
	GEN_ALL_OK;
}
