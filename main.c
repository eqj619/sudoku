//
//  main.c
//  sudoku
//
//  Created by EIJI OGA on 12/9/19.
//  Copyright © 2019 Eiji Oga. All rights reserved.
//
#define DEBUG_PRINT

struct aslot
{
    int fixed;
    int try;
    int numList[9];
};

void initSudokuForm(int pForm[9][9])
{
    int i, j;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for (j=0; j<9; j++) pForm[i][j] = 0;
    }
}

void printSudokuForm(int pForm[9][9])
{
    int i, j;
    
     printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for (j=0; j<9; j++)
        {
            printf("%d ", pForm[i][j]);
        }
        printf("\n");
    }
}

void initSudokuForm2(struct aslot f[9][9])
{
    int i, j, k;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for (j=0; j<9; j++)
        {
            f[i][j].fixed = 0;
            f[i][j].try = 0;
            for(k=0; k<9; k++) f[i][j].numList[k] = k+1;
        }
    }
}

void printSufokuForm2(struct aslot f[9][9])
{
    int i, j;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for (j=0; j<9; j++)
        {
            if(f[i][j].fixed != 0) printf("%d ", f[i][j].fixed);
            else printf("_ ");
        }
        printf("\n");
    }
    
}

void printSufokuForm2try(struct aslot f[9][9])
{
    int i, j;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for (j=0; j<9; j++)
        {
            if (f[i][j].try != 0) printf("%d ", f[i][j].try);
            else printf("_ ");
        }
        printf("\n");
    }
    
}

//
// check at horizontal line then fix the number when candidate is only one.
//
int CheckHorizontal(int row, int column, struct aslot f[9][9])
{
    int i;
    int cnt=0;
    
    if(f[row][column].fixed != 0 ) return(f[row][column].fixed);
#ifdef DEBUG_PRINT
     printf("%s %d-%d\t", __FUNCTION__, row, column);
#endif
    
    // check num in a line
    for(i=0; i<9; i++)
    {
        if(i != column)
        {
            if(f[row][i].fixed != 0)    // exit this number. then, drop this number from list
            {
                f[row][column].numList[ f[row][i].fixed - 1] = 0;
            }
        }
    }
    // check how many candidate in numList
    for(i=0; i<9; i++)
    {
        if( f[row][column].numList[i] != 0 ) {
            cnt++;
            f[row][column].fixed = f[row][column].numList[i];
        }
#ifdef DEBUG_PRINT
        printf("%d", f[row][column].numList[i]);
#endif
    }
#ifdef DEBUG_PRINT
    printf("\n");
#endif
    if(cnt > 1) f[row][column].fixed = 0;
    if(f[row][column].fixed == 0 && cnt == 0) return (-1);
    return(f[row][i].fixed);
}

//
// check at Vertical line then fix the number when candidate is only one.
//
int CheckVertical(int row, int column, struct aslot f[9][9])
{
    int result = 0; // this number is fixed.
    int i;
    int cnt=0;

    if(f[row][column].fixed != 0 ) return(f[row][column].fixed);
#ifdef DEBUG_PRINT
     printf("%s %d-%d\t", __FUNCTION__, row, column);
#endif
     
    
    // check num in a line
    for(i=0; i<9; i++)
    {
        if(i != row)
        {
            if(f[i][column].fixed != 0)    // exit this number. then, drop this number from list
            {
                f[row][column].numList[ f[i][column].fixed - 1] = 0;
            }
        }
    }
    // check how many candidate in numList
    for(i=0; i<9; i++)
    {
        if( f[row][column].numList[i] != 0 ) {
            cnt++;
            f[row][column].fixed = f[row][column].numList[i];
        }
#ifdef DEBUG_PRINT
        printf("%d", f[row][column].numList[i]);
#endif
    }
#ifdef DEBUG_PRINT
    printf("\n");
#endif
    if(cnt > 1) f[row][column].fixed = 0;
    
    if(f[row][column].fixed == 0 && cnt == 0) return (-1);
    return(f[row][i].fixed);
}

//
// check at block then fix the number when candidate is only one.
//
int CheckBlock(int row, int column, struct aslot f[9][9])
{
    const int map[9][9] = {
        {0,0,0,1,1,1,2,2,2},
        {0,0,0,1,1,1,2,2,2},
        {0,0,0,1,1,1,2,2,2},
        {3,3,3,4,4,4,5,5,5},
        {3,3,3,4,4,4,5,5,5},
        {3,3,3,4,4,4,5,5,5},
        {6,6,6,7,7,7,8,8,8},
        {6,6,6,7,7,7,8,8,8},
        {6,6,6,7,7,7,8,8,8}
    };
    
    int targetBlock;
    int checkCnt = 8;
    int cnt=0;
    int i, j;
    
   if(f[row][column].fixed != 0 ) return(f[row][column].fixed);
#ifdef DEBUG_PRINT
    printf("%s %d-%d\t", __FUNCTION__, row, column);
#endif
    
    targetBlock = map[row][column];
    
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if(map[i][j] == targetBlock)
            {
                if(i != row && j != column)
                {
                    if(f[i][j].fixed != 0)    // exit this number. then, drop this number from list
                    {
                        f[row][column].numList[ f[i][j].fixed - 1] = 0;
                    }
                    checkCnt--;
                    if (checkCnt == 0) break;
                }
            }
        }
    }
    
    // check how many candidate in numList
    for(i=0; i<9; i++)
    {
        if( f[row][column].numList[i] != 0 ) {
            cnt++;
            f[row][column].fixed = f[row][column].numList[i];
        }
#ifdef DEBUG_PRINT
        printf("%d", f[row][column].numList[i]);
#endif
    }
