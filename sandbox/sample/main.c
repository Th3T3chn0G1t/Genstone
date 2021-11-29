// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+genstone@pm.me>

#include <gencommon.h>
#include <genfs.h>
#include <genlocale.h>

typedef struct gen_json_t gen_json_t;
typedef struct gen_json_t {
	const char* key;

	size_t n_values;
	gen_json_t* values;
} gen_json_t;

static gen_error_t gen_internal_parse_json_recursive(const char* const restrict json, const size_t json_len, const size_t offset, gen_json_t* const restrict out_json) {
	out_json->key = json + offset;
	out_json->n_values = 0;
	out_json->values = NULL;

	size_t local_offset = offset;

	do {
		char current = json[local_offset];

		switch(current) {
			case ' ':
			case '\t':
			case '\n':
				continue;
		}
	} while(++local_offset < json_len);

	GEN_ALL_OK;
}

static void dump_json(const gen_json_t* json) {
	printf("%s", json->key);

	GEN_FOREACH_PTR(i, val, json->n_values, json->values) {
		dump_json(val);
	}
}

int main() {
	glog(INFO, "Hello, Genstone!");

	// gen_filesystem_handle_t handle;
	// (void) gzalloc((void**) &handle.path, GEN_PATH_MAX, sizeof(char));
	// (void) gen_handle_open(&handle, "sandbox/sample/test.json");
	// size_t len;
	// (void) gen_handle_size(&len, &handle);
	// uint8_t source[len + 1];
	// (void) gen_file_read(source, &handle, 0, len);
	// source[len] = 0;

	// gen_json_t json;
	// gen_internal_parse_json_recursive((char*) source, len, 0, &json);

	const char source[] = "{}";

	gen_json_t json;
	gen_internal_parse_json_recursive(source, sizeof(source), 0, &json);

	dump_json(&json);
}
