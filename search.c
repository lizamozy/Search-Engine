#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"
#include <math.h>
#include <string.h>

//array to hold the document scores, gonna use this in my rank function 
double documentScores[3] = {0.0};
double scoreD1;
double scoreD2;
double scoreD3;

//print result to txt file
void writeScoresToFile(double docScores[], char* arrayDocs[]){

  int i;
  //open the file
  FILE * searchScores = fopen("search_scores.txt", "a");
    //prints the scores in order to text file 
    for(i=0; i<3; i++){
      if(docScores[i]!=0){
       fprintf(searchScores, "%s %.2f\n", arrayDocs[i], docScores[i]);
      }
    }

  fclose(searchScores);
  return;
}

//function to print the result to terminal
void printResult(char* arrayDocs[]){
  //checking that if no words can be found 
    if(documentScores[0]== 0.000000 && documentScores[1]== 0.000000 && documentScores[2] == 0.000000){
      printf("No documents were found\n");
      return;
    }
    int i; //iterator
    //print the documents to the terminal
    for(i=0; i<3; i++){
       printf("%s\n", arrayDocs[i]);
    }
    return;
}

void sortArray(double docScores[]){

//array to hold the .txt files
char* arrayDocs[] = {"D1.txt","D2.txt", "D3.txt" };

int j; 
int i;
//sorting the result and make the matching document have that index as well
  for(i=0; i<3; i++){
        for(j=2; j>0; j--){
            if(docScores[j] > docScores[j-1]){
              //sorting the scores 
              double tmp = docScores[j];
              docScores[j] = docScores[j-1];
              docScores[j-1] = tmp;
              //sorting the correspondong document so that they are the same index 
              char* temp = arrayDocs[j];
              arrayDocs[j] = arrayDocs[j-1];
              arrayDocs[j-1] = temp;
            }
        }
    }

  printResult(arrayDocs);
  writeScoresToFile(docScores, arrayDocs);

  return;

 }

//rank function 
void rank(struct hashmap*hm, char*token){
  
  //having a seperate 
  double N = 3.0;
  
  //iterate through each document, only do this three times this is for D1
  //getting the term frequency and using arrayDocs to keep track of doc IDs
          int tf = hm_getDocs(hm, token, "D1.txt");
          if(tf==-1){
             tf=0;
          }
         //making var for document freq
          int df = hm_getWord(hm, token);
          if(df==-1){
            df=1;
          }
          //makng var for the inverse doc frequency
          double idf = log(N/df);
          //caluating my score
          scoreD1 += tf*idf;
        
          //getting the term frequency and using arrayDocs to keep track of doc IDs
           tf = hm_getDocs(hm, token, "D2.txt");
          if(tf==-1){
             tf=0;
          }
         //making var for document freq
           df = hm_getWord(hm, token);
          if(df==-1){
            df=1;
          }
          //makng var for the inverse doc frequency
           idf = log(N/df);
          //caluating my score
          scoreD2 += tf*idf;
        
          //getting the term frequency and using arrayDocs to keep track of doc IDs
           tf = hm_getDocs(hm, token, "D3.txt");
          if(tf==-1){
             tf=0;
          }
         //making var for document freq
           df = hm_getWord(hm, token);
          if(df==-1){
            df=1;
          }
          //makng var for the inverse doc frequency
           idf = log(N/df);
          //caluating my score for 
          scoreD3 += tf*idf;
        
  return;
  
}

void stop_word(struct hashmap* hm){
    
    double N = 3.0; 
    int i; //iterator for num buckets
    for(i = 0; i < hm->num_buckets; i++){
        struct llnode_Word* curNode = hm->map[i]; 
        while(curNode!= NULL){
            //variable the df
            double df =  (double) curNode->docFreq;
            if(df == 0){
              df = 0;
            }
            //variable for idf
            double idf = log(N/df);
            
            //check if idf is zero and if it is, it occurs in all three files
            if(idf == 0.00000){
                hm_remove(hm, curNode->word); 
            }
            curNode = curNode->next;
        }
    }
}
       
struct hashmap* trainingPhase(char* word, FILE * D1, FILE * D2, FILE * D3, int num_buckets){
  //scanning in D1
  //error checking for D1
  struct hashmap* hm = hm_create(num_buckets); 
  
  if(D1 == NULL)
  {
    printf("Error opening file D1.text\n");
    return hm;
  }
  //scan in D1 and put 
  while(fscanf(D1, "%1023s", word) != EOF)
  {
    hash_table_insert(hm,word,"D1.txt", 1); 
  }

  //scanning in D2
  //error checking for D1
  if(D2 == NULL)
  {
    printf("Error opening file D2.text\n");
    return hm;
  }
  //scan in D2 and put those words into the hashmap 
  while(fscanf(D2, "%1023s", word) != EOF)
  {
    //put words in hashmap
    hash_table_insert(hm,word,"D2.txt", 1);
  }  
   //close the file
  fclose(D2);

  //error checking for D1
  if(D3 == NULL)
  {
    printf("Error opening file D3.text\n");
    return hm;
  }
  //scan in D1 and put 
  while(fscanf(D3, "%1023s", word) != EOF)
  {
    hash_table_insert(hm,word,"D3.txt", 1);
  }  

  fclose(D3);

  stop_word(hm);

  return hm;

}

//fucntion to read query 
 void read_query(struct hashmap*hm, char*query){

  //making an array of the words spaced out by a spaceto use inside of the ranking function 
  char* tokenize = strtok(query, " ");
  //we are going to iterate through the whole list of words from the serach query and calculate the scores in rank
  while(tokenize!=NULL){
    //here we are going to call rank on each tokenized word
    rank(hm, tokenize);
    tokenize = strtok(NULL, " ");
  }
   
  return;
}

int main(void){ 

//making space of length 1024 for query string, a buffer, and word
char query[1024];
char buffer[1024];
//word of size 21
char word[21];

int num_buckets=0;

printf("how many buckets?\n");
fgets(buffer, 1024, stdin);
sscanf(buffer,"%d", &num_buckets);

//creating variables for each file to pass in to 
FILE * D1 = fopen("p5docs/D1.txt", "r");
FILE * D2 = fopen("p5docs/D2.txt", "r");
FILE * D3 = fopen("p5docs/D3.txt", "r");


//in my training phase i will put in all of the words and remove stop words
struct hashmap* hmDocs = trainingPhase(word, D1, D2, D3, num_buckets);

while(1){
    
      //prompting the user to input a string 
      printf("enter search string or X to exit:\n");
      //puts user input into query
      fgets(query, 1024, stdin);
      //if user enters 
      if(query[0]=='X' || query[0]=='x'){
        hm_destroy(hmDocs);
        break;
      }
      query[strlen(query)-1] = '\0';

    //need to now read the query and then call rank inside query 
    read_query(hmDocs, query);
    //sorts the global array of scores and then calls the print functions 
    //storing the score in the bucket for that corresponding document 
    documentScores[0]=scoreD1;
    documentScores[1]=scoreD2;
    documentScores[2]=scoreD3;
    //calling the sort array function 
    sortArray(documentScores);

    //clearing out all of the scores to be done again
    documentScores[0]=0.0;
    documentScores[1]=0.0;
    documentScores[2]=0.0;

    scoreD1=0;
    scoreD2=0;
    scoreD3=0;

    int i;
    //clearing out the query
    for(i = 0; i < query[strlen(query)]; ++i){
        query[i] = 0;
    }
    
  }
    return 0;



}
