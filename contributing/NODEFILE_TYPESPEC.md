`.nodefile`s represent a runtime application node on the filesystem.
`.nodefile`s have human-readable contents which is converted to runtime-ready data by a set of nodedata handlers which handle loading of a segment of a `.nodefile`.
`.nodefile`s can be generated using export handlers, which are similar to node handlers but return strings to populate the `.nodefile`

A `.nodefile` is of the following format:
```
TYPENAME\n # A name for the type of the node
DATA\n # A representation of the nodes data - usually a filepath
CHILD CHILD CHILD...\n # The node's children in the form of paths to their nodefiles
```

This is kept human-readable for the sake of contribution and ease of development.

For release build etc. you can also just opt to export binary data of nodes to file, this can also be done via export handlers.
