#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "board.h"
#include "board.c"

#define MAX_CHARNAME 200
#define N_PLAYER 3
#define MAX_DIE 6

#define PLAYERSTATUS_LIVE 0
#define PLAYERSTATUS_DIE 1
#define PLAYERSTATUS_END 2

int player_position[N_PLAYER];
char player_name[N_PLAYER][MAX_CHARNAME];
int player_coin[N_PLAYER];
int player_status[N_PLAYER]; // 0 - live, 1 - die, 2 - end
char player_statusString[3][MAX_CHARNAME] = {"LIVE", "DIE", "END"};

void opening(void)
{
    printf("----------------------------------------\n");
    printf("               GAME START               \n");
    printf("----------------------------------------\n");
}

int rolldie(void)
{
    return rand()%MAX_DIE+1;
}

void printPlayerPosition(int player)
{
    int i;
    for(i = 0; i < N_BOARD; i++)
    {
        printf("|");
        if(player_position[player] == i)
            printf("%c", player_name[player][0]);
        else
        {
            if (board_getBoardStatus(i) == BOARDSTATUS_NOK)
                printf("X");
            else 
                printf(" ");
        }
    }
    printf("|\n");
}

void printPlayerStatus(void)
{
    int i;
    printf("\n");
    printf("-------------player status-------------\n");
    for (i = 0; i < N_PLAYER; i++)
    {
        printf("%s : pos %i, coin %i, status %s\n", player_name[i], player_position[i], player_coin[i], player_statusString[player_status[i]]);
    }
    printf("----------------------------------------\n");
}

void checkDie(void)
{
    int i;
    for (i = 0; i < N_PLAYER; i++)
    {
        if (board_getBoardStatus(player_position[i]) == BOARDSTATUS_NOK)
        {
            player_status[i] = PLAYERSTATUS_DIE;
            printf("Player is die.\n");
        }    
    }
}

int main(void){
    int pos = 0;
    int turn = 0;
    int i;
    int flag = 0;

    srand((unsigned)time(NULL));

    // 0. opening
    opening();

    // 1-1. board init
    board_initBoard();

    // 1-2. player init
    for (i = 0; i < N_PLAYER; i++)
    {
        player_position[i] = 0;
        player_coin[i] = 0;
        player_status[i] = PLAYERSTATUS_LIVE;

        //player name
        printf("Player %i' name: ", i);
        scanf("%s", player_name[i]);
    }

    // 2. game loop
    do{
        int step;
        int coinResult;
        char c;

        if (player_status[turn] == PLAYERSTATUS_END || player_status[turn] == PLAYERSTATUS_DIE)
        {
            turn = (turn + 1)%N_PLAYER;
            flag += 1;
            continue;
        }

        // 2-1. status printing
        board_printBoardStatus();
        for (i = 0; i < N_PLAYER; i++)
            printPlayerPosition(i);
        printPlayerStatus();

        // 2-2. roll die
        printf("%s's turn!!", player_name[turn]);
        printf("press any key to continue");
        scanf("%d", &c);
        fflush(stdin);
        step = rolldie();

        // 2-3. moving
        player_position[turn] += step;
        if (player_position[turn] >= N_BOARD)
            player_position[turn] = N_BOARD - 1;
        
        if (player_position[turn] == N_BOARD -1)
            player_status[turn] = PLAYERSTATUS_END;
        
        printf("die's number is %d\n", step);

        // 2-4. get coin
        coinResult = board_getBoardCoin(pos);
        player_coin[turn] += coinResult;

        printf("get %d coins\n", coinResult);

        // 2-5. next turn
        turn = (turn + 1) % N_PLAYER; //wrap around

        // 2-6.
        // shark
        if (turn == 0)
        {
            int shark_pos = board_stepShark();
            printf("Shark moved to %i\n", shark_pos);
            checkDie();
        }

    } while (flag < 4);
    
    // 3. print end (winner)
    int winner = -1; // -1 indicates no winner
    
    // Check if any player has reached the end
    for (i = 0; i < N_PLAYER; i++) {
        if (player_status[i] == PLAYERSTATUS_END) {
        winner = i;
        break;
        }
    }
    // If no one has reached the end, find the player with the highest coins
    if (winner == -1) {
        int maxCoins = -1;
        for (i = 0; i < N_PLAYER; i++) {
            if (player_coin[i] > maxCoins) {
            maxCoins = player_coin[i];
            winner = i;
            }
        }
    }

    // Print the winner
    printf("\n");
    printf("----------------------------------------\n");
    printf("               GAME OVER               \n");
    printf("----------------------------------------\n");

    if (winner != -1) {
        printf("Winner: %s (Coins: %d)\n", player_name[winner], player_coin[winner]);
    } else {
        printf("No winner. All players are dead.\n");
    }

    return 0;
}