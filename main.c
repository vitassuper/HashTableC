#include "hashtable.h"
#include <assert.h>
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

  char **keys = get_keys(hash_table);

  for (size_t i = 0; i < hash_table->length; i++) {
    printf("%s\n", keys[i]);
  }

  assert(get_value(hash_table, "extra_key"));
  remove_key_value(hash_table, "extra_key");
  assert(get_value(hash_table, "extra_key") == NULL);

  destroy_hash_table(hash_table);

  return 0;
}
