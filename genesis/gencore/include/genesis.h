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
#include "gencommon.h"

/**
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
    size_t type;
    /**
     * The size of the node data
     */
    size_t size;
    /**
     * The node data
     */
    void* data;
    
    /**
     * The number of child nodes this node has
     */
    size_t n_children;
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
 * Buffer type with the same size as `gen_node_t` for handling incomplete types
 */ 
typedef uintmax_t gen_internal_node_sized_buffer_t[sizeof(gen_node_t) / sizeof(uintmax_t)];

/**
 * Runtime handler for a node type
 */
typedef void (*gen_node_handler_t)(gen_node_t* const restrict, void* const restrict);

/**
 * Loadtime handler for all type-strings
 */
typedef size_t (*gen_node_importer_type_handler_t)(const char* const restrict, void* const restrict);
/**
 * Loadtime handler for a node type's data-string
 */
typedef void (*gen_node_importer_data_handler_t)(gen_node_t* const restrict, const char* const restrict, void* const restrict);

/**
 * Constructs a node from a nodefile source
 * @param output_node pointer to storage for the generated node
 * @param source the contents of a nodefile
 * @param type_handler a handler for converting type-strings to node types
 * @param data_handlers the data-string import handlers, should be at least the count of the largest node type enumeration value
 * @param passthrough a passthrough to the handler
 * @return an error code
 */
extern gen_error_t gen_node_import(gen_node_t* restrict output_node, const char* const restrict source, const gen_node_importer_type_handler_t type_handler, const gen_node_importer_data_handler_t* const restrict data_handlers, void* const restrict passthrough);

/**
 * Export handler for all node types
 */
typedef void (*gen_node_exporter_type_handler_t)(char* restrict, const size_t, void* const restrict);
/**
 * Export handler for a node type
 */
typedef void (*gen_node_exporter_data_handler_t)(char* restrict, const gen_node_t* const restrict, void* const restrict);

/**
 * Exports a node to nodefile source string
 * @param output_source pointer to storage for the generated source
 * @param node the node to export
 * @param type_handler a handler for converting types to node type-strings
 * @param data_handlers the export handlers, should be at least the count of the largest node type enumeration value
 * @param passthrough a passthrough to the handler
 * @return an error code
 */
extern gen_error_t gen_node_export(char* restrict output_source, const gen_node_t* const restrict node, const gen_node_exporter_type_handler_t type_handler, const gen_node_exporter_data_handler_t* const restrict data_handlers, void* const restrict passthrough);

/**
 * Runs a tree of nodes with their handlers
 * @param root the root node of the tree
 * @param handlers the node handlers to execute the tree with, should be at least the count of the largest node type enumeration value
 * @param passthrough a passthrough to the handler
 * @return an error code
 */
extern gen_error_t gen_tree_run(gen_node_t* const restrict root, const gen_node_handler_t* const restrict handlers, void* const restrict passthrough);

/**
 * Built in node type for the apptree root
 */
#define GEN_ROOT_TYPE 0ul

#endif
