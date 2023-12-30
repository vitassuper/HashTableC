#include "hashtable.h"
#include <assert.h>
#include <stdio.h>

int main() {
  HashTable *hash_table = hash_table_init();

  int num0 = 100;
  int num1 = 1;
  int num2 = 2;

  hash_table_insert(hash_table, "extra_key", &num0);

  hash_table_insert(hash_table, "key1", &num0);
  hash_table_insert(hash_table, "key2", &num0);
  hash_table_insert(hash_table, "key3", &num0);
  hash_table_insert(hash_table, "key4", &num1);
  if (!hash_table_has(hash_table, "key4"))
    hash_table_insert(hash_table, "key4", &num2);

  assert((*(int *)hash_table_get(hash_table, "key4")) == num1);
  assert(hash_table_get(hash_table, "key_4") == NULL);

  HashTableItem **items = hash_table_items(hash_table);

  for (size_t i = 0; i < hash_table->length; i++) {
    printf("%s\n", items[i]->key);
  }

  assert(hash_table_get(hash_table, "extra_key"));
  hash_table_remove(hash_table, "extra_key");
  assert(hash_table_get(hash_table, "extra_key") == NULL);

  hash_table_free(hash_table);

  return 0;
}
