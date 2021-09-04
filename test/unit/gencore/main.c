// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include <gencommon.h>
#include <genesis.h>
#include <gentooling.h>

static int root_data_storage;
static gen_node_t root;

#define ROOT_TYPE_NAME "ROOT"
static unsigned long type_loader(const char* type_name, __unused void* passthrough) {
	GEN_REQUIRE_EQUAL_STRING(ROOT_TYPE_NAME, type_name);

	return GEN_ROOT_TYPE;
}

static void root_data_loader(gen_node_t* node, const char* data_string, __unused void* passthrough) {
	node->size = sizeof(int);
	root_data_storage = atoi(data_string);
	node->data = &root_data_storage;
}

#define OUTPUT_LENGTH ((4 + 1 /* ROOT\n */) + (3 + 1 /* 127\n */) + (1 /* \n */) + (1 /* \0 */))

static void type_exporter(char* output, unsigned long type, __unused void* passthrough) {
	switch(type) {
		case GEN_ROOT_TYPE: {
			sprintf_s(output, OUTPUT_LENGTH, "%s\n", ROOT_TYPE_NAME);
			break;
		}
	}
}

static void root_data_exporter(char* output, const gen_node_t* node, __unused void* passthrough) {
	sprintf_s(output + (4 + 1 /* ROOT\n */), OUTPUT_LENGTH - (4 + 1 /* ROOT\n */), "%i\n", *(int*) node->data);
}

static void root_handler(gen_node_t* const restrict node, __unused void* const restrict passthrough) {
	GEN_REQUIRE_EQUAL_MEMREGION((void*) &root, (void*) node, sizeof(gen_node_t));
}

noreturn static void no_use_handler(__unused gen_node_t* const restrict node, __unused void* const restrict passthrough) {
	GEN_REQUIRE_NO_REACH;
}

static gen_node_handler_t handlers[] = {root_handler, no_use_handler};

// This is to make sure the test value is constant for require
#define source "ROOT\n127\n\n"

int main() {
	gen_error_t error;

	glog(INFO, "Testing gen_node_import()...");
	gen_node_importer_data_handler_t data_handlers[] = {root_data_loader};
	error = gen_node_import(&root, source, type_loader, data_handlers, NULL);

	GEN_REQUIRE_EQUAL(GEN_OK, error);
	GEN_REQUIRE_EQUAL(GEN_ROOT_TYPE, root.type);
	GEN_REQUIRE_EQUAL(127, *(int*) root.data);

	glog(INFO, "Testing gen_tree_run()...");
	error = gen_tree_run(&root, handlers, NULL);

	GEN_REQUIRE_EQUAL(GEN_OK, error);

	glog(INFO, "Testing gen_node_export()...");
	gen_node_exporter_data_handler_t export_handlers[] = {root_data_exporter};
	char* output = malloc(OUTPUT_LENGTH);
	error = gen_node_export(output, &root, type_exporter, export_handlers, NULL);
	output[OUTPUT_LENGTH - 2] = '\n'; // In an actual impl the child nodes would be loaded
	output[OUTPUT_LENGTH - 1] = '\0';

	GEN_REQUIRE_EQUAL(GEN_OK, error);
	GEN_REQUIRE_EQUAL_STRING(source, output);
}
