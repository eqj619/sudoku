//
//  main.c
//  sudoku
//
//  Created by EIJI OGA on 12/9/19.
//  Copyright © 2019 Eiji Oga. All rights reserved.
//
#define DEBUG_PRINT
#define SUDOKU_RESOLVER
#define TEST00

#include <stdio.h>
#include <stdlib.h>

struct aslot
{
    int fixed;      // 0 ... not find yet.
    int try1;
    int try2;
    int numList[9]; // candidate numbers
};

const struct aslot aslotInit = {0, 0, 0, {1,2,3,4,5,6,7,8,9}};

struct checkPoint
{
    struct checkPoint   *pPrev;     // Null = head
    int                 slotNum;    // 0 to 80
    int                 capMap[9*9];
    int                 try1;       // 0 is fail
    struct checkPoint   *pNextTry1; // next pointer
    int                 try2;
    struct checkPoint   *pNextTry2;
};

void initSudokuForm2(struct aslot f[9][9])
{
    int i, j;
    
    printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++){
        for (j=0; j<9; j++){
            f[i][j] = aslotInit;
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
    
#ifdef DEBUG_PRINT1
     printf("%s %d-%d\t", __FUNCTION__, row, column);
#endif
    
    // it's alreay fixed number
    if(f[row][column].fixed != 0 ){
#ifdef DEBUG_PRINT1
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
#ifdef DEBUG_PRINT1
        printf("%d", f[row][column].numList[i]);
#endif
    }
#ifdef DEBUG_PRINT1
    printf("\n");
#endif
    // cnt = 0 ... ERROR cannot find a number. all number are already there.
    // cnt = 1 ... find a number
    // cnt > 1 ... cannot fix because of 2 and more candidate remain. then, reset fixed value.
    if(cnt > 1) f[row][column].fixed = 0;
    if(f[row][column].fixed == 0 && cnt == 0) return (-1);
    return(f[row][column].fixed);
}

int checkWholeTable(struct aslot f[9][9])
{
    int i, j;
    int result = 1; // success
    
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            result = VerifyNumber(i, j, f);
            if (result == -1) return (-1);
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

// count the number of none-resolved slot
int IsComplete(struct aslot f[9][9])
{
    int i, j;
    int result = 0;
    
    //printf("%s\n", __FUNCTION__);
    for(i=0; i<9; i++){
        for(j=0; j<9; j++){
            if (f[i][j].fixed == 0 ) result++;
        }
    }
    return (result);
}

void fillSlot(int pSudoku[], struct aslot f[9][9])
{
    int i;
    
    for (i=0; i< (9*9); i++){
        f[i/9][i%9].fixed = pSudoku[i];
    }
}

void captureNumMap(int *pNm, struct aslot f[9][9])
{
    int i;
    
    printf(">>>> %s\n", __FUNCTION__);
    for(i=0; i<9*9; i++)
    {
        pNm[i] = f[i/9][i%9].fixed;
    }
}

void restoreNumMap(int *pNm, struct aslot f[9][9])
{
    int i;
    printf(">>>> %s\n", __FUNCTION__);
    
    for(i=0; i<9*9; i++)
    {
        if(pNm[i] == 0 && f[i/9][i%9].fixed != 0)
        {
            f[i/9][i%9] = aslotInit;
        }
        else if(f[i/9][i%9].fixed == 0)
        {   // clean up numList for next verification
            int j;
            for(j=0; j<9; j++) f[i/9][i%9].numList[j] = j+1;
        }
        else if(pNm[i] != f[i/9][i%9].fixed)
        {
            printf("ERROR %s: pNm[%d]=%d, f[%d][%d]=%d\n", __FUNCTION__, i, pNm[i], i/9, i%9, f[i/9][i%9].fixed);
        }
    }

}
// Check then fill the number in resolved slot
int sudokuChecker(struct aslot f[9][9])
{
    static int lastNum = 0;
    int openSlot = -1;
    int result = 0;
    
    while(result != -1)
    {
        result = checkWholeTable(f);
        printf("checkWholeTable = %d\n", result);
        
        printSudokuForm2(f);
        printNumOfList(f);
        openSlot = IsComplete(f);
        printf("Is complete %d\n", openSlot);
        if (openSlot == lastNum) break;
        else lastNum = openSlot;
    }
    
    return (result);
}

struct checkPoint *addCheckPoint(struct checkPoint *pCP, int flag)
{
    struct checkPoint *pTmp;
    
    pTmp = malloc(sizeof(struct checkPoint));

    if (flag == 1){
        pCP->pNextTry1 = pTmp;
    }
    if (flag == 2){
        pCP->pNextTry2 = pTmp;
    }
    
    pTmp->pPrev = pCP;
    pTmp->slotNum = -1;
    pTmp->try1 = 0;
    pTmp->pNextTry1 = NULL;
    pTmp->try2 = 0;
    pTmp->pNextTry2 = NULL;
    
    return(pTmp);
}

int main(int argc, const char * argv[]) {
    struct aslot form[9][9];
    
    int result;
    struct checkPoint topCP =
        {NULL, -1,
        {    0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,
             0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,
         },
            0, NULL, 0, NULL};
    struct checkPoint *pHead = NULL;
    
    // insert code here...
    printf("Hello, World!\n");
    
    //initSudokuForm(sudokuForm);
    // printSudokuForm(sudokuForm);

    initSudokuForm2(form);
    printSudokuForm2(form);

#if 0
    int test02[] = {
        1,5,7,6,4,0,0,9,8,
        2,0,9,0,0,0,0,0,0,
        0,0,0,9,1,0,0,0,4,
        
        8,0,0,4,3,0,0,5,0,
        0,0,0,0,0,0,0,0,0,
        0,2,0,0,6,8,0,0,7,
        
        7,0,0,0,8,6,0,0,0,
        0,0,0,0,0,0,0,0,1,
        0,9,0,0,0,4,8,6,2
    };
    

    int test02[] = {
        1,5,7,6,4,2,0,9,8,
        2,0,9,0,5,3,0,0,0,
        0,0,0,9,1,0,0,0,4,
        
        8,0,0,4,3,0,0,5,0,
        0,0,0,0,0,0,0,0,0,
        0,2,0,0,6,8,0,0,7,
        
        7,0,0,0,8,6,0,0,0,
        0,0,0,0,0,0,0,0,1,
        0,9,0,0,0,4,8,6,2
    };
    
    fillSlot (test02, form);
#endif
    
#ifdef TEST00
    // fill test pattern 00
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
    //form[0][2].fixed = 9; //19
    //form[0][0].fixed = 5; //45
    //form[0][3].fixed = 1; //14
#endif

#ifdef TEST02
    form[0][0].fixed = 7; //56
    form[0][1].fixed = 5; //35
    form[1][1].fixed = 3; //9, 4
    form[1][3].fixed = 4;  //57
    form[1][7].fixed = 7;
    form[2][0].fixed = 8; //56
    form[2][4].fixed = 6; //35
    form[2][6].fixed = 1; //9, 4
    
    form[3][1].fixed = 9;  //57
    form[3][5].fixed = 4;
    form[3][7].fixed = 6; //56
    form[4][2].fixed = 5; //35
    form[4][3].fixed = 3; //9, 4
    form[4][7].fixed = 1;  //57
    form[5][2].fixed = 2;
    form[5][4].fixed = 9; //56
    form[5][8].fixed = 7; //35
    
    form[6][2].fixed = 4; //9, 4
    form[6][3].fixed = 9;  //57
    form[6][8].fixed = 2;
    form[7][0].fixed = 9; //9, 4
     form[7][5].fixed = 8;  //57
     form[7][6].fixed = 6;
    form[8][6].fixed = 4; //9, 4
     form[8][8].fixed = 8;  //57
    
     // TRY
    form[0][2].fixed = 6; //16
    form[1][4].fixed = 8; //58
    
    form[0][3].fixed = 2; //12 HEAD
    form[0][4].fixed = 1; //13
    form[0][5].fixed = 3; //39
        form[0][6].fixed = 9; //89
        form[2][3].fixed = 5; //57
#endif
    
    //========================
    sudokuChecker(form);
    
#ifdef SUDOKU_RESOLVER
    int i, loop, flag = 3;
    
    if(topCP.slotNum == -1){        // initial loop
        pHead = &topCP;
    }
    captureNumMap(pHead->capMap, form);
    
    loop = 0;
    while(loop < 20)
    {
        loop++;
        printf("===== LOOP %d ============================\n", loop);
        
        // find 2 candidates slot if checkPoint is new.
        // then, capture the number's map before try
        if(pHead->try1 == 0 && pHead->try2 == 0)
        {
            captureNumMap(pHead->capMap, form);
            for (i=0; i < (9*9); i++){
                if ( numOfList(i/9, i%9, form) == 2)
                {
                    pHead->slotNum = i;
                    pHead->try1 = form[i/9][i%9].try1;
                    pHead->try2 = form[i/9][i%9].try2;
                    break;
                }
            }
        }
        printf("pHead pPrev 0x%08x, slotNum %d(%d-%d), try1 %d, 0x%08x, try2 %d, try2 ptr 0x%08x\n",
               pHead->pPrev, pHead->slotNum, pHead->slotNum/9,pHead->slotNum%9, pHead->try1, pHead->pNextTry1, pHead->try2, pHead->pNextTry2 );
        
        // Before try to fill-in number re-store captured numbers
        restoreNumMap(pHead->capMap, form);
        // Try candidate
        if (pHead->try1 > 0 && pHead->pNextTry1 == NULL){
            form[pHead->slotNum/9][pHead->slotNum%9].fixed = pHead->try1;
            pHead->try1 = -1;
            flag = 1;
        }
        else if (pHead->try2 > 0 && pHead->pNextTry2 == NULL){
            form[pHead->slotNum/9][pHead->slotNum%9].fixed = pHead->try2;
            pHead->try2 = -1;
            flag = 2;
        }
        else if (pHead->try1 == -1 && pHead->try2 == -1){
            // roll back previous check point
            printf("BOTH Trial number are failed... Rollback\n");
            form[pHead->slotNum/9][pHead->slotNum%9].fixed = 0;

            pHead = pHead->pPrev;
            continue;
        }
        
        // check
        result = sudokuChecker(form);
        
        if (result == 1)
        {
            struct checkPoint *pTmp;
            // if result is good. add next slot, link then move a head pointer
            pTmp = addCheckPoint(pHead, flag);
            
            if (pTmp != 0) pHead = pTmp;
            else{
                printf("Error %d\n", __LINE__);
                return(-1);
            }
        }
        else
        {
            printf("sudokuChecker FAILED flag = %d\n", flag);
            form[pHead->slotNum/9][pHead->slotNum%9].fixed = 0;
            // TODO: HAVE TO CLEAN UP fixed value in here.
        }
    } // while(loop)
    
    
#endif
    

    
    return 0;
}
