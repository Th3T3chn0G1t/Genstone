#include "include/genesis.h"

void gen_node_load(gen_node_t* output_node, char* source, gen_node_loader_type_handler_t type_handler, gen_node_loader_data_handler_t* data_handlers) {
    char* typestring_mark_out = source;
    for(unsigned long i = 0; *(++typestring_mark_out) != '\n';);
    char save_char = *typestring_mark_out;
    *typestring_mark_out = '\0';
    unsigned long type = type_handler(source);
    *typestring_mark_out = save_char;

    data_handlers[type](output_node, typestring_mark_out + 1);
}

void gen_node_export(char* output_source, gen_node_t* node, gen_node_exporter_type_handler_t type_handler, gen_node_exporter_data_handler_t* data_handlers) {
    type_handler(output_source, node->type);
    data_handlers[node->type](output_source, node);
}
