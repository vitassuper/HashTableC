#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct HashTableNode {
  struct HashTableNode *next_node;
  char *key;
  void *value;
} HashTableNode;

typedef struct {
  HashTableNode **values;
  size_t capacity;
  size_t length;
} HashTable;

unsigned long hash(const char *str);

HashTable *new_hash_table();
void destroy_hash_table(HashTable *hash_table);
void remove_key_value(HashTable *hash_table, const char *key);
void insert_key_value(HashTable *hash_table, const char *key, void *value);
void *get_value(HashTable *hash_table, const char *key);
char **get_keys(HashTable *hash_table);
bool has_key(HashTable *hash_table, const char *key);

HashTableNode *create_new_node(const char *key, void *value);
HashTableNode *get_node(HashTable *hash_table, const char *key);
void destroy_node(HashTableNode *node);
void destroy_node_chain(HashTableNode *node);

#endif
