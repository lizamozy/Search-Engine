#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdio.h>


//node for the doc Id
struct llnode_Doc {
        char* document_id;
        int termFreq;
        struct llnode_Doc* next;
};
//doc ID node
struct llnode_Word {
        char* word;
        int docFreq;
        struct llnode_Doc* documents;
        struct llnode_Word* next;
};

struct hashmap {
        struct llnode_Word** map;
        int num_buckets;
};

struct hashmap* hm_create(int num_buckets);
int hm_getDocs(struct hashmap* hm, char* word, char* document_id);
int hm_getWord(struct hashmap* hm, char* word );
void hash_table_insert(struct hashmap* hm, char* word, char* document_id, int num_occurrences);
void hm_remove(struct hashmap* hm, char* word);
void hm_destroy(struct hashmap* hm);
int hash_code(struct hashmap* hm, char* word);
void stop_word(struct hashmap*hm);
struct hashmap* trainingPhase( char*word, FILE * D1, FILE * D2, FILE * D3, int num_buckets);
void rank(struct hashmap*hm, char*token);
void read_query(struct hashmap*hm, char*query);
void sortArray(double docScores[]);
void writeScoresToFile(double docScores[], char * arrayDocs[]);
void printResult(char* arrayDocs[]);

#endif
