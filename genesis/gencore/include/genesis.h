struct gen_node_t;
typedef struct {
    unsigned long type;
    unsigned long size;
    void* data;
    
    unsigned long n_children;
    struct gen_node_t* children;

    struct gen_node_t* parent;
} gen_node_t;

typedef void (*gen_node_handler_t)(gen_node_t*);

#define GEN_ROOT_TYPE 0

