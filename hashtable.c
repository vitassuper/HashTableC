#include "hashtable.h"

HashTable *hash_table_init() {
  HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
  ht->size = INIT_SIZE;
  ht->length = 0;
  ht->elements = (HashTableNode **)malloc(ht->size * sizeof(HashTableNode *));
  return ht;
}

// djb2 hash function
static unsigned long hash(const char *str) {
  unsigned long hash = 5381;
  int c;
  while ((c = *str++))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  return hash;
}

static HashTableItem *create_item(char *key, void *value) {
  HashTableItem *item = (HashTableItem *)malloc(sizeof(HashTableItem));
  item->key = key;
  item->value = value;
  return item;
}

static HashTableNode *create_node(HashTableItem *item) {
  HashTableNode *node = (HashTableNode *)malloc(sizeof(HashTableNode));
  node->item = item;
  node->next = NULL;
  return node;
}

static bool insert(HashTableNode **elements, size_t size, HashTableItem *item) {
  size_t index = hash(item->key) % size;
  HashTableNode *node = create_node(item);
  HashTableNode *current = elements[index];
  HashTableNode *prev = NULL;
  HashTableNode *replace = NULL;
  while (current != NULL) {
    prev = current;
    if (strcmp(prev->item->key, node->item->key) == 0) {
      replace = prev;
      break;
    }
    current = current->next;
  }
  if (replace) {
    replace->item = node->item;
    return false;
  }
  if (prev == NULL)
    elements[index] = node;
  else
    prev->next = node;
  return true;
}

static void resize(HashTable *ht) {
  size_t new_size = ht->size * 2;
  HashTableNode **elements =
      (HashTableNode **)malloc(new_size * sizeof(HashTableNode *));
  for (size_t i = 0; i < ht->size; i++) {
    HashTableNode *node = ht->elements[i];
    while (node != NULL) {
      insert(elements, ht->size, node->item);
      HashTableNode *prev = node;
      node = node->next;
      free(prev);
    }
  }
  free(ht->elements);
  ht->elements = elements;
  ht->size = new_size;
}

bool hash_table_insert(HashTable *ht, char *key, void *value) {
  if ((ht->size * 0.7) < (ht->length + 1))
    resize(ht);
  HashTableItem *item = create_item(key, value);
  if (insert(ht->elements, ht->size, item))
    ht->length++;
  return true;
}

HashTableNode *get_node(HashTable *ht, char *key) {
  for (size_t i = 0; i < ht->size; i++) {
    HashTableNode *node = ht->elements[i];
    while (node != NULL) {
      if (strcmp(node->item->key, key) == 0)
        return node;
      node = node->next;
    }
  }
  return NULL;
}

void *hash_table_get(HashTable *ht, char *key) {
  HashTableNode *node = get_node(ht, key);
  if (node) {
    return node->item->value;
  }
  return NULL;
}

bool hash_table_has(HashTable *ht, char *key) {
  return get_node(ht, key) != NULL;
}

HashTableItem **hash_table_items(HashTable *ht) {
  size_t length = ht->length;
  HashTableItem **items =
      (HashTableItem **)malloc(sizeof(HashTableItem *) * length);
  size_t index = 0;
  for (size_t i = 0; i < ht->size; i++) {
    HashTableNode *node = ht->elements[i];
    while (node != NULL) {
      items[index++] = node->item;
      node = node->next;
    }
  }
  return items;
}

void hash_table_print(HashTable *ht) {
  for (size_t i = 0; i < ht->size; i++) {
    HashTableNode *node = ht->elements[i];
    while (node != NULL) {
      printf("%zu %s %p\n", i, node->item->key, node);
      node = node->next;
    }
  }
}

void hash_table_remove(HashTable *ht, char *key) {
  HashTableNode *current = NULL;
  HashTableNode *prev = NULL;
  for (size_t i = 0; i < ht->size; i++) {
    current = ht->elements[i];
    prev = NULL;
    while (current != NULL) {
      if (strcmp(current->item->key, key) == 0) {
        if (prev == NULL)
          ht->elements[i] = current->next;
        else
          prev->next = current->next;
        free(current);
        ht->length--;
        return;
      }
      prev = current;
      current = current->next;
    }
  }
}

void clear(HashTable *ht) {
  for (size_t i = 0; i < ht->size; i++) {
    HashTableNode *node = ht->elements[i];
    while (node != NULL) {
      HashTableNode *prev = node;
      node = node->next;
      free(prev->item);
      free(prev);
    }
  }
  free(ht->elements);
}

void hash_table_clear(HashTable *ht) {
  clear(ht);
  ht->size = INIT_SIZE;
  ht->elements = (HashTableNode **)calloc(ht->size, sizeof(HashTableNode *));
  ht->length = 0;
}

void hash_table_free(HashTable *ht) {
  clear(ht);
  free(ht);
}
