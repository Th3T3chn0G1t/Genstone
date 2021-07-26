#include <genesis.h>
#include <gencommon.h>

static int root_data_storage;
static gen_node_t root;

static unsigned long type_loader(const char* type_name) {
    const static char ROOT_TYPE_NAME[] = "ROOT";
    if(!strncmp(type_name, ROOT_TYPE_NAME, sizeof(ROOT_TYPE_NAME))) return GEN_ROOT_TYPE;

    return ULONG_MAX;
}

static void root_data_loader(gen_node_t* node, const char* data_string) {
    node->size = sizeof(int);
    root_data_storage = atoi(data_string);
    node->data = &root_data_storage;
}

int main() {
    char* source = "ROOT\n127\n\n";
    gen_node_loader_data_handler_t data_handlers[] = { root_data_loader };
    gen_node_load(&root, source, type_loader, data_handlers);

    assert(root.type == GEN_ROOT_TYPE);
    assert(*(int*) root.data == 127);
}
