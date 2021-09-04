// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genesis.h"

#include "include/gencommon.h"
#include "include/gentooling.h"

GEN_ERRORABLE_RETURN gen_node_import(gen_node_t* restrict output_node, const char* const restrict source, const gen_node_importer_type_handler_t type_handler, const gen_node_importer_data_handler_t* const restrict data_handlers, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_node_import);

	if(!output_node) return GEN_INVALID_PARAMETER;
	if(!source) return GEN_INVALID_PARAMETER;
	if(!type_handler) return GEN_INVALID_PARAMETER;
	if(!data_handlers) return GEN_INVALID_PARAMETER;

	size_t typestring_mark_out = 0;
	while(source[++typestring_mark_out] != '\n')
		;

	char typestring[typestring_mark_out + 1];

	GEN_FOREACH_PTR(i, src, typestring_mark_out, source)
		typestring[i] = *src;
	typestring[typestring_mark_out] = '\0';

	data_handlers[type_handler(typestring, passthrough)](output_node, source + typestring_mark_out + 1, passthrough);

	return GEN_OK;
}

GEN_ERRORABLE_RETURN gen_node_export(char* restrict output_source, const gen_node_t* const restrict node, const gen_node_exporter_type_handler_t type_handler, const gen_node_exporter_data_handler_t* const restrict data_handlers, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_node_export);

	if(!output_source) return GEN_INVALID_PARAMETER;
	if(!node) return GEN_INVALID_PARAMETER;
	if(!type_handler) return GEN_INVALID_PARAMETER;
	if(!data_handlers) return GEN_INVALID_PARAMETER;

	type_handler(output_source, node->type, passthrough);
	data_handlers[node->type](output_source, node, passthrough);

	return GEN_OK;
}

static GEN_ERRORABLE_RETURN gen_internal_tree_run_propogate(gen_node_t* const restrict node, const gen_node_handler_t* const restrict handlers, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_internal_tree_run_propogate);

	if(!node) return GEN_INVALID_PARAMETER;
	if(!handlers) return GEN_INVALID_PARAMETER;

	gen_error_t error = GEN_OK;
	GEN_FOREACH_DIRECT_PTR(i, child, node->n_children, (gen_internal_node_sized_buffer_t*) node->children) {
		const gen_node_handler_t handler = handlers[((gen_node_t*) child)->type];
		if(!handler) return GEN_TOO_SHORT;
		handler((gen_node_t*) child, passthrough);
		if((error = gen_internal_tree_run_propogate((gen_node_t*) child, handlers, passthrough))) return error;
	}
	return GEN_OK;
}

GEN_ERRORABLE_RETURN gen_tree_run(gen_node_t* const restrict root, const gen_node_handler_t* const restrict handlers, void* const restrict passthrough) {
	GEN_FRAME_BEGIN(gen_tree_run);

	if(!root) return GEN_INVALID_PARAMETER;
	if(!handlers) return GEN_INVALID_PARAMETER;

	handlers[root->type](root, passthrough);
	return gen_internal_tree_run_propogate(root, handlers, passthrough);
}
