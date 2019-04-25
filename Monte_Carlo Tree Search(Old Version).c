#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// board information
#define BOARD_SIZE 12
#define EMPTY 0
#define BLACK 1
#define WHITE 2

// bool
typedef int BOOL;
#define TRUE 1
#define FALSE 0

// option
typedef int OPTION;
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
#define UP_LEFT 4
#define UP_RIGHT 5
#define DOWN_LEFT 6
#define DOWN_RIGHT 7

#define MAX_BYTE 10000

#define START "START"
#define PLACE "PLACE"
#define TURN  "TURN"
#define END   "END"



char buffer[MAX_BYTE] = {0};
char board[BOARD_SIZE][BOARD_SIZE] = {0};
int me_flag;
int other_flag;

const int DIR[8][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

void debug(const char *str) {
    printf("DEBUG %s\n", str);
    fflush(stdout);
}

void printBoard() {
    char visual_board[BOARD_SIZE][BOARD_SIZE] = {0};
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board[i][j] == EMPTY) {
                visual_board[i][j] = '.';
            } else if (board[i][j] == BLACK) {
                visual_board[i][j] = 'O';
            } else if (board[i][j] == WHITE) {
                visual_board[i][j] = 'X';
            }
        }
        printf("%s\n", visual_board[i]);
    }
}

