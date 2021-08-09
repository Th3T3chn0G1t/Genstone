// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

#include "include/genesis.h"

void gen_node_import(gen_node_t* output_node, const char* source, const gen_node_importer_type_handler_t type_handler, const gen_node_importer_data_handler_t* data_handlers, void* passthrough) {
    unsigned long typestring_mark_out = 0;
    while(source[++typestring_mark_out] != '\n');
    char typestring[typestring_mark_out];
    typestring[typestring_mark_out - 1] = '\0';
    for(unsigned long i = 0; i < typestring_mark_out; i++) typestring[i] = source[i];

    unsigned long type = type_handler(typestring, passthrough);

    data_handlers[type](output_node, source + typestring_mark_out + 1, passthrough);
}

void gen_node_export(char* output_source, const gen_node_t* node, const gen_node_exporter_type_handler_t type_handler, const gen_node_exporter_data_handler_t* data_handlers, void* passthrough) {
    type_handler(output_source, node->type, passthrough);
    data_handlers[node->type](output_source, node, passthrough);
}
