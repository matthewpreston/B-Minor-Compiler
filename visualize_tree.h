#ifndef VISUALIZE_TREE_H
#define VISUALIZE_TREE_H

#include "ast.h"

// Creates a SVG file based on the given abstract syntax tree
// Returns number of errors whilst making graph
int visualize_tree(struct decl *d, char *outputSVG);

#endif /* VISUALIZE_TREE_H */