BOOL isInBound(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

/**
 * YOUR CODE BEGIN
 * 娴ｇ姷娈戞禒锝囩垳瀵偓婵?
 */

/**
 * You can define your own struct and variable here
 * 娴ｇ姴褰叉禒銉ユ躬鏉╂瑩鍣风€规矮绠熸担鐘哄殰瀹歌京娈戠紒鎾寸€担鎾虫嫲閸欐﹢鍣?
 */


#include <limits.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
int Search_Depth=4;
#define MAX 0
#define MIN 1
#define hashlen 9999991
#define para_beta 2

#define isInBound(x, y) (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE)

//struct Command nextmove[140];
int ChessCount,direction;
char cur_board[BOARD_SIZE][BOARD_SIZE] = {0};
struct Command ans_command,second_command;
float cen_x,cen_y;
int theta1=1,theta2=1,theta3=0;
long long hash=0;
int turns=0;

//hashing
struct move{
    int x;
    int y;
    OPTION option;
};
struct move bestmove[hashlen];
struct move repetition[hashlen];
int changeit=FALSE;
int changed=FALSE;
unsigned int hashtable[BOARD_SIZE][BOARD_SIZE][3]={0};

struct Command
{
    int x;
    int y;
    int value;      //閻犲洢鍎查幖閿嬫媴濠婂懏鐣卞ù鐘插槻閳?
    OPTION option;
    char pro_board[BOARD_SIZE][BOARD_SIZE];
    int count;
    int dir;
    unsigned long hash;
};

struct Command sortedmove[140];
int num_SortArray,alphasort,betasort;
int eaten;

/*
int Evaluate(){
  int value=1,bonus=0;
    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++)
            if (cur_board[i][j] == me_flag){
                value=value*2;
                bonus=bonus+i*i+j*j;
        }
  return 100*(value)-bonus/10;
}*/

int Evaluate(){
    if (ChessCount==0) return INT_MIN;
    int hello; //请问这个网站怎么注册？

    double value=1;
    double x1=2000,x2=100,x3=601,x4=599,x5=40,x6=609,x7=611,x8=0,x9=0,x10=0,x11=0,x12=0,x13=3,x14=0,x15=0;
    double Dis=0;
    double tiao=0,jia=0,rec=0,otherjia=0,othertiao=0,me_tiao=0,me_jia=0,ozzo=0,other_jia=0,other_tiao=0,num_Diamond=0,MeMoveActions=0,OtherMoveActions=0;
    int defense_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    int Rec_dir[8][2] = { {1, 3}, {1, -3}, {-1, 3}, {-1, -3}, {3, 1}, {3, -1}, {-3, 1}, {-3, -1} };
    int Diamond[8][3][2] = {{{-2,-1},{-2,1},{-4,0}},{{-1,2},{1,2},{0,4}},{{2,-1},{2,1},{4,0}},{{-1,-2},{1,-2},{0,-4}},
                            {{-1,-2},{-1,2},{-2,0}},{{-2,1},{2,1},{0,2}},{{1,-2},{1,2},{2,0}},{{-2,-1},{2,-1},{0,-2}}};

    if (ChessCount>=10) x10=30;
    if (me_flag == WHITE) {
        //x10=30;
    }
/*
    char tiao_board[BOARD_SIZE][BOARD_SIZE] = {0};
    char jia_board[BOARD_SIZE][BOARD_SIZE] = {0};
    char intiao_board[BOARD_SIZE][BOARD_SIZE] = {0};
    char injia_board[BOARD_SIZE][BOARD_SIZE] = {0};
    */
/*
    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++) {
            if (cur_board[i][j] == other_flag)
                for (int k = 0; k < 8; k++)
                    for (int t = 0; t < 8; t++)
                        if (cur_board[i + defense_dir[k][0]][j + defense_dir[k][1]] == EMPTY) {
                            int x1 = i + defense_dir[k][0] + defense_dir[t][0];
                            int y1 = j + defense_dir[k][1] + defense_dir[t][1];
                            int x2 = i + defense_dir[k][0] + defense_dir[t][0] * 2;
                            int y2 = j + defense_dir[k][1] + defense_dir[t][1] * 2;
                            int x3 = i + defense_dir[k][0] - defense_dir[t][0];
                            int y3 = j + defense_dir[k][1] - defense_dir[t][1];
                            if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == other_flag &&
                                cur_board[x1][y1] == EMPTY)
                                jia_board[x1][y1] = TRUE;
                            if (isInBound(x1, y1) && isInBound(x3, y3) && cur_board[x1][y1] == me_flag &&
                                cur_board[x3][y3] == EMPTY)
                                tiao_board[x3][y3] = TRUE;
                        }
            if (cur_board[i][j] == me_flag)
                for (int k = 0; k < 8; k++)
                    for (int t = 0; t < 8; t++)
                        if (cur_board[i + defense_dir[k][0]][j + defense_dir[k][1]] == EMPTY) {
                            int x1 = i + defense_dir[k][0] + defense_dir[t][0];
                            int y1 = j + defense_dir[k][1] + defense_dir[t][1];
                            int x2 = i + defense_dir[k][0] + defense_dir[t][0] * 2;
                            int y2 = j + defense_dir[k][1] + defense_dir[t][1] * 2;
                            int x3 = i + defense_dir[k][0] - defense_dir[t][0];
                            int y3 = j + defense_dir[k][1] - defense_dir[t][1];
                            if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == me_flag &&
                                cur_board[x1][y1] == EMPTY)
                                injia_board[x1][y1] = TRUE;
                            if (isInBound(x1, y1) && isInBound(x3, y3) && cur_board[x1][y1] == other_flag &&
                                cur_board[x3][y3] == EMPTY)
                                intiao_board[x3][y3] = TRUE;
                        }
        }

    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++) {
            if (cur_board[i][j] == me_flag)
                for (int k = 0; k < 8; k++) {
                    if (tiao_board[i + defense_dir[k][0]][j + defense_dir[k][1]]) me_tiao++;
                    if (jia_board[i + defense_dir[k][0]][j + defense_dir[k][1]]) me_jia++;
                    if (!tiao_board[i + defense_dir[k][0]][j + defense_dir[k][1]] && !jia_board[i + defense_dir[k][0]][j + defense_dir[k][1]])
                        if (cur_board[i + defense_dir[k][0]][j + defense_dir[k][1]] == EMPTY) MeMoveActions++;
                }
            if (cur_board[i][j] == other_flag)
                for (int k = 0; k < 8; k++) {
                    if (intiao_board[i + defense_dir[k][0]][j + defense_dir[k][1]]) other_tiao++;
                    if (injia_board[i + defense_dir[k][0]][j + defense_dir[k][1]]) other_jia++;
                    if (!intiao_board[i + defense_dir[k][0]][j + defense_dir[k][1]] && !injia_board[i + defense_dir[k][0]][j + defense_dir[k][1]])
                        if (cur_board[i + defense_dir[k][0]][j + defense_dir[k][1]] == EMPTY) OtherMoveActions++;
                }
        }
*/
    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++) {
            if (cur_board[i][j] == me_flag){
                for (int t = 0; t < 8; t++) {
                    int x1 = i + defense_dir[t][0];
                    int y1 = j + defense_dir[t][1];
                    int x2 = i + defense_dir[t][0] * 2;
                    int y2 = j + defense_dir[t][1] * 2;
                    if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == me_flag && cur_board[x1][y1] == EMPTY)
                        for (int k = 0; k < 8; k++)
                            if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == other_flag) {
                                tiao++;
                                break;
                            }

                }

                for (int t = 0; t < 8; t++) {
                    int x1 = i + defense_dir[t][0];
                    int y1 = j + defense_dir[t][1];
                    int x2 = i - defense_dir[t][0];
                    int y2 = j - defense_dir[t][1];
                    if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == other_flag && cur_board[x1][y1] == EMPTY)
                        for (int k = 0; k < 8; k++)
                            if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == other_flag) {
                                jia++;
                                break;
                            }
                }

                for (int t = 0; t < 8; t++) {
                    int x1 = i + Diamond[t][0][0];
                    int y1 = j + Diamond[t][0][1];
                    int x2 = i + Diamond[t][1][0];
                    int y2 = j + Diamond[t][1][1];
                    int x3 = i + Diamond[t][2][0];
                    int y3 = j + Diamond[t][2][1];
                    if (isInBound(x1, y1) && isInBound(x2, y2) && isInBound(x3, y3))
                        if (cur_board[x1][y1] == me_flag && cur_board[x2][y2] == me_flag && cur_board[x3][y3] == me_flag)
                            num_Diamond++;
                }

                for (int t=0; t<8; t++) {
                    if (cur_board[i+Rec_dir[t][0]][j+Rec_dir[t][1]] == me_flag)
                        rec++;
                    if (isInBound(i+defense_dir[t][0]*3,j+defense_dir[t][1]*3))
                        if (cur_board[i+defense_dir[t][0]*3][j+defense_dir[t][1]*3] == me_flag && cur_board[i+defense_dir[t][0]*2][j+defense_dir[t][1]*2] == EMPTY && cur_board[i+defense_dir[t][0]][j+defense_dir[t][1]] == EMPTY)
                            ozzo++;
                }

                Dis=Dis+pow((i-cen_x),2)+pow((j-cen_y),2);
            }

            if (cur_board[i][j] == other_flag){
                for (int t = 0; t < 8; t++) {
                    int x1 = i + defense_dir[t][0];
                    int y1 = j + defense_dir[t][1];
                    int x2 = i + defense_dir[t][0] * 2;
                    int y2 = j + defense_dir[t][1] * 2;
                    if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == other_flag && cur_board[x1][y1] == EMPTY)
                        for (int k = 0; k < 8; k++)
                            if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == me_flag) {
                                othertiao++;
                                break;
                            }

                }

                for (int t = 0; t < 8; t++) {
                    int x1 = i + defense_dir[t][0];
                    int y1 = j + defense_dir[t][1];
                    int x2 = i - defense_dir[t][0];
                    int y2 = j - defense_dir[t][1];
                    if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == me_flag && cur_board[x1][y1] == EMPTY)
                        for (int k = 0; k < 8; k++)
                            if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == me_flag) {
                                otherjia++;
                                break;
                            }
                }

            }
        }

    Dis=Dis/ChessCount;
    MeMoveActions=MeMoveActions/ChessCount;
    OtherMoveActions=OtherMoveActions/(16-ChessCount);
    value=ChessCount*x1-Dis*x2-tiao*x3-jia*x4+rec*x5+otherjia*x6+othertiao*x7-me_tiao*x8-me_jia*x9+ozzo*x10+other_tiao*x11+other_jia*x12+num_Diamond*x13+MeMoveActions*x14-OtherMoveActions*x15;
    return (int)(value);
}



