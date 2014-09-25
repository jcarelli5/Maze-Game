// Jonathan Carelli
// COP 3502 - Fall 2013
// Assignment #3 -- Maze Game that uses 2D array pointers and implements a queued linked list

#include <stdio.h>
#include <stdlib.h>

#define EMPTY -1

// Stores one node of the linked list.
struct node {
    int data;
    struct node* next;
};

// Stores the queue.
struct queue {
    struct node* front;
    struct node* back;
};

int doMaze(int rows, int col, char **mazeArrP, int **moveStorageP, int y, int x);
void init(struct queue* qPtr);
int enqueue(struct queue* qPtr, int val);
int dequeue(struct queue* qPtr);
int empty(struct queue* qPtr);

    int main(){

        int rows, col, i, j, k;
        int posX, posY;
        int numCases;
        char **mazeArrP;
        int **moveStorageP;

        scanf("%d", &numCases);

        for(k=0; k<numCases; k++){

            scanf("%d %d", &rows, &col);

            // Allocates space for the maze and storage array.
            mazeArrP = (char **) malloc(rows*sizeof(*mazeArrP));
            moveStorageP = (int **) malloc(rows*sizeof(*moveStorageP));

            // Populating the maze with the scanned in values to a 2 dimensional array.
            for(i = 0; i<rows; i++){
                mazeArrP[i] = malloc(col);

                for(j = 0; j<col; j++){
                    scanf(" %c", &mazeArrP[i][j]);
                }
            }

            // Populating a move tracker 2 dimensional array with -1 and finding the S in the inputted maze and placing a zero in that spot.
            for(i = 0; i<rows; i++){
                moveStorageP[i] = (int *) malloc(col*sizeof(int *));

                for(j=0; j<col; j++){
                    if(mazeArrP[i][j] == 'S'){
                        moveStorageP[i][j] = 0;
                        posY = i;
                        posX = j;
                        j++;
                    }
                    moveStorageP[i][j] = -1;
                }
            }
            // Calling the function that actually does the maze.
            doMaze(rows, col, mazeArrP, moveStorageP, posY, posX);

        }

        return 0;
    }

/*This function performs the moves and changes the board appropriately. This impliments a queue system that will set temp values to the X Queue and Y Queue
  then it will dequeue and proceed to check around the temp coordinates. If a valid space is found, it will enqueue that space and proceed as planned. Once
  a boarder position is found, it will stop the game and return how many tries it took to get to that space. The program assumes the first boarder it comes
  across is the shortest length out of the maze. */

int doMaze(int rows, int col, char **mazeArrP, int **moveStorageP, int y, int x) {

    int i, j, k, locX, locY, runTotal = 0, gameFinish=0;
    int tempX, tempY;
    int runValue = 0;

    struct queue* qX = (struct queue*)malloc(sizeof(struct queue));
    struct queue* qY = (struct queue*)malloc(sizeof(struct queue));
    init(qX);
    init(qY);
    enqueue(qX, x);
    enqueue(qY, y);


    while(gameFinish == 0){
            runTotal += 1;

      for(k=0; k < runTotal; k++){
            tempX = qX->front->data;
            tempY = qY->front->data;
            dequeue(qX);
            dequeue(qY);

        for(i =0; i < 4; i++){

                    // checking for a boarder
                    if(mazeArrP[(tempY)+1][tempX] == '~' || mazeArrP[(tempY)-1][tempX] == '~' || mazeArrP[(tempY)][(tempX)-1] == '~' || mazeArrP[(tempY)][(tempX)+1] == '~'){
                        if(tempX > tempY){
                            printf("%d\n", tempX);
                        }
                        else
                            printf("%d\n", tempY);
                            return 1;
                    }
                    // checking below
                    if(mazeArrP[(tempY)+1][tempX] == '-'){
                        moveStorageP[(tempY)+1][(tempX)] = runTotal;
                        mazeArrP[(tempY)+1][(tempX)] = 'o';
                        enqueue(qX, tempX);
                        enqueue(qY, tempY+1);
                        break;
                    }
                    // checking above
                    if(mazeArrP[(tempY)-1][tempX] == '-'){
                        moveStorageP[(tempY)-1][(tempX)] = runTotal;
                        mazeArrP[(tempY)-1][(tempX)] = 'o';
                        enqueue(qX, tempX);
                        enqueue(qY, tempY-1);
                        break;
                    }
                    // checking to the left
                    if(mazeArrP[(tempY)][(tempX)-1] == '-'){
                        moveStorageP[(tempY)][(tempX)-1] = runTotal;
                        mazeArrP[(tempY)][(tempX)-1] = 'o';
                        enqueue(qX, tempX-1);
                        enqueue(qY, tempY);
                        break;
                    }
                    // checking to the right
                    if(mazeArrP[(tempY)][(tempX)+1] == '-'){
                        moveStorageP[(tempY)][(tempX)+1] = runTotal;
                        mazeArrP[(tempY)][(tempX)+1] = 'o';
                        enqueue(qX, tempX+1);
                        enqueue(qY, tempY);
                        break;
                    }
                    // checking if there is no way out of the maze
                    if(mazeArrP[(tempY)+1][tempX] != '-' && mazeArrP[(tempY)-1][tempX] != '-' && mazeArrP[(tempY)][(tempX)-1] != '-' && mazeArrP[(tempY)][(tempX)+1] != '-'){
                        if(empty(qX) == 1){
                            printf("%d\n", EMPTY);
                            return 0;
                        }
                    }
        }
      }
    }

    return 1;
}


// This function initializes the queue and sets both the front and back pointers to 'NULL'.
void init(struct queue* qPtr) {

     qPtr->front = NULL;
     qPtr->back = NULL;
}

// This is the enqueue function as shown by Arup. This will take in a pointer and new value to be queued. It will also allocate space appropriately.
int enqueue(struct queue* qPtr, int val) {

    struct node* temp;
    temp = (struct node*)malloc(sizeof(struct node));

    if (temp != NULL) {
        temp->data = val;
        temp->next = NULL;

        if (qPtr->back != NULL)
            qPtr->back->next = temp;

        qPtr->back = temp;

        if (qPtr->front == NULL)
            qPtr->front = temp;

        return 1;
    }

    else
        return 0;
}

// This function will remove the first instance of data in the queue, it will then reset the front and back pointers appropriately.
// It will also free the allocated memory associated with the newly dequeued data.
int dequeue(struct queue* qPtr) {

    struct node* tmp;
    int retval;

    if (qPtr->front == NULL)
        return EMPTY;

    retval = qPtr->front->data;
    tmp = qPtr->front;
    qPtr->front = qPtr->front->next;

    if (qPtr->front == NULL)
        qPtr->back = NULL;

    free(tmp);

    return retval;
}

// This function checks if the queue is empty. This is used in the 'no way out' scenario.
int empty(struct queue* qPtr) {
    return qPtr->front == NULL;
}

