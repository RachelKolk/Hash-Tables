#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(capacity * sizeof(HashTable));

  ht->storage = calloc(capacity, sizeof(LinkedPair *));
  ht->capacity = capacity;

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // hash our key to get the index
  int index = hash(key, ht->capacity);
  // create a new LinkedPair
  LinkedPair *new_pair = create_pair(key, value);
  // set the pointer for the stored_pair
  LinkedPair *stored_pair = ht->storage[index];
  // check to see if the bucket is full
  if (stored_pair != NULL) {
    // check to see if the key is different than the existing key
    if (strcmp(key, stored_pair->key) != 0) {
      // if so check to see if we are at the tail
      if (stored_pair->next == NULL) {
        // if so set the new_pair to be next
        stored_pair->next = new_pair;
        new_pair->next = NULL;
      }
      // otherwise we traverse the LinkedList to find the tail
      else {
        for (int i = 0; i < ht->capacity; i++) {
          // when we find it
          if (ht->storage[i]->next == NULL) {
            // we set its next to the new_pair
            ht->storage[i]->next = new_pair;
            new_pair->next = NULL;
          }
        }
      }
    }
    // if the key is the same we print a warning
    else {
      fprintf(stderr, "You are overwriting the current value.");
      // destroy the current pair
      destroy_pair(stored_pair);
      // and set the new_pair as the stored_pair
      ht->storage[index] = new_pair;
    }
  }
  // if the bucket is empty
  else {
    // we simply set the new pair
    ht->storage[index] = new_pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  // hash our key to get the index
  int index = hash(key, ht->capacity);
  // set the pointer to the stored_pair to be the storage index
  LinkedPair *stored_pair = ht->storage[index];
  // setting a boolean to toggle whether the key has been located or not
  int found = 0;
  // see if there is an existing element in the bucket index
  //if yes, see if the keys match
  if (ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0) {
    // use a reference so we can still delete correctly after reassigning new value
    LinkedPair *pair_to_remove = stored_pair;
    // reassign the next pointer
    stored_pair = stored_pair->next;
    // remove the old pair
    destroy_pair(pair_to_remove);
    // set the removed storage to NULL
    ht->storage[index] = NULL;
  }
  else {
    while (stored_pair && !found) {
      if (ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0) {
        found = 1;
        
        // use a reference so we can still delete correctly after reassigning new value
        LinkedPair *pair_to_remove = stored_pair;
        // reassign the next pointer
        stored_pair = stored_pair->next;
        // remove the old pair
        destroy_pair(pair_to_remove);
        // set the removed storage to NULL
        ht->storage[index] = NULL;
      } else {
          // previous = stored_pair;
          stored_pair = stored_pair->next;
      }
    
    }
  } 
 
  // otherwise we print an error
  fprintf(stderr, "Unable to remove that entry");
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // hash our key to get the index
  int index = hash(key, ht->capacity);
  // set the pointer to the stored_pair to be the storage index
  LinkedPair *stored_pair = ht->storage[index];
  // check if there's a valid element in our bucket
  // if yes, compare the keys
  if (stored_pair != NULL && strcmp(stored_pair->key, key) == 0) {
    // if they match, return the value
    return stored_pair->value;
  }
  // otherwise we traverse the LinkedList to find the key
  else {
    while (stored_pair) {
      // check if there's a valid element in our bucket
      // if yes, compare the keys
      if (stored_pair != NULL && strcmp(stored_pair->key, key) == 0) {
        // if they match, return the value
        return stored_pair->value;
      }
      // otherwise we move the stored_pair value to the next pair in the linkeList
      stored_pair = stored_pair->next;
    }
  }
  //otherwise we print an error
  fprintf(stderr, "Unable to find that entry.");
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
 

  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      // use a reference so we can still delete correctly after reassigning new value
        LinkedPair *pair_to_remove = ht->storage[i];
        // reassign the next pointer
        // LinkedPair* stored_pair = ht->storage[i]->next;
        // remove the old pair
        destroy_pair(pair_to_remove);
  }

  free(ht->storage);

  }
  free(ht);  
}


/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  
  HashTable *new_ht = create_hash_table(2 * ht->capacity);
  printf("New Capacity %d", new_ht->capacity);
  int i = 0;
  while (ht->storage[i] != NULL) {
   
    LinkedPair *stored_pair = ht->storage[i];
    printf("\nInserting %s - %s", stored_pair->key, stored_pair->value);
    hash_table_insert(new_ht, stored_pair->key, stored_pair->value);
  
    i++;
   // }
  }
  free(ht->storage);
  free(ht);

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
