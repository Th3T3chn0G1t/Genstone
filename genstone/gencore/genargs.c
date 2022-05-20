// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include "include/genargs.h"

#include "include/gencommon.h"
#include "include/genstring.h"

gen_error_t gen_parse_args(const int argc, const char* const restrict* restrict const argv, const gen_arg_handler_t handler, const size_t n_short_args, const char* restrict short_args, const size_t n_long_args, const char* const restrict* const restrict long_args, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_parse_args);

	GEN_NULL_CHECK(argv);
	if(argc < 0) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`argc` was invalid (`argc` < 0)");
	GEN_NULL_CHECK(handler);
	if(n_short_args && !short_args) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`short_args` was NULL but `n_short_args` > 0");
	if(n_long_args && !long_args) GEN_ERROR_OUT(GEN_INVALID_PARAMETER, "`long_args` was NULL but `n_long_args` > 0");

	gen_error_t error = GEN_OK;

	GEN_FOREACH_PTR(i, arg, argc - 1, argv + 1) {
		if((*arg)[0] == '-' && (*arg)[1] == '-' && n_long_args) {
			GEN_FOREACH_PTR(j, long_arg, n_long_args, long_args) {
				bool equal = false;
				error = gen_string_compare(*long_arg, GEN_STRING_NO_BOUND, *arg, GEN_STRING_NO_BOUND, GEN_STRING_NO_BOUND, &equal);
				GEN_ERROR_OUT_IF(error, "`gen_string_compare` failed");
				if(equal) {
					const char* param = 0;
					error = gen_string_character_first(*arg, GEN_STRING_NO_BOUND, '=', GEN_STRING_NO_BOUND, &param);
					GEN_ERROR_OUT_IF(error, "`gen_string_character_first` failed");
					const size_t param_offset = (size_t) (param - *arg);
					error = handler(GEN_ARG_LONG, j, (*arg)[param_offset + 1] ? &(*arg)[param_offset + 1] : NULL, passthrough);
					GEN_ERROR_OUT_IF(error, "Call to arg handler failed");
				}
			}
		}
		else if((*arg)[0] == '-' && n_short_args) {
			GEN_FOREACH_PTR(j, short_arg, n_short_args, short_args) {
				if(*short_arg == (*arg)[1]) {
					error = handler(GEN_ARG_SHORT, j, (*arg)[2] ? &(*arg)[2] : NULL, passthrough);
					GEN_ERROR_OUT_IF(error, "Call to arg handler failed");
				}
			}
		}
		else {
			error = handler(GEN_ARG_RAW, 0, (*arg), passthrough);
			GEN_ERROR_OUT_IF(error, "Call to arg handler failed");
		}
	}

	GEN_ALL_OK;
}
