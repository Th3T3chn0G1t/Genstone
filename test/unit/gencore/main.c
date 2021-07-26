#include <genesis.h>
#include <gencommon.h>

static int root_data_storage;
static gen_node_t root;

const static char ROOT_TYPE_NAME[] = "ROOT";
static unsigned long type_loader(const char* type_name) {
    if(!strncmp(type_name, ROOT_TYPE_NAME, sizeof(ROOT_TYPE_NAME))) return GEN_ROOT_TYPE;

    return ULONG_MAX;
}

static void root_data_loader(gen_node_t* node, const char* data_string) {
    node->size = sizeof(int);
    root_data_storage = atoi(data_string);
    node->data = &root_data_storage;
}

static void type_exporter(char* output, unsigned long type) {
    switch(type) {
        case GEN_ROOT_TYPE: {
            sprintf(output, "%s\n", ROOT_TYPE_NAME);
            break;
        }
    }
}

static void root_data_exporter(char* output, gen_node_t* node) {
    sprintf(output + (4 + 1 /* ROOT\n */), "%i\n", *(int*) node->data);
}

int main() {
    char* source = strdup("ROOT\n127\n\n");
    gen_node_loader_data_handler_t data_handlers[] = { root_data_loader };
    gen_node_load(&root, source, type_loader, data_handlers);

    assert(root.type == GEN_ROOT_TYPE);
    assert(*(int*) root.data == 127);

    gen_node_exporter_data_handler_t export_handlers[] = { root_data_exporter };
    size_t output_length = (4 + 1 /* ROOT\n */) + (3 + 1 /* 127\n */) + (1 /* \n */) + (1 /* \0 */);
    char* output = malloc(output_length);
    gen_node_export(output, &root, type_exporter, export_handlers);
    output[output_length - 2] = '\n'; // In an actual impl the child nodes would be loaded
    output[output_length - 1] = '\0';

    assert(!strcmp(output, source));

    free(source);
}
