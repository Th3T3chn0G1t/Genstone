// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genesis.h"
#include "include/gencommon.h"

gen_error_t gen_node_import(gen_node_t* output_node, const char* source, const gen_node_importer_type_handler_t type_handler, const gen_node_importer_data_handler_t* data_handlers, void* passthrough) {
    if(!output_node) return GEN_INVALID_PARAMETER;
    if(!source) return GEN_INVALID_PARAMETER;
    if(!type_handler) return GEN_INVALID_PARAMETER;
    if(!data_handlers) return GEN_INVALID_PARAMETER;

    size_t typestring_mark_out = 0;
    while(source[++typestring_mark_out] != '\n');

    char typestring[typestring_mark_out + 1];

    GEN_FOREACH_PTR(i, src, typestring_mark_out, source) typestring[i] = *src;
    typestring[typestring_mark_out] = '\0';

    data_handlers[type_handler(typestring, passthrough)](output_node, source + typestring_mark_out + 1, passthrough);

    return GEN_OK;
}

gen_error_t gen_node_export(char* output_source, const gen_node_t* node, const gen_node_exporter_type_handler_t type_handler, const gen_node_exporter_data_handler_t* data_handlers, void* passthrough) {
    if(!output_source) return GEN_INVALID_PARAMETER;
    if(!node) return GEN_INVALID_PARAMETER;
    if(!type_handler) return GEN_INVALID_PARAMETER;
    if(!data_handlers) return GEN_INVALID_PARAMETER;

    type_handler(output_source, node->type, passthrough);
    data_handlers[node->type](output_source, node, passthrough);

    return GEN_OK;
}