int Compare_Evaluate(const void *a, const void *b)
{
    return (*(struct Command *)b).value - (*(struct Command *)a).value;
}


void modify(struct Command *move){
    int value=0;
    int cur_me_flag=cur_board[move->x][move->y];
    int cur_other_flag=3-cur_me_flag;

    int Rec_dir[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

    for (int i=0; i<8; i++){
        if (move->pro_board[move->x+Rec_dir[i][0]][move->y+Rec_dir[i][1]] == cur_me_flag) value=value-3*theta2;
        if (move->pro_board[move->x+DIR[i][0]][move->y+DIR[i][1]] == cur_me_flag) value=value+1*theta3;
        if (move->pro_board[move->x+DIR[i][0]][move->y+DIR[i][1]] == cur_other_flag) value=value-5;
        if (isInBound(move->x+DIR[i][0]*3,move->y+DIR[i][1]*3))
            if (move->pro_board[move->x+DIR[i][0]][move->y+DIR[i][1]] == EMPTY && move->pro_board[move->x+2*DIR[i][0]][move->y+2*DIR[i][1]] == EMPTY && move->pro_board[move->x+3*DIR[i][0]][move->y+3*DIR[i][1]] == me_flag)
                value=value-20;
    }
    //閻犙囶暒缂嶅懏瀵奸弶搴撳亾?缂佸矁妗ㄧ紞?

    int new_x = move->x + DIR[move->option][0];
    int new_y = move->y + DIR[move->option][1];


    move->hash=hash^hashtable[move->x][move->y][cur_me_flag]^hashtable[new_x][new_y][cur_me_flag];

    memcpy(move->pro_board,cur_board,sizeof(move->pro_board));
    move->count=ChessCount;
    move->pro_board[move->x][move->y] = EMPTY;
    move->pro_board[new_x][new_y] = cur_me_flag;
    move->dir=direction;
    eaten=FALSE;

    // 闁?
    int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (int i = 0; i < 4; i++) {
        int x1 = new_x + intervention_dir[i][0];
        int y1 = new_y + intervention_dir[i][1];
        int x2 = new_x - intervention_dir[i][0];
        int y2 = new_y - intervention_dir[i][1];
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x1][y1] == cur_other_flag && move->pro_board[x2][y2] == cur_other_flag) {
            move->pro_board[x1][y1] = cur_me_flag;
            move->pro_board[x2][y2] = cur_me_flag;
            move->hash=move->hash^hashtable[x1][y1][cur_other_flag]^hashtable[x2][y2][cur_other_flag]^hashtable[x1][y1][cur_me_flag]^hashtable[x2][y2][cur_me_flag];
            if (cur_me_flag==me_flag) move->count=move->count+2;
            else move->count=move->count-2;
            value=value+1100;
            eaten=TRUE;
        }
    }

    // 濠?
    int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (int i = 0; i < 8; i++) {
        int x1 = new_x + custodian_dir[i][0];
        int y1 = new_y + custodian_dir[i][1];
        int x2 = new_x + custodian_dir[i][0] * 2;
        int y2 = new_y + custodian_dir[i][1] * 2;
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x2][y2] == cur_me_flag && move->pro_board[x1][y1] == cur_other_flag) {
            move->pro_board[x1][y1] = cur_me_flag;
            move->hash=move->hash^hashtable[x1][y1][cur_other_flag]^hashtable[x1][y1][cur_me_flag];
            if (cur_me_flag==me_flag) move->count=move->count+1;
            else move->count=move->count-1;
            value=value+500;
            eaten=TRUE;
        }
    }

    /*

    int defense_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    //闃插畧璧板埌浣嶇疆鎸?
    for (int i = 0; i < 8; i++) {
        int x1 = new_x + custodian_dir[i][0];
        int y1 = new_y + custodian_dir[i][1];
        int x2 = new_x + custodian_dir[i][0] * 2;
        int y2 = new_y + custodian_dir[i][1] * 2;
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x2][y2] == cur_me_flag && move->pro_board[x1][y1] == EMPTY)
            for (int j = 0; j < 8; j++)
                if (move->pro_board[x1+defense_dir[j][0]][y1+defense_dir[j][1]] == cur_other_flag) {
                    move->value=move->value-1000;
                    break;
                }
    }

    //闃插畧璧板埌浣嶇疆澶?
    for (int i = 0; i < 8; i++) {
        int x1 = new_x + defense_dir[i][0];
        int y1 = new_y + defense_dir[i][1];
        int x2 = new_x - defense_dir[i][0];
        int y2 = new_y - defense_dir[i][1];
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x1][y1] == EMPTY && move->pro_board[x2][y2] == cur_other_flag)
            for (int j = 0; j < 8; j++)
                if (move->pro_board[x1+defense_dir[j][0]][y1+defense_dir[j][1]] == cur_other_flag) {
                    move->value=move->value-500;
                    break;
                }
    }

    //闃插畧璧板紑浣嶇疆鎸?
    for (int i = 0; i < 8; i++) {
        int x1 = move->x + defense_dir[i][0];
        int y1 = move->y + defense_dir[i][1];
        int x2 = move->x - defense_dir[i][0];
        int y2 = move->y - defense_dir[i][1];
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x1][y1] == cur_me_flag && move->pro_board[x2][y2] == cur_me_flag)
            for (int j = 0; j < 8; j++)
                if (move->pro_board[move->x+defense_dir[j][0]][move->y+defense_dir[j][1]] == cur_other_flag) {
                    move->value=move->value-1000;
                    break;
                }
    }

    //闃插畧璧板紑浣嶇疆澶?
    for (int i = 0; i < 8; i++) {
        int x1 = move->x + custodian_dir[i][0];
        int y1 = move->y + custodian_dir[i][1];
        int x2 = move->x + custodian_dir[i][0] * 2;
        int y2 = move->y + custodian_dir[i][1] * 2;
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x2][y2] == cur_other_flag && move->pro_board[x1][y1] == me_flag)
            for (int j = 0; j < 8; j++)
                if (move->pro_board[move->x+defense_dir[j][0]][move->y+defense_dir[j][1]] == cur_other_flag) {
                    move->value=move->value-500;
                    break;
                }
    }

    //闃插畧涓嶈蛋寮€浣嶇疆鎸?
    for (int i = 0; i < 8; i++) {
        int x1 = move->x + custodian_dir[i][0];
        int y1 = move->y + custodian_dir[i][1];
        int x2 = move->x + custodian_dir[i][0] * 2;
        int y2 = move->y + custodian_dir[i][1] * 2;
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x2][y2] == cur_me_flag && move->pro_board[x1][y1] == EMPTY)
            for (int j = 0; j < 8; j++)
                if (move->pro_board[x1+defense_dir[j][0]][y1+defense_dir[j][1]] == cur_other_flag) {
                    move->value=move->value+1000;
                    break;
                }
    }

    //闃插畧涓嶈蛋寮€浣嶇疆澶?
    for (int i = 0; i < 8; i++) {
        int x1 = move->x + defense_dir[i][0];
        int y1 = move->y + defense_dir[i][1];
        int x2 = move->x - defense_dir[i][0];
        int y2 = move->y - defense_dir[i][1];
        if (isInBound(x1, y1) && isInBound(x2, y2) && move->pro_board[x1][y1] == cur_other_flag && move->pro_board[x2][y2] == EMPTY)
            for (int j = 0; j < 8; j++)
                if (move->pro_board[x2+defense_dir[j][0]][y2+defense_dir[j][1]] == cur_other_flag) {
                    move->value=move->value+500;
                    break;
                }
    }
*/
    for (int i=0; i<8; i++){
        if (move->pro_board[move->x+Rec_dir[i][0]+DIR[move->option][0]][move->y+Rec_dir[i][1]+DIR[move->option][1]] == cur_me_flag) value=value+3*para_beta*theta2;
        if (move->pro_board[move->x+DIR[i][0]+DIR[move->option][0]][move->y+DIR[i][1]+DIR[move->option][1]] == cur_me_flag) value=value-para_beta*theta3;
        if (move->pro_board[move->x+DIR[i][0]+DIR[move->option][0]][move->y+DIR[i][1]+DIR[move->option][1]] == cur_other_flag) value=value+5*para_beta;
        if (isInBound(move->x+DIR[i][0]*3+DIR[move->option][0],move->y+DIR[i][1]*3+DIR[move->option][1]))
            if (move->pro_board[move->x+DIR[i][0]+DIR[move->option][0]][move->y+DIR[i][1]+DIR[move->option][1]] == EMPTY && move->pro_board[move->x+2*DIR[i][0]+DIR[move->option][0]][move->y+2*DIR[i][1]+DIR[move->option][1]] == EMPTY && move->pro_board[move->x+3*DIR[i][0]+DIR[move->option][0]][move->y+3*DIR[i][1]+DIR[move->option][1]] == me_flag)
                value=value+20*para_beta;
    }


    move->value=value;
}


