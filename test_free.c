#include <stdio.h>
#include <stdlib.h>

struct blah_node {
	struct blah_node *n;
	int a;
};

struct blah {
	struct blah_node *n;
	int a;
	char b;
};

struct blah *blah_create() {
	struct blah *b = malloc(sizeof(struct blah));
	b->n = malloc(sizeof(struct blah_node));
	return b;
}

void blah_node_free(struct blah_node *n, void (*data_free)(void *)) {
	if (n == NULL) return;
	
	printf("Freeing blah_node\n");
	if (n->n != NULL) (*data_free)((void *) n->n);
	free(n);
}

void _blah_node_free(void *node) {
	blah_node_free((struct blah_node *) node, free);
}

void blah_free(struct blah *b, void (*data_free)(void *)) {
	if (b == NULL) return;

	printf("Freeing blah\n");
	if (b->n != NULL) (*data_free)((void *) b->n);
	free(b);
}

int main() {
	struct blah *b = blah_create();
	
	blah_free(b, _blah_node_free);
	return 0;
}
