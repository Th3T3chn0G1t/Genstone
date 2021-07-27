/**
 * @file genesis.h
 * Includes the basic utilities for constructing an apptree
 * Defines the apptree types and controlling handler types
 * @note does not use stdlib
 */

#ifndef GENESIS_H
#define GENESIS_H

/**
 * @internal
 * Forward declaration of node type for pointers to children and parent
 */
struct gen_node_t;
/**
 * A node in the apptree
 */
typedef struct {
    /**
     * The type of the node
     * Determines that handler that will be called
     */
    unsigned long type;
    /**
     * The size of the node data
     */
    unsigned long size;
    /**
     * The node data
     */
    void* data;
    
    /**
     * The number of child nodes this node has
     */
    unsigned long n_children;
    /**
     * The child nodes of this node
     */
    struct gen_node_t* children;

    /**
     * The parent node of this node
     */
    struct gen_node_t* parent;
} gen_node_t;

/**
 * Runtime handler for a node type
 */
typedef void (*gen_node_handler_t)(gen_node_t*);

/**
 * Loadtime handler for all type-strings
 */
typedef unsigned long (*gen_node_loader_type_handler_t)(const char*);
/**
 * Loadtime handler for a node type's data-string
 */
typedef void (*gen_node_loader_data_handler_t)(gen_node_t*, const char*);

/**
 * Constructs a node from a nodefile source
 * @param output_node pointer to storage for the generated node
 * @param source the contents of a nodefile
 * @param type_handler a handler for converting type-strings to node types
 * @param data_handlers the data-string load handlers, should be at least the count of the largest type enumeration value
 */
extern void gen_node_load(gen_node_t* output_node, char* source, gen_node_loader_type_handler_t type_handler, gen_node_loader_data_handler_t* data_handlers);

/**
 * Export handler for all node types
 */
typedef void (*gen_node_exporter_type_handler_t)(char*, unsigned long);
/**
 * Export handler for a node type
 */
typedef void (*gen_node_exporter_data_handler_t)(char*, gen_node_t*);

/**
 * Exports a node to nodefile source string
 * @param output_source pointer to storage for the generated source
 * @param node the node to export
 * @param handlers the export handlers, should be at least the count of the largest type enumeration value
 */
extern void gen_node_export(char* output_source, gen_node_t* node, gen_node_exporter_type_handler_t type_handler, gen_node_exporter_data_handler_t* data_handlers);

/**
 * Built in node type for the apptree root
 */
#define GEN_ROOT_TYPE 0

#endif