int minmax(int cur_depth,int alpha,int beta){
    int NodeType=cur_depth%2;
    if (cur_depth >= Search_Depth)
        return Evaluate();

    int cur_flag;
    if (NodeType == MAX) cur_flag=me_flag;
    else cur_flag=other_flag;

    struct Command nextmove[140];
    int num_actions=0;
    int count=0;
    //鍒濆鍖?

    if (cur_depth == 0 && Search_Depth!=1) {
        memcpy(nextmove,sortedmove,sizeof(nextmove));
        num_actions= num_SortArray;
        goto skip;
    }

    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++)
            if (cur_board[i][j] == cur_flag) {
                for (int k=0; k<8; k++)
                    if (isInBound(i+DIR[k][0],j+DIR[k][1]) && cur_board[i+DIR[k][0]][j+DIR[k][1]] == EMPTY){
                        nextmove[num_actions].x=i;
                        nextmove[num_actions].y=j;
                        nextmove[num_actions].option=k;

                        modify(&nextmove[num_actions]);

                        if  (NodeType == MAX) nextmove[num_actions].value=nextmove[num_actions].value/10+theta1*(1.1*pow((i-cen_x),2)+1.1*pow((j-cen_y),2)-pow((i+DIR[k][0]-cen_x),2)-pow((j+DIR[k][1]-cen_y),2));
                        else nextmove[num_actions].value=nextmove[num_actions].value/10;
                        //鍑濊仛 鍙湁宸辨柟鑰冭檻鍑濊仛


                        if (cur_depth<4) num_actions++;
                        if (cur_depth==4) num_actions++;
                        if (cur_depth>4 && (eaten || num_actions == 0)) num_actions++;

                        //闈欐€佹悳绱?

                    }
                count++;
            }

    if (count == 0 || count == 16 || num_actions == 0) return Evaluate();
    if (Search_Depth == 1) num_SortArray=num_actions;

    skip:

    if (Search_Depth == 4)          //鏈€娣卞眰鍐嶅姞鍏ュ巻鍙茶〃鎼滅储淇℃伅
        if (cur_depth == 0 && bestmove[(unsigned long)(hash+INT_MAX)%hashlen].x!=0)
            if (cur_board[bestmove[(unsigned long)(hash+INT_MAX)%hashlen].x][bestmove[(unsigned long)(hash+INT_MAX)%hashlen].y]==me_flag)
                if (cur_board[bestmove[(unsigned long)(hash+INT_MAX)%hashlen].x+DIR[bestmove[(unsigned long)(hash+INT_MAX)%hashlen].option][0]][bestmove[(unsigned long)(hash+INT_MAX)%hashlen].y+DIR[bestmove[(unsigned long)(hash+INT_MAX)%hashlen].option][1]]==EMPTY) {
                    nextmove[num_actions].x=bestmove[(unsigned long)(hash+INT_MAX)%hashlen].x;
                    nextmove[num_actions].y=bestmove[(unsigned long)(hash+INT_MAX)%hashlen].y;
                    nextmove[num_actions].option=bestmove[(unsigned long)(hash+INT_MAX)%hashlen].option;
                    modify(&nextmove[num_actions]);
                    nextmove[num_actions].value = INT_MAX;
                    num_actions++;
                    memset(bestmove, 0, sizeof(bestmove));
                }
    //鍘嗗彶琛?

