#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


#define MAX_INT 2147483647
#define HASH_TABLE_SIZE 20

typedef struct state {
    char *name;
    int score;
    int Ascore;
    int blank;
    struct state *next;
    struct state *nextH;
    struct state *prev;
}State;



///
int N;
int n;
int pushes;
char first[100];
int flag = 0;
int m_counter = 0;
int a_counter = 0;
int b_counter = 0;
// Global var
State *root;
State *hashTable[HASH_TABLE_SIZE];

// Main Functions
State* aStar();
void print_best(State *bestState);
void print_queue();
void find_next(State *myState);
void constructRoot(char* buffer);
int is_gold(State *myState);
State* change(State *myState, int index);
void calculate_ascore(State* myState);
// Priority Queue Functions
void push(State** myState);
int isEmpty();
void pop();
State* peek();
// HashTable Functions
int hash(char*name);
void initializeHashTable();
void printHashTable();
int insertInHashTable(State * node);
State *searchInHashTable(char * name);

int main(int argc,char *argv[]){
    printf("\n---                            Astar                            ---\n");
    initializeHashTable();
    ///OPTIMAL
    while(flag!=1){
     printf("\nInsert the initial balls positions(space with - )\n");
     scanf("%s",first);
     n = strlen(first);
     N = n/2;
     if((n>1) && (n%2==1)){
        for(int i = 0;i<n;i++){
           if(first[i]=='M'){
                m_counter++;
           }
           else if(first[i]=='A'){
                a_counter++;
           }
           else{
                b_counter++;
           }
        }
     }
     if((m_counter == N) && (a_counter == N) && (b_counter == 1)){
        flag = 1;
     }
     else{
        printf("\nSomething went wrong\n");
     }
    }
    ///OPTIMAL
    clock_t time_now = clock();

    constructRoot(first);
    printf(" %s (name)  %d (blank)  %d (score) %d (final)\n", root->name, root->blank, root->score, is_gold(root));
    insertInHashTable(root);

    State* bestState = malloc(sizeof(State));
    bestState = aStar();

    if (bestState->score != MAX_INT){
        print_best(bestState);
    } else {
        printf("\n No paths found!\n");
    }
    printf("\n Pushes : %d \n",pushes);
    time_now = clock() - time_now;
    double time_taken = ((double)time_now)/CLOCKS_PER_SEC; // in seconds
    printf("\n -- program took %f seconds to execute -- \n", time_taken);
}

State* aStar(){

    State* bestState = malloc(sizeof(State));
    bestState->score = MAX_INT;

    int count_gold = 0;
    find_next(root);

    while(!isEmpty()){

        State* nowState;
        nowState = peek();
        pop();

        if (is_gold(nowState) == 1){
            count_gold++;
            if (nowState->score < bestState->score){
                bestState = nowState;
                printf(" %s (name)  %d (blank)  %d (score) %d (final)\n", bestState->name, bestState->blank, bestState->score, is_gold(bestState));
                return bestState;
            }
        }

        find_next(nowState);
    }
    return bestState;
}

// Hash the name of the State
int hash(char*name){

    int hash_value = 0;
    for(int i = 0;i<n;i++){
        if(name[i]=='M'){
            hash_value+=3*i;
            hash_value=hash_value % HASH_TABLE_SIZE;
        }
        else{
            hash_value+=2*i;
            hash_value=hash_value % HASH_TABLE_SIZE;
        }
    }
    return hash_value;
}

// Insert to HashTable
int insertInHashTable(State * node){
    if(node == NULL){
      return 0;
    }
    int pos = hash(node->name);
    node->nextH = hashTable[pos];
    hashTable[pos] = node;
    return 1;
}

// Look in HashTable of name
State *searchInHashTable(char * name){
    int pos = hash(name);
    State * cur = hashTable[pos];
    while(cur != NULL && strcmp(cur->name,name)!=0){
        cur = cur->nextH;
    }
    return cur;
}