#ifdef DEBUG_PRINT
    printf("\n");
#endif
    if(cnt > 1) f[row][column].fixed = 0;
    
    if(f[row][column].fixed == 0 && cnt == 0) return (-1);
    return(f[row][i].fixed);
}

int checkWholeTable(struct aslot f[9][9])
{
    int i, j;
    int result = 1; // success
    
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            result = CheckHorizontal(i, j, f);
            if (result < 0) return (-1);
            result = CheckVertical(i,j, f);
            if (result < 0) return (-1);
            result = CheckBlock(i,j, f);
            if (result < 0) return (-1);
        }
    }
    return(1);
}

// look at the number of candidates in each slot
// then, return one of trial number when candiadtes numbers is list on 2.
//
int numOfList(int row, int column, struct aslot f[9][9])
{
    int i;
    int cnt = 0;
    int try = 0;
    
    if(f[row][column].fixed != 0) return (0);
    
    for(i=0; i<9; i++)
    {
        if(f[row][column].numList[i] != 0)
        {
            cnt++;
            // if(try == 0) try = f[row][column].numList[i];
            try = f[row][column].numList[i];
        }
    }
    
    f[row][column].try = 0;
    if(cnt == 2)
    {
        f[row][column].try = try;
    }
    return(cnt);
}

void printNumOfList(struct aslot f[9][9])
{
    int i, j;
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            printf("%d ", numOfList(i,j,f));
        }
        printf("\n");
    }
}
int IsComplete(struct aslot f[9][9])
{
    int i, j;
    int result = 0;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            if (f[i][j].fixed == 0 ) result++;
        }
    }
    return (result);
}

#include <stdio.h>

int main(int argc, const char * argv[]) {
    struct aslot form[9][9];
    
    // insert code here...
    printf("Hello, World!\n");
    
    //initSudokuForm(sudokuForm);
    // printSudokuForm(sudokuForm);

    initSudokuForm2(form);
    printSufokuForm2(form);
    
    // fill test pattern
    form[0][1].fixed = 3;
    form[0][5].fixed = 7;
    form[0][6].fixed = 2;
    
    form[1][0].fixed = 8;
    form[1][4].fixed = 9;
    form[1][6].fixed = 7;
    
    form[2][0].fixed = 6;
    form[2][3].fixed = 8;
    form[2][8].fixed = 1;
    
    form[3][1].fixed = 2;
    form[3][5].fixed = 4;
    form[3][6].fixed = 3;
    
    //sudokuForm[4][0] =
    //sudokuForm[4][0] =
    //sudokuForm[4][0] =
    
    form[5][2].fixed = 4;
    form[5][3].fixed = 9;
    form[5][7].fixed = 5;
    
    form[6][0].fixed = 1;
    form[6][5].fixed = 9;
    form[6][8].fixed = 4;
    
    form[7][1].fixed = 7;
    form[7][4].fixed = 3;
    form[7][8].fixed = 2;
    
    form[8][2].fixed = 5;
    form[8][3].fixed = 6;
    form[8][7].fixed = 8;
    
    // TRY
    //form[0][2].fixed = 1;
    //form[0][4].fixed = 4;
    //form[0][0].fixed = 9;
    //form[1][1].fixed = 4;
    //form[1][3].fixed = 1;
    
    printf("checkWholeTable = %d\n", checkWholeTable(form));
    printSufokuForm2(form);
    printNumOfList(form);
    printf("Is complete %d\n", IsComplete(form));
    
#if 0
    int i, j, loop, cnt;

    for (loop = 0; loop < 20; loop++)
    {
        printf("---- LOOP %d ----\n", loop);
        for(i=0; i<9; i++)
        {
            for(j=0; j<9; j++)
            {
                cnt = numOfList(i,j,form);
                printf("%d ", cnt);
                if(cnt != 0 && form[i][j].try != 0) {
                    form[i][j].fixed = form[i][j].try;
                    checkWholeTable(form);
                    printSufokuForm2(form);
                    printNumOfList(form);
                    break;
                }
            }
            printf("\n");
        }
        //printSufokuForm2try(form);
    }
#endif
    

    
    return 0;
}