////
    qsort(&nextmove[0],num_actions,sizeof(nextmove[0]),Compare_Evaluate);
////


    char temp_board[BOARD_SIZE][BOARD_SIZE] = {0};
    int tempCount,temphash;
    int MinmaxValue;

    //if (cur_depth<=4 && Search_Depth == 4) num_actions=num_actions*0.9;

    if(NodeType == MAX) {
        int fFoundPV = FALSE;
        for (int i=0; i<num_actions; i++){
            memcpy(temp_board,cur_board,sizeof(temp_board));
            memcpy(cur_board,nextmove[i].pro_board,sizeof(cur_board));
            tempCount=ChessCount;
            ChessCount=nextmove[i].count;
            temphash=hash;
            hash=nextmove[i].hash;

            if (fFoundPV && Search_Depth == 4) {
                MinmaxValue=minmax(cur_depth+1,alpha,alpha+1);
                if (MinmaxValue>alpha && MinmaxValue<beta) MinmaxValue=minmax(cur_depth+1,alpha,beta);
            }
            else MinmaxValue=minmax(cur_depth+1,alpha,beta);

            if (changeit && cur_depth == 0 && Search_Depth == 4) {
                if (MinmaxValue>second_command.value) {
                    if (MinmaxValue>ans_command.value) {
                        second_command=ans_command;
                        ans_command=nextmove[i];
                    }
                    else second_command=nextmove[i];
                    changed=FALSE;
                }
                if (MinmaxValue == ans_command.value && changed == FALSE){
                    ans_command=nextmove[i];
                    changed=TRUE;
                }
            }

            if (MinmaxValue>alpha) {
                if (!changeit && cur_depth == 0) {
                    ans_command=nextmove[i];
                    changed=FALSE;
                }
                if (cur_depth == 2 && Search_Depth == 4) {
                    bestmove[(unsigned long)(temphash+INT_MAX)%hashlen].x=nextmove[i].x;
                    bestmove[(unsigned long)(temphash+INT_MAX)%hashlen].y=nextmove[i].y;
                    bestmove[(unsigned long)(temphash+INT_MAX)%hashlen].option=nextmove[i].option;
                }
                alpha=MinmaxValue;
                fFoundPV = TRUE;
            }


            if (cur_depth == 0) {
                sortedmove[i]=nextmove[i];
                sortedmove[i].value=MinmaxValue;
            }

            memcpy(cur_board,temp_board,sizeof(cur_board));
            ChessCount=tempCount;
            hash=temphash;
            if (alpha>=beta) return alpha;
        }
    }

    if(NodeType == MIN){
        int fFoundPV = FALSE;
        for (int i=0; i<num_actions; i++){
            memcpy(temp_board,cur_board,sizeof(temp_board));
            memcpy(cur_board,nextmove[i].pro_board,sizeof(cur_board));
            tempCount=ChessCount;
            ChessCount=nextmove[i].count;
            temphash=hash;
            hash=nextmove[i].hash;

            if (fFoundPV && Search_Depth == 4) {
                MinmaxValue=minmax(cur_depth+1,beta-1,beta);
                if (MinmaxValue>alpha && MinmaxValue<beta) MinmaxValue=minmax(cur_depth+1,alpha,beta);
            }
            else MinmaxValue=minmax(cur_depth+1,alpha,beta);

            if (MinmaxValue<beta) {
                beta=MinmaxValue;
                fFoundPV = TRUE;
            }

            memcpy(cur_board,temp_board,sizeof(cur_board));
            ChessCount=tempCount;
            hash=temphash;
            if (alpha>=beta) return beta;
        }
    }

    if (cur_depth == 0) {
        alphasort=alpha-12000;
        betasort=alpha+12000;
    }

    if(NodeType == MIN) return beta;
    else return alpha;
}

