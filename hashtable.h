#ifndef HASHTABLE_H
#define HASHTABLE_H

#define INIT_CAPACITY 4

#include <stdbool.h>
#include <stddef.h>

typedef struct {
  char *key;
  void *value;
} HashTableEntry;

typedef struct HashTableNode {
  struct HashTableNode *next_node;
  HashTableEntry *entry;
} HashTableNode;

typedef struct {
  HashTableNode **values;
  size_t capacity;
  size_t length;
} HashTable;

HashTable *new_hash_table();
void destroy_hash_table(HashTable *hash_table);

void resize(HashTable *hash_table, size_t capacity);
void clear(HashTable *hash_table);
void remove_key_value(HashTable *hash_table, const char *key);
void insert_key_value(HashTable *hash_table, const char *key, void *value);
void *get_value(HashTable *hash_table, const char *key);
HashTableEntry **get_entries(HashTable *hash_table);
char **get_keys(HashTable *hash_table);
bool has_key(HashTable *hash_table, const char *key);

#endif
