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
 * 浣犵殑浠ｇ爜寮€�?
 */

/**
 * You can define your own struct and variable here
 * 浣犲彲浠ュ湪杩欓噷瀹氫箟浣犺嚜宸辩殑缁撴瀯浣撳拰鍙橀�?
 */

#include <limits.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
int Search_Depth=4;
#define MAX 0
#define MIN 1
#define hashlen 3999991
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
    unsigned int key;
    OPTION option;
};
struct move bestmove[hashlen];
struct move repetition[hashlen], records[3][hashlen];
struct Command RepeHash;
int changeit=FALSE;
int changed=FALSE;
unsigned int hashtable[BOARD_SIZE][BOARD_SIZE][3]={0 ,1036451840 ,2125137919 ,0 ,1547628543 ,1656961023 ,0 ,1903583231 ,1587341311 ,0 ,1265192959 ,1749293055 ,0 ,1744027647 ,1713686527 ,0 ,1943121919 ,839579648 ,0 ,1100947455 ,874686464 ,0 ,2027425791 ,1442306047 ,0 ,1020821504 ,225871872 ,0 ,198995968 ,1072347136 ,0 ,369090560 ,1023436800 ,0 ,1279184895 ,298031104 ,
                                                   0 ,1735737343 ,1939884031 ,0 ,660549632 ,56784896 ,0 ,160997376 ,674637824 ,0 ,949850112 ,1118263295 ,0 ,1925840895 ,2007107583 ,0 ,693760000 ,1399994367 ,0 ,392781824 ,79947776 ,0 ,284446720 ,1872959487 ,0 ,1238810623 ,1484507135 ,0 ,1422700543 ,1729755135 ,0 ,443686912 ,1101637631 ,0 ,138612736 ,2142689279 ,
                                                   0 ,932937728 ,892176384 ,0 ,1287090175 ,1431943167 ,0 ,1303568383 ,770140160 ,0 ,831115264 ,971151360 ,0 ,1729642495 ,213338112 ,0 ,337260544 ,355563520 ,0 ,1277927423 ,2021468159 ,0 ,2143850495 ,1160140799 ,0 ,1665990655 ,1041827840 ,0 ,234156032 ,2107717631 ,0 ,282877952 ,812312576 ,0 ,1174360063 ,679614464 ,
                                                   0 ,389660672 ,139642880 ,0 ,2101850111 ,1168701439 ,0 ,1992630271 ,1152428031 ,0 ,925765632 ,1257625599 ,0 ,1306427391 ,174360576 ,0 ,85094400 ,71874560 ,0 ,954589184 ,72247296 ,0 ,1716834303 ,1937872895 ,0 ,842743808 ,1129203711 ,0 ,676671488 ,813725696 ,0 ,1916706815 ,507783168 ,0 ,1181978623 ,1227184127 ,
                                                   0 ,1330642943 ,30040512 ,0 ,1242558463 ,558905344 ,0 ,500129792 ,563210240 ,0 ,713707520 ,1390483455 ,0 ,270319616 ,900319232 ,0 ,759345152 ,230160384 ,0 ,379068416 ,2100799487 ,0 ,1167659007 ,2008340479 ,0 ,1067548672 ,1293649919 ,0 ,1961717759 ,213366784 ,0 ,395894784 ,3500032 ,0 ,715116544 ,2124453887 ,
                                                   0 ,148783104 ,1466255359 ,0 ,605040640 ,357529600 ,0 ,1003593728 ,1502291967 ,0 ,1285459967 ,245651456 ,0 ,1993539583 ,864487424 ,0 ,497741824 ,2122266623 ,0 ,2118279167 ,406382592 ,0 ,2123714559 ,784340992 ,0 ,1954529279 ,545310720 ,0 ,616411136 ,2135357439 ,0 ,971710464 ,725526528 ,0 ,1938034687 ,1173608447 ,
                                                   0 ,1290043391 ,1028818944 ,0 ,1548251135 ,782678016 ,0 ,701227008 ,1637640191 ,0 ,1047187456 ,457152512 ,0 ,721502208 ,150751232 ,0 ,1196109823 ,60717056 ,0 ,1759813631 ,1783769087 ,0 ,1380552703 ,1436768255 ,0 ,433455104 ,1652426751 ,0 ,1220931583 ,1429092351 ,0 ,1916100607 ,1415571455 ,0 ,35672064 ,2082412543 ,
                                                   0 ,73580544 ,148359168 ,0 ,599306240 ,1515583487 ,0 ,549330944 ,247834624 ,0 ,15667200 ,1974654975 ,0 ,900169728 ,453609472 ,0 ,2065919999 ,706066432 ,0 ,796844032 ,1753442303 ,0 ,1639305215 ,157194240 ,0 ,1487159295 ,403916800 ,0 ,1376137215 ,997029888 ,0 ,964141056 ,278472704 ,0 ,393515008 ,1579309055 ,
                                                   0 ,2019098623 ,1593116671 ,0 ,190902272 ,1700607999 ,0 ,967335936 ,369551360 ,0 ,1965772799 ,2063472639 ,0 ,2143666175 ,783206400 ,0 ,1781772287 ,1592063999 ,0 ,185671680 ,1741465599 ,0 ,769265664 ,1190254591 ,0 ,971669504 ,641763328 ,0 ,293498880 ,1057273856 ,0 ,1756487679 ,1424648191 ,0 ,1417727999 ,150837248 ,
                                                   0 ,1372012543 ,1152010239 ,0 ,71380992 ,2092726271 ,0 ,1837801471 ,207628288 ,0 ,1545572351 ,1747015679 ,0 ,1381761023 ,1760299007 ,0 ,628879360 ,1863071743 ,0 ,345726976 ,489547776 ,0 ,397824000 ,1801263103 ,0 ,648593408 ,1376110591 ,0 ,942583808 ,1291057151 ,0 ,954474496 ,1985193983 ,0 ,977604608 ,2041632767 ,
                                                   0 ,430800896 ,519538688 ,0 ,1599696895 ,762558464 ,0 ,358932480 ,1724999679 ,0 ,1456197631 ,1511823359 ,0 ,1449803775 ,1321289727 ,0 ,503066624 ,126580736 ,0 ,1159569407 ,497494016 ,0 ,1615499263 ,866146304 ,0 ,1742667775 ,1080231935 ,0 ,1461121023 ,842741760 ,0 ,1125015551 ,701818880 ,0 ,700534784 ,222042112 ,
                                                   0 ,957071360 ,853329920 ,0 ,1302966271 ,1686304767 ,0 ,1781997567 ,2052761599 ,0 ,1714425855 ,1307564031 ,0 ,351305728 ,1160677375 ,0 ,615804928 ,895662080 ,0 ,1972887551 ,1580754943 ,0 ,680972288 ,1018927104 ,0 ,646791168 ,1985497087 ,0 ,1597452287 ,467302400 ,0 ,3186688 ,74328064 ,0 ,1677037567 ,10442752};

