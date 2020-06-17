#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>

#define MAX_KEY_SIZE 256

struct hash_node {
	char *key;
	void *value;
	struct hash_node *next;
};

struct hash_table {
	struct hash_node **table;
	size_t size;
};

// Constructor for a hash table
struct hash_table *hash_table_create(const size_t table_size);

// Generates a hash given maximum number of bins and the keys (based on djb2 hash algorithm)
size_t hash(const size_t table_size, const char *key);

// Inserts a key and value into the hash table (0 if success, 1 if malloc error, 2 if NULL given)
int hash_table_insert(struct hash_table *table, const char *key, void *value);

// Searches given hash table for a particular key and returns it's contents (NULL if search failed)
void *hash_table_search(struct hash_table *table, const char *key);

// Removes a node in the hash table using its key and returns it's contents (NULL if removal failed)
void *hash_table_remove(struct hash_table *table, const char *key);

// Destructor for a hash table, with a destructor to use for the data within
void hash_table_free(struct hash_table *table, void (*data_free)(void *));

#endif /* HASH_TABLE_H */

