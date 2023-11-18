#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 100000

//Hash item HI
typedef struct HI {
  char *key;
  unsigned int *count;
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
HI *hash_item_pair(const char *key, unsigned int *value){
  HI * hash_item = malloc(sizeof(HI) * 1);
  hash_item->key = malloc(strlen(key) +1);
  hash_item->count = malloc(sizeof(value));
  strcpy(hash_item->key, key);
  hash_item->count = value;
  hash_item->next = NULL;

  return hash_item;
}

//set item
void hash_table_set(HT *hash_table, const char *key){
  unsigned int index = hash(key);
  unsigned int *count;
  *count = 1;
  HI *hash_item = hash_table->item[index];

  if (hash_item == NULL){
    //primeira ocorrencia da palavra count = 1;
    hash_table->item[index] = hash_item_pair(key, count);
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

  prev->next = hash_item_pair(key, count);
}

unsigned int hash_table_get(HT *hash_table, const char *key){
  unsigned int index = hash(key);

  HI *hash_item = hash_table->item[index];

  if(hash_item == NULL){
    return 0;
  }

  while(hash_item != NULL){
    if(strcmp(hash_item->key, key) == 0){
      return *hash_item->count;
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

// Function to check if a character is punctuation
int isPunctuation(char ch) {
    return (ch == '.' || ch == ',' || ch == ';' || ch == ':' || ch == '!' || ch == '?' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '\"' || ch == '\'');
}

// Function to check if a character is a space or hyphen
int isSeparator(char ch) {
    return (ch == ' ' || ch == '-');
}

int main(int argc, char const *argv[])
{

  unsigned int i = 0;
  char *freq_word = "--freq-word";

  //Searching for number of ocurrencies of word in file
  if(strcmp(argv[1], freq_word) == 0){
    //Getting parameters
    const char *word_to_find = tolowerstr(argv[2]);
    const char *file_name = argv[3];
    char ch;
    char word[32];
    char wordsArray[100000][32];
    int wordCount = 0;
    unsigned int i = 0;

    HT *hash_table = create_hash_table();

    //setting file
    FILE *file = fopen(file_name, "r");

    if(file == NULL){
      printf("Erro ao abrir arquivo %s", file_name);
      return 0;
    }

    // Read the file character by character
    while ((ch = fgetc(file)) != EOF) {
        if(isalpha(ch)) {
            // Convert the character to lowercase and add it to the current word
            word[i] = tolower(ch);
            i++;
        } else if (i > 1) {
            word[i] = '\0'; // Null-terminate the word
            hash_table_set(hash_table, word);
            //strcpy(wordsArray[wordCount], word);
            //wordCount++;
            i = 0; // Reset the index for the next word
        } else if (isSeparator(ch)) {
            // If we encounter a space or hyphen, treat it as a separator
            i = 0;
        }
        // Ignore punctuation and reset the index for the next word
        if (isPunctuation(ch)) {
            i = 0;
        }
    }

    //printf("Palavra: %s | ocorrencias: %d", word_to_find, hash_table_get(hash_table, word_to_find));

    // // Close the file
    // fclose(file);

    // // Display the words
    // printf("Words in the file (ignoring words with less than 2 characters and punctuation):\n");
    // for (int j = 0; j < wordCount; j++) {
    //     printf("%s\n", wordsArray[j]);
    // }

    return 0;
  }

}