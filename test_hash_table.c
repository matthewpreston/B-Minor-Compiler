#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

#define HASH_BINS 2

void sanity_check(struct hash_table *table) {
	struct hash_node *tail;
	int count = 0;
	
	for (size_t i = 0; i < table->size; i++) {
		count = 0;
		printf("Bin %zu: ", i);
		for (tail = table->table[i]; tail != NULL; tail = tail->next) {
			if (count != 0) printf("->");
			printf("%s", tail->key);
			count++;
		}
		printf((count == 0) ? "NULL" : "->NULL");
		printf("\n");
	}
}

int main(int argc, char *argv[]) {
	struct hash_table *table = hash_table_create(HASH_BINS);
	
	char *k1 = "A", *k2 = "B", *k3 = "Q", *k4 = "C", *k5 = "D";
	void *v1 = malloc(sizeof(int)); *((int *) v1) = 5;
	void *v2 = malloc(sizeof(int)); *((int *) v2) = 32;
	void *v3 = malloc(sizeof(int)); *((int *) v3) = 84932;
	void *v4 = malloc(sizeof(int)); *((int *) v4) = 84932;
	void *v5 = malloc(sizeof(int)); *((int *) v4) = 123;
	
	printf("Initialization\n");
	sanity_check(table);
	
	printf("\nInserting\n");
	printf("Inserting k: %s\n", k1); hash_table_insert(table, k1, v1); sanity_check(table);
	printf("Inserting k: %s\n", k2); hash_table_insert(table, k2, v2); sanity_check(table);
	printf("Inserting k: %s\n", k3); hash_table_insert(table, k3, v3); sanity_check(table);
	printf("Inserting k: %s\n", k4); hash_table_insert(table, k4, v4); sanity_check(table);
	printf("Inserting k: %s\n", k5); hash_table_insert(table, k5, v5); sanity_check(table);
	
	printf("\nSearching\n");
	printf("Search k: %s; Got v: %d\n", k1, *((int *) hash_table_search(table, k1))); sanity_check(table);
	printf("Search k: %s; Got v: %d\n", k2, *((int *) hash_table_search(table, k2))); sanity_check(table);
	printf("Search k: %s; Got v: %d\n", k3, *((int *) hash_table_search(table, k3))); sanity_check(table);
	printf("Search k: %s; Got v: %d\n", k4, *((int *) hash_table_search(table, k4))); sanity_check(table);
	printf("Search k: %s; Got v: %d\n", k5, *((int *) hash_table_search(table, k5))); sanity_check(table);
	
	printf("\nRemoving\n");
	printf("Removing k: %s\n", k2); hash_table_remove(table, k2); sanity_check(table);
	printf("Removing k: %s\n", k1); hash_table_remove(table, k1); sanity_check(table);
	printf("Removing k: %s\n", k3); hash_table_remove(table, k3); sanity_check(table);
	printf("Removing k: %s\n", k4); hash_table_remove(table, k4); sanity_check(table);
	
	printf("\nClean-up\n");
	hash_table_free(table, free); // Also frees v5 since it was left in the table
	free(v1);
	free(v2);
	free(v3);
	free(v4);
	
	printf("Done\n");
	return 0;
}
