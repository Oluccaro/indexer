#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLE_SIZE 1000000
#define MAX_WORD_LENGTH 40
//Hash item HI
typedef struct HI {
  char *key;
  unsigned count;
  struct HI *next;
} HI;

//Hash Table HT
typedef struct HT {
  HI **items;
  unsigned long int n_items;
} HT;

typedef struct array_list_of_HI{
  HI **items;
  unsigned int n_words;
} array_HI;

typedef struct FileRelevance {
    const char *file_name;
    unsigned int term_occurrences;
    double tfidf;
} FileRelevance;


array_HI *create_array_list(void){
  
  array_HI *array_list = (array_HI *)malloc(sizeof(array_HI) * 1);
  
  array_list->items = (HI**)malloc(sizeof(HI*) * TABLE_SIZE);
  
  int i;

  for(i = 0; i < TABLE_SIZE; i++){
    array_list->items[i] = NULL;
  }
  array_list->n_words = 0;
  return array_list;
}
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

  hash_table->items = (HI**)malloc(sizeof(HI*) * TABLE_SIZE);

  int i = 0;
  for(; i < TABLE_SIZE; i++){
    hash_table->items[i] = NULL;
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
  HI *hash_item = hash_table->items[index];

  if (hash_item == NULL){
    //primeira ocorrencia da palavra count = 1;
    hash_table->items[index] = hash_item_pair(key, 1);
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

//Get number of ocorrencies on and hash_table, given a key
unsigned int hash_table_get_count(HT *hash_table, const char *key){
  unsigned int index = hash(key);

  HI *hash_item = hash_table->items[index];

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

HI *hash_table_get_item(HT *hash_table, const char *key){
  unsigned int index = hash(key);
  HI *hash_item = hash_table->items[index];
  while(hash_item != NULL){
    if(strcmp(hash_item->key, key) == 0){
      return hash_item;
    }
    hash_item = hash_item->next;
  }
  return NULL;
}
// Transform string to lowercase
char *to_lower_str(const char *s){
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

//Generate hash_map given a filename
HT *generate_hash_for_file(HT *hash_table, const char *file_name){
  FILE *file = fopen(file_name, "r");
  char ch;
  char word[MAX_WORD_LENGTH];
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
      } else if (i > 2) {
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

int HI_cmp(const void *a, const void *b){
  int count_a = (*(HI**)a)->count;
  int count_b = (*(HI**)b)->count;
  int diff = (count_b - count_a);
  return diff;
}

int compareFileRelevance(const void *a, const void *b) 
{
    const FileRelevance *fileA = (const FileRelevance *)a;
    const FileRelevance *fileB = (const FileRelevance *)b;
    return fileB->tfidf - fileA->tfidf;
}


int main(int argc, char const *argv[])
{

  unsigned int i = 0;
  char *freq_word = "--freq-word";
  char *freq = "--freq";

  //Searching for number of ocurrencies of word in file
  if(strcmp(argv[1], freq_word) == 0){
    //Getting parameters
    const char *word_to_find = to_lower_str(argv[2]);
    const char *file_name = argv[3];

    HT *hash_table = create_hash_table();

    generate_hash_for_file(hash_table, file_name);

    printf("Palavra: %s | ocorrencias: %d\n", word_to_find, hash_table_get_count(hash_table, word_to_find));

    free(hash_table);

    return 0;
  }

  // returning the top N words with most ocurrencies 
  if(strcmp(argv[1], freq) == 0){

    // getting parameters
    unsigned int top_n = (unsigned int)strtol(argv[2], (char **)NULL, 10);
    const char *file_name = argv[3];
    unsigned int j = 0;
    char ch;
    char word[MAX_WORD_LENGTH];
    array_HI *array_list = create_array_list();
    unsigned int i = 0;

    HT * hash_table = create_hash_table();

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

          if(strlen(word) > 2){
            if(hash_table_get_count(hash_table, word) == 1){
              array_list->items[array_list->n_words++] = hash_table_get_item(hash_table, word); 
            }
          }
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

    qsort(array_list->items, array_list->n_words, sizeof(HI*) , HI_cmp);  

    printf("Top %d palavras com mais ocorrências no arquivo %s\n", top_n, file_name);
    for(j=0; j < top_n; j++){
      printf("Palavra %s -> Ocorrências %d\n", array_list->items[j]->key, array_list->items[j]->count);
    }
    printf("Total de palavras diferentes no arquivo %d\n", array_list->n_words);
    free(hash_table);
    free(array_list);
  }
  
  	if(strcmp(argv[1], search) == 0) 
	{
		int j;
		FileRelevance relevanceArray[100];
		const char *termo = tolowerstr(argv[2]);
		double TF_value, IDF_value, TFIDF_value;
		double term_occurrences = 0;
		double numFiles = 0;
	
		for(j = 3; j <= argc; j++) 
		{
			
			// criando hash table para cada arquivo
			
			HT *hash_table = create_hash_table();
			hash_table = generate_hash_for_file(hash_table, argv[j]);
			
			double TF_value = (double)hash_table_get(hash_table, termo) / (double)hash_table->n_items;
	
		    relevanceArray[numFiles].file_name = argv[j];
		    relevanceArray[numFiles].tfidf = TF_value;
		    relevanceArray[numFiles].term_occurrences = (hash_table_get(hash_table, termo) > 0) ? 1 : 0;
		
		    numFiles += 1;
		    term_occurrences += (hash_table_get(hash_table, termo) > 0) ? 1 : 0;
		}
		
		double IDF_value = log(numFiles / term_occurrences);

		for (unsigned int i = 0; i < numFiles; i++) 
		{
    		relevanceArray[i].tfidf *= IDF_value;
		}
		
		qsort(relevanceArray, numFiles, sizeof(FileRelevance), compareFileRelevance);
		
		for (unsigned int i = 0; i < numFiles; i++) 
		{
    		printf("Arquivo: %s | TF-IDF: %lf\n", relevanceArray[i].file_name, relevanceArray[i].tfidf);
		}
		
}
}
