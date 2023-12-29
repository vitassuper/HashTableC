#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_CAPACITY 5

HashTable *new_hash_table() {
  HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));

  hash_table->capacity = INIT_CAPACITY;
  hash_table->values =
      (HashTableNode **)malloc(sizeof(HashTableNode *) * INIT_CAPACITY);
  hash_table->length = 0;

  return hash_table;
}

void destroy_hash_table(HashTable *hash_table) {
  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];

    if (node) {
      destroy_node_chain(node);
    }
  }

  free(hash_table->values);
  free(hash_table);
}

void destroy_node(HashTableNode *node) {
  free(node->key);
  free(node);
}

void destroy_node_chain(HashTableNode *node) {
  HashTableNode *next_node;

  while (node) {
    next_node = node->next_node;
    destroy_node(node);
    node = next_node;
  }
}

void remove_key_value(HashTable *hash_table, char *key) {
  unsigned long hash_number = hash(key);
  size_t index = hash_number % hash_table->capacity;

  HashTableNode *node = hash_table->values[index];

  if (!node) {
    return;
  }

  HashTableNode *prev = NULL;

  while (node) {
    if (strcmp(node->key, key) == 0) {
      if (!prev) {
        hash_table->values[index] = node->next_node;
      } else {
        prev->next_node = node->next_node;
      }

      destroy_node(node);
      hash_table->length--;

      return;
    }

    prev = node;
    node = node->next_node;
  }
}

void insert_key_value(HashTable *hash_table, char *key, void *value) {
  unsigned long hash_number = hash(key);
  size_t index = hash_number % hash_table->capacity;

  HashTableNode *node = hash_table->values[index];

  if (node) {
    HashTableNode *prev = NULL;

    do {
      prev = node;

      if (strcmp(prev->key, key) == 0) {
        prev->value = value;

        return;
      }
    } while ((node = node->next_node));

    prev->next_node = create_new_node(key, value);
    hash_table->length++;

  } else {
    hash_table->values[index] = create_new_node(key, value);
    hash_table->length++;
  }
}

HashTableNode *create_new_node(char *key, void *value) {
  HashTableNode *new_node = (HashTableNode *)malloc(sizeof(HashTableNode));
  new_node->next_node = NULL;

  new_node->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(new_node->key, key);
  new_node->value = value;

  return new_node;
}

char **get_keys(HashTable *hash_table) {
  size_t length = hash_table->length;

  char **keys = (char **)malloc(sizeof(char *) * length);
  size_t index = 0;

  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];

    if (node) {
      do {
        keys[index++] = node->key;
      } while ((node = node->next_node));
    }
  }

  return keys;
}

HashTableNode *get_node(HashTable *hash_table, char *key) {
  unsigned long hash_number = hash(key);
  size_t index = hash_number % hash_table->capacity;

  HashTableNode *node = hash_table->values[index];

  if (node) {
    do {
      if (strcmp(node->key, key) == 0) {
        return node;
      }
    } while ((node = node->next_node));
  }

  return NULL;
}

bool has_key(HashTable *hash_table, char *key) {
  if (get_node(hash_table, key)) {
    return true;
  }

  return false;
}

void *get_value(HashTable *hash_table, char *key) {
  HashTableNode *node = get_node(hash_table, key);

  if (node) {
    return node->value;
  }

  return NULL;
}

// djb2 hash function
unsigned long hash(char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}