void Refresh(){
    memcpy(cur_board,board,sizeof(cur_board));
    hash=0;
    ChessCount=0;
    cen_x=0;
    cen_y=0;
    int cenme_x=0;
    int cenme_y=0;
    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++) {
            if (cur_board[i][j] == me_flag) {
                ChessCount++;
                cenme_x=cenme_x+i;
                cenme_y=cenme_y+j;
                hash=hash^hashtable[i][j][me_flag];
            }
            if (cur_board[i][j] == other_flag) {
                cen_x=cen_x+i;
                cen_y=cen_y+j;
                hash=hash^hashtable[i][j][other_flag];
            }
        }
    cen_x=cen_x/(16-ChessCount);
    cen_y=cen_y/(16-ChessCount);
    cenme_x=cenme_x/ChessCount;
    cenme_y=cenme_y/ChessCount;

    if (me_flag == BLACK && turns<=15) {
        cen_y=cen_y+2;
        cen_x=cen_x+1;
        for (int i=5; i<=11; i++)
            for (int j=5; j<=11; j++)
                if (cur_board[i][j] == other_flag){
                    cen_y=j;
                    cen_x=i;
                }
    }

    if (me_flag == WHITE && turns<=15) {
        cen_y=cen_y-2;
        cen_x=cen_x-1;
        for (int i=0; i<=6; i++)
            for (int j=0; j<=6; j++)
                if (cur_board[i][j] == other_flag){
                    cen_y=j;
                    cen_x=i;
                    break;
                }
    }

    if (ChessCount>11) {
        theta1=1;
        theta2=2;
        cen_x=cenme_x;
        cen_y=cenme_y;
    }
    else
    {
        theta1=1;
        theta2=2;
        theta3=0;
    }
    direction=0;
}