struct Command
{
    int x;
    int y;
    int value;      //鐠囥儲鎼锋担婊呮畱娴犲嘲�?
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
    int hello; //���������վ��ôע�᣿

    double value=1;
    double x1=2000,x2=100,x3=601,x4=599,x5=40,x6=609,x7=611,x8=0,x9=0,x10=-5,x11=0,x12=0,x13=3,x14=0,x15=0;
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
    //鐠ч缍呮导鏉库偓?缁岃桨缍?

    int new_x = move->x + DIR[move->option][0];
    int new_y = move->y + DIR[move->option][1];


    move->hash=hash^hashtable[move->x][move->y][cur_me_flag]^hashtable[new_x][new_y][cur_me_flag];

    memcpy(move->pro_board,cur_board,sizeof(move->pro_board));
    move->count=ChessCount;
    move->pro_board[move->x][move->y] = EMPTY;
    move->pro_board[new_x][new_y] = cur_me_flag;
    move->dir=direction;
    eaten=FALSE;

    // �?
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

    // �?
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
    //防守走到位置�?
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

    //防守走到位置�?
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

    //防守走开位置�?
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

    //防守走开位置�?
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

    //防守不走开位置�?
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

    //防守不走开位置�?
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
    //初始�?

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

                        if  (NodeType == MAX) nextmove[num_actions].value=nextmove[num_actions].value/10+2*theta1*(1.1*pow((i-cen_x),2)+1.1*pow((j-cen_y),2)-pow((i+DIR[k][0]-cen_x),2)-pow((j+DIR[k][1]-cen_y),2));
                        else nextmove[num_actions].value=nextmove[num_actions].value/10;
                        //凝聚 只有己方考虑凝聚


