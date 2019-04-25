//632+藐视
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

/*
观自在菩萨，行深般若波罗蜜多时，照见五蕴皆空，度一切苦厄。
舍利子，色不异空，空不异色，色即是空，空即是色，受想行识，亦复如是。
舍利子，是诸法空相，不生不灭，不垢不净，不增不减。
是故空中无色，无受想行识，无眼耳鼻舌身意，无色声香味触法，无眼界，乃至无意识界，无无明，亦无无明尽，乃至无老死，亦无老死尽。
无苦集灭道，无智亦无得。以无所得故。
菩提萨埵，依般若波罗蜜多故，心无挂碍。
无挂碍故，无有恐怖，远离颠倒梦想，究竟涅盘。
三世诸佛，依般若波罗蜜多故，得阿耨多罗三藐三菩提。
故知般若波罗蜜多，是大神咒，是大明咒，是无上咒，是无等等咒，能除一切苦，真实不虚。
故说般若波罗蜜多咒，即说咒曰：揭谛揭谛，波罗揭谛，波罗僧揭谛，菩提萨婆诃。
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
int turns=0,v2=0;

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
    double x1 = 2200, x2 = 20, x3 = 550, x4 = 550, x5 = 10, x6 = 700, x7 = 700, x8 = 0, x9 = 0, x10 = -5, x11 = 0, x12 = 0, x13 = 3, x14 = 0, x15 = 0, x16 = 280, x17 = 50;
    double Dis=0;
    double tiao=0,jia=0,rec=0,otherjia=0,othertiao=0,me_tiao=0,me_jia=0,ozzo=0,other_jia=0,other_tiao=0,num_Diamond=0,MeMoveActions=0,OtherMoveActions=0,shuan=0,beshuan=0;
    int defense_dir[8][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1} };
    int Rec_dir[8][2] = { {1, 3}, {1, -3}, {-1, 3}, {-1, -3}, {3, 1}, {3, -1}, {-3, 1}, {-3, -1} };
    int Diamond[8][3][2] = {{{-2,-1},{-2,1},{-4,0}},{{-1,2},{1,2},{0,4}},{{2,-1},{2,1},{4,0}},{{-1,-2},{1,-2},{0,-4}},
                            {{-1,-2},{-1,2},{-2,0}},{{-2,1},{2,1},{0,2}},{{1,-2},{1,2},{2,0}},{{-2,-1},{2,-1},{0,-2}}};

    if (ChessCount>=10) x10=30;
    if (me_flag == WHITE) {
        //x10=30;
    }

    for (int i=0; i<BOARD_SIZE; i++)
        for (int j=0; j<BOARD_SIZE; j++) {
            if (cur_board[i][j] == me_flag){
                int found=FALSE;
                for (int t = 0; t < 8; t++)
                    if (!found) {
                        int x1 = i + defense_dir[t][0];
                        int y1 = j + defense_dir[t][1];
                        int x2 = i + defense_dir[t][0] * 2;
                        int y2 = j + defense_dir[t][1] * 2;
                        if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == me_flag && cur_board[x1][y1] == EMPTY)
                            for (int k = 0; k < 8; k++)
                                if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == other_flag) {
                                    tiao++;
                                    found = TRUE;
                                    break;
                                }

                    }

                for (int t = 0; t < 8; t++)
                    if (!found) {
                        int x1 = i + defense_dir[t][0];
                        int y1 = j + defense_dir[t][1];
                        int x2 = i - defense_dir[t][0];
                        int y2 = j - defense_dir[t][1];
                        if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == other_flag && cur_board[x1][y1] == EMPTY)
                            for (int k = 0; k < 8; k++)
                                if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == other_flag) {
                                    jia++;
                                    found = TRUE;
                                    for (int w=0; w<8; w++) {
                                        int x3 = i + defense_dir[w][0];
                                        int y3 = j + defense_dir[w][1];
                                        int x4 = i + defense_dir[w][0] * 2;
                                        int y4 = j + defense_dir[w][1] * 2;
                                        if (isInBound(x3, y3) && isInBound(x4, y4) && cur_board[x3][y3] == me_flag && cur_board[x4][y4] == other_flag) {
                                            beshuan++;
                                            break;
                                        }
                                    }
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
                int found=FALSE;
                for (int t = 0; t < 8; t++)
                    if (!found) {
                        int x1 = i + defense_dir[t][0];
                        int y1 = j + defense_dir[t][1];
                        int x2 = i + defense_dir[t][0] * 2;
                        int y2 = j + defense_dir[t][1] * 2;
                        if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == other_flag && cur_board[x1][y1] == EMPTY)
                            for (int k = 0; k < 8; k++)
                                if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == me_flag) {
                                    othertiao++;
                                    found = TRUE;
                                    break;
                                }

                    }

                for (int t = 0; t < 8; t++)
                    if (!found) {
                        int x1 = i + defense_dir[t][0];
                        int y1 = j + defense_dir[t][1];
                        int x2 = i - defense_dir[t][0];
                        int y2 = j - defense_dir[t][1];
                        if (isInBound(x1, y1) && isInBound(x2, y2) && cur_board[x2][y2] == me_flag && cur_board[x1][y1] == EMPTY)
                            for (int k = 0; k < 8; k++)
                                if (cur_board[x1+defense_dir[k][0]][y1+defense_dir[k][1]] == me_flag) {
                                    otherjia++;
                                    found = TRUE;
                                    for (int w=0; w<8; w++) {
                                        int x3 = i + defense_dir[w][0];
                                        int y3 = j + defense_dir[w][1];
                                        int x4 = i + defense_dir[w][0] * 2;
                                        int y4 = j + defense_dir[w][1] * 2;
                                        if (isInBound(x3, y3) && isInBound(x4, y4) && cur_board[x3][y3] == other_flag && cur_board[x4][y4] == me_flag) {
                                            shuan++;
                                            break;
                                        }
                                    }
                                    break;
                                }
                    }

            }
        }


    Dis=Dis/ChessCount;
    MeMoveActions=MeMoveActions/ChessCount;
    OtherMoveActions=OtherMoveActions/(16-ChessCount);
    value=ChessCount*x1-Dis*x2-tiao*x3-jia*x4+rec*x5+otherjia*x6+othertiao*x7-me_tiao*x8-me_jia*x9+ozzo*x10+other_tiao*x11+other_jia*x12+num_Diamond*x13+MeMoveActions*x14-OtherMoveActions*x15+shuan*x16-x17*beshuan;

    if (turns>=59) value=ChessCount*x1;
    if (ChessCount==8) value=value-100;
    if (v2!=0) return (int)((value-v2)*0.9+v2);
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
        //if (move->pro_board[move->x+DIR[i][0]][move->y+DIR[i][1]] == cur_me_flag) value=value+1*theta3;
        if (move->pro_board[move->x+DIR[i][0]][move->y+DIR[i][1]] == cur_other_flag) value=value-5;

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


    for (int i=0; i<8; i++){
        if (move->pro_board[move->x+Rec_dir[i][0]+DIR[move->option][0]][move->y+Rec_dir[i][1]+DIR[move->option][1]] == cur_me_flag) value=value+3*para_beta*theta2;
        if (move->pro_board[move->x+DIR[i][0]+DIR[move->option][0]][move->y+DIR[i][1]+DIR[move->option][1]] == cur_other_flag) value=value+5*para_beta;
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

                        if  (NodeType == MAX) nextmove[num_actions].value=nextmove[num_actions].value/10+2*theta1*(1.1*pow((i-cen_x),2)+1.1*pow((j-cen_y),2)-pow((i+DIR[k][0]-cen_x),2)-pow((j+DIR[k][1]-cen_y),2));
                        else nextmove[num_actions].value=nextmove[num_actions].value/10;
                        //鍑濊仛 鍙湁宸辨柟鑰冭檻鍑濊仛


                        if (cur_depth<=4) num_actions++;
                        if (Search_Depth==1 && cur_depth==0 && changeit)
                            if (nextmove[num_actions-1].x==RepeHash.x && nextmove[num_actions-1].y==RepeHash.y && nextmove[num_actions-1].option==RepeHash.option)
                                num_actions--;
                        if (cur_depth>4 && (eaten || num_actions == 0)) num_actions++;

                        //闈欐�佹悳绱?

                    }
                count++;
            }

    if (count == 0 || count == 16 || num_actions == 0) return Evaluate();
    if (Search_Depth == 1) num_SortArray=num_actions;

    skip:

    if (Search_Depth == 4 && !changeit)          //鏈�娣卞眰鍐嶅姞鍏ュ巻鍙茶〃鎼滅储淇℃伅
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

            if (cur_depth == 2 && Search_Depth == 4)
                v2=Evaluate();

            if (fFoundPV && Search_Depth == 4) {
                MinmaxValue=minmax(cur_depth+1,alpha,alpha+1);
                if (MinmaxValue>alpha && MinmaxValue<beta) MinmaxValue=minmax(cur_depth+1,alpha,beta);
            }
            else MinmaxValue=minmax(cur_depth+1,alpha,beta);


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

    if (records[me_flag][(unsigned long)(hash+INT_MAX)%hashlen].x != 0 && records[me_flag][(unsigned long)(hash+INT_MAX)%hashlen].key==hash && turns<=58) {
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

    if (me_flag == BLACK && turns == 60){
        Search_Depth=1;
        minmax(0, (-1)*INT_MAX, INT_MAX);
        Search_Depth=2;
        minmax(0, (-1)*INT_MAX, INT_MAX);
    }
    else
    if (me_flag == WHITE && turns == 60){
        Search_Depth=1;
        minmax(0, (-1)*INT_MAX, INT_MAX);
        Search_Depth=1;
        minmax(0, (-1)*INT_MAX, INT_MAX);
    }
    else
    if (me_flag == WHITE && turns == 59){
        Search_Depth=1;
        minmax(0, (-1)*INT_MAX, INT_MAX);
        Search_Depth=3;
        minmax(0, (-1)*INT_MAX, INT_MAX);
    }
    else {
        Search_Depth=1;
        minmax(0, (-1)*INT_MAX, INT_MAX);

        Search_Depth=2;
        minmax(0, (-1)*INT_MAX, INT_MAX);

        Search_Depth=4;
        minmax(0, (-1)*INT_MAX, INT_MAX);
    }


    repetition[(unsigned long)(hash+INT_MAX)%hashlen].x=ans_command.x;
    repetition[(unsigned long)(hash+INT_MAX)%hashlen].y=ans_command.y;
    repetition[(unsigned long)(hash+INT_MAX)%hashlen].option=ans_command.option;
    return ans_command;
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
     * 鏉╂瑩鍣烽張澶夌娑擃亞銇氭笟濠閿涘苯鐣犻崣顏冪窗鐎电粯澹樼粭顑跨娑擃亜褰叉稉瀣畱娴ｅ秶鐤嗘潻娑滎攽閽�钘夌摍閵?
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

void load1() {
    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][1770559].key=158281728;
    records[1][1770559].x=5;
    records[1][1770559].y=4;
    records[1][1770559].option=1;

    records[1][3419505].key=1547927551;
    records[1][3419505].x=4;
    records[1][3419505].y=8;
    records[1][3419505].option=1;

    records[1][3447878].key=1983954943;
    records[1][3447878].x=5;
    records[1][3447878].y=3;
    records[1][3447878].option=5;

    records[1][3433617].key=423944192;
    records[1][3433617].x=5;
    records[1][3433617].y=8;
    records[1][3433617].option=2;

    records[1][2169299].key=1362677759;
    records[1][2169299].x=8;
    records[1][2169299].y=7;
    records[1][2169299].option=2;

    records[1][7279].key=2112514047;
    records[1][7279].x=5;
    records[1][7279].y=7;
    records[1][7279].option=6;

    records[1][1046740].key=337557504;
    records[1][1046740].x=4;
    records[1][1046740].y=4;
    records[1][1046740].option=1;

    records[1][457688].key=224968704;
    records[1][457688].x=5;
    records[1][457688].y=4;
    records[1][457688].option=0;

    records[1][2464121].key=70975488;
    records[1][2464121].x=5;
    records[1][2464121].y=3;
    records[1][2464121].option=1;

    records[1][3344387].key=2067851263;
    records[1][3344387].x=9;
    records[1][3344387].y=9;
    records[1][3344387].option=4;

    records[1][534740].key=337045504;
    records[1][534740].x=6;
    records[1][534740].y=5;
    records[1][534740].option=5;

    records[1][26977].key=1992534015;
    records[1][26977].x=6;
    records[1][26977].y=4;
    records[1][26977].option=3;

    records[1][889497].key=197400576;
    records[1][889497].x=9;
    records[1][889497].y=7;
    records[1][889497].option=1;


    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][1770559].key=158281728;
    records[1][1770559].x=5;
    records[1][1770559].y=4;
    records[1][1770559].option=1;

    records[1][2806774].key=923316224;
    records[1][2806774].x=9;
    records[1][2806774].y=7;
    records[1][2806774].option=5;

    records[1][3101542].key=859611136;
    records[1][3101542].x=5;
    records[1][3101542].y=3;
    records[1][3101542].option=5;

    records[1][3813647].key=1732321279;
    records[1][3813647].x=6;
    records[1][3813647].y=4;
    records[1][3813647].option=7;

    records[1][3748576].key=1028257792;
    records[1][3748576].x=8;
    records[1][3748576].y=7;
    records[1][3748576].option=4;

    records[1][1855893].key=1562363903;
    records[1][1855893].x=4;
    records[1][1855893].y=4;
    records[1][1855893].option=3;

    records[1][268220].key=1236776959;
    records[1][268220].x=8;
    records[1][268220].y=8;
    records[1][268220].option=0;

    records[1][2252363].key=846761984;
    records[1][2252363].x=7;
    records[1][2252363].y=6;
    records[1][2252363].option=5;

    records[1][2679477].key=1691187199;
    records[1][2679477].x=3;
    records[1][2679477].y=5;
    records[1][2679477].option=7;

    records[1][2086401].key=1610594303;
    records[1][2086401].x=7;
    records[1][2086401].y=7;
    records[1][2086401].option=4;

    records[1][2800381].key=1723308031;
    records[1][2800381].x=4;
    records[1][2800381].y=8;
    records[1][2800381].option=7;

    records[1][2089831].key=62601216;
    records[1][2089831].x=5;
    records[1][2089831].y=6;
    records[1][2089831].option=2;

    records[1][3164318].key=1567672319;
    records[1][3164318].x=4;
    records[1][3164318].y=5;
    records[1][3164318].option=6;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][215496].key=1924722687;
    records[2][215496].x=2;
    records[2][215496].y=3;
    records[2][215496].option=7;

    records[2][3591074].key=1456099327;
    records[2][3591074].x=8;
    records[2][3591074].y=2;
    records[2][3591074].option=5;

    records[2][220066].key=1452728319;
    records[2][220066].x=6;
    records[2][220066].y=6;
    records[2][220066].option=2;

    records[2][748216].key=97259520;
    records[2][748216].x=7;
    records[2][748216].y=3;
    records[2][748216].option=4;

    records[2][3136119].key=1891643391;
    records[2][3136119].x=6;
    records[2][3136119].y=5;
    records[2][3136119].option=7;

    records[2][2094019].key=558604288;
    records[2][2094019].x=6;
    records[2][2094019].y=2;
    records[2][2094019].option=3;

    records[2][660851].key=749170688;
    records[2][660851].x=6;
    records[2][660851].y=3;
    records[2][660851].option=5;

    records[2][2925599].key=1055434752;
    records[2][2925599].x=7;
    records[2][2925599].y=6;
    records[2][2925599].option=5;

    records[2][3893663].key=1796401151;
    records[2][3893663].x=6;
    records[2][3893663].y=7;
    records[2][3893663].option=0;

    records[2][491268].key=1041000448;
    records[2][491268].x=6;
    records[2][491268].y=8;
    records[2][491268].option=3;

    records[2][3162221].key=407672832;
    records[2][3162221].x=7;
    records[2][3162221].y=7;
    records[2][3162221].option=4;

    records[2][3024992].key=1767532543;
    records[2][3024992].x=5;
    records[2][3024992].y=4;
    records[2][3024992].option=2;

    records[2][415038].key=1180923903;
    records[2][415038].x=4;
    records[2][415038].y=4;
    records[2][415038].option=2;

    records[2][3027999].key=1055537152;
    records[2][3027999].x=2;
    records[2][3027999].y=4;
    records[2][3027999].option=3;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][3482520].key=1223991295;
    records[2][3482520].x=2;
    records[2][3482520].y=3;
    records[2][3482520].option=7;

    records[2][407681].key=868917248;
    records[2][407681].x=2;
    records[2][407681].y=4;
    records[2][407681].option=7;

    records[2][2435827].key=1490943999;
    records[2][2435827].x=3;
    records[2][2435827].y=4;
    records[2][2435827].option=6;

    records[2][2605420].key=1431113727;
    records[2][2605420].x=7;
    records[2][2605420].y=7;
    records[2][2605420].option=2;

    records[2][114643].key=108625920;
    records[2][114643].x=8;
    records[2][114643].y=2;
    records[2][114643].option=0;

    records[2][2524807].key=1443033087;
    records[2][2524807].x=6;
    records[2][2524807].y=6;
    records[2][2524807].option=5;

    records[2][2842012].key=883351552;
    records[2][2842012].x=7;
    records[2][2842012].y=2;
    records[2][2842012].option=0;

    records[2][2787561].key=7299072;
    records[2][2787561].x=5;
    records[2][2787561].y=7;
    records[2][2787561].option=6;

    records[2][3148317].key=599658496;
    records[2][3148317].x=3;
    records[2][3148317].y=5;
    records[2][3148317].option=3;

    records[2][2037660].key=2134544383;
    records[2][2037660].x=2;
    records[2][2037660].y=2;
    records[2][2037660].option=7;

    records[2][2862472].key=419373056;
    records[2][2862472].x=4;
    records[2][2862472].y=3;
    records[2][2862472].option=7;

    records[2][1124973].key=1657632767;
    records[2][1124973].x=8;
    records[2][1124973].y=6;
    records[2][1124973].option=3;

    records[2][2879039].key=1411387391;
    records[2][2879039].x=6;
    records[2][2879039].y=2;
    records[2][2879039].option=3;

    records[2][434388].key=336945152;
    records[2][434388].x=4;
    records[2][434388].y=5;
    records[2][434388].option=7;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][3845481].key=520355840;
    records[2][3845481].x=2;
    records[2][3845481].y=3;
    records[2][3845481].option=7;

    records[2][2888919].key=1251397631;
    records[2][2888919].x=8;
    records[2][2888919].y=2;
    records[2][2888919].option=5;

    records[2][1719327].key=1054228480;
    records[2][1719327].x=3;
    records[2][1719327].y=4;
    records[2][1719327].option=7;

    records[2][2451912].key=1926959103;
    records[2][2451912].x=6;
    records[2][2451912].y=6;
    records[2][2451912].option=2;

    records[2][152750].key=1116661759;
    records[2][152750].x=2;
    records[2][152750].y=2;
    records[2][152750].option=1;

    records[2][3481359].key=479991808;
    records[2][3481359].x=7;
    records[2][3481359].y=3;
    records[2][3481359].option=4;

    records[2][2738378].key=107249664;
    records[2][2738378].x=7;
    records[2][2738378].y=7;
    records[2][2738378].option=4;

    records[2][3889704].key=36401152;
    records[2][3889704].x=6;
    records[2][3889704].y=2;
    records[2][3889704].option=0;

    records[2][184900].key=1524692991;
    records[2][184900].x=4;
    records[2][184900].y=2;
    records[2][184900].option=3;

    records[2][37258].key=2124543999;
    records[2][37258].x=5;
    records[2][37258].y=2;
    records[2][37258].option=1;

    records[2][3536375].key=1380044799;
    records[2][3536375].x=6;
    records[2][3536375].y=2;
    records[2][3536375].option=7;

    records[2][1155971].key=1553663999;
    records[2][1155971].x=6;
    records[2][1155971].y=4;
    records[2][1155971].option=3;

    records[2][834795].key=1713342463;
    records[2][834795].x=6;
    records[2][834795].y=5;
    records[2][834795].option=7;

    records[2][1741073].key=934250496;
    records[2][1741073].x=5;
    records[2][1741073].y=5;
    records[2][1741073].option=7;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][171420].key=880680960;
    records[1][171420].x=8;
    records[1][171420].y=7;
    records[1][171420].option=5;

    records[1][1005253].key=1241513983;
    records[1][1005253].x=5;
    records[1][1005253].y=4;
    records[1][1005253].option=0;

    records[1][1740958].key=1566248959;
    records[1][1740958].x=3;
    records[1][1740958].y=9;
    records[1][1740958].option=6;

    records[1][1390204].key=2005897215;
    records[1][1390204].x=9;
    records[1][1390204].y=7;
    records[1][1390204].option=4;

    records[1][754635].key=1585262591;
    records[1][754635].x=5;
    records[1][754635].y=5;
    records[1][754635].option=1;

    records[1][3747115].key=720257024;
    records[1][3747115].x=6;
    records[1][3747115].y=5;
    records[1][3747115].option=1;

    records[1][518202].key=41029632;
    records[1][518202].x=9;
    records[1][518202].y=9;
    records[1][518202].option=1;

    records[1][3018995].key=239529984;
    records[1][3018995].x=7;
    records[1][3018995].y=8;
    records[1][3018995].option=6;

    records[1][1892636].key=370403328;
    records[1][1892636].x=7;
    records[1][1892636].y=5;
    records[1][1892636].option=3;

    records[1][3201431].key=767711232;
    records[1][3201431].x=8;
    records[1][3201431].y=6;
    records[1][3201431].option=4;

    records[1][144909].key=1044654080;
    records[1][144909].x=7;
    records[1][144909].y=5;
    records[1][144909].option=6;

    records[1][435643].key=780945408;
    records[1][435643].x=8;
    records[1][435643].y=4;
    records[1][435643].option=5;

    records[1][2868161].key=1355376639;
    records[1][2868161].x=7;
    records[1][2868161].y=5;
    records[1][2868161].option=6;

    records[1][2748206].key=379258880;
    records[1][2748206].x=2;
    records[1][2748206].y=9;
    records[1][2748206].option=6;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][273944].key=480784384;
    records[1][273944].x=4;
    records[1][273944].y=8;
    records[1][273944].option=1;

    records[1][611594].key=1613119487;
    records[1][611594].x=5;
    records[1][611594].y=4;
    records[1][611594].option=4;

    records[1][266604].key=1428774911;
    records[1][266604].x=4;
    records[1][266604].y=3;
    records[1][266604].option=3;

    records[1][1597858].key=202108928;
    records[1][1597858].x=5;
    records[1][1597858].y=5;
    records[1][1597858].option=0;

    records[1][1360010].key=1101869055;
    records[1][1360010].x=5;
    records[1][1360010].y=8;
    records[1][1360010].option=6;

    records[1][1734723].key=1070243840;
    records[1][1734723].x=4;
    records[1][1734723].y=5;
    records[1][1734723].option=1;

    records[1][1553954].key=714063872;
    records[1][1553954].x=6;
    records[1][1553954].y=6;
    records[1][1553954].option=5;

    records[1][2737572].key=1911244799;
    records[1][2737572].x=6;
    records[1][2737572].y=7;
    records[1][2737572].option=4;

    records[1][3101221].key=1627609087;
    records[1][3101221].x=8;
    records[1][3101221].y=7;
    records[1][3101221].option=5;

    records[1][147781].key=500658176;
    records[1][147781].x=9;
    records[1][147781].y=7;
    records[1][147781].option=0;

    records[1][3641060].key=1144150015;
    records[1][3641060].x=5;
    records[1][3641060].y=6;
    records[1][3641060].option=1;

    records[1][2186765].key=1046695936;
    records[1][2186765].x=8;
    records[1][2186765].y=7;
    records[1][2186765].option=2;

    records[1][301927].key=60813312;
    records[1][301927].x=7;
    records[1][301927].y=7;
    records[1][301927].option=5;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][894712].key=1605402623;
    records[2][894712].x=8;
    records[2][894712].y=2;
    records[2][894712].option=5;

    records[2][2301317].key=2010808319;
    records[2][2301317].x=7;
    records[2][2301317].y=3;
    records[2][2301317].option=0;

    records[2][2268478].key=1182777343;
    records[2][2268478].x=7;
    records[2][2268478].y=7;
    records[2][2268478].option=2;

    records[2][3824527].key=992333824;
    records[2][3824527].x=3;
    records[2][3824527].y=4;
    records[2][3824527].option=6;

    records[2][3065829].key=1371574271;
    records[2][3065829].x=4;
    records[2][3065829].y=3;
    records[2][3065829].option=3;

    records[2][2774230].key=2047281151;
    records[2][2774230].x=8;
    records[2][2774230].y=6;
    records[2][2774230].option=5;

    records[2][1150128].key=321660928;
    records[2][1150128].x=6;
    records[2][1150128].y=7;
    records[2][1150128].option=5;

    records[2][94975].key=924604416;
    records[2][94975].x=8;
    records[2][94975].y=7;
    records[2][94975].option=3;

    records[2][549724].key=625059840;
    records[2][549724].x=4;
    records[2][549724].y=4;
    records[2][549724].option=1;

    records[2][3792655].key=480303104;
    records[2][3792655].x=5;
    records[2][3792655].y=3;
    records[2][3792655].option=7;

    records[2][1800116].key=210311168;
    records[2][1800116].x=6;
    records[2][1800116].y=3;
    records[2][1800116].option=1;

    records[2][1289937].key=677799936;
    records[2][1289937].x=5;
    records[2][1289937].y=4;
    records[2][1289937].option=5;

    records[2][984260].key=785494016;
    records[2][984260].x=7;
    records[2][984260].y=7;
    records[2][984260].option=0;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][649291].key=2097156095;
    records[1][649291].x=4;
    records[1][649291].y=8;
    records[1][649291].option=1;

    records[1][1571862].key=26083328;
    records[1][1571862].x=5;
    records[1][1571862].y=3;
    records[1][1571862].option=5;

    records[1][492709].key=885002240;
    records[1][492709].x=9;
    records[1][492709].y=7;
    records[1][492709].option=4;

    records[1][340025].key=836849664;
    records[1][340025].x=5;
    records[1][340025].y=4;
    records[1][340025].option=5;

    records[1][139566].key=1628647423;
    records[1][139566].x=5;
    records[1][139566].y=8;
    records[1][139566].option=1;

    records[1][1050658].key=1965557759;
    records[1][1050658].x=5;
    records[1][1050658].y=5;
    records[1][1050658].option=2;

    records[1][3291378].key=1035800576;
    records[1][3291378].x=4;
    records[1][3291378].y=5;
    records[1][3291378].option=4;

    records[1][3181805].key=919691264;
    records[1][3181805].x=4;
    records[1][3181805].y=4;
    records[1][3181805].option=5;

    records[1][1180148].key=465690624;
    records[1][1180148].x=5;
    records[1][1180148].y=4;
    records[1][1180148].option=1;

    records[1][2255572].key=1590763519;
    records[1][2255572].x=7;
    records[1][2255572].y=7;
    records[1][2255572].option=3;

    records[1][3245376].key=387756032;
    records[1][3245376].x=6;
    records[1][3245376].y=8;
    records[1][3245376].option=5;

    records[1][1356852].key=721866752;
    records[1][1356852].x=2;
    records[1][1356852].y=9;
    records[1][1356852].option=7;

    records[1][1167417].key=837677056;
    records[1][1167417].x=9;
    records[1][1167417].y=9;
    records[1][1167417].option=5;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][273944].key=480784384;
    records[1][273944].x=4;
    records[1][273944].y=8;
    records[1][273944].option=1;

    records[1][611594].key=1613119487;
    records[1][611594].x=5;
    records[1][611594].y=3;
    records[1][611594].option=0;

    records[1][395082].key=1868902399;
    records[1][395082].x=5;
    records[1][395082].y=5;
    records[1][395082].option=0;

    records[1][1201246].key=57712640;
    records[1][1201246].x=4;
    records[1][1201246].y=5;
    records[1][1201246].option=5;

    records[1][3219789].key=1531727871;
    records[1][3219789].x=3;
    records[1][3219789].y=6;
    records[1][3219789].option=7;

    records[1][740032].key=1125249023;
    records[1][740032].x=8;
    records[1][740032].y=7;
    records[1][740032].option=0;

    records[1][2912558].key=1631420415;
    records[1][2912558].x=9;
    records[1][2912558].y=9;
    records[1][2912558].option=0;

    records[1][2113778].key=1034622976;
    records[1][2113778].x=9;
    records[1][2113778].y=7;
    records[1][2113778].option=0;

    records[1][1738375].key=1442246655;
    records[1][1738375].x=5;
    records[1][1738375].y=4;
    records[1][1738375].option=1;

    records[1][45226].key=1000554496;
    records[1][45226].x=4;
    records[1][45226].y=3;
    records[1][45226].option=7;

    records[1][3374184].key=291885056;
    records[1][3374184].x=4;
    records[1][3374184].y=7;
    records[1][3374184].option=1;

    records[1][576029].key=597086208;
    records[1][576029].x=5;
    records[1][576029].y=8;
    records[1][576029].option=4;

    records[1][1514219].key=462024704;
    records[1][1514219].x=6;
    records[1][1514219].y=4;
    records[1][1514219].option=7;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][561804].key=1557069823;
    records[2][561804].x=8;
    records[2][561804].y=2;
    records[2][561804].option=5;

    records[2][592771].key=1553100799;
    records[2][592771].x=2;
    records[2][592771].y=4;
    records[2][592771].option=7;

    records[2][191510].key=24702976;
    records[2][191510].x=7;
    records[2][191510].y=7;
    records[2][191510].option=2;

    records[2][1672213].key=822181888;
    records[2][1672213].x=3;
    records[2][1672213].y=5;
    records[2][1672213].option=6;

    records[2][1674313].key=390184960;
    records[2][1674313].x=2;
    records[2][1674313].y=2;
    records[2][1674313].option=1;

    records[2][729814].key=793239552;
    records[2][729814].x=3;
    records[2][729814].y=2;
    records[2][729814].option=2;

    records[2][2998617].key=967507968;
    records[2][2998617].x=9;
    records[2][2998617].y=2;
    records[2][2998617].option=0;

    records[2][2522357].key=1947029503;
    records[2][2522357].x=3;
    records[2][2522357].y=1;
    records[2][2522357].option=6;

    records[2][3891970].key=1584399935;
    records[2][3891970].x=7;
    records[2][3891970].y=3;
    records[2][3891970].option=2;

    records[2][2373342].key=1566881343;
    records[2][2373342].x=4;
    records[2][2373342].y=0;
    records[2][2373342].option=3;

    records[2][160653].key=1784668159;
    records[2][160653].x=7;
    records[2][160653].y=2;
    records[2][160653].option=4;

    records[2][3406313].key=1031915520;
    records[2][3406313].x=5;
    records[2][3406313].y=1;
    records[2][3406313].option=3;

    records[2][1104815].key=93616128;
    records[2][1104815].x=8;
    records[2][1104815].y=6;
    records[2][1104815].option=7;

    records[2][379100].key=1364887551;
    records[2][379100].x=8;
    records[2][379100].y=2;
    records[2][379100].option=5;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][273944].key=480784384;
    records[1][273944].x=4;
    records[1][273944].y=8;
    records[1][273944].option=1;

    records[1][611594].key=1613119487;
    records[1][611594].x=5;
    records[1][611594].y=3;
    records[1][611594].option=0;

    records[1][395082].key=1868902399;
    records[1][395082].x=5;
    records[1][395082].y=5;
    records[1][395082].option=0;

    records[1][957965].key=1045467136;
    records[1][957965].x=5;
    records[1][957965].y=8;
    records[1][957965].option=3;

    records[1][3138798].key=123650048;
    records[1][3138798].x=4;
    records[1][3138798].y=5;
    records[1][3138798].option=1;

    records[1][3940163].key=48451584;
    records[1][3940163].x=5;
    records[1][3940163].y=5;
    records[1][3940163].option=7;

    records[1][1378863].key=1857886207;
    records[1][1378863].x=5;
    records[1][1378863].y=4;
    records[1][1378863].option=7;

    records[1][2266892].key=2070773759;
    records[1][2266892].x=7;
    records[1][2266892].y=6;
    records[1][2266892].option=5;

    records[1][3984990].key=1312493567;
    records[1][3984990].x=8;
    records[1][3984990].y=7;
    records[1][3984990].option=4;

    records[1][1481160].key=1925988351;
    records[1][1481160].x=9;
    records[1][1481160].y=9;
    records[1][1481160].option=0;

    records[1][849154].key=585359360;
    records[1][849154].x=2;
    records[1][849154].y=9;
    records[1][849154].option=6;

    records[1][1679891].key=366190592;
    records[1][1679891].x=3;
    records[1][1679891].y=8;
    records[1][1679891].option=6;

    records[1][1318311].key=317829120;
    records[1][1318311].x=4;
    records[1][1318311].y=3;
    records[1][1318311].option=7;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][3551670].key=668061696;
    records[1][3551670].x=4;
    records[1][3551670].y=8;
    records[1][3551670].option=1;

    records[1][501846].key=1533009919;
    records[1][501846].x=5;
    records[1][501846].y=5;
    records[1][501846].option=4;

    records[1][2043961].key=838553600;
    records[1][2043961].x=5;
    records[1][2043961].y=8;
    records[1][2043961].option=6;

    records[1][578425].key=1321086975;
    records[1][578425].x=6;
    records[1][578425].y=7;
    records[1][578425].option=4;

    records[1][955208].key=161466368;
    records[1][955208].x=8;
    records[1][955208].y=7;
    records[1][955208].option=5;

    records[1][1209211].key=525719552;
    records[1][1209211].x=5;
    records[1][1209211].y=3;
    records[1][1209211].option=7;

    records[1][3864723].key=2132371455;
    records[1][3864723].x=5;
    records[1][3864723].y=4;
    records[1][3864723].option=7;

    records[1][2992154].key=939501568;
    records[1][2992154].x=7;
    records[1][2992154].y=8;
    records[1][2992154].option=0;

    records[1][140629].key=52652032;
    records[1][140629].x=2;
    records[1][140629].y=9;
    records[1][140629].option=1;

    records[1][3249789].key=1211758591;
    records[1][3249789].x=6;
    records[1][3249789].y=4;
    records[1][3249789].option=6;

    records[1][566034].key=1389074431;
    records[1][566034].x=7;
    records[1][566034].y=7;
    records[1][566034].option=2;

    records[1][235598].key=504745984;
    records[1][235598].x=6;
    records[1][235598].y=8;
    records[1][235598].option=2;

    records[1][847188].key=849356800;
    records[1][847188].x=3;
    records[1][847188].y=9;
    records[1][847188].option=2;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][3551670].key=668061696;
    records[1][3551670].x=4;
    records[1][3551670].y=8;
    records[1][3551670].option=1;

    records[1][199352].key=96710656;
    records[1][199352].x=5;
    records[1][199352].y=5;
    records[1][199352].option=7;

    records[1][2351097].key=1834858495;
    records[1][2351097].x=6;
    records[1][2351097].y=6;
    records[1][2351097].option=4;

    records[1][1548139].key=974057472;
    records[1][1548139].x=5;
    records[1][1548139].y=5;
    records[1][1548139].option=0;

    records[1][3067107].key=687577088;
    records[1][3067107].x=5;
    records[1][3067107].y=3;
    records[1][3067107].option=1;

    records[1][3947364].key=1656455167;
    records[1][3947364].x=4;
    records[1][3947364].y=5;
    records[1][3947364].option=3;

    records[1][863817].key=1641371647;
    records[1][863817].x=4;
    records[1][863817].y=6;
    records[1][863817].option=2;

    records[1][3947364].key=1656455167;
    records[1][3947364].x=4;
    records[1][3947364].y=5;
    records[1][3947364].option=3;

    records[1][863817].key=1641371647;
    records[1][863817].x=4;
    records[1][863817].y=6;
    records[1][863817].option=2;

    records[1][3947364].key=1656455167;
    records[1][3947364].x=4;
    records[1][3947364].y=5;
    records[1][3947364].option=3;

    records[1][863817].key=1641371647;
    records[1][863817].x=4;
    records[1][863817].y=6;
    records[1][863817].option=2;

    records[1][3947364].key=1656455167;
    records[1][3947364].x=4;
    records[1][3947364].y=5;
    records[1][3947364].option=3;

    records[1][863817].key=1641371647;
    records[1][863817].x=4;
    records[1][863817].y=6;
    records[1][863817].option=2;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][894712].key=1605402623;
    records[2][894712].x=8;
    records[2][894712].y=2;
    records[2][894712].option=5;

    records[2][239005].key=84750336;
    records[2][239005].x=6;
    records[2][239005].y=6;
    records[2][239005].option=3;

    records[2][1964087].key=1634471935;
    records[2][1964087].x=3;
    records[2][1964087].y=4;
    records[2][1964087].option=6;

    records[2][308171].key=1584816127;
    records[2][308171].x=7;
    records[2][308171].y=7;
    records[2][308171].option=4;

    records[2][1662671].key=1474170879;
    records[2][1662671].x=6;
    records[2][1662671].y=8;
    records[2][1662671].option=1;

    records[2][1897789].key=1978404863;
    records[2][1897789].x=2;
    records[2][1897789].y=3;
    records[2][1897789].option=7;

    records[2][3748687].key=1988255743;
    records[2][3748687].x=3;
    records[2][3748687].y=4;
    records[2][3748687].option=1;

    records[2][1298150].key=345808896;
    records[2][1298150].x=4;
    records[2][1298150].y=4;
    records[2][1298150].option=5;

    records[2][2006179].key=1682513919;
    records[2][2006179].x=3;
    records[2][2006179].y=5;
    records[2][2006179].option=7;

    records[2][3091331].key=1555599359;
    records[2][3091331].x=6;
    records[2][3091331].y=7;
    records[2][3091331].option=1;

    records[2][230203].key=1520738303;
    records[2][230203].x=7;
    records[2][230203].y=3;
    records[2][230203].option=3;

    records[2][3905674].key=1104414719;
    records[2][3905674].x=7;
    records[2][3905674].y=7;
    records[2][3905674].option=6;

    records[2][1122721].key=997632000;
    records[2][1122721].x=6;
    records[2][1122721].y=6;
    records[2][1122721].option=1;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][3551670].key=668061696;
    records[1][3551670].x=4;
    records[1][3551670].y=8;
    records[1][3551670].option=1;

    records[1][199352].key=96710656;
    records[1][199352].x=5;
    records[1][199352].y=5;
    records[1][199352].option=7;

    records[1][2351097].key=1834858495;
    records[1][2351097].x=6;
    records[1][2351097].y=6;
    records[1][2351097].option=4;

    records[1][2873379].key=1171382271;
    records[1][2873379].x=5;
    records[1][2873379].y=3;
    records[1][2873379].option=4;

    records[1][567722].key=1229076479;
    records[1][567722].x=4;
    records[1][567722].y=2;
    records[1][567722].option=7;

    records[1][2873379].key=1171382271;
    records[1][2873379].x=5;
    records[1][2873379].y=3;
    records[1][2873379].option=4;

    records[1][567722].key=1229076479;
    records[1][567722].x=4;
    records[1][567722].y=2;
    records[1][567722].option=7;

    records[1][2873379].key=1171382271;
    records[1][2873379].x=5;
    records[1][2873379].y=3;
    records[1][2873379].option=4;

    records[1][567722].key=1229076479;
    records[1][567722].x=4;
    records[1][567722].y=2;
    records[1][567722].option=7;

    records[1][2873379].key=1171382271;
    records[1][2873379].x=5;
    records[1][2873379].y=3;
    records[1][2873379].option=4;

    records[1][567722].key=1229076479;
    records[1][567722].x=4;
    records[1][567722].y=2;
    records[1][567722].option=7;

    records[1][2873379].key=1171382271;
    records[1][2873379].x=5;
    records[1][2873379].y=3;
    records[1][2873379].option=4;

    records[1][567722].key=1229076479;
    records[1][567722].x=4;
    records[1][567722].y=2;
    records[1][567722].option=7;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][1257980].key=1493766143;
    records[2][1257980].x=2;
    records[2][1257980].y=2;
    records[2][1257980].option=7;

    records[2][1751908].key=1654259711;
    records[2][1751908].x=3;
    records[2][1751908].y=3;
    records[2][1751908].option=6;

    records[2][1033855].key=413544448;
    records[2][1033855].x=6;
    records[2][1033855].y=6;
    records[2][1033855].option=3;

    records[2][443102].key=1820950527;
    records[2][443102].x=6;
    records[2][443102].y=8;
    records[2][443102].option=5;

    records[2][3888756].key=980398080;
    records[2][3888756].x=3;
    records[2][3888756].y=4;
    records[2][3888756].option=3;

    records[2][3627814].key=1856135167;
    records[2][3627814].x=8;
    records[2][3627814].y=2;
    records[2][3627814].option=5;

    records[2][1205382].key=985714688;
    records[2][1205382].x=7;
    records[2][1205382].y=3;
    records[2][1205382].option=4;

    records[2][2626726].key=91138048;
    records[2][2626726].x=7;
    records[2][2626726].y=7;
    records[2][2626726].option=4;

    records[2][390122].key=1484898303;
    records[2][390122].x=5;
    records[2][390122].y=9;
    records[2][390122].option=6;

    records[2][1048332].key=2069555199;
    records[2][1048332].x=6;
    records[2][1048332].y=8;
    records[2][1048332].option=7;

    records[2][405016].key=480915456;
    records[2][405016].x=7;
    records[2][405016].y=9;
    records[2][405016].option=4;

    records[2][1048332].key=2069555199;
    records[2][1048332].x=6;
    records[2][1048332].y=8;
    records[2][1048332].option=7;

    records[2][405016].key=480915456;
    records[2][405016].x=7;
    records[2][405016].y=9;
    records[2][405016].option=4;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][273944].key=480784384;
    records[1][273944].x=4;
    records[1][273944].y=8;
    records[1][273944].option=1;

    records[1][1336092].key=1621843967;
    records[1][1336092].x=5;
    records[1][1336092].y=4;
    records[1][1336092].option=0;

    records[1][3712729].key=1708220415;
    records[1][3712729].x=8;
    records[1][3712729].y=7;
    records[1][3712729].option=4;

    records[1][1999119].key=1730506751;
    records[1][1999119].x=9;
    records[1][1999119].y=7;
    records[1][1999119].option=0;

    records[1][3212988].key=1011722240;
    records[1][3212988].x=5;
    records[1][3212988].y=3;
    records[1][3212988].option=7;

    records[1][2583244].key=563093504;
    records[1][2583244].x=8;
    records[1][2583244].y=7;
    records[1][2583244].option=5;

    records[1][2831278].key=891340800;
    records[1][2831278].x=5;
    records[1][2831278].y=8;
    records[1][2831278].option=2;

    records[1][3713162].key=1104222207;
    records[1][3713162].x=6;
    records[1][3713162].y=4;
    records[1][3713162].option=3;

    records[1][3525424].key=2088032255;
    records[1][3525424].x=7;
    records[1][3525424].y=5;
    records[1][3525424].option=7;

    records[1][1000500].key=721510400;
    records[1][1000500].x=4;
    records[1][1000500].y=4;
    records[1][1000500].option=3;

    records[1][2246433].key=1738754047;
    records[1][2246433].x=8;
    records[1][2246433].y=6;
    records[1][2246433].option=0;

    records[1][3363758].key=891873280;
    records[1][3363758].x=5;
    records[1][3363758].y=5;
    records[1][3363758].option=4;

    records[1][713767].key=2081220607;
    records[1][713767].x=2;
    records[1][713767].y=9;
    records[1][713767].option=6;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][2553623].key=1507061759;
    records[2][2553623].x=2;
    records[2][2553623].y=3;
    records[2][2553623].option=1;

    records[2][626990].key=1629134847;
    records[2][626990].x=6;
    records[2][626990].y=8;
    records[2][626990].option=1;

    records[2][263539].key=748773376;
    records[2][263539].x=3;
    records[2][263539].y=3;
    records[2][263539].option=1;

    records[2][2524189].key=1851031551;
    records[2][2524189].x=8;
    records[2][2524189].y=2;
    records[2][2524189].option=5;

    records[2][2411878].key=2110918655;
    records[2][2411878].x=7;
    records[2][2411878].y=8;
    records[2][2411878].option=6;

    records[2][43590].key=728553472;
    records[2][43590].x=3;
    records[2][43590].y=4;
    records[2][43590].option=7;

    records[2][580874].key=1613088767;
    records[2][580874].x=8;
    records[2][580874].y=7;
    records[2][580874].option=4;

    records[2][2008250].key=554518528;
    records[2][2008250].x=9;
    records[2][2008250].y=2;
    records[2][2008250].option=5;

    records[2][183315].key=1616691199;
    records[2][183315].x=7;
    records[2][183315].y=7;
    records[2][183315].option=1;

    records[2][3905972].key=1464414207;
    records[2][3905972].x=8;
    records[2][3905972].y=8;
    records[2][3905972].option=1;

    records[2][2150756].key=402661376;
    records[2][2150756].x=4;
    records[2][2150756].y=5;
    records[2][2150756].option=5;

    records[2][1209983].key=413720576;
    records[2][1209983].x=4;
    records[2][1209983].y=3;
    records[2][1209983].option=6;

    records[2][2590024].key=1415098367;
    records[2][2590024].x=8;
    records[2][2590024].y=3;
    records[2][2590024].option=5;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][1770559].key=158281728;
    records[1][1770559].x=5;
    records[1][1770559].y=4;
    records[1][1770559].option=1;

    records[1][937772].key=1173446655;
    records[1][937772].x=8;
    records[1][937772].y=7;
    records[1][937772].option=4;

    records[1][2628320].key=1027137536;
    records[1][2628320].x=9;
    records[1][2628320].y=9;
    records[1][2628320].option=0;

    records[1][1411263].key=669921280;
    records[1][1411263].x=5;
    records[1][1411263].y=3;
    records[1][1411263].option=0;

    records[1][1016576].key=129527808;
    records[1][1016576].x=5;
    records[1][1016576].y=5;
    records[1][1016576].option=1;

    records[1][2593677].key=1787101183;
    records[1][2593677].x=8;
    records[1][2593677].y=9;
    records[1][2593677].option=0;

    records[1][77663].key=1536585727;
    records[1][77663].x=7;
    records[1][77663].y=9;
    records[1][77663].option=5;

    records[1][2398409].key=902907904;
    records[1][2398409].x=6;
    records[1][2398409].y=4;
    records[1][2398409].option=0;

    records[1][3971685].key=632481792;
    records[1][3971685].x=6;
    records[1][3971685].y=10;
    records[1][3971685].option=2;

    records[1][2082492].key=1010591744;
    records[1][2082492].x=9;
    records[1][2082492].y=7;
    records[1][2082492].option=5;

    records[1][2029396].key=2102536191;
    records[1][2029396].x=6;
    records[1][2029396].y=9;
    records[1][2029396].option=7;

    records[1][1337059].key=1937844223;
    records[1][1337059].x=4;
    records[1][1337059].y=8;
    records[1][1337059].option=4;

    records[1][3812116].key=596322304;
    records[1][3812116].x=3;
    records[1][3812116].y=7;
    records[1][3812116].option=4;

    records[2][3537126].key=1600045055;
    records[2][3537126].x=6;
    records[2][3537126].y=7;
    records[2][3537126].option=7;

    records[2][1082781].key=85594112;
    records[2][1082781].x=2;
    records[2][1082781].y=3;
    records[2][1082781].option=7;

    records[2][3863931].key=1780371455;
    records[2][3863931].x=3;
    records[2][3863931].y=4;
    records[2][3863931].option=6;

    records[2][2119611].key=2034626559;
    records[2][2119611].x=6;
    records[2][2119611].y=8;
    records[2][2119611].option=3;

    records[2][2893066].key=1615400959;
    records[2][2893066].x=6;
    records[2][2893066].y=6;
    records[2][2893066].option=2;

    records[2][2021987].key=1426530303;
    records[2][2021987].x=6;
    records[2][2021987].y=5;
    records[2][2021987].option=6;

    records[2][1900056].key=482410496;
    records[2][1900056].x=7;
    records[2][1900056].y=4;
    records[2][1900056].option=5;

    records[2][2021987].key=1426530303;
    records[2][2021987].x=6;
    records[2][2021987].y=5;
    records[2][2021987].option=6;

    records[2][1900056].key=482410496;
    records[2][1900056].x=7;
    records[2][1900056].y=4;
    records[2][1900056].option=5;

    records[2][2021987].key=1426530303;
    records[2][2021987].x=6;
    records[2][2021987].y=5;
    records[2][2021987].option=6;

    records[2][1900056].key=482410496;
    records[2][1900056].x=7;
    records[2][1900056].y=4;
    records[2][1900056].option=5;

    records[2][2021987].key=1426530303;
    records[2][2021987].x=6;
    records[2][2021987].y=5;
    records[2][2021987].option=6;

    records[2][1900056].key=482410496;
    records[2][1900056].x=7;
    records[2][1900056].y=4;
    records[2][1900056].option=5;

    records[2][2021987].key=1426530303;
    records[2][2021987].x=6;
    records[2][2021987].y=5;
    records[2][2021987].option=6;

    records[2][1900056].key=482410496;
    records[2][1900056].x=7;
    records[2][1900056].y=4;
    records[2][1900056].option=5;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][1770559].key=158281728;
    records[1][1770559].x=5;
    records[1][1770559].y=4;
    records[1][1770559].option=1;

    records[1][937772].key=1173446655;
    records[1][937772].x=8;
    records[1][937772].y=7;
    records[1][937772].option=4;

    records[1][2628320].key=1027137536;
    records[1][2628320].x=9;
    records[1][2628320].y=9;
    records[1][2628320].option=0;

    records[1][909027].key=1937416191;
    records[1][909027].x=5;
    records[1][909027].y=5;
    records[1][909027].option=1;

    records[1][3547173].key=376057856;
    records[1][3547173].x=8;
    records[1][3547173].y=9;
    records[1][3547173].option=5;

    records[1][3695995].key=1780203519;
    records[1][3695995].x=7;
    records[1][3695995].y=10;
    records[1][3695995].option=5;

    records[1][2797368].key=611307520;
    records[1][2797368].x=4;
    records[1][2797368].y=8;
    records[1][2797368].option=7;

    records[1][604071].key=1569112063;
    records[1][604071].x=9;
    records[1][604071].y=7;
    records[1][604071].option=0;

    records[1][1835094].key=1534343167;
    records[1][1835094].x=7;
    records[1][1835094].y=6;
    records[1][1835094].option=0;

    records[1][2987922].key=1903495167;
    records[1][2987922].x=2;
    records[1][2987922].y=9;
    records[1][2987922].option=2;

    records[1][2892780].key=691402752;
    records[1][2892780].x=6;
    records[1][2892780].y=4;
    records[1][2892780].option=5;

    records[1][2651175].key=2083158015;
    records[1][2651175].x=6;
    records[1][2651175].y=5;
    records[1][2651175].option=6;

    records[1][2928080].key=451438592;
    records[1][2928080].x=5;
    records[1][2928080].y=3;
    records[1][2928080].option=5;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][1770559].key=158281728;
    records[1][1770559].x=5;
    records[1][1770559].y=4;
    records[1][1770559].option=1;

    records[1][937772].key=1173446655;
    records[1][937772].x=8;
    records[1][937772].y=7;
    records[1][937772].option=4;

    records[1][3813238].key=412323840;
    records[1][3813238].x=9;
    records[1][3813238].y=9;
    records[1][3813238].option=0;

    records[1][1655027].key=238166016;
    records[1][1655027].x=4;
    records[1][1655027].y=8;
    records[1][1655027].option=7;

    records[1][3732450].key=460242944;
    records[1][3732450].x=5;
    records[1][3732450].y=9;
    records[1][3732450].option=2;

    records[1][195140].key=1524703231;
    records[1][195140].x=9;
    records[1][195140].y=7;
    records[1][195140].option=5;

    records[1][1896905].key=1130405887;
    records[1][1896905].x=5;
    records[1][1896905].y=8;
    records[1][1896905].option=4;

    records[1][2566397].key=471076864;
    records[1][2566397].x=7;
    records[1][2566397].y=6;
    records[1][2566397].option=6;

    records[1][3815047].key=1444323327;
    records[1][3815047].x=5;
    records[1][3815047].y=5;
    records[1][3815047].option=3;

    records[1][2436208].key=1318944767;
    records[1][2436208].x=8;
    records[1][2436208].y=8;
    records[1][2436208].option=5;

    records[1][2719346].key=1775226879;
    records[1][2719346].x=4;
    records[1][2719346].y=7;
    records[1][2719346].option=7;

    records[1][2602963].key=111114240;
    records[1][2602963].x=5;
    records[1][2602963].y=8;
    records[1][2602963].option=7;

    records[1][2859948].key=1687367679;
    records[1][2859948].x=6;
    records[1][2859948].y=9;
    records[1][2859948].option=4;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][894712].key=1605402623;
    records[2][894712].x=8;
    records[2][894712].y=2;
    records[2][894712].option=5;

    records[2][239005].key=84750336;
    records[2][239005].x=6;
    records[2][239005].y=6;
    records[2][239005].option=3;

    records[2][1964087].key=1634471935;
    records[2][1964087].x=3;
    records[2][1964087].y=4;
    records[2][1964087].option=6;

    records[2][1060816].key=1701568511;
    records[2][1060816].x=2;
    records[2][1060816].y=3;
    records[2][1060816].option=7;

    records[2][2473947].key=1138982911;
    records[2][2473947].x=4;
    records[2][2473947].y=3;
    records[2][2473947].option=3;

    records[2][2452409].key=326963200;
    records[2][2452409].x=7;
    records[2][2452409].y=3;
    records[2][2452409].option=7;

    records[2][103487].key=156614656;
    records[2][103487].x=6;
    records[2][103487].y=8;
    records[2][103487].option=1;

    records[2][1171614].key=1565679615;
    records[2][1171614].x=7;
    records[2][1171614].y=7;
    records[2][1171614].option=7;

    records[2][1055137].key=997564416;
    records[2][1055137].x=8;
    records[2][1055137].y=8;
    records[2][1055137].option=2;

    records[2][2038919].key=190550016;
    records[2][2038919].x=3;
    records[2][2038919].y=4;
    records[2][2038919].option=5;

    records[2][3096077].key=1047605248;
    records[2][3096077].x=2;
    records[2][3096077].y=5;
    records[2][3096077].option=1;

    records[2][472948].key=1204981759;
    records[2][472948].x=7;
    records[2][472948].y=8;
    records[2][472948].option=1;

    records[2][1930444].key=1814437887;
    records[2][1930444].x=8;
    records[2][1930444].y=7;
    records[2][1930444].option=6;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][1229938].key=521740288;
    records[1][1229938].x=3;
    records[1][1229938].y=9;
    records[1][1229938].option=1;

    records[1][3508714].key=236019712;
    records[1][3508714].x=4;
    records[1][3508714].y=9;
    records[1][3508714].option=6;

    records[1][1652834].key=970162176;
    records[1][1652834].x=8;
    records[1][1652834].y=7;
    records[1][1652834].option=2;

    records[1][3565798].key=1600073727;
    records[1][3565798].x=9;
    records[1][3565798].y=9;
    records[1][3565798].option=0;

    records[1][1758330].key=1550266367;
    records[1][1758330].x=5;
    records[1][1758330].y=8;
    records[1][1758330].option=6;

    records[1][3784854].key=540295168;
    records[1][3784854].x=8;
    records[1][3784854].y=6;
    records[1][3784854].option=5;

    records[1][3263447].key=1023772672;
    records[1][3263447].x=2;
    records[1][3263447].y=9;
    records[1][3263447].option=6;

    records[1][3102785].key=1867610111;
    records[1][3102785].x=6;
    records[1][3102785].y=6;
    records[1][3102785].option=3;

    records[1][334744].key=1220843519;
    records[1][334744].x=6;
    records[1][334744].y=7;
    records[1][334744].option=5;

    records[1][2113056].key=1510621183;
    records[1][2113056].x=5;
    records[1][2113056].y=8;
    records[1][2113056].option=6;

    records[1][508548].key=529018880;
    records[1][508548].x=5;
    records[1][508548].y=4;
    records[1][508548].option=4;

    records[1][1000994].key=713510912;
    records[1][1000994].x=5;
    records[1][1000994].y=3;
    records[1][1000994].option=5;

    records[1][1708314].key=1166217215;
    records[1][1708314].x=4;
    records[1][1708314].y=3;
    records[1][1708314].option=7;

    records[1][195369].key=260706304;
    records[1][195369].x=4;
    records[1][195369].y=4;
    records[1][195369].option=3;

    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][273944].key=480784384;
    records[1][273944].x=4;
    records[1][273944].y=8;
    records[1][273944].option=1;

    records[1][611594].key=1613119487;
    records[1][611594].x=5;
    records[1][611594].y=3;
    records[1][611594].option=0;

    records[1][395082].key=1868902399;
    records[1][395082].x=5;
    records[1][395082].y=5;
    records[1][395082].option=0;

    records[1][957965].key=1045467136;
    records[1][957965].x=5;
    records[1][957965].y=8;
    records[1][957965].option=3;

    records[1][3947251].key=1492455423;
    records[1][3947251].x=9;
    records[1][3947251].y=9;
    records[1][3947251].option=5;

    records[1][1531238].key=2110038015;
    records[1][1531238].x=4;
    records[1][1531238].y=3;
    records[1][1531238].option=3;

    records[1][1468144].key=577978368;
    records[1][1468144].x=4;
    records[1][1468144].y=5;
    records[1][1468144].option=5;

    records[1][644980].key=1205153791;
    records[1][644980].x=4;
    records[1][644980].y=4;
    records[1][644980].option=0;

    records[1][3804809].key=648314880;
    records[1][3804809].x=4;
    records[1][3804809].y=5;
    records[1][3804809].option=0;

    records[1][1141192].key=1925648383;
    records[1][1141192].x=2;
    records[1][1141192].y=9;
    records[1][1141192].option=6;

    records[1][937578].key=745447424;
    records[1][937578].x=3;
    records[1][937578].y=5;
    records[1][937578].option=0;

    records[1][3530789].key=376041472;
    records[1][3530789].x=3;
    records[1][3530789].y=8;
    records[1][3530789].option=6;

    records[1][3905365].key=1308413951;
    records[1][3905365].x=2;
    records[1][3905365].y=3;
    records[1][3905365].option=0;


}

void load2() {
    records[1][3690047].key=1412198399;
    records[1][3690047].x=9;
    records[1][3690047].y=8;
    records[1][3690047].option=4;

    records[1][531303].key=61042688;
    records[1][531303].x=3;
    records[1][531303].y=9;
    records[1][531303].option=6;

    records[1][3551670].key=668061696;
    records[1][3551670].x=4;
    records[1][3551670].y=8;
    records[1][3551670].option=1;

    records[1][199352].key=96710656;
    records[1][199352].x=5;
    records[1][199352].y=5;
    records[1][199352].option=7;

    records[1][2351097].key=1834858495;
    records[1][2351097].x=6;
    records[1][2351097].y=6;
    records[1][2351097].option=4;

    records[1][2834345].key=775344128;
    records[1][2834345].x=5;
    records[1][2834345].y=3;
    records[1][2834345].option=0;

    records[1][3210189].key=791719936;
    records[1][3210189].x=8;
    records[1][3210189].y=7;
    records[1][3210189].option=5;

    records[1][2770322].key=651280384;
    records[1][2770322].x=5;
    records[1][2770322].y=5;
    records[1][2770322].option=6;

    records[1][766534].key=729276416;
    records[1][766534].x=6;
    records[1][766534].y=4;
    records[1][766534].option=5;

    records[1][3004945].key=1163513855;
    records[1][3004945].x=5;
    records[1][3004945].y=4;
    records[1][3004945].option=4;

    records[1][356338].key=8867840;
    records[1][356338].x=9;
    records[1][356338].y=7;
    records[1][356338].option=4;

    records[1][1408598].key=281919488;
    records[1][1408598].x=4;
    records[1][1408598].y=3;
    records[1][1408598].option=7;

    records[1][105257].key=260616192;
    records[1][105257].x=5;
    records[1][105257].y=8;
    records[1][105257].option=2;

    records[1][3985712].key=836495360;
    records[1][3985712].x=5;
    records[1][3985712].y=4;
    records[1][3985712].option=7;

    records[1][3777979].key=784287744;
    records[1][3777979].x=7;
    records[1][3777979].y=8;
    records[1][3777979].option=6;

    records[1][1731757].key=1914238975;
    records[1][1731757].x=6;
    records[1][1731757].y=5;
    records[1][1731757].option=6;

    records[1][2122660].key=658632704;
    records[1][2122660].x=5;
    records[1][2122660].y=7;
    records[1][2122660].option=0;

    records[1][2942763].key=1971449855;
    records[1][2942763].x=4;
    records[1][2942763].y=7;
    records[1][2942763].option=6;

    records[1][2931496].key=1059440640;
    records[1][2931496].x=6;
    records[1][2931496].y=8;
    records[1][2931496].option=4;

    records[1][3872918].key=540383232;
    records[1][3872918].x=6;
    records[1][3872918].y=6;
    records[1][3872918].option=4;

    records[1][3559901].key=1596067839;
    records[1][3559901].x=8;
    records[1][3559901].y=7;
    records[1][3559901].option=0;

    records[1][3866380].key=2072373247;
    records[1][3866380].x=7;
    records[1][3866380].y=5;
    records[1][3866380].option=6;

    records[1][465602].key=328976384;
    records[1][465602].x=8;
    records[1][465602].y=4;
    records[1][465602].option=5;

    records[1][1062506].key=745572352;
    records[1][1062506].x=5;
    records[1][1062506].y=6;
    records[1][1062506].option=5;

    records[1][3940831].key=800450560;
    records[1][3940831].x=5;
    records[1][3940831].y=7;
    records[1][3940831].option=2;

    records[1][1638148].key=1042147328;
    records[1][1638148].x=4;
    records[1][1638148].y=7;
    records[1][1638148].option=2;

    records[1][1933480].key=1798440959;
    records[1][1933480].x=5;
    records[1][1933480].y=5;
    records[1][1933480].option=2;

    records[1][2801248].key=515311616;
    records[1][2801248].x=4;
    records[1][2801248].y=4;
    records[1][2801248].option=3;

    records[1][3039609].key=71550976;
    records[1][3039609].x=4;
    records[1][3039609].y=5;
    records[1][3039609].option=7;

    records[1][2491730].key=1646999551;
    records[1][2491730].x=7;
    records[1][2491730].y=6;
    records[1][2491730].option=7;

    records[1][913926].key=1725421567;
    records[1][913926].x=8;
    records[1][913926].y=7;
    records[1][913926].option=4;

    records[1][3738347].key=464248832;
    records[1][3738347].x=5;
    records[1][3738347].y=4;
    records[1][3738347].option=4;

    records[1][3174228].key=2103681023;
    records[1][3174228].x=5;
    records[1][3174228].y=5;
    records[1][3174228].option=6;

    records[1][1246055].key=61757440;
    records[1][1246055].x=5;
    records[1][1246055].y=6;
    records[1][1246055].option=2;

    records[1][774282].key=1101283327;
    records[1][774282].x=3;
    records[1][774282].y=6;
    records[1][774282].option=6;

    records[1][3164989].key=727674880;
    records[1][3164989].x=5;
    records[1][3164989].y=4;
    records[1][3164989].option=6;

    records[1][1977559].key=1250486271;
    records[1][1977559].x=9;
    records[1][1977559].y=9;
    records[1][1977559].option=4;

    records[1][908412].key=753418240;
    records[1][908412].x=6;
    records[1][908412].y=3;
    records[1][908412].option=6;

    records[1][2269673].key=1030778880;
    records[1][2269673].x=8;
    records[1][2269673].y=8;
    records[1][2269673].option=6;

    records[1][456720].key=704966656;
    records[1][456720].x=7;
    records[1][456720].y=7;
    records[1][456720].option=2;

    records[1][1752083].key=1618259967;
    records[1][1752083].x=6;
    records[1][1752083].y=6;
    records[1][1752083].option=6;

    records[1][1609443].key=1938116607;
    records[1][1609443].x=6;
    records[1][1609443].y=4;
    records[1][1609443].option=0;

    records[1][3794343].key=320305152;
    records[1][3794343].x=6;
    records[1][3794343].y=3;
    records[1][3794343].option=0;

    records[1][514607].key=1857021951;
    records[1][514607].x=5;
    records[1][514607].y=3;
    records[1][514607].option=4;

    records[1][1188031].key=669698048;
    records[1][1188031].x=7;
    records[1][1188031].y=5;
    records[1][1188031].option=2;

    records[1][1776662].key=26288128;
    records[1][1776662].x=7;
    records[1][1776662].y=3;
    records[1][1776662].option=1;

    records[1][285221].key=1624793087;
    records[1][285221].x=9;
    records[1][285221].y=7;
    records[1][285221].option=0;

    records[1][2386739].key=494897152;
    records[1][2386739].x=6;
    records[1][2386739].y=5;
    records[1][2386739].option=6;

    records[1][971612].key=625481728;
    records[1][971612].x=7;
    records[1][971612].y=6;
    records[1][971612].option=4;

    records[1][3218114].key=331728896;
    records[1][3218114].x=4;
    records[1][3218114].y=2;
    records[1][3218114].option=1;

    records[1][681175].key=1249189887;
    records[1][681175].x=4;
    records[1][681175].y=5;
    records[1][681175].option=7;

    records[1][3803327].key=672313344;
    records[1][3803327].x=9;
    records[1][3803327].y=2;
    records[1][3803327].option=3;

    records[1][2760576].key=643270656;
    records[1][2760576].x=7;
    records[1][2760576].y=5;
    records[1][2760576].option=2;

    records[1][2713738].key=1103222783;
    records[1][2713738].x=6;
    records[1][2713738].y=5;
    records[1][2713738].option=3;

    records[1][3490930].key=524001280;
    records[1][3490930].x=7;
    records[1][3490930].y=5;
    records[1][3490930].option=4;

    records[1][1286696].key=33798144;
    records[1][1286696].x=9;
    records[1][1286696].y=3;
    records[1][1286696].option=2;

    records[1][2445244].key=1238953983;
    records[1][2445244].x=8;
    records[1][2445244].y=3;
    records[1][2445244].option=4;

    records[1][1149016].key=737658880;
    records[1][1149016].x=5;
    records[1][1149016].y=5;
    records[1][1149016].option=4;

    records[1][2702283].key=1587210239;
    records[1][2702283].x=6;
    records[1][2702283].y=4;
    records[1][2702283].option=4;

    records[1][2050700].key=1558558719;
    records[1][2050700].x=7;
    records[1][2050700].y=4;
    records[1][2050700].option=5;

	
	
    records[2][3537126].key=1600045055;
    records[2][3537126].x=6;
    records[2][3537126].y=7;
    records[2][3537126].option=7;

    records[2][1082781].key=85594112;
    records[2][1082781].x=2;
    records[2][1082781].y=3;
    records[2][1082781].option=7;

    records[2][986964].key=2101493759;
    records[2][986964].x=3;
    records[2][986964].y=4;
    records[2][986964].option=1;

    records[2][1515678].key=1566023679;
    records[2][1515678].x=6;
    records[2][1515678].y=8;
    records[2][1515678].option=2;

    records[2][3247341].key=919756800;
    records[2][3247341].x=2;
    records[2][3247341].y=4;
    records[2][3247341].option=6;

    records[2][190025].key=1640697855;
    records[2][190025].x=8;
    records[2][190025].y=2;
    records[2][190025].option=0;

    records[2][3354630].key=475865088;
    records[2][3354630].x=4;
    records[2][3354630].y=4;
    records[2][3354630].option=2;

    records[2][229125].key=244740096;
    records[2][229125].x=2;
    records[2][229125].y=2;
    records[2][229125].option=6;

    records[2][3141810].key=2031648767;
    records[2][3141810].x=3;
    records[2][3141810].y=1;
    records[2][3141810].option=6;

    records[2][537658].key=297048512;
    records[2][537658].x=6;
    records[2][537658].y=6;
    records[2][537658].option=1;

    records[2][3904745].key=264415680;
    records[2][3904745].x=5;
    records[2][3904745].y=5;
    records[2][3904745].option=7;

    records[2][3431780].key=659941824;
    records[2][3431780].x=6;
    records[2][3431780].y=7;
    records[2][3431780].option=3;

    records[2][3937757].key=1340446271;
    records[2][3937757].x=6;
    records[2][3937757].y=6;
    records[2][3937757].option=6;

    records[2][3987642].key=812497344;
    records[2][3987642].x=7;
    records[2][3987642].y=5;
    records[2][3987642].option=1;

    records[2][1368227].key=1425876543;
    records[2][1368227].x=5;
    records[2][1368227].y=8;
    records[2][1368227].option=6;

    records[2][10138].key=680520128;
    records[2][10138].x=7;
    records[2][10138].y=2;
    records[2][10138].option=2;

    records[2][3866777].key=1196375615;
    records[2][3866777].x=4;
    records[2][3866777].y=0;
    records[2][3866777].option=7;

    records[2][2350315].key=1714857983;
    records[2][2350315].x=5;
    records[2][2350315].y=1;
    records[2][2350315].option=4;

    records[2][1245388].key=1557753407;
    records[2][1245388].x=8;
    records[2][1245388].y=5;
    records[2][1245388].option=2;

    records[2][3426693].key=1755934271;
    records[2][3426693].x=6;
    records[2][3426693].y=1;
    records[2][3426693].option=6;

    records[2][2725495].key=1635233343;
    records[2][2725495].x=4;
    records[2][2725495].y=3;
    records[2][2725495].option=7;

    records[2][3835810].key=1200344639;
    records[2][3835810].x=4;
    records[2][3835810].y=0;
    records[2][3835810].option=1;

    records[2][530487].key=1633038335;
    records[2][530487].x=3;
    records[2][530487].y=3;
    records[2][530487].option=7;

    records[2][2014363].key=654524416;
    records[2][2014363].x=5;
    records[2][2014363].y=4;
    records[2][2014363].option=7;

    records[2][3571481].key=712081408;
    records[2][3571481].x=4;
    records[2][3571481].y=4;
    records[2][3571481].option=7;

    records[2][2692879].key=479203328;
    records[2][2692879].x=9;
    records[2][2692879].y=2;
    records[2][2692879].option=5;

    records[2][1051690].key=489562112;
    records[2][1051690].x=5;
    records[2][1051690].y=5;
    records[2][1051690].option=5;

    records[2][1611461].key=1242120191;
    records[2][1611461].x=8;
    records[2][1611461].y=4;
    records[2][1611461].option=3;

    records[2][871754].key=617381888;
    records[2][871754].x=7;
    records[2][871754].y=6;
    records[2][871754].option=0;

    records[2][1006264].key=97517568;
    records[2][1006264].x=8;
    records[2][1006264].y=3;
    records[2][1006264].option=3;

    records[2][1383556].key=1781891071;
    records[2][1383556].x=4;
    records[2][1383556].y=6;
    records[2][1383556].option=7;

    records[2][2046236].key=370556928;
    records[2][2046236].x=7;
    records[2][2046236].y=5;
    records[2][2046236].option=3;

    records[2][3573786].key=940083200;
    records[2][3573786].x=6;
    records[2][3573786].y=6;
    records[2][3573786].option=4;

    records[2][2092660].key=978601984;
    records[2][2092660].x=8;
    records[2][2092660].y=5;
    records[2][2092660].option=3;

    records[2][2561280].key=1383069695;
    records[2][2561280].x=8;
    records[2][2561280].y=6;
    records[2][2561280].option=3;

    records[2][495426].key=841005056;
    records[2][495426].x=8;
    records[2][495426].y=7;
    records[2][495426].option=3;

    records[2][3343790].key=2143850495;
    records[2][3343790].x=8;
    records[2][3343790].y=8;
    records[2][3343790].option=4;

    records[2][137569].key=1992644607;
    records[2][137569].x=7;
    records[2][137569].y=6;
    records[2][137569].option=6;

    records[2][1395002].key=1065904128;
    records[2][1395002].x=5;
    records[2][1395002].y=5;
    records[2][1395002].option=0;

    records[2][2142345].key=1898649599;
    records[2][2142345].x=7;
    records[2][2142345].y=8;
    records[2][2142345].option=5;

    records[2][1376746].key=233887744;
    records[2][1376746].x=6;
    records[2][1376746].y=9;
    records[2][1376746].option=0;

    records[2][2720985].key=455231488;
    records[2][2720985].x=7;
    records[2][2720985].y=7;
    records[2][2720985].option=3;

    records[2][3833273].key=1580341247;
    records[2][3833273].x=5;
    records[2][3833273].y=9;
    records[2][3833273].option=3;

    records[2][1409408].key=1893916671;
    records[2][1409408].x=6;
    records[2][1409408].y=9;
    records[2][1409408].option=2;

    records[2][569576].key=801079296;
    records[2][569576].x=7;
    records[2][569576].y=0;
    records[2][569576].option=5;

    records[2][194494].key=440705024;
    records[2][194494].x=4;
    records[2][194494].y=5;
    records[2][194494].option=6;

    records[2][3211682].key=203722752;
    records[2][3211682].x=5;
    records[2][3211682].y=4;
    records[2][3211682].option=0;

    records[2][3384454].key=987893760;
    records[2][3384454].x=4;
    records[2][3384454].y=4;
    records[2][3384454].option=3;

    records[2][45649].key=164556800;
    records[2][45649].x=9;
    records[2][45649].y=4;
    records[2][45649].option=0;

    records[2][3973504].key=1896480767;
    records[2][3973504].x=6;
    records[2][3973504].y=1;
    records[2][3973504].option=5;

    records[2][3079622].key=219590656;
    records[2][3079622].x=6;
    records[2][3079622].y=8;
    records[2][3079622].option=4;

    records[2][3875347].key=368386048;
    records[2][3875347].x=7;
    records[2][3875347].y=8;
    records[2][3875347].option=0;

    records[2][3371436].key=435881984;
    records[2][3371436].x=4;
    records[2][3371436].y=5;
    records[2][3371436].option=3;

    records[2][755193].key=581265408;
    records[2][755193].x=8;
    records[2][755193].y=4;
    records[2][755193].option=0;

    records[2][2506995].key=239017984;
    records[2][2506995].x=7;
    records[2][2506995].y=4;
    records[2][2506995].option=0;

    records[2][1473331].key=493983744;
    records[2][1473331].x=6;
    records[2][1473331].y=4;
    records[2][1473331].option=3;

    records[2][1041306].key=425551872;
    records[2][1041306].x=7;
    records[2][1041306].y=4;
    records[2][1041306].option=0;

    records[2][381232].key=832890880;
    records[2][381232].x=6;
    records[2][381232].y=8;
    records[2][381232].option=1;

    records[2][3689275].key=1524197375;
    records[2][3689275].x=7;
    records[2][3689275].y=8;
    records[2][3689275].option=6;

    records[2][3055262].key=315566080;
    records[2][3055262].x=7;
    records[2][3055262].y=6;
    records[2][3055262].option=6;

	
	
    records[2][3537126].key=1600045055;
    records[2][3537126].x=6;
    records[2][3537126].y=7;
    records[2][3537126].option=7;

    records[2][3253607].key=1315762175;
    records[2][3253607].x=2;
    records[2][3253607].y=3;
    records[2][3253607].option=7;

    records[2][2935847].key=2083442687;
    records[2][2935847].x=6;
    records[2][2935847].y=8;
    records[2][2935847].option=6;

    records[2][3122064].key=195633152;
    records[2][3122064].x=3;
    records[2][3122064].y=4;
    records[2][3122064].option=6;

    records[2][3601717].key=952111104;
    records[2][3601717].x=7;
    records[2][3601717].y=8;
    records[2][3601717].option=5;

    records[2][3314594].key=1455822847;
    records[2][3314594].x=2;
    records[2][3314594].y=4;
    records[2][3314594].option=7;

    records[2][3225727].key=1667733503;
    records[2][3225727].x=6;
    records[2][3225727].y=6;
    records[2][3225727].option=3;

    records[2][2386943].key=1154895871;
    records[2][2386943].x=6;
    records[2][2386943].y=7;
    records[2][2386943].option=7;

    records[2][793241].key=197304320;
    records[2][793241].x=8;
    records[2][793241].y=2;
    records[2][793241].option=5;

    records[2][3899872].key=4411392;
    records[2][3899872].x=7;
    records[2][3899872].y=3;
    records[2][3899872].option=7;

    records[2][3009349].key=1755516927;
    records[2][3009349].x=7;
    records[2][3009349].y=8;
    records[2][3009349].option=6;

    records[2][514188].key=305025024;
    records[2][514188].x=6;
    records[2][514188].y=9;
    records[2][514188].option=6;

    records[2][2459353].key=1706967039;
    records[2][2459353].x=7;
    records[2][2459353].y=8;
    records[2][2459353].option=2;

    records[2][3788456].key=548298752;
    records[2][3788456].x=8;
    records[2][3788456].y=7;
    records[2][3788456].option=4;

    records[2][1043271].key=957552640;
    records[2][1043271].x=8;
    records[2][1043271].y=4;
    records[2][1043271].option=7;

    records[2][505428].key=1077014527;
    records[2][505428].x=9;
    records[2][505428].y=5;
    records[2][505428].option=0;

    records[2][509835].key=77021184;
    records[2][509835].x=7;
    records[2][509835].y=7;
    records[2][509835].option=2;

    records[2][3535543].key=896045056;
    records[2][3535543].x=8;
    records[2][3535543].y=6;
    records[2][3535543].option=4;

    records[2][2614386].key=523124736;
    records[2][2614386].x=6;
    records[2][2614386].y=8;
    records[2][2614386].option=2;

    records[2][777052].key=625287168;
    records[2][777052].x=4;
    records[2][777052].y=3;
    records[2][777052].option=5;

    records[2][1289348].key=1781796863;
    records[2][1289348].x=7;
    records[2][1289348].y=5;
    records[2][1289348].option=5;

    records[2][129655].key=1888636927;
    records[2][129655].x=3;
    records[2][129655].y=5;
    records[2][129655].option=7;

    records[2][1483887].key=2113990655;
    records[2][1483887].x=5;
    records[2][1483887].y=6;
    records[2][1483887].option=6;

    records[2][3508053].key=1308016639;
    records[2][3508053].x=3;
    records[2][3508053].y=4;
    records[2][3508053].option=6;

    records[2][3623368].key=1928130559;
    records[2][3623368].x=6;
    records[2][3623368].y=5;
    records[2][3623368].option=1;

    records[2][3535224].key=868044800;
    records[2][3535224].x=8;
    records[2][3535224].y=5;
    records[2][3535224].option=4;

    records[2][611769].key=1577119743;
    records[2][611769].x=2;
    records[2][611769].y=2;
    records[2][611769].option=7;

    records[2][1831883].key=1586339839;
    records[2][1831883].x=7;
    records[2][1831883].y=4;
    records[2][1831883].option=4;

    records[2][2341703].key=958851072;
    records[2][2341703].x=6;
    records[2][2341703].y=6;
    records[2][2341703].option=4;

    records[2][632159].key=285143040;
    records[2][632159].x=6;
    records[2][632159].y=7;
    records[2][632159].option=2;

    records[2][792487].key=1569300479;
    records[2][792487].x=6;
    records[2][792487].y=3;
    records[2][792487].option=2;

    records[2][1433521].key=1801940991;
    records[2][1433521].x=6;
    records[2][1433521].y=6;
    records[2][1433521].option=6;

    records[2][3344213].key=1307852799;
    records[2][3344213].x=6;
    records[2][3344213].y=2;
    records[2][3344213].option=3;

    records[2][932062].key=569442304;
    records[2][932062].x=3;
    records[2][932062].y=3;
    records[2][932062].option=5;

    records[2][379359].key=796889088;
    records[2][379359].x=4;
    records[2][379359].y=3;
    records[2][379359].option=5;

    records[2][72322].key=1324580863;
    records[2][72322].x=7;
    records[2][72322].y=5;
    records[2][72322].option=0;

    records[2][3447695].key=991956992;
    records[2][3447695].x=6;
    records[2][3447695].y=3;
    records[2][3447695].option=5;

    records[2][1024179].key=1233532927;
    records[2][1024179].x=5;
    records[2][1024179].y=4;
    records[2][1024179].option=5;

    records[2][3764158].key=444274688;
    records[2][3764158].x=6;
    records[2][3764158].y=4;
    records[2][3764158].option=0;

    records[2][155683].key=1168664575;
    records[2][155683].x=3;
    records[2][155683].y=5;
    records[2][155683].option=3;

    records[2][3637457].key=1932144639;
    records[2][3637457].x=3;
    records[2][3637457].y=6;
    records[2][3637457].option=7;

    records[2][510863].key=989020160;
    records[2][510863].x=4;
    records[2][510863].y=6;
    records[2][510863].option=7;

    records[2][460044].key=816969728;
    records[2][460044].x=2;
    records[2][460044].y=4;
    records[2][460044].option=4;

    records[2][1930004].key=594440192;
    records[2][1930004].x=5;
    records[2][1930004].y=6;
    records[2][1930004].option=7;

    records[2][820325].key=1881327615;
    records[2][820325].x=4;
    records[2][820325].y=5;
    records[2][820325].option=7;

    records[2][3074918].key=859584512;
    records[2][3074918].x=5;
    records[2][3074918].y=4;
    records[2][3074918].option=5;

    records[2][1753123].key=1170262015;
    records[2][1753123].x=5;
    records[2][1753123].y=7;
    records[2][1753123].option=7;

    records[2][172594].key=264683520;
    records[2][172594].x=4;
    records[2][172594].y=4;
    records[2][172594].option=5;

    records[2][2756985].key=71268352;
    records[2][2756985].x=6;
    records[2][2756985].y=7;
    records[2][2756985].option=5;

    records[2][2293313].key=614803456;
    records[2][2293313].x=6;
    records[2][2293313].y=6;
    records[2][2293313].option=3;

    records[2][3536429].key=152047616;
    records[2][3536429].x=5;
    records[2][3536429].y=8;
    records[2][3536429].option=7;

    records[2][3522028].key=1944029183;
    records[2][3522028].x=6;
    records[2][3522028].y=8;
    records[2][3522028].option=4;

    records[2][321468].key=1236830207;
    records[2][321468].x=6;
    records[2][321468].y=7;
    records[2][321468].option=6;

    records[2][1899233].key=230410240;
    records[2][1899233].x=5;
    records[2][1899233].y=6;
    records[2][1899233].option=6;

    records[2][1820133].key=118331392;
    records[2][1820133].x=9;
    records[2][1820133].y=2;
    records[2][1820133].option=5;

    records[2][2484670].key=1694992383;
    records[2][2484670].x=6;
    records[2][2484670].y=6;
    records[2][2484670].option=6;

    records[2][3483387].key=15994880;
    records[2][3483387].x=7;
    records[2][3483387].y=6;
    records[2][3483387].option=5;

    records[2][2039443].key=878548992;
    records[2][2039443].x=6;
    records[2][2039443].y=7;
    records[2][2039443].option=6;

    records[2][2772923].key=2035279871;
    records[2][2772923].x=5;
    records[2][2772923].y=3;
    records[2][2772923].option=1;

    records[2][3514209].key=744024064;
    records[2][3514209].x=6;
    records[2][3514209].y=3;
    records[2][3514209].option=6;

	
	
    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][2897800].key=1671405567;
    records[2][2897800].x=8;
    records[2][2897800].y=2;
    records[2][2897800].option=5;

    records[2][2775496].key=1927282687;
    records[2][2775496].x=6;
    records[2][2775496].y=6;
    records[2][2775496].option=2;

    records[2][1866444].key=562376704;
    records[2][1866444].x=2;
    records[2][1866444].y=3;
    records[2][1866444].option=1;

    records[2][3184150].key=1279692799;
    records[2][3184150].x=6;
    records[2][3184150].y=5;
    records[2][3184150].option=2;

    records[2][934766].key=1885442047;
    records[2][934766].x=9;
    records[2][934766].y=2;
    records[2][934766].option=4;

    records[2][2631995].key=271142912;
    records[2][2631995].x=3;
    records[2][2631995].y=4;
    records[2][2631995].option=7;

    records[2][2420248].key=482930688;
    records[2][2420248].x=3;
    records[2][2420248].y=3;
    records[2][2420248].option=1;

    records[2][2690553].key=583200768;
    records[2][2690553].x=8;
    records[2][2690553].y=1;
    records[2][2690553].option=5;

    records[2][3089211].key=1523597311;
    records[2][3089211].x=4;
    records[2][3089211].y=3;
    records[2][3089211].option=4;

    records[2][1147287].key=765657088;
    records[2][1147287].x=3;
    records[2][1147287].y=2;
    records[2][1147287].option=6;

    records[2][1801443].key=686311424;
    records[2][1801443].x=6;
    records[2][1801443].y=4;
    records[2][1801443].option=1;

    records[2][3442455].key=1507950591;
    records[2][3442455].x=7;
    records[2][3442455].y=4;
    records[2][3442455].option=7;

    records[2][968337].key=1673476095;
    records[2][968337].x=8;
    records[2][968337].y=5;
    records[2][968337].option=4;

    records[2][325029].key=1112834047;
    records[2][325029].x=7;
    records[2][325029].y=4;
    records[2][325029].option=0;

    records[2][1915370].key=234426368;
    records[2][1915370].x=4;
    records[2][1915370].y=5;
    records[2][1915370].option=2;

    records[2][2079292].key=498589696;
    records[2][2079292].x=4;
    records[2][2079292].y=4;
    records[2][2079292].option=6;

    records[2][2233157].key=1754740735;
    records[2][2233157].x=6;
    records[2][2233157].y=3;
    records[2][2233157].option=5;

    records[2][1449875].key=1105958911;
    records[2][1449875].x=5;
    records[2][1449875].y=3;
    records[2][1449875].option=5;

    records[2][2488583].key=702998528;
    records[2][2488583].x=4;
    records[2][2488583].y=4;
    records[2][2488583].option=3;

    records[2][2828283].key=1039337472;
    records[2][2828283].x=4;
    records[2][2828283].y=5;
    records[2][2828283].option=7;

    records[2][1714696].key=930224128;
    records[2][1714696].x=6;
    records[2][1714696].y=8;
    records[2][1714696].option=1;

    records[2][3672482].key=204183552;
    records[2][3672482].x=7;
    records[2][3672482].y=8;
    records[2][3672482].option=0;

    records[2][1714696].key=930224128;
    records[2][1714696].x=6;
    records[2][1714696].y=8;
    records[2][1714696].option=1;

    records[2][3672482].key=204183552;
    records[2][3672482].x=7;
    records[2][3672482].y=8;
    records[2][3672482].option=0;

    records[2][1711370].key=1614219263;
    records[2][1711370].x=7;
    records[2][1711370].y=2;
    records[2][1711370].option=1;

    records[2][2663384].key=227174400;
    records[2][2663384].x=6;
    records[2][2663384].y=8;
    records[2][2663384].option=4;

    records[2][1923219].key=2130429951;
    records[2][1923219].x=6;
    records[2][1923219].y=4;
    records[2][1923219].option=7;

    records[2][980596].key=977489920;
    records[2][980596].x=5;
    records[2][980596].y=7;
    records[2][980596].option=7;

    records[2][69340].key=112580608;
    records[2][69340].x=6;
    records[2][69340].y=8;
    records[2][69340].option=4;

    records[2][117594].key=1420625919;
    records[2][117594].x=5;
    records[2][117594].y=7;
    records[2][117594].option=4;

    records[2][3778916].key=404289536;
    records[2][3778916].x=6;
    records[2][3778916].y=6;
    records[2][3778916].option=5;

    records[2][2125779].key=110637056;
    records[2][2125779].x=5;
    records[2][2125779].y=7;
    records[2][2125779].option=5;

    records[2][3825441].key=1740333055;
    records[2][3825441].x=6;
    records[2][3825441].y=7;
    records[2][3825441].option=2;

    records[2][1361370].key=681871360;
    records[2][1361370].x=4;
    records[2][1361370].y=6;
    records[2][1361370].option=4;

    records[2][1259204].key=2037766143;
    records[2][1259204].x=8;
    records[2][1259204].y=2;
    records[2][1259204].option=5;

    records[2][3290419].key=1747798015;
    records[2][3290419].x=5;
    records[2][3290419].y=5;
    records[2][3290419].option=1;

    records[2][2075708].key=1750583295;
    records[2][2075708].x=6;
    records[2][2075708].y=4;
    records[2][2075708].option=5;

    records[2][2934191].key=1347442687;
    records[2][2934191].x=7;
    records[2][2934191].y=7;
    records[2][2934191].option=0;

    records[2][3170215].key=1571678207;
    records[2][3170215].x=3;
    records[2][3170215].y=5;
    records[2][3170215].option=6;

    records[2][732274].key=521242624;
    records[2][732274].x=6;
    records[2][732274].y=6;
    records[2][732274].option=0;

    records[2][2138609].key=2058645503;
    records[2][2138609].x=6;
    records[2][2138609].y=7;
    records[2][2138609].option=5;

    records[2][3815873].key=104327168;
    records[2][3815873].x=4;
    records[2][3815873].y=5;
    records[2][3815873].option=5;

    records[2][3856997].key=632367104;
    records[2][3856997].x=4;
    records[2][3856997].y=4;
    records[2][3856997].option=6;

    records[2][3151518].key=315662336;
    records[2][3151518].x=5;
    records[2][3151518].y=6;
    records[2][3151518].option=7;

    records[2][3864191].key=416374784;
    records[2][3864191].x=5;
    records[2][3864191].y=4;
    records[2][3864191].option=4;

    records[2][537519].key=93048832;
    records[2][537519].x=4;
    records[2][537519].y=3;
    records[2][537519].option=3;

    records[2][1469973].key=2073976831;
    records[2][1469973].x=3;
    records[2][1469973].y=6;
    records[2][1469973].option=0;

    records[2][2078013].key=1978585087;
    records[2][2078013].x=6;
    records[2][2078013].y=7;
    records[2][2078013].option=4;

    records[2][2894560].key=1027403776;
    records[2][2894560].x=6;
    records[2][2894560].y=5;
    records[2][2894560].option=3;

    records[2][406065].key=1060915200;
    records[2][406065].x=7;
    records[2][406065].y=6;
    records[2][406065].option=3;

    records[2][1463523].key=685973504;
    records[2][1463523].x=7;
    records[2][1463523].y=7;
    records[2][1463523].option=0;

    records[2][1219201].key=2121725951;
    records[2][1219201].x=6;
    records[2][1219201].y=6;
    records[2][1219201].option=4;

    records[2][1360967].key=1185869823;
    records[2][1360967].x=2;
    records[2][1360967].y=6;
    records[2][1360967].option=3;

    records[2][3929222].key=988438528;
    records[2][3929222].x=5;
    records[2][3929222].y=6;
    records[2][3929222].option=4;

    records[2][3135865].key=71647232;
    records[2][3135865].x=4;
    records[2][3135865].y=6;
    records[2][3135865].option=5;

    records[2][2350418].key=1646858239;
    records[2][2350418].x=5;
    records[2][2350418].y=4;
    records[2][2350418].option=2;

    records[2][3321667].key=47833088;
    records[2][3321667].x=5;
    records[2][3321667].y=3;
    records[2][3321667].option=7;

    records[2][402608].key=320913408;
    records[2][402608].x=6;
    records[2][402608].y=4;
    records[2][402608].option=3;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][894712].key=1605402623;
    records[2][894712].x=8;
    records[2][894712].y=2;
    records[2][894712].option=5;

    records[2][3827787].key=2100334591;
    records[2][3827787].x=7;
    records[2][3827787].y=3;
    records[2][3827787].option=0;

    records[2][2631982].key=1631139839;
    records[2][2631982].x=6;
    records[2][2631982].y=3;
    records[2][2631982].option=3;

    records[2][3041132].key=179552256;
    records[2][3041132].x=7;
    records[2][3041132].y=7;
    records[2][3041132].option=3;

    records[2][3283259].key=271794176;
    records[2][3283259].x=7;
    records[2][3283259].y=5;
    records[2][3283259].option=1;

    records[2][778009].key=709287936;
    records[2][778009].x=3;
    records[2][778009].y=4;
    records[2][778009].option=7;

    records[2][2149037].key=662659072;
    records[2][2149037].x=4;
    records[2][2149037].y=5;
    records[2][2149037].option=6;

    records[2][1277371].key=781787136;
    records[2][1277371].x=8;
    records[2][1277371].y=5;
    records[2][1277371].option=4;

    records[2][3770528].key=772280320;
    records[2][3770528].x=6;
    records[2][3770528].y=8;
    records[2][3770528].option=4;

    records[2][3752094].key=1568260095;
    records[2][3752094].x=7;
    records[2][3752094].y=4;
    records[2][3752094].option=7;

    records[2][2998833].key=1063507968;
    records[2][2998833].x=8;
    records[2][2998833].y=5;
    records[2][2998833].option=3;

    records[2][600870].key=1853108223;
    records[2][600870].x=6;
    records[2][600870].y=4;
    records[2][600870].option=3;

    records[2][3516008].key=1544024063;
    records[2][3516008].x=5;
    records[2][3516008].y=4;
    records[2][3516008].option=3;

    records[2][2352600].key=1478860799;
    records[2][2352600].x=2;
    records[2][2352600].y=3;
    records[2][2352600].option=7;

    records[2][3544765].key=216055808;
    records[2][3544765].x=3;
    records[2][3544765].y=4;
    records[2][3544765].option=7;

    records[2][3481205].key=183992320;
    records[2][3481205].x=4;
    records[2][3481205].y=5;
    records[2][3481205].option=5;

    records[2][2560847].key=1987067903;
    records[2][2560847].x=9;
    records[2][2560847].y=2;
    records[2][2560847].option=5;

    records[2][3228128].key=3739648;
    records[2][3228128].x=3;
    records[2][3228128].y=6;
    records[2][3228128].option=5;

    records[2][2429078].key=538939392;
    records[2][2429078].x=2;
    records[2][2429078].y=7;
    records[2][2429078].option=7;

    records[2][2968132].key=1527476223;
    records[2][2968132].x=7;
    records[2][2968132].y=5;
    records[2][2968132].option=3;

    records[2][2995178].key=1487503359;
    records[2][2995178].x=6;
    records[2][2995178].y=5;
    records[2][2995178].option=5;

    records[2][949835].key=845459456;
    records[2][949835].x=5;
    records[2][949835].y=5;
    records[2][949835].option=5;

    records[2][2544062].key=1695051775;
    records[2][2544062].x=4;
    records[2][2544062].y=6;
    records[2][2544062].option=7;

    records[2][2533637].key=1499041791;
    records[2][2533637].x=8;
    records[2][2533637].y=3;
    records[2][2533637].option=5;

    records[2][2539783].key=703049728;
    records[2][2539783].x=7;
    records[2][2539783].y=4;
    records[2][2539783].option=3;

    records[2][2203994].key=170715136;
    records[2][2203994].x=4;
    records[2][2203994].y=8;
    records[2][2203994].option=1;

    records[2][274013].key=2104780799;
    records[2][274013].x=5;
    records[2][274013].y=3;
    records[2][274013].option=7;

    records[2][2592329].key=1643100159;
    records[2][2592329].x=3;
    records[2][2592329].y=8;
    records[2][2592329].option=7;

    records[2][77560].key=1604585471;
    records[2][77560].x=4;
    records[2][77560].y=9;
    records[2][77560].option=1;

    records[2][1083993].key=1193592831;
    records[2][1083993].x=6;
    records[2][1083993].y=4;
    records[2][1083993].option=1;

    records[2][1677058].key=1838184447;
    records[2][1677058].x=5;
    records[2][1677058].y=9;
    records[2][1677058].option=7;

    records[2][2777248].key=771287040;
    records[2][2777248].x=7;
    records[2][2777248].y=4;
    records[2][2777248].option=7;

    records[2][3650077].key=600160256;
    records[2][3650077].x=8;
    records[2][3650077].y=5;
    records[2][3650077].option=7;

    records[2][629151].key=1793136639;
    records[2][629151].x=5;
    records[2][629151].y=8;
    records[2][629151].option=6;

    records[2][2864178].key=1519372287;
    records[2][2864178].x=6;
    records[2][2864178].y=5;
    records[2][2864178].option=7;

    records[2][3744138].key=2128250879;
    records[2][3744138].x=9;
    records[2][3744138].y=6;
    records[2][3744138].option=3;

    records[2][790204].key=1009299456;
    records[2][790204].x=6;
    records[2][790204].y=7;
    records[2][790204].option=1;

    records[2][3357663].key=2051864575;
    records[2][3357663].x=7;
    records[2][3357663].y=7;
    records[2][3357663].option=5;

    records[2][618811].key=269129728;
    records[2][618811].x=8;
    records[2][618811].y=7;
    records[2][618811].option=0;

    records[2][2293231].key=350803968;
    records[2][2293231].x=6;
    records[2][2293231].y=9;
    records[2][2293231].option=6;

    records[2][3023551].key=1923530751;
    records[2][3023551].x=5;
    records[2][3023551].y=6;
    records[2][3023551].option=2;

    records[2][1648152].key=482158592;
    records[2][1648152].x=7;
    records[2][1648152].y=9;
    records[2][1648152].option=1;

    records[2][2802679].key=127313920;
    records[2][2802679].x=6;
    records[2][2802679].y=10;
    records[2][2802679].option=4;

    records[2][912179].key=493422592;
    records[2][912179].x=6;
    records[2][912179].y=7;
    records[2][912179].option=4;

    records[2][181535].key=1280690175;
    records[2][181535].x=8;
    records[2][181535].y=9;
    records[2][181535].option=7;

    records[2][591824].key=1701099519;
    records[2][591824].x=9;
    records[2][591824].y=10;
    records[2][591824].option=6;

    records[2][1034460].key=1365542911;
    records[2][1034460].x=10;
    records[2][1034460].y=9;
    records[2][1034460].option=6;

    records[2][2635082].key=619145216;
    records[2][2635082].x=5;
    records[2][2635082].y=9;
    records[2][2635082].option=0;

    records[2][106873].key=68618240;
    records[2][106873].x=7;
    records[2][106873].y=8;
    records[2][106873].option=7;

    records[2][1203777].key=613713920;
    records[2][1203777].x=6;
    records[2][1203777].y=8;
    records[2][1203777].option=1;

    records[2][925020].key=1877432319;
    records[2][925020].x=7;
    records[2][925020].y=5;
    records[2][925020].option=1;

    records[2][2529348].key=275040256;
    records[2][2529348].x=7;
    records[2][2529348].y=8;
    records[2][2529348].option=1;

    records[2][3066788].key=659576832;
    records[2][3066788].x=8;
    records[2][3066788].y=9;
    records[2][3066788].option=1;

    records[2][163782].key=1468671999;
    records[2][163782].x=9;
    records[2][163782].y=9;
    records[2][163782].option=6;

    records[2][2193373].key=342704128;
    records[2][2193373].x=7;
    records[2][2193373].y=6;
    records[2][2193373].option=4;

    records[2][2497061].key=1627004927;
    records[2][2497061].x=8;
    records[2][2497061].y=5;
    records[2][2497061].option=6;

    records[2][2736800].key=2023243775;
    records[2][2736800].x=2;
    records[2][2736800].y=2;
    records[2][2736800].option=3;

    records[2][3122166].key=923631616;
    records[2][3122166].x=8;
    records[2][3122166].y=8;
    records[2][3122166].option=6;

    records[2][568463].key=1217077247;
    records[2][568463].x=6;
    records[2][568463].y=7;
    records[2][568463].option=1;

    records[2][2364510].key=58875904;
    records[2][2364510].x=2;
    records[2][2364510].y=4;
    records[2][2364510].option=1;

    records[2][882221].key=149393408;
    records[2][882221].x=6;
    records[2][882221].y=6;
    records[2][882221].option=2;

    records[2][2629471].key=1539137535;
    records[2][2629471].x=2;
    records[2][2629471].y=2;
    records[2][2629471].option=7;

    records[2][733210].key=937242624;
    records[2][733210].x=7;
    records[2][733210].y=7;
    records[2][733210].option=2;

    records[2][2814441].key=1031323648;
    records[2][2814441].x=3;
    records[2][2814441].y=3;
    records[2][2814441].option=6;

    records[2][3949414].key=860459008;
    records[2][3949414].x=7;
    records[2][3949414].y=6;
    records[2][3949414].option=3;

    records[2][3699041].key=1996206079;
    records[2][3699041].x=6;
    records[2][3699041].y=5;
    records[2][3699041].option=1;

    records[2][922543].key=93433856;
    records[2][922543].x=7;
    records[2][922543].y=5;
    records[2][922543].option=1;

    records[2][1975562].key=1614483455;
    records[2][1975562].x=6;
    records[2][1975562].y=8;
    records[2][1975562].option=2;

    records[2][673744].key=1701181439;
    records[2][673744].x=8;
    records[2][673744].y=2;
    records[2][673744].option=5;

    records[2][2316067].key=942825472;
    records[2][2316067].x=4;
    records[2][2316067].y=2;
    records[2][2316067].option=6;

    records[2][3682812].key=1496190975;
    records[2][3682812].x=7;
    records[2][3682812].y=6;
    records[2][3682812].option=1;

    records[2][2351542].key=666861568;
    records[2][2351542].x=2;
    records[2][2351542].y=3;
    records[2][2351542].option=6;

    records[2][2078829].key=406589440;
    records[2][2078829].x=6;
    records[2][2078829].y=2;
    records[2][2078829].option=2;

    records[2][1688605].key=1850195967;
    records[2][1688605].x=8;
    records[2][1688605].y=6;
    records[2][1688605].option=4;

    records[2][2205082].key=1678712831;
    records[2][2205082].x=3;
    records[2][2205082].y=2;
    records[2][2205082].option=4;

    records[2][346681].key=2088853503;
    records[2][346681].x=3;
    records[2][346681].y=4;
    records[2][346681].option=7;

    records[2][152049].key=2056658943;
    records[2][152049].x=4;
    records[2][152049].y=5;
    records[2][152049].option=3;

    records[2][3929964].key=180441088;
    records[2][3929964].x=7;
    records[2][3929964].y=5;
    records[2][3929964].option=3;

    records[2][1213048].key=1093722111;
    records[2][1213048].x=2;
    records[2][1213048].y=1;
    records[2][1213048].option=2;

    records[2][752134].key=1725259775;
    records[2][752134].x=6;
    records[2][752134].y=1;
    records[2][752134].option=2;

    records[2][1938772].key=850448384;
    records[2][1938772].x=6;
    records[2][1938772].y=0;
    records[2][1938772].option=0;

    records[2][2469369].key=582979584;
    records[2][2469369].x=2;
    records[2][2469369].y=0;
    records[2][2469369].option=1;

    records[2][852316].key=1621360191;
    records[2][852316].x=5;
    records[2][852316].y=0;
    records[2][852316].option=7;

    records[2][538450].key=649048512;
    records[2][538450].x=5;
    records[2][538450].y=1;
    records[2][538450].option=5;

    records[2][3042125].key=535552448;
    records[2][3042125].x=4;
    records[2][3042125].y=6;
    records[2][3042125].option=6;

    records[2][26313].key=1896533567;
    records[2][26313].x=7;
    records[2][26313].y=6;
    records[2][26313].option=0;

    records[2][1472276].key=849981888;
    records[2][1472276].x=3;
    records[2][1472276].y=0;
    records[2][1472276].option=5;

    records[2][935556].key=785445312;
    records[2][935556].x=5;
    records[2][935556].y=5;
    records[2][935556].option=3;

    records[2][1861396].key=850371008;
    records[2][1861396].x=9;
    records[2][1861396].y=2;
    records[2][1861396].option=4;

    records[2][3736099].key=176247232;
    records[2][3736099].x=7;
    records[2][3736099].y=3;
    records[2][3736099].option=5;

    records[2][1027850].key=617537984;
    records[2][1027850].x=5;
    records[2][1027850].y=6;
    records[2][1027850].option=4;

    records[2][3258305].key=1099767359;
    records[2][3258305].x=4;
    records[2][3258305].y=5;
    records[2][3258305].option=5;

    records[2][3673485].key=792183232;
    records[2][3673485].x=6;
    records[2][3673485].y=4;
    records[2][3673485].option=5;

    records[2][69973].key=308580800;
    records[2][69973].x=5;
    records[2][69973].y=5;
    records[2][69973].option=2;

    records[2][3091082].key=107602368;
    records[2][3091082].x=3;
    records[2][3091082].y=6;
    records[2][3091082].option=3;

    records[2][442173].key=980951488;
    records[2][442173].x=3;
    records[2][442173].y=7;
    records[2][442173].option=3;

    records[2][1838588].key=1238347327;
    records[2][1838588].x=3;
    records[2][1838588].y=8;
    records[2][1838588].option=7;

    records[2][2569621].key=1307078207;
    records[2][2569621].x=6;
    records[2][2569621].y=6;
    records[2][2569621].option=0;

    records[2][1861633].key=614371776;
    records[2][1861633].x=4;
    records[2][1861633].y=9;
    records[2][1861633].option=7;

    records[2][3518].key=1436511807;
    records[2][3518].x=5;
    records[2][3518].y=6;
    records[2][3518].option=3;

    records[2][757401].key=453267904;
    records[2][757401].x=5;
    records[2][757401].y=4;
    records[2][757401].option=7;

    records[2][3434466].key=715944384;
    records[2][3434466].x=5;
    records[2][3434466].y=10;
    records[2][3434466].option=4;

    records[2][1908191].key=1054417344;
    records[2][1908191].x=6;
    records[2][1908191].y=5;
    records[2][1908191].option=7;

    records[2][2972473].key=71483840;
    records[2][2972473].x=4;
    records[2][2972473].y=9;
    records[2][2972473].option=3;

    records[2][1031428].key=2037538367;
    records[2][1031428].x=8;
    records[2][1031428].y=1;
    records[2][1031428].option=3;

    records[2][2639060].key=595149248;
    records[2][2639060].x=8;
    records[2][2639060].y=5;
    records[2][2639060].option=1;

    records[2][2551613].key=983060928;
    records[2][2551613].x=7;
    records[2][2551613].y=6;
    records[2][2551613].option=1;

    records[2][277150].key=568787392;
    records[2][277150].x=8;
    records[2][277150].y=2;
    records[2][277150].option=5;

    records[2][1250722].key=457761216;
    records[2][1250722].x=4;
    records[2][1250722].y=10;
    records[2][1250722].option=2;

    records[2][1872313].key=1322380863;
    records[2][1872313].x=4;
    records[2][1872313].y=9;
    records[2][1872313].option=3;

    records[2][1250722].key=457761216;
    records[2][1250722].x=4;
    records[2][1250722].y=10;
    records[2][1250722].option=7;

    records[2][88216].key=1988595263;
    records[2][88216].x=5;
    records[2][88216].y=11;
    records[2][88216].option=4;

    records[2][1250722].key=457761216;
    records[2][1250722].x=4;
    records[2][1250722].y=10;
    records[2][1250722].option=7;

    records[2][2010465].key=1738518079;
    records[2][2010465].x=9;
    records[2][2010465].y=5;
    records[2][2010465].option=1;

    records[2][899935].key=1281408575;
    records[2][899935].x=5;
    records[2][899935].y=11;
    records[2][899935].option=1;

    records[2][2076094].key=1438584383;
    records[2][2076094].x=10;
    records[2][2076094].y=5;
    records[2][2076094].option=1;

    records[2][2550760].key=35062208;
    records[2][2550760].x=8;
    records[2][2550760].y=6;
    records[2][2550760].option=7;

    records[2][931529].key=1897438783;
    records[2][931529].x=5;
    records[2][931529].y=7;
    records[2][931529].option=1;


}

int main(int argc, char *argv[]) {
    load1();
    load2();
    loop();
    return 0;
}
