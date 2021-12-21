// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

// https://www.llvm.org/docs/tutorial/MyFirstLanguageFrontend/index.html

#include "include/dcs.h"

static void dcs_arg_callback(const gen_arg_type_t type, __unused const size_t argn, const char* const restrict value, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(dcs_arg_callback);

	dcs_options_t* const options = (dcs_options_t*) passthrough;

	switch(type) {
		case GEN_ARG_SHORT: {
			break;
		}
		case GEN_ARG_LONG: {
			break;
		}
		case GEN_ARG_RAW: {
			if(options->source_filename) {
				glog(ERROR, "Multiple source files specified");
				exit(1);
			}
			options->source_filename = value;
			break;
		}
	}
}

int main(const int argc, const char* const* const argv) {
	GEN_FRAME_BEGIN(main);

	gen_error_t error;

	dcs_options_t options = {0};
	error = gen_parse_args(argc, argv, dcs_arg_callback, 0, NULL, 0, NULL, &options);
	GEN_REQUIRE_NO_ERROR(error);

	if(!options.source_filename) {
		glog(ERROR, "No source files specified");
		exit(1);
	}

	gen_filesystem_handle_t source_handle = {0};
	error = gen_handle_open(&source_handle, options.source_filename);
	GEN_REQUIRE_NO_ERROR(error);
	size_t source_size = 0;
	error = gen_handle_size(&source_size, &source_handle);
	GEN_REQUIRE_NO_ERROR(error);
	char source[source_size + 1];
	memset_s(source, sizeof(source), 0, sizeof(source));
	GEN_REQUIRE_NO_ERRNO(memset_s, errno);
	error = gen_handle_read((uint8_t*) source, &source_handle, 0, sizeof(source));
	GEN_REQUIRE_NO_ERROR(error);
	error = gen_handle_close(&source_handle);
	GEN_REQUIRE_NO_ERROR(error);

	error = dcs_tokenize(source, source_size);
	GEN_REQUIRE_NO_ERROR(error);
}
