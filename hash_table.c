#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

// Constructor for a hash table
struct hash_table *hash_table_create(const size_t table_size) {
	struct hash_table *table = malloc(sizeof(struct hash_table));
	table->table = calloc(table_size, sizeof(struct hash_node));
	table->size = table_size;
	return table;
}

// Generates a hash given maximum number of bins and the keys (based on djb2 hash algorithm)
size_t hash(const size_t table_size, const char *key) {
	if (key == NULL) return 0;
	
	unsigned long hash = 5381;
	int c;
	while ((c = *key++) != '\0') hash = ((hash << 5) + hash) + c; // hash*33 + c
	return (size_t) (hash % table_size);
}

// Inserts a key and value into the hash table (0 if success, 1 if malloc error, 2 if NULL given)
int hash_table_insert(struct hash_table *table, const char *key, void *value) {
	if (table == NULL || key == NULL) return 2;
	
	struct hash_node *node = malloc(sizeof(struct hash_node));
	if (node == NULL) return 1; // malloc failed to allocate space, no insertion
	
	// Initialize new node
	node->key = malloc(MAX_KEY_SIZE); if (node->key == NULL) return 1;
	strncpy(node->key, key, MAX_KEY_SIZE - 1);
	node->value = value;
	node->next = NULL;
	
	// Insert at tail of singly-linked list
	struct hash_node *tail;
	
	size_t bin = hash(table->size, key);
	if (table->table[bin] == NULL) {	// Empty list
		table->table[bin] = node;
	} else {			// Not empty, find tail-end of list and add node there
		tail = table->table[bin];
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = node;
	}
	
	/* Somehow, this doesn't work...
	size_t bin = hash(table->size, key);
	struct hash_node *tail = table->table[bin];
	if (tail == NULL) {	// Empty list
		tail = node;
	} else {			// Not empty, find tail-end of list and add node there
		for (; tail->next != NULL; tail = tail->next)
		tail->next = node;
	}
	*/
	
	return 0;
}

// Searches given hash table for a particular key and returns it's contents (NULL if search failed)
void *hash_table_search(struct hash_table *table, const char *key) {
	if (table == NULL || key == NULL) return NULL;
	
	struct hash_node *tail;
	size_t bin = hash(table->size, key);

	for (tail = table->table[bin]; tail != NULL; tail = tail->next)
		if (strcmp(tail->key, key) == 0)
			return tail->value;
	return NULL;
}

// Removes a node in the hash table using its key and returns it's contents (NULL if removal failed)
void *hash_table_remove(struct hash_table *table, const char *key) {
	if (table == NULL || key == NULL) return NULL;

	struct hash_node *tail, *prev = NULL;
	size_t bin = hash(table->size, key);
	void *result;
	
	for (tail = table->table[bin]; tail != NULL; prev = tail, tail = tail->next) {
		if (strcmp(tail->key, key) == 0) {
			result = tail->value;
			
			// Set up 'next' connections between nodes in the list
			if (prev == NULL) {	// Is first node?
				if (tail->next == NULL) {	// Only node in the list
					table->table[bin] = NULL;
				} else {					// Delete the first node, but is not the only node
					table->table[bin] = tail->next;
				}
			} else {			// List is more than one node
				prev->next = tail->next;
			}
			
			// Remove node
			free(tail->key);
			free(tail);
			
			return result;
		}
	}
	return NULL;
}

// Destructor for a hash table, with a destructor to use for the data within
void hash_table_free(struct hash_table *table, void (*data_free)(void *)) {
	if (table == NULL || data_free == NULL) return;
	
	struct hash_node *tail, *to_free;
	for (size_t i = 0; i < table->size; i++) {
		tail = table->table[i];
		
		// Free every node in the singly-linked list
		while (tail != NULL) {
			to_free = tail;
			tail = tail->next;
			free(to_free->key);
			(*data_free)(to_free->value);
			free(to_free);
		}
	}
	free(table->table);
	free(table);
}
