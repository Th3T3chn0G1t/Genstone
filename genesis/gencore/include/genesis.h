// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2021 TTG <prs.ttg+gengine@pm.me>

/**
 * @file genesis.h
 * Includes the basic utilities for constructing an apptree
 * Defines the apptree types and controlling handler types
 */

#ifndef GEN_GENESIS_H
#define GEN_GENESIS_H

#include "generrors.h"

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
typedef void (*gen_node_handler_t)(gen_node_t*, void*);

/**
 * Loadtime handler for all type-strings
 */
typedef unsigned long (*gen_node_importer_type_handler_t)(const char*, void*);
/**
 * Loadtime handler for a node type's data-string
 */
typedef void (*gen_node_importer_data_handler_t)(gen_node_t*, const char*, void*);

/**
 * Constructs a node from a nodefile source
 * @param output_node pointer to storage for the generated node
 * @param source the contents of a nodefile
 * @param type_handler a handler for converting type-strings to node types
 * @param data_handlers the data-string import handlers, should be at least the count of the largest type enumeration value
 * @param passthrough a passthrough to the handler
 */
extern gen_error_t gen_node_import(gen_node_t* restrict output_node, const char* const restrict source, const gen_node_importer_type_handler_t type_handler, const gen_node_importer_data_handler_t* const restrict data_handlers, void* restrict passthrough);

/**
 * Export handler for all node types
 */
typedef void (*gen_node_exporter_type_handler_t)(char*, const unsigned long, void*);
/**
 * Export handler for a node type
 */
typedef void (*gen_node_exporter_data_handler_t)(char*, const gen_node_t*, void*);

/**
 * Exports a node to nodefile source string
 * @param output_source pointer to storage for the generated source
 * @param node the node to export
 * @param type_handler a handler for converting types to node type-strings
 * @param data_handlers the export handlers, should be at least the count of the largest type enumeration value
 * @param passthrough a passthrough to the handler
 */
extern gen_error_t gen_node_export(char* restrict output_source, const gen_node_t* const restrict node, const gen_node_exporter_type_handler_t type_handler, const gen_node_exporter_data_handler_t* const restrict data_handlers, void* restrict passthrough);

/**
 * Built in node type for the apptree root
 */
#define GEN_ROOT_TYPE 0ul

#endif
