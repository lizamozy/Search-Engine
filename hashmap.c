#include "hashmap.h"
#include <stdlib.h> 
#include "string.h"
#include <stdio.h>

struct hashmap* hm_create(int num_buckets){
 //create the hashmap
  struct hashmap* hm = calloc(sizeof(struct hashmap),1);

  //allocating the array of buckets
  hm -> map = calloc(sizeof(struct llnode *), num_buckets);
  int i=0;
  for(i=0; i < hm ->num_buckets; i++){
    hm -> map[i] = NULL;
  }
  hm -> num_buckets = num_buckets;

// if errors with size make size 0
  return hm;
}

//fucntion to see if the word is in the hashmap
int hm_getWord(struct hashmap* hm, char* word){
//TODO check if a word is in the hashmap
if ( hm == NULL || word == NULL  ){
    printf("null\n");
}
//initialze a current node to keep track of current spot in linked list
  struct llnode_Word* curWord;
  //hash and then find correct bucket and traverse through linked list to find the value in the while loop. if we dont find it exit and return false
  unsigned int hashvalue = hash_code(hm, word);
  curWord = hm -> map[hashvalue];

  while(curWord != NULL){
    //if it is the right word, return the num occreunces
    if (strcmp(curWord -> word, word)==0 ){
      
        return curWord -> docFreq;
    } 
    //keep going through the list of words 
    curWord = curWord -> next;
  }
  //if the word isnt in there then return -1
  return -1; //otherwise false and we did not find the value
}

//function to check if the word has the document ID
int hm_getDocs(struct hashmap* hm, char* word, char* document_id){
  if ( hm == NULL || word == NULL ){
    printf("null\n");
}
//making nodes for the wanted word and curDoc node
struct llnode_Word* wantedWord;
struct llnode_Doc* curDoc;
  
 //hash and then find correct bucket and traverse through linked list to find the value in the while loop. if we dont find it exit and return false
  unsigned int hashvalue = hash_code(hm, word);
   wantedWord = hm -> map[hashvalue];

  //if the word is in the linked list with the correct docID, make curDoc pointers equal to the word node pointing to the documents node
  if (hm_getWord(hm, word)!= -1){
      while(wantedWord!=NULL){
      //if the passed in word matches, made cur doc equal to where 
      if(strcmp(wantedWord->word, word)==0){
        curDoc = wantedWord -> documents;
        while(curDoc != NULL){
            if(strcmp(curDoc -> document_id, document_id)==0 ){   
              return curDoc -> termFreq;
          } 
    //keep going through the list of words for the inner while loop
        curDoc = curDoc -> next;
        }
      }
    //iterate through the wantedWord so we can compare it for the outer whileloop
    wantedWord = wantedWord->next;
    }

  }
  return -1; //otherwise false and we did not find the value document 

}