struct Command findValidPos(const char board[BOARD_SIZE][BOARD_SIZE], int flag) {
    Refresh();
    turns++;
    memset(sortedmove, 0, sizeof(sortedmove));

    changeit=FALSE;
    if (repetition[(unsigned long)(hash+INT_MAX)%hashlen].x != 0) {
        if (ChessCount >= 8) {
            ans_command.x = repetition[(unsigned long) (hash + INT_MAX) % hashlen].x;
            ans_command.y = repetition[(unsigned long) (hash + INT_MAX) % hashlen].y;
            ans_command.option = repetition[(unsigned long) (hash + INT_MAX) % hashlen].option;
            if (cur_board[ans_command.x][ans_command.y] == me_flag && cur_board[ans_command.x+DIR[ans_command.option][0]][ans_command.y+DIR[ans_command.option][1]] == EMPTY) return ans_command;
        } else {
            changeit = TRUE;
            changed = FALSE;
            second_command.value=INT_MIN;
            ans_command.value=INT_MIN;
        }
    }

    Search_Depth=1;
    minmax(0, (-1)*INT_MAX, INT_MAX);
    //Search_Depth=2;
    //minmax(0, (-1)*INT_MAX, INT_MAX);
    Search_Depth=2;
    minmax(0, (-1)*INT_MAX, INT_MAX);
    Search_Depth=4;
    minmax(0, alphasort, betasort);
    //Search_Depth=5;
    //minmax(0, alphasort, betasort);

    if (!changeit) {
        repetition[(unsigned long)(hash+INT_MAX)%hashlen].x=ans_command.x;
        repetition[(unsigned long)(hash+INT_MAX)%hashlen].y=ans_command.y;
        repetition[(unsigned long)(hash+INT_MAX)%hashlen].option=ans_command.option;
        return ans_command;
    }
    else {
        repetition[(unsigned long)(hash+INT_MAX)%hashlen].x=second_command.x;
        repetition[(unsigned long)(hash+INT_MAX)%hashlen].y=second_command.y;
        repetition[(unsigned long)(hash+INT_MAX)%hashlen].option=second_command.option;
        return second_command;
    }
}





/**
 * 鏉烆喖鍩屾担鐘烘儰鐎涙劑鈧?
 * 濡娲忔稉?鐞涖劎銇氱粚铏规閿?鐞涖劎銇氭鎴烆棎閿?鐞涖劎銇氶惂鑺ユ
 * me鐞涖劎銇氭担鐘冲娴狅綀銆冮惃鍕棎鐎?1閹?)
 * 娴ｇ娀娓剁憰浣界箲閸ョ偘绔存稉顏嗙波閺嬪嫪缍婥ommand閿涘苯婀獂鐏炵偞鈧冩嫲y鐏炵偞鈧冿綖娑撳﹣缍橀幆瀹狀洣缁夎濮╅惃鍕棎鐎涙劗娈戞担宥囩枂閿涘ption婵夘偂绗傛担鐘冲厒鐟曚胶些閸斻劎娈戦弬鐟版倻閵?
 */
struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me) {
    /*
     * TODO閿涙艾婀潻娆撳櫡閸愭瑤绗呮担鐘垫畱AI閵?
     * 鏉╂瑩鍣烽張澶夌娑擃亞銇氭笟濠閿涘苯鐣犻崣顏冪窗鐎电粯澹樼粭顑跨娑擃亜褰叉稉瀣畱娴ｅ秶鐤嗘潻娑滎攽閽€钘夌摍閵?
     */
    struct Command preferedPos = findValidPos(board, me);

    return preferedPos;
}

/**
 * 娴ｇ姷娈戞禒锝囩垳缂佹挻娼?
 */


BOOL place(int x, int y, OPTION option, int cur_flag) {
    // 缁夎濮╂稊瀣閻ㄥ嫪缍呯純顔界梾閺堝鍨滈弬瑙勵棎鐎?
    if (board[x][y] != cur_flag) {
        return FALSE;
    }

    int new_x = x + DIR[option][0];
    int new_y = y + DIR[option][1];
    // 缁夎濮╂稊瀣倵閻ㄥ嫪缍呯純顔跨Т閸戦缚绔熼悾? 閹存牞鈧懍绗夐弰顖溾敄閸?
    if (!isInBound(new_x, new_y) || board[new_x][new_y] != EMPTY) {
        return FALSE;
    }

    board[x][y] = EMPTY;
    board[new_x][new_y] = cur_flag;

    int cur_other_flag = 3 - cur_flag;

    // 閹?
    int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    for (int i = 0; i < 4; i++) {
        int x1 = new_x + intervention_dir[i][0];
        int y1 = new_y + intervention_dir[i][1];
        int x2 = new_x - intervention_dir[i][0];
        int y2 = new_y - intervention_dir[i][1];
        if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == cur_other_flag && board[x2][y2] == cur_other_flag) {
            board[x1][y1] = cur_flag;
            board[x2][y2] = cur_flag;
        }
    }

    // 婢?
    int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    for (int i = 0; i < 8; i++) {
        int x1 = new_x + custodian_dir[i][0];
        int y1 = new_y + custodian_dir[i][1];
        int x2 = new_x + custodian_dir[i][0] * 2;
        int y2 = new_y + custodian_dir[i][1] * 2;
        if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == cur_flag && board[x1][y1] == cur_other_flag) {
            board[x1][y1] = cur_flag;
        }
    }

    return TRUE;
}


//........
//........
//..XXXX..
//XXXX....
//....OOOO
//..OOOO..
//........
//........
void start(int flag) {
    memset(board, 0, sizeof(board));

    srand((unsigned)time(NULL));
    for (int i=0; i<12; i++)
        for (int j=0; j<12; j++)
            for (int k=1; k<3; k++)
                hashtable[i][j][k]=(unsigned int)((double)(rand()*(pow(2,31)-1)/RAND_MAX));


    for (int i = 0; i < 3; i++) {
        board[2][2 + i] = WHITE;
        board[6][6 + i] = WHITE;
        board[5][3 + i] = BLACK;
        board[9][7 + i] = BLACK;
    }

    for (int i = 0; i < 2; i++) {
        board[8 + i][2] = WHITE;
        board[2 + i][9] = BLACK;
    }
}

void turn() {
    // AI
    struct Command command = aiTurn((const char (*)[BOARD_SIZE])board, me_flag);
    place(command.x, command.y, command.option, me_flag);
    printf("%d %d %d\n", command.x, command.y, command.option);
    fflush(stdout);
}

void end(int x) {

}

void loop() {
//  freopen("../input", "r", stdin);
    while (TRUE)
    {
        memset(buffer, 0, sizeof(buffer));
        gets(buffer);

        if (strstr(buffer, START))
        {
            char tmp[MAX_BYTE] = {0};
            sscanf(buffer, "%s %d", tmp, &me_flag);
            other_flag = 3 - me_flag;
            start(me_flag);
            printf("OK\n");
            fflush(stdout);
        }
        else if (strstr(buffer, PLACE))
        {
            char tmp[MAX_BYTE] = {0};
            int x, y;
            OPTION option;
            sscanf(buffer, "%s %d %d %d", tmp, &x, &y, &option);
            place(x, y, option, other_flag);
        }
        else if (strstr(buffer, TURN))
        {
            turn();
        }
        else if (strstr(buffer, END))
        {
            char tmp[MAX_BYTE] = {0};
            int x;
            sscanf(buffer, "%s %d", tmp, &x);
            end(x);
        }
        //printBoard();
    }
}


int main(int argc, char *argv[]) {
    loop();
    return 0;
}
