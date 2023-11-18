#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 100000

//Hash item HI
typedef struct HI {
  char *key;
  unsigned int count;
  struct HI *next;
} HI;

//Hash Table HT
typedef struct HT {
  HI **item;
} HT;

//Hash function
unsigned int hash(const char *key){
  unsigned long int value = 0;
  unsigned int i;
  unsigned int key_len = strlen(key);

  for(i = 0; i < key_len; ++i){
    value = value * 37 + key[i];
  }

  value = value % TABLE_SIZE;
  return value;
}

//Create Table
HT *create_hash_table(){
  HT *hash_table = malloc(sizeof(HT) * 1);

  hash_table->item = malloc(sizeof(HI*) * TABLE_SIZE);

  int i = 0;
  for(; i < TABLE_SIZE; i++){
    hash_table->item[i] = NULL;
  }

  return hash_table;
}

//create pair
HI *hash_item_pair(const char *key, unsigned int value){
  HI * hash_item = malloc(sizeof(HI) * 1);
  hash_item->key = malloc(strlen(key) +1);

  strcpy(hash_item->key, key);
  hash_item->count = value;
  hash_item->next = NULL;

  return hash_item;
}

//set item
void hash_table_set(HT *hash_table, const char *key, unsigned int value){
  unsigned int index = hash(key);

  HI *hash_item = hash_table->item[index];

  if (hash_item == NULL){
    //primeira ocorrencia da palavra count = 1;
    hash_table->item[index] = hash_item_pair(key, 1);
  }

  HI *prev;

  while(hash_item != NULL){
    if(strcmp(hash_item->key, key) == 0){
      hash_item->count++;
      return;
    }
    prev = hash_item;
    hash_item = prev->next; 
  }

  prev->next = hash_item_pair(key, 1);
}

unsigned int hash_table_get(HT *hash_table, char *key){
  unsigned int index = hash(key);

  HI *hash_item = hash_table->item[index];

  if(hash_item == NULL){
    return 0;
  }

  while(hash_item != NULL){
    if(strcmp(hash_item->key, key) == 0){
      return hash_item->count;
    }
    hash_item = hash_item->next;
  }

  return 0;
}

char *tolowerstr(const char *s){
  char *lower_s = (char *)malloc((strlen(s)+1)*sizeof(char));
  char *ls = lower_s;
  while(*s){
    *lower_s = tolower(*s++);
    lower_s++;
  }
  return ls;
}

int main(int argc, char const *argv[])
{
  unsigned int i = 0;
  char *freq_word = "--freq-word";

  //Searching for number of ocurrencies of word in file
  if(strcmp(argv[1], freq_word) == 0){
    const char *word = tolowerstr(argv[2]);
    printf("%s", word);
    const char *file_name = argv[3];
  }

}