void hash_table_insert(struct hashmap* hm, char* word, char* document_id, int num_occurrences){

  if ( hm == NULL || word == NULL || document_id == NULL ){
      printf("hashmap is null");
}
  
  //hash and then find correct bucket and traverse through linked list to find the value in the while loop. if we dont find it exit and return false
  unsigned int hashvalue = hash_code(hm, word);
  struct llnode_Word* wordHead;
  wordHead = hm -> map[hashvalue];
  struct llnode_Word* wordCur;
  wordCur = wordHead;
  struct llnode_Doc* docCur;

  //if the word is not in the hashmap, add the word and doc node in
    //we need to make the tf equal to 1
  if(hm_getWord(hm, word) == -1){
    //iterate through all of the buckets 
    if(wordCur != NULL){
    while(wordCur->next!= NULL){
        //keep iterating
        wordCur = wordCur -> next;
      }
    }
   //we have exited the while loop and we have not found the word, add it to the end of the list
    struct llnode_Word* wordTemp = calloc(sizeof(struct llnode_Word ), 1);
    struct llnode_Doc* tempDoc = calloc(sizeof(struct llnode_Doc),1);
    //making temp have the value of word
    wordTemp -> word = strdup(word);
    //if current in null (the head)
    if(wordCur == NULL){
      //make the node equal to the head of the list
      hm -> map[hashvalue] = wordTemp;

        //if the bucket isnt null
        }else{
        //if the node is anything else,making the next node the new word
        wordCur -> next = wordTemp;
        }
         //make new node point to NULL and set the document frequency equal to one
          wordTemp -> next = NULL;
          wordTemp -> docFreq = 1;
          //tempDoc equal to wordtemp pointing to the document list and make the next pointer NULL
          wordTemp -> documents = tempDoc;
          tempDoc ->next = NULL;
          //stringdup the document id for the new doc node
          tempDoc->document_id = strdup(document_id);
          //making the term frequency 1 because the Doc node is created from the word node and it did not exist before 
          tempDoc-> termFreq = 1;
          //printf("word is: %s & docID is :%s\n", word, document_id);
          return;
    }else{

      if((hm_getWord(hm,word)!=-1) && (hm_getDocs(hm,word, document_id)!=-1)){
        // printf("word is: %s & docID is :%s\n", word, document_id);
        while(wordCur!=NULL){
        //if we find the word, make docCur = wordCur->documents
          if(strcmp(wordCur->word, word)==0){
            docCur = wordCur -> documents;

            //if we go through the document list and doc is the same, just update number of occurances 
          while(docCur!=NULL){
              if(strcmp(docCur-> document_id, document_id)==0){
                //printf("termFreq is %d\n for word %s in document %s before increment\n", docCur -> termFreq, word,document_id);
                docCur -> termFreq++;
                return;
              }
            //getting a segfault here
             docCur = docCur -> next;
          }

          }
          wordCur = wordCur->next;
        }
          //else; there is no node for that document, add one in for that word and make num_occurances equal to 1
          }else{
            //we have left the while loop and have not found a match for the documentID so need to make a new docID node here
            struct llnode_Doc* docNode = calloc(sizeof(struct llnode_Doc), 1);
            //need to make docCur->next point to docNode, need to find a way to somehow access hm 
            //docNode->next=NULL;
            docNode->document_id = strdup(document_id);
            //printf("docNode->document_id is %s: and document_id is %s\n", docNode->document_id, document_id);
            docNode->termFreq++;
            while(wordCur!=NULL){
              //if we find the word, make docCur = wordCur->documents
                if(strcmp(wordCur->word, word)==0){
                    docCur = wordCur->documents;
                    docNode ->next = docCur;
                    wordCur->documents =docNode;
                    wordCur->docFreq += num_occurrences;
                  return;
                }
                wordCur = wordCur->next;
          }
  
    }
        return;
 }
}



void hm_remove(struct hashmap* hm, char* word){
    //get the hash value
    int hashvalue = hash_code(hm, word);

    //get the bucket
    struct llnode_Word * curWord = hm->map[hashvalue];

    //search the bucket for the word; remove it if found
    struct llnode_Word * prevWord = NULL;
    while(curWord!=NULL){
        if(strcmp(curWord->word, word) == 0){
            if(prevWord == NULL){
                hm->map[hashvalue] = curWord->next;
            } else {
                prevWord->next = curWord->next;
            }
            //free the word 
            free(curWord->word);

            //free the DocIDs 
            struct llnode_Doc * curDoc = NULL;
            while(curWord->documents){
                curDoc = curWord->documents;
                curWord->documents = curWord->documents->next;
                free(curDoc->document_id);
                free(curDoc);
            }

            free(curWord);
            return;
        }
        prevWord = curWord;
        curWord = curWord->next;
    }

    return;
}

void hm_destroy(struct hashmap* hm){
  int i;
  struct llnode_Word* prevWord;
    for( i=0; i < hm -> num_buckets; i++){ 
      struct llnode_Word* curWord = hm->map[i];
      while(curWord!=NULL){
      //make a previous to store cur and just free cur doc and do this until doclist is fully deleted
      struct llnode_Doc* curDoc = curWord->documents;
      struct llnode_Doc* prevDoc = curDoc;
        while(curDoc!=NULL){
          prevDoc = curDoc;
          curDoc = curDoc->next;
          free(prevDoc->document_id);
          free(prevDoc);
        }
      //deleting the word after we delete the doclist
      prevWord= curWord -> next;
      free(curWord->word);
      free(curWord);
      curWord = prevWord;
      }
    }
  //delete the hashmap
  free(hm -> map);
  free(hm);
}

//method to find the hash value of the buckets 
int hash_code(struct hashmap* hm, char* word){
double k = 0;
  double len = (double) strlen(word);
  int i = 0;
  char c;
  //sum up all the ascii codes of each 
  for(c = word[0]; i < len; i++){
    c = word[i];
    k += (int)(c);
  }
  //return the calcuated value for which bucket for the word
  return (unsigned int) (((unsigned long) k) % hm->num_buckets);
}
