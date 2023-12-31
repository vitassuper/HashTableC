#include "hashtable.h"
#include <assert.h>
#include <stddef.h>
#include <stdio.h>

int main() {
  HashTable *hash_table = new_hash_table();

  int num0 = 100;
  int num1 = 1;
  int num2 = 2;

  insert_key_value(hash_table, "extra_key", &num0);

  insert_key_value(hash_table, "key1", &num0);
  insert_key_value(hash_table, "key2", &num0);
  insert_key_value(hash_table, "key3", &num0);
  insert_key_value(hash_table, "key4", &num1);
  if (!has_key(hash_table, "key4"))
    insert_key_value(hash_table, "key4", &num2);

  assert((*(int *)get_value(hash_table, "key4")) == num1);
  assert(get_value(hash_table, "key_4") == NULL);

  insert_key_value(hash_table, "key4", &num0);

  assert((*(int *)get_value(hash_table, "key4") == num0));

  char **keys = get_keys(hash_table);

  printf("Keys: \n");

  for (size_t i = 0; i < hash_table->length; i++) {
    printf("%s\n", keys[i]);
  }

  printf("\n");

  for (size_t i = 0; i < hash_table->capacity; i++) {
    HashTableNode *node = hash_table->values[i];

    if (node) {
      do {
        printf("%s\t", node->entry->key);
        node = node->next_node;
      } while (node);

      printf("\n");
    }
  }

  assert(get_value(hash_table, "extra_key"));
  remove_key_value(hash_table, "extra_key");
  assert(get_value(hash_table, "extra_key") == NULL);

  destroy_hash_table(hash_table);

  return 0;
}
