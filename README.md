![Genstone Logo](brand/Logo256x256.png)

# Genstone
*A C System Layer*

Genstone is a minimal systems layer for creating portable applications and
providing shared infrastructure.

It uses a layered backends system to ensure maximal code sharing and tries to
be as in-house as possible for the sake of easy backend implementation and
increasing ease of use on bare-metal.

The Makefile system is designed to make toolchains and targets easy to specify
and can be used as a submodule to specify the subproject in the same make
context as Genstone.

Genstone only currently supports the Clang C compiler.