// Print the HashTable
void printHashTable(){
    for(int i = 0;i<HASH_TABLE_SIZE;i++){
        if(hashTable[i] == NULL){
          printf("%d : ----\n", i);
        }
        else{
          printf("%d : %s\n",i,hashTable[i]->name);
        }
    }
}

//Initialize the HashTable
void initializeHashTable(){
    for(int i = 0;i<HASH_TABLE_SIZE;i++){
        hashTable[i]=NULL;
    }
}

// Function to push according to score
void push(State** myState) {

    State* start = root;

    // Traverse the list and find a position to insert new node
    while (start->next != NULL && start->next->Ascore < (*myState)->Ascore) {
        start = start->next;
    }

    (*myState)->next = start->next;
    start->next = (*myState);
}

// Function to check if list is empty
int isEmpty() {
    return root->next == NULL;
}

// Removes the element with the lowest score form the list
void pop() {
    root->next = root->next->next;
}

// Return the value at head
State* peek(){
    return root->next;
}

// Construct root
void constructRoot(char* buffer){
    root = (State*)malloc(sizeof(State));
    root->name = strdup(buffer);
    int i;
    for(i = 0; i < n; i++){
        if (buffer[i] == '-'){
                root->blank = i;
                break;
        }
    }
    root->score = 0;
    root->next = NULL;
    root->prev = NULL;
    ///OPTIMAL
    calculate_ascore(root);
    ///OPTIMAL
}

// Function that finds the next states from myState
void find_next(State *myState){

    int left = myState->blank - N;
    if (left < 0)
        left = 0;
    int right = myState->blank + N;
    if (right > n-1)
        right = n-1;

     int i;
     for (i = left; i < right+1; i++){
        if (i != myState->blank){
            State* newState = (State*)malloc(sizeof(State));
            newState = change(myState, i);
            if(searchInHashTable(newState->name)==NULL){
                insertInHashTable(newState);
                push(&newState);
            }
        }
     }
}

// Function that swaps the blank space with "index"
State* change(State *myState, int index){
    int k = abs(myState->blank - index);
    State *newState = (State*)malloc(sizeof(State));
    newState->name = strdup(myState->name);
    newState->name[index] = '-';
    newState->name[myState->blank] = myState->name[index];
    newState->blank = index;
    newState->score = myState->score + k;
    newState->prev = myState;
    newState->next = NULL;
    calculate_ascore(newState);
    return newState;
}

// Calculate the A score of State
void calculate_ascore(State* myState){
    myState->Ascore = myState->score;
    //if (myState->name[n-1] == 'A'){
        //myState->Ascore += grade;
    //}
    int i;
    for (i = 0; i < n; i++) {
        if ((myState->name[i] == 'A') && (i < N)){
            myState->Ascore += (N-i);
        }
        if ((myState->name[i] == 'M') && (i > N)){
            myState->Ascore += (i-N);
        }
    }
}

// Function that checks if a state is golden
int is_gold(State *myState){
    pushes++;
    if (myState->name[n-1] != 'A'){
        return 0;
    }
    int i;
    for (i = 0; i < n-1; i++) {
        if ((myState->name[i] == 'A')){
            if (myState->name[i+1] == 'M'){
                return 0;
            } else if ((myState->name[i+1] == '-') && (myState->name[i+2] == 'M')) {
                return 0;
            }
        }
    }
    return 1;
}

// Function that prints the best path
void print_best(State *bestState){
    int steps = 0;
    while (bestState != NULL){
        steps++;
        printf(" %s , W = %d , H = %d <-",bestState->name,bestState->score,(bestState->Ascore)-(bestState->score));
        bestState = bestState->prev;
    }
    printf("Steps: %d\n", steps);
}

// Function that prints the queue
void print_queue(){
    State * start = root;
    while (start != NULL){
        printf("%s / ", start->name);
        start = start->next;
    }
    printf("\n");
}


