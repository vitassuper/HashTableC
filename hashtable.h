#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 4

typedef struct {
  char *key;
  void *value;
} HashTableItem;

typedef struct Node {
  HashTableItem *item;
  struct Node *next;
} HashTableNode;

typedef struct {
  size_t size;
  size_t length;
  HashTableNode **elements;
} HashTable;

HashTable *hash_table_init();
bool hash_table_insert(HashTable *ht, char *key, void *value);
void *hash_table_get(HashTable *ht, char *key);
bool hash_table_has(HashTable *ht, char *key);
HashTableItem **hash_table_items(HashTable *ht);
void hash_table_print(HashTable *ht);
void hash_table_remove(HashTable *ht, char *key); // tood
void hash_table_clear(HashTable *ht);             // todo
void hash_table_free(HashTable *ht);

#endif /* HASHTABLE_H */
