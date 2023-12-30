#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

#define ROWS 15
#define COLS 30


typedef struct
{
    int x, y;
    int vx, vy;
} Entity;


typedef struct
{
    int x, y;
    int score;
} Pacman;


typedef enum
{
    EMPTY,
    WALL,
    DOT,
    PACMAN,
    GHOST,
} CellType;


void initializeGame(CellType map[ROWS][COLS], Pacman *pacman, Entity ghosts[], int numGhosts)
{
    srand(time(NULL));

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            if (i == 0 || i == ROWS - 1 || j == 0 || j == COLS - 1)
                map[i][j] = WALL;
            else
                map[i][j] = rand() % 3 == 0 ? DOT : EMPTY;
        }
    }

    pacman->x = ROWS / 2;
    pacman->y = COLS / 2;
    pacman->score = 0;
    map[pacman->x][pacman->y] = PACMAN;

    for (int i = 0; i < numGhosts; i++)
    {
        ghosts[i].x = rand() % (ROWS - 2) + 1;
        ghosts[i].y = rand() % (COLS - 2) + 1;
        ghosts[i].vx = (rand() % 2 == 0) ? 1 : -1;
        ghosts[i].vy = (rand() % 2 == 0) ? 1 : -1;
        map[ghosts[i].x][ghosts[i].y] = GHOST;
    }
}


void printGame(CellType map[ROWS][COLS], Pacman *pacman)
{
    system("cls");

    printf("        Your Score: %d\n", pacman->score);

    for (int i = 0; i < ROWS; i++)
    {
        for (int j = 0; j < COLS; j++)
        {
            switch (map[i][j])
            {
                case EMPTY:
                    printf(" ");
                    break;
                case WALL:
                    printf("#");
                    break;
                case DOT:
                    printf(".");
                    break;
                case PACMAN:
                    printf("P");
                    break;
                case GHOST:
                    printf("G");
                    break;
                default:
                    break;
            }
        }
        printf("\n");
    }
}


void moveEntity(CellType map[ROWS][COLS], Entity *entity)
{
    map[entity->x][entity->y] = EMPTY;

    entity->x += entity->vx;
    entity->y += entity->vy;

    map[entity->x][entity->y] = (entity->vx || entity->vy) ? GHOST : DOT;
}


void movePacman(CellType map[ROWS][COLS], Pacman *pacman, char move)
{
    map[pacman->x][pacman->y] = EMPTY;

    switch (move)
    {
        case 'w':
            pacman->x--;
            break;
        case 'a':
            pacman->y--;
            break;
        case 's':
            pacman->x++;
            break;
        case 'd':
            pacman->y++;
            break;
        default:
            break;
    }

    if (map[pacman->x][pacman->y] == DOT)
        pacman->score += 10;

    map[pacman->x][pacman->y] = PACMAN;
}


void updateGame(CellType map[ROWS][COLS], Pacman *pacman, Entity ghosts[], int numGhosts)
{
    for (int i = 0; i < numGhosts; i++)
    {
        moveEntity(map, &ghosts[i]);
    }

    for (int i = 0; i < numGhosts; i++)
    {
        if (rand() % 2 == 0)
        {
            ghosts[i].vx = (rand() % 2 == 0) ? 1 : -1;
            ghosts[i].vy = 0;
        }
        else
        {
            ghosts[i].vy = (rand() % 2 == 0) ? 1 : -1;
            ghosts[i].vx = 0;
        }
    }


    for (int i = 0; i < numGhosts; i++)
    {
        if (pacman->x == ghosts[i].x && pacman->y == ghosts[i].y)
        {
            printf("Game Over! Your Score: %d\n", pacman->score);
            exit(0);
        }
    }

    if (pacman->score == (ROWS - 2) * (COLS - 2) * 10)
    {
        printf("Congratulations! You Win!\n");
        exit(0);
    }
}


int main()
{
    CellType map[ROWS][COLS];
    Pacman pacman;
    Entity ghosts[5];

    initializeGame(map, &pacman, ghosts, sizeof(ghosts) / sizeof(ghosts[0]));

    char move;
    do
    {
        printGame(map, &pacman);

        printf("      Keys For Movement\n");
        printf("              W\n\n");
        printf("         A");
        printf("          D\n\n");
        printf("              S\n\n\n\n");
        move = getch();

        movePacman(map, &pacman, move);
        updateGame(map, &pacman, ghosts, sizeof(ghosts) / sizeof(ghosts[0]));

    } while (move != 'q');

    return 0;
}