                        if (cur_depth<=4) num_actions++;
                        if (Search_Depth==1 && cur_depth==0 && changeit)
							if (nextmove[num_actions-1].x==RepeHash.x && nextmove[num_actions-1].y==RepeHash.y && nextmove[num_actions-1].option==RepeHash.option)
								num_actions--;
                        if (cur_depth>4 && (eaten || num_actions == 0)) num_actions++;

                        //静态搜�?

                    }
                count++;
            }

    if (count == 0 || count == 16 || num_actions == 0) return Evaluate();
    if (Search_Depth == 1) num_SortArray=num_actions;

    skip:

    if (Search_Depth == 4 && !changeit)          //最深层再加入历史表搜索信息
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
    //历史�?

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

			/*
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
            */

            if (MinmaxValue>alpha) {
                if (cur_depth == 0) {
                    ans_command=nextmove[i];
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
                    cen_x=i+0.5;
                }
    }

    if (me_flag == WHITE && turns<=15) {
        cen_y=cen_y-2;
        cen_x=cen_x-1;
        for (int i=0; i<=6; i++)
            for (int j=0; j<=6; j++)
                if (cur_board[i][j] == other_flag){
                    cen_y=j;
                    cen_x=i-0.5;
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
    
    if (records[me_flag][(unsigned long)(hash+INT_MAX)%hashlen].x != 0 && records[me_flag][(unsigned long)(hash+INT_MAX)%hashlen].key==hash) {
            ans_command.x = records[me_flag][(unsigned long) (hash + INT_MAX) % hashlen].x;
            ans_command.y = records[me_flag][(unsigned long) (hash + INT_MAX) % hashlen].y;
            ans_command.option = records[me_flag][(unsigned long) (hash + INT_MAX) % hashlen].option;
            if (cur_board[ans_command.x][ans_command.y] == me_flag && cur_board[ans_command.x+DIR[ans_command.option][0]][ans_command.y+DIR[ans_command.option][1]] == EMPTY) return ans_command;
    }
        	
    if (repetition[(unsigned long)(hash+INT_MAX)%hashlen].x != 0) {
        if (ChessCount > 8) {
            ans_command.x = repetition[(unsigned long) (hash + INT_MAX) % hashlen].x;
            ans_command.y = repetition[(unsigned long) (hash + INT_MAX) % hashlen].y;
            ans_command.option = repetition[(unsigned long) (hash + INT_MAX) % hashlen].option;
            if (cur_board[ans_command.x][ans_command.y] == me_flag && cur_board[ans_command.x+DIR[ans_command.option][0]][ans_command.y+DIR[ans_command.option][1]] == EMPTY) return ans_command;
        } else {
            changeit = TRUE;
            RepeHash.x = repetition[(unsigned long) (hash + INT_MAX) % hashlen].x;
            RepeHash.y = repetition[(unsigned long) (hash + INT_MAX) % hashlen].y;
            RepeHash.option = repetition[(unsigned long) (hash + INT_MAX) % hashlen].option;          
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

    repetition[(unsigned long)(hash+INT_MAX)%hashlen].x=ans_command.x;
    repetition[(unsigned long)(hash+INT_MAX)%hashlen].y=ans_command.y;
    repetition[(unsigned long)(hash+INT_MAX)%hashlen].option=ans_command.option;
    return ans_command;
}








/**
 * 杞埌浣犺惤瀛愩�?
 * 妫嬬洏涓?琛ㄧず绌虹櫧�?琛ㄧず榛戞�?琛ㄧず鐧芥棗
 * me琛ㄧず浣犳墍浠ｈ〃鐨勬�?1�?)
 * 浣犻渶瑕佽繑鍥炰竴涓粨鏋勪綋Command锛屽湪x灞炴€у拰y灞炴€у～涓婁綘鎯宠绉诲姩鐨勬瀛愮殑浣嶇疆锛宱ption濉笂浣犳兂瑕佺Щ鍔ㄧ殑鏂瑰悜�?
 */
struct Command aiTurn(const char board[BOARD_SIZE][BOARD_SIZE], int me) {
    /*
     * TODO锛氬湪杩欓噷鍐欎笅浣犵殑AI�?
     * 杩欓噷鏈変竴涓ず渚婣I锛屽畠鍙細瀵绘壘绗竴涓彲涓嬬殑浣嶇疆杩涜钀藉瓙�?
     */
    struct Command preferedPos = findValidPos(board, me);

    return preferedPos;
}

/**
 * 浣犵殑浠ｇ爜缁撴�?
 */


BOOL place(int x, int y, OPTION option, int cur_flag) {
    // 绉诲姩涔嬪墠鐨勪綅缃病鏈夋垜鏂规�?
    if (board[x][y] != cur_flag) {
        return FALSE;
    }

    int new_x = x + DIR[option][0];
    int new_y = y + DIR[option][1];
    // 绉诲姩涔嬪悗鐨勪綅缃秴鍑鸿竟鐣? 鎴栬€呬笉鏄┖�?
    if (!isInBound(new_x, new_y) || board[new_x][new_y] != EMPTY) {
        return FALSE;
    }

    board[x][y] = EMPTY;
    board[new_x][new_y] = cur_flag;

    int cur_other_flag = 3 - cur_flag;

    // �?
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

    // �?
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

    /*
    srand((unsigned)time(NULL));
    for (int i=0; i<12; i++)
        for (int j=0; j<12; j++)
            for (int k=1; k<3; k++)
                hashtable[i][j][k]=(unsigned int)((double)(rand()*(pow(2,31)-1)/RAND_MAX));
    */

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
    //loop();
    char temp;
    int flag;
    start(1);
	unsigned int hash=0;
    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++) {
            if (board[i][j] == 1) 
                hash=hash^hashtable[i][j][1];  
            if (board[i][j] == 2) 
                hash=hash^hashtable[i][j][2];       
        }
        
    FILE *in=fopen("in.txt","r"),*out1=fopen("out1.txt","w"),*out2=fopen("out2.txt","w");
    
	while (temp!=' ') 
		fscanf(in,"%c",&temp);
    fscanf(in,"%d",&flag);
    flag=3-flag;
    for (int i=0;i<10;i++){
    	while (temp!='E') 
			fscanf(in,"%c",&temp);
		fscanf(in,"%d%d%d",&records[flag][i].x,&records[flag][i].y,&records[flag][i].option);	
		fscanf(in,"%c",&temp);
	}
    
	while (temp!=' ') 
		fscanf(in,"%c",&temp);
    fscanf(in,"%d",&flag);
    flag=3-flag;
    for (int i=0;i<10;i++){
    	while (temp!='E') 
			fscanf(in,"%c",&temp);
		fscanf(in,"%d%d%d",&records[flag][i].x,&records[flag][i].y,&records[flag][i].option);	
		fscanf(in,"%c",&temp);
	}
   
   for (int i=0;i<10;i++){
    	fprintf(out1,"    records[1][%d].key=%d;\n",(unsigned long)(hash+INT_MAX)%hashlen,hash);
    	fprintf(out1,"    records[1][%d].x=%d;\n",(unsigned long)(hash+INT_MAX)%hashlen,records[1][i].x);
    	fprintf(out1,"    records[1][%d].y=%d;\n",(unsigned long)(hash+INT_MAX)%hashlen,records[1][i].y);
    	fprintf(out1,"    records[1][%d].option=%d;\n\n",(unsigned long)(hash+INT_MAX)%hashlen,records[1][i].option);
    	hash=hash^hashtable[records[1][i].x][records[1][i].y][1]^hashtable[records[1][i].x+DIR[records[1][i].option][0]][records[1][i].y+DIR[records[1][i].option][1]][1];
    	board[records[1][i].x][records[1][i].y]=0;
    	board[records[1][i].x+DIR[records[1][i].option][0]][records[1][i].y+DIR[records[1][i].option][1]]=1;
    	int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    	for (int j = 0; j < 4; j++) {
        	int x1 = records[1][i].x+DIR[records[1][i].option][0] + intervention_dir[j][0];
        	int y1 = records[1][i].y+DIR[records[1][i].option][1] + intervention_dir[j][1];
        	int x2 = records[1][i].x+DIR[records[1][i].option][0] - intervention_dir[j][0];
        	int y2 = records[1][i].y+DIR[records[1][i].option][1] - intervention_dir[j][1];
        	if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == 2 && board[x2][y2] == 2) {
            	hash=hash^hashtable[x1][y1][2]^hashtable[x2][y2][2]^hashtable[x1][y1][1]^hashtable[x2][y2][1];
            	board[x1][y1]=1;
            	board[x2][y2]=1;
        	}
    	}


    	int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    	for (int j = 0; j < 8; j++) {
        	int x1 = records[1][i].x+DIR[records[1][i].option][0] + custodian_dir[j][0];
        	int y1 = records[1][i].y+DIR[records[1][i].option][1] + custodian_dir[j][1];
        	int x2 = records[1][i].x+DIR[records[1][i].option][0] + custodian_dir[j][0] * 2;
        	int y2 = records[1][i].y+DIR[records[1][i].option][1] + custodian_dir[j][1] * 2;
        	if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == 1 && board[x1][y1] == 2) {
            	board[x1][y1] = 1;
            	hash=hash^hashtable[x1][y1][2]^hashtable[x1][y1][1];
        	}
    	}
    
    	fprintf(out2,"    records[2][%d].key=%d;\n",(unsigned long)(hash+INT_MAX)%hashlen,hash);
    	fprintf(out2,"    records[2][%d].x=%d;\n",(unsigned long)(hash+INT_MAX)%hashlen,records[2][i].x);
    	fprintf(out2,"    records[2][%d].y=%d;\n",(unsigned long)(hash+INT_MAX)%hashlen,records[2][i].y);
    	fprintf(out2,"    records[2][%d].option=%d;\n\n",(unsigned long)(hash+INT_MAX)%hashlen,records[2][i].option);
    	hash=hash^hashtable[records[2][i].x][records[2][i].y][2]^hashtable[records[2][i].x+DIR[records[2][i].option][0]][records[2][i].y+DIR[records[2][i].option][1]][2];
    	board[records[2][i].x][records[2][i].y]=0;
    	board[records[2][i].x+DIR[records[2][i].option][0]][records[2][i].y+DIR[records[2][i].option][1]]=2;
		//int intervention_dir[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} };
    	for (int j = 0; j < 4; j++) {
        	int x1 = records[2][i].x+DIR[records[2][i].option][0] + intervention_dir[j][0];
        	int y1 = records[2][i].y+DIR[records[2][i].option][1] + intervention_dir[j][1];
        	int x2 = records[2][i].x+DIR[records[2][i].option][0] - intervention_dir[j][0];
        	int y2 = records[2][i].y+DIR[records[2][i].option][1] - intervention_dir[j][1];
        	if (isInBound(x1, y1) && isInBound(x2, y2) && board[x1][y1] == 1 && board[x2][y2] == 1) {
            	hash=hash^hashtable[x1][y1][2]^hashtable[x2][y2][2]^hashtable[x1][y1][1]^hashtable[x2][y2][1];
            	board[x1][y1]=2;
            	board[x2][y2]=2;
        	}
    	}


    	//int custodian_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    	for (int j = 0; j < 8; j++) {
        	int x1 = records[2][i].x+DIR[records[2][i].option][0] + custodian_dir[j][0];
        	int y1 = records[2][i].y+DIR[records[2][i].option][1] + custodian_dir[j][1];
        	int x2 = records[2][i].x+DIR[records[2][i].option][0] + custodian_dir[j][0] * 2;
        	int y2 = records[2][i].y+DIR[records[2][i].option][1] + custodian_dir[j][1] * 2;
        	if (isInBound(x1, y1) && isInBound(x2, y2) && board[x2][y2] == 2 && board[x1][y1] == 1) {
            	board[x1][y1] = 2;
            	hash=hash^hashtable[x1][y1][2]^hashtable[x1][y1][1];
        	}
    	}
	}
	
	close(in);
    close(out1);
    close(out2);
    return 0;
}
