#include "hashtable.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Internal functions

// djb2 hash function
static unsigned long hash(const char *str) {
  unsigned long hash = 5381;
  int c;

  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash;
}

static void init_hash_table(HashTable *hash_table) {
  hash_table->capacity = INIT_CAPACITY;
  hash_table->values =
      (HashTableNode **)malloc(sizeof(HashTableNode *) * INIT_CAPACITY);
  hash_table->length = 0;
}

static HashTableNode *init_new_node() {
  HashTableNode *new_node = (HashTableNode *)malloc(sizeof(HashTableNode));
  HashTableEntry *new_entry = (HashTableEntry *)malloc(sizeof(HashTableEntry));
  new_node->next_node = NULL;
  new_node->entry = new_entry;

  return new_node;
}

static HashTableNode *create_new_node(const char *key, void *value) {
  HashTableNode *new_node = init_new_node();

  new_node->entry->key = (char *)malloc(sizeof(char) * (strlen(key) + 1));
  strcpy(new_node->entry->key, key);
  new_node->entry->value = value;

  return new_node;
}

static HashTableNode *get_node(HashTable *hash_table, const char *key) {
  unsigned long hash_number = hash(key);
  size_t index = hash_number % hash_table->capacity;

  HashTableNode *node = hash_table->values[index];

  while (node) {
    if (strcmp(node->entry->key, key) == 0) {
      return node;
    }

    node = node->next_node;
  }

  return NULL;
}

static void destroy_node(HashTableNode *node) {
  free(node->entry->key);
  free(node->entry);
  free(node);
}

static void destroy_node_chain(HashTableNode *node) {
  HashTableNode *next_node;

  while (node) {
    next_node = node->next_node;
    destroy_node(node);
    node = next_node;
  }
}
// End Internal functions

HashTable *new_hash_table() {
  HashTable *hash_table = (HashTable *)malloc(sizeof(HashTable));
  init_hash_table(hash_table);

  return hash_table;
}

void resize(HashTable *hash_table, size_t capacity) {
  HashTableNode **values = malloc(sizeof(HashTableNode *) * capacity);
  HashTableEntry **entries = get_entries(hash_table);

  for (size_t i = 0; i < hash_table->length; i++) {
    unsigned long hash_number = hash(entries[i]->key);
    size_t index = hash_number % capacity;

    HashTableNode **existed_node = &(values[index]);

    while (*existed_node) {
      existed_node = &((*existed_node)->next_node);
    }

    *existed_node = (HashTableNode *)malloc(sizeof(HashTableNode));
    (*existed_node)->entry = entries[i];
  }

  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];
    HashTableNode *next_node;

    while (node) {
      next_node = node->next_node;
      free(node);
      node = next_node;
    }
  }

  free(entries);
  free(hash_table->values);
  hash_table->values = values;
  hash_table->capacity = capacity;
}

void clear(HashTable *hash_table) {
  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];

    if (node) {
      destroy_node_chain(node);
    }
  }

  free(hash_table->values);
  init_hash_table(hash_table);
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

void remove_key_value(HashTable *hash_table, const char *key) {
  unsigned long hash_number = hash(key);
  size_t index = hash_number % hash_table->capacity;

  HashTableNode *node = hash_table->values[index];

  if (!node) {
    return;
  }

  HashTableNode *prev = NULL;

  while (node) {
    if (strcmp(node->entry->key, key) == 0) {
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

HashTableEntry **get_entries(HashTable *hash_table) {
  HashTableEntry **entries =
      (HashTableEntry **)malloc(sizeof(HashTableEntry *) * hash_table->length);
  size_t index = 0;

  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];

    while (node) {
      entries[index++] = node->entry;
      node = node->next_node;
    }
  }

  return entries;
}

void insert_key_value(HashTable *hash_table, const char *key, void *value) {
  if (hash_table->capacity * 0.7 < hash_table->length + 1) {
    resize(hash_table, hash_table->capacity * 2);
  }

  unsigned long hash_number = hash(key);
  size_t index = hash_number % hash_table->capacity;

  HashTableNode **current_node = &(hash_table->values[index]);

  while (*current_node) {
    if (strcmp((*current_node)->entry->key, key) == 0) {
      (*current_node)->entry->value = value;

      return;
    }

    current_node = &((*current_node)->next_node);
  }

  *current_node = create_new_node(key, value);
  hash_table->length++;
}

char **get_keys(HashTable *hash_table) {
  char **keys = (char **)malloc(sizeof(char *) * hash_table->length);
  size_t index = 0;

  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];

    while (node) {
      keys[index++] = node->entry->key;

      node = node->next_node;
    }
  }

  return keys;
}

bool has_key(HashTable *hash_table, const char *key) {
  if (get_node(hash_table, key)) {
    return true;
  }

  return false;
}

void *get_value(HashTable *hash_table, const char *key) {
  HashTableNode *node = get_node(hash_table, key);

  if (node) {
    return node->entry->value;
  }

  return NULL;
}
