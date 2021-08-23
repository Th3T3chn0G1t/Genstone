# Genesis Engine
### An approach to app engine development with a more generally applicable paradigm

#### Note
This document is an explanation of ideas behind Genesis and their practical implications

For development information see the `development/` folder
For contribution information see the `contributing/` folder

## About

Genesis is an application engine attempting to use a different paradigm that can be more generally applied to the entire engine API, and indeed application design in general if I find the system to be effective, in order to create an "elegant" solution to the APIs design and interface.

The basic concept is that Genesis applications consist of a tree, where each node has a type. For each node type there is a global callback that handles updating nodes every time the application tree is updated.
This effectively produces a sort of "retained mode" application design paradigm where data is set to the tree before runtime.

In principle, most applications written in a app engine can be reduced to this paradigm in that there is a tree of data in the form of scene graphs and their contents, and code applied to subsets of those graphs and their data in components. That being said, the reduction of the entire runtime to this paradigm is not so easily translatable.

As for some more tangible ideas that can power this application design paradigm, it is extremely simple to illustrate the core concepts of this paradigm in code like so for example:
```
struct node_t;
typedef struct node_t {
    unsigned long type;
    unsigned long size;
    void* data;
    
    unsigned long n_children;
    struct node_t* children;
} node_t;

typedef void (*node_handler_t)(node_t*);
```
The application tree can be constructed from `node_t`s and the `node_handler_t`s don't at their core require any extra metadata, though an implementation of the paradigm might choose to use mappings from type to handler instead of array indices.

So if one was to (for example) construct a traditional game entity in Genesis consisting of a transform, a renderer and a script - you could approach it like so:
```
// This probably wouldn't be an `enum` in a final application as it would remove some of the self-modifying capabilities of the application tree
enum {
    ROOT,
    TRANSFORM,
    RENDERER,
    SCRIPT,
    ENTITY
};

// Constructing the scene statically for illustration purposes
float transform[3][3] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } };

// The specifics of constructing a renderer are extremely use-case specific so this field is left as purely an abstract illustration
renderer_t renderer = { ... };
node_t renderer_node = { RENDERER, sizeof(renderer), /* This obviously won't work as is, but is purely written this way for ease of illustration */ renderer, 0, (void*) 0 };

// The script itself can be inferred from just a dummy-node with a tag, or you could attach public data to the node depending on your needs
node_t script_node = { SCRIPT, 0, (void*) 0, 0, (void*) 0 };

// The same can be said for the entity, a simple tagged dummy-node can suffice, or you could attach entity metadata to the node
node_t entity_children[] = { transform_node, renderer_node, script_node };

// We can now insert our entity into the scene
node_t scene[] = {
    { ENTITY, 0, (void*), sizeof(entity_children) / sizeof(entity_children[0]), entity_children }
};

// And we can insert our scene directly into our root, if it makes sense to do so for our application
// In most usages you would probably end up with some sort of runtime layer with initialization steps or debug layers in nodes running above the scene
node_t root = { ROOT, 0, (void*) 0, sizeof(scene) / sizeof(scene[0]), scene };

void render(node_t* renderable) {
    draw(renderable->data);
}

void execute_script(node_t* script) {
    user_can_implement_this();
    or_there_could_be_builtins_aswell();
    it_depends_on_the_use_case();
}

// The actual specifics of how handler management is implemented and how the tree is run are up to the use case
node_handler_t handlers[] = {
    (node_handler_t) 0, // ROOT
    (node_handler_t) 0, // TRANSFORM
    render, // RENDERER
    execute_script, // SCRIPT
    (node_handler_t) 0 // ENTITY
};
```
So here we can see a super simplistic rundown of how Genesis could be used, and how the paradigms could very well be applied entirely generically, or absent of any actual engine code at all - in this case the only real "dependency" (ignoring the absense of actual implementation which would be in a real-world implementation of the example) is effectively just a few `typedef`s.

Ultimately, Genesis is going to be more than `typedef`s as I plan to provide a lot of boilerplate code for building and interacting with the application tree as well as more use-case specific code such as renderable node implementations or physics nodes, but in implementation-side I plan to use configurable modules so that no more than is strictly neccsary for a use of Genesis makes its way into the final executable.
