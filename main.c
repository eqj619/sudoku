//
//  main.c
//  sudoku
//
//  Created by EIJI OGA on 12/9/19.
//  Copyright © 2019 Eiji Oga. All rights reserved.
//
#define DEBUG_PRINT

#include <stdio.h>

struct aslot
{
    int fixed;      // 0 ... not find yet.
    int try1;
    int try2;
    int numList[9]; // candidate numbers
};

void initSudokuForm2(struct aslot f[9][9])
{
    int i, j, k;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++){
        for (j=0; j<9; j++){
            f[i][j].fixed = 0;
            f[i][j].try1 = 0;
            f[i][j].try2 = 0;
            for(k=0; k<9; k++) f[i][j].numList[k] = k+1;
        }
    }
}

void printSudokuForm2(struct aslot f[9][9])
{
    int i, j;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++){
        for (j=0; j<9; j++){
            if(f[i][j].fixed != 0) printf("%d ", f[i][j].fixed);
            else printf("_ ");
        }
        printf("\n");
    }
    
}

int VerifyNumber(int row, int column, struct aslot f[9][9])
{
    int i;
    int cnt;
    
#ifdef DEBUG_PRINT
     printf("%s %d-%d\t", __FUNCTION__, row, column);
#endif
    
    // it's alreay fixed number
    if(f[row][column].fixed != 0 ){
#ifdef DEBUG_PRINT
        printf("\n");
#endif
        return(f[row][column].fixed);
    }
    
    // Check at Horizontal Line
    for(i=0; i<9; i++){
        if(i != column){                // skip by itself
            if(f[row][i].fixed != 0){   // exit this number. then, drop this number from list
                f[row][column].numList[ f[row][i].fixed - 1] = 0;
            }
        }
    }
    // Check at Vertical Line
    for(i=0; i<9; i++){
        if(i != row){                   // skip by itself
            if(f[i][column].fixed != 0){    // exit this number. then, drop this number from list
                f[row][column].numList[ f[i][column].fixed - 1] = 0;
            }
        }
    }
    // Check Block
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
    int j;
    
    targetBlock = map[row][column];
    
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            if(map[i][j] == targetBlock){       // in same block
                if(i != row && j != column){    // skip by itself
                    if(f[i][j].fixed != 0){     // exit this number. then, drop this number from list
                        f[row][column].numList[ f[i][j].fixed - 1] = 0;
                    }
                    checkCnt--;
                    if (checkCnt == 0) break;   // verify all number in a block
                }
            }
        }
    }
    
    // check how many candidate in numList
    cnt = 0;
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
    // cnt = 0 ... ERROR cannot find a number. all number are already there.
    // cnt = 1 ... find a number
    // cnt > 1 ... cannot fix because of 2 and more candidate remain. then, reset fixed value.
    if(cnt > 1) f[row][column].fixed = 0;
    if(f[row][column].fixed == 0 && cnt == 0) return (-1);
    return(f[row][i].fixed);
}

int checkWholeTable(struct aslot f[9][9])
{
    int i, j;
    int result = 1; // success
    
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            result = VerifyNumber(i, j, f);
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
    int try[2];
    
    if(f[row][column].fixed != 0) return (0);
    
    for(i=0; i<9; i++){
        if(f[row][column].numList[i] != 0){
            cnt++;
            // if(try == 0) try = f[row][column].numList[i];
            if(cnt==1) try[0] = f[row][column].numList[i];
            if(cnt==2) try[1] = f[row][column].numList[i];
        }
    }
    
    if(cnt == 2)
    {
        f[row][column].try1 = try[0];
        f[row][column].try2 = try[1];
    }
    return(cnt);
}

void printNumOfList(struct aslot f[9][9])
{
    int i, j;
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
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
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            if (f[i][j].fixed == 0 ) result++;
        }
    }
    return (result);
}

#include <stdio.h>

int main(int argc, const char * argv[]) {
    struct aslot form[9][9];
    int result;
    
    // insert code here...
    printf("Hello, World!\n");
    
    //initSudokuForm(sudokuForm);
    // printSudokuForm(sudokuForm);

    initSudokuForm2(form);
    printSudokuForm2(form);
    
    // fill test pattern
    form[0][1].fixed = 3;
    form[0][5].fixed = 7;
    form[0][6].fixed = 2;
    
    form[1][0].fixed = 8;
    form[1][4].fixed = 9;
    form[1][7].fixed = 7;
    
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
    form[0][2].fixed = 9; //19
    form[0][0].fixed = 5; //45
    form[0][3].fixed = 1; //14
    
    //form[1][6].fixed = 6; //56
    //form[1][8].fixed = 5; //35
    //form[2][6].fixed = 4; //9, 4
    //form[3][0].fixed = 5;  //57
    //form[3][3].fixed = 7;
    
    int lastNum = 0;
    int openSlot = -1;
    
    result = 0;
    while(result != -1)
    {
        result = checkWholeTable(form);
        printf("checkWholeTable = %d\n", result);
        
        printSudokuForm2(form);
        printNumOfList(form);
        openSlot = IsComplete(form);
        printf("Is complete %d\n", openSlot);
        if (openSlot == lastNum) break;
        else lastNum = openSlot;
    }
    
    
#if 0
    int i, j, loop, cnt;

    loop = 0;
    printf("---- LOOP %d ----\n", loop++);
    for(i=0; i<9; i++)
    {
        for(j=0; j<9; j++)
        {
            cnt = numOfList(i,j,form);
            if (cnt == 2)
            {
                printf("======== Count 2 challenge %d,%d\n", i, j);
                //first trial
                printf("============ first try %d\n",form[i][j].try1 );
                form[i][j].fixed = form[i][j].try1;
                result = checkWholeTable(form);
                if (result != -1)
                {
                    printf("first challege success\n");
                    printSudokuForm2(form);
                    printNumOfList(form);
                    printf("Is complete %d\n", IsComplete(form));
                    continue;
                }
                // second trial
                printf("============ second try %d\n",form[i][j].try2 );
                form[i][j].fixed = form[i][j].try2;
                result = checkWholeTable(form);
                if (result != -1)
                {
                    printf("second challege success\n");
                    printSudokuForm2(form);
                    printNumOfList(form);
                    printf("Is complete %d\n", IsComplete(form));
                    continue;
                }
                
                printf("second trail FAILD\n");
                form[i][j].fixed = 0;
            }
        }
        printf("\n");
    }
    //printSufokuForm2try(form);
#endif
    

    
    return 0;
}
