#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1000000

//Hash item HI
typedef struct HI {
  char *key;
  unsigned int count;
  struct HI *next;
} HI;

//Hash Table HT
typedef struct HT {
  HI **item;
  unsigned long int n_items;
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
  HT *hash_table = (HT*)malloc(sizeof(HT) * 1);

  hash_table->item = (HI**)malloc(sizeof(HI*) * TABLE_SIZE);

  int i = 0;
  for(; i < TABLE_SIZE; i++){
    hash_table->item[i] = NULL;
  }
  hash_table->n_items = 0;

  return hash_table;
}

//create pair
HI *hash_item_pair(const char *key, unsigned int value){
  HI * hash_item = (HI*)malloc(sizeof(HI) * 1);
  hash_item->key = malloc(strlen(key) + 1);
  hash_item->key = strdup(key);
  hash_item->count = value;
  hash_item->next = NULL;

  return hash_item;
}

//set item
void hash_table_set(HT *hash_table, const char *key){
  unsigned int index = hash(key);
  HI *hash_item = hash_table->item[index];

  if (hash_item == NULL){
    //primeira ocorrencia da palavra count = 1;
    hash_table->item[index] = hash_item_pair(key, 1);
    hash_table->n_items++;
    return;
  }

  HI *prev = (HI*)malloc(sizeof(HI) * 1);

  while(hash_item != NULL){
    if(strcmp(hash_item->key, key) == 0){
      hash_item->count++;
      return;
    }
    prev = hash_item;
    hash_item = prev->next; 
  }
  hash_table->n_items++;
  prev->next = hash_item_pair(key, 1);
}

unsigned int hash_table_get(HT *hash_table, const char *key){
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

// Function to check if a character is punctuation
int is_ponctuation(char ch) {
    return (ch == '.' || ch == ',' || ch == ';' || ch == ':' || ch == '!' || ch == '?' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '\"' || ch == '\'');
}

// Function to check if a character is a space or hyphen
int is_separator(char ch) {
    return (ch == ' ' || ch == '-');
}

HT *generate_hash_for_file(HT *hash_table, const char *file_name){
  FILE *file = fopen(file_name, "r");
  char ch;
  char word[40];
  int wordCount = 0;
  unsigned int i = 0;

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
          // strcpy(wordsArray[wordCount], word);
          // wordCount++;
          i = 0; // Reset the index for the next word
      } else if (is_separator(ch)) {
          // If we encounter a space or hyphen, treat it as a separator
          i = 0;
      }
      // Ignore punctuation and reset the index for the next word
      if (is_ponctuation(ch)) {
          i = 0;
      }
  }
  // Close the file
  fclose(file);
  return hash_table;
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

    hash_table = generate_hash_for_file(hash_table, file_name);
    // //setting file
    // FILE *file = fopen(file_name, "r");

    // if(file == NULL){
    //   printf("Erro ao abrir arquivo %s", file_name);
    //   return 0;
    // }

    // // Read the file character by character
    // while ((ch = fgetc(file)) != EOF) {
    //     if(isalpha(ch)) {
    //         // Convert the character to lowercase and add it to the current word
    //         word[i] = tolower(ch);
    //         i++;
    //     } else if (i > 1) {
    //         word[i] = '\0'; // Null-terminate the word
    //         hash_table_set(hash_table, word);
    //         // strcpy(wordsArray[wordCount], word);
    //         // wordCount++;
    //         i = 0; // Reset the index for the next word
    //     } else if (is_separator(ch)) {
    //         // If we encounter a space or hyphen, treat it as a separator
    //         i = 0;
    //     }
    //     // Ignore punctuation and reset the index for the next word
    //     if (is_ponctuation(ch)) {
    //         i = 0;
    //     }
    // }

    printf("Palavra: %s | ocorrencias: %d\n", word_to_find, hash_table_get(hash_table, word_to_find));
    // Close the file

    free(hash_table);
    // // Display the words
    // printf("Words in the file (ignoring words with less than 2 characters and punctuation):\n");
    // for (int j = 0; j < wordCount; j++) {
    //     printf("%s\n", wordsArray[j]);
    // }

    return 0;
  }

}