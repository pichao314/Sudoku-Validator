#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void readSolution(FILE* sudoku, int* solution);
void* rowCheck(void* arg);
void* columnCheck(void* arg);
void* gridCheck(void* arg);
void output(void* result[]);

int main(int argc, char *argv[])
{
    //read the solution file
    FILE* sudoku;
    if(argc == 2)
    {
        sudoku = fopen(argv[1], "r");
        if(!sudoku)
        {
            printf("Cannot read the file!\n");
            exit(0);
        }
    }
    else
    {
        printf("Wrong argument!\n");
        exit(0);
    }

    //import the solution file into local matrix
    int solution[82] = {0};
    readSolution(sudoku, solution);

    //create the thread and validate the solution
    pthread_t validator[27];
    void* result[27];
    int pid = 0;

    for (int i = 0; i < 9; i++)
    {
        solution[81] = i;
        pthread_create(&validator[pid++], NULL, rowCheck, (void*)solution);
        pthread_join(validator[i * 3],&result[i*3]);
        pthread_create(&validator[pid++], NULL, columnCheck, (void*)solution);
        pthread_join(validator[i * 3 + 1],&result[i * 3 + 1]);
        pthread_create(&validator[pid++], NULL, gridCheck, (void*)solution);
        pthread_join(validator[i * 3 + 2],&result[i * 3 + 2]);
    }

    //Check the result array and output the result
    output(result);
}

void readSolution(FILE* sudoku, int* solution)
{
    for (int i = 0; i < 81; i++)
    {
        fscanf(sudoku, "%d", &solution[i]);
    }
    fclose(sudoku); 

    for (int i = 0; i < 9; i++)
    {
        for ( int j = 0; j < 9; j++)
            printf(" %d |", solution[i*9 + j]);
        printf("\n");   
    }
}

void* rowCheck(void* arg)
{
    int *solution = (int*) arg;
    int row = solution[81];
    int number[9] = {0};
    int val;
    for(int i = 0; i < 9; i++)
    {
        val = solution[row * 9 + i];
        if (number[val - 1] == 0)
            number[val - 1] = 1;
        else
            pthread_exit((void*) 0);
    }
    pthread_exit((void*) 1);
}

void* columnCheck(void* arg)
{
    int *solution = (int*) arg;
    int col = solution[81];
    int number[9] = {0};
    int val;
    for(int i = 0; i < 9; i++)
    {
        val = solution[col + i * 9];
        if (number[val - 1] == 0)
            number[val - 1] = 1;
        else
            pthread_exit((void*) 0);
    }
    pthread_exit((void*) 1);
}
void* gridCheck(void* arg)
{
    int *solution = (int*) arg;
    int grid = solution[81];
    int number[9] = {0};
    int val;
    for(int i = 0; i < 9; i++)
    {
        int index = grid / 3 * 27 + grid % 3 * 3+ i / 3 * 9 + i % 3;
        val = solution[index];
        if (number[val - 1] == 0)
            number[val - 1] = 1;
        else
            pthread_exit((void*) 0);
    }
    pthread_exit((void*) 1);
}
void output(void* result[])
{
    int count = 0;
    for(int i = 0; i < 27; i++)
    {
        if ((int)(result[i]) == 0)
        {
            count++;
            switch(i%3)
            {
                case 0:
                {
                    printf("The no.%d row is invalid!\n", i / 3 + 1);
                    break;
                }
                case 1:
                {
                    printf("The no.%d column is invalid!\n", i / 3 + 1);
                    break;
                }
                case 2:
                {
                    printf("The no.%d grid is invalid!\n", i / 3 + 1);
                    break;
                }
                default:
                    break;
            }
        }
    }
    if (count == 0)
        printf("The solution is valid!\n");
    else
        printf("The solution is invalid!\n");
}
