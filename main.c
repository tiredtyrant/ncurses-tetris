#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <sys/time.h>

enum bool
{
	false,
	true
};
// In microseconds, equivalent to half a second
#define TICK_DURATION 500000

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

typedef struct Coords {
    int x;
    int y;
} Coords;

typedef struct Piece {
    Coords position;
    char body[4][3];
} Piece;

int grid[GRID_WIDTH][GRID_HEIGHT];

/*
 * Tests if game tick has elapsed
 * Returns true if delta between t1 and t2 is equal to or longer than TICK_DURATION, false otherwise
 */
bool tick( struct timeval *t1, struct timeval *t2)
{
    long start = t1->tv_usec;
    long end = 0;
    int sec_delta = t2->tv_sec - t1->tv_sec;

    // Add 'sec_delta' seconds to 'end' if t1 and t2 are not the same second
    if( sec_delta != 0 )
        end += sec_delta * 1000000; 

    end += t2->tv_usec;

    if( (end - start) > TICK_DURATION)
        return true;
    else
        return false;
}

void drawgrid(Coords origin, Piece p)
{
    //draw grid
    for(int x = 0; x < GRID_WIDTH; x++)
    {
        for(int y = 0; y < GRID_HEIGHT; y++)
        {
            mvprintw(origin.y+y,origin.x+x,".");
        }
    }

    //then draw piece
    for(int x = 0; x < 4; x++)
    {
        for(int y = 0; y < 3; y++)
        {
            if(p.body[x][y])
                mvprintw(p.position.y+x, p.position.x+y, "O");
        }
    }
}

int main(int argc, char *argv[])
{
    //checks for version option from command line
    for(int i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            if( (argv[i][1] == '-' && strcmp(argv[i], "--version") == 0) || (argv[i][1] != '-' && strchr(argv[i], 'v')) )
            {
                printf("ncurses-tetris 0.1.0\n");
                return 0;
            }
        }
    }

    initscr();
    cbreak(); //disables line buffering
    noecho(); //disable getch() echo
    keypad(stdscr,TRUE); //accept keyboard input
    nodelay(stdscr,TRUE); // disable getch() blocking
    curs_set(0); //make cursor invisible

    int max_y = 0;
    int max_x = 0;

    //get window size
    getmaxyx(stdscr, max_y, max_x);

    //initialize grid
    memset(grid, 0, sizeof(int)*GRID_WIDTH*GRID_HEIGHT);

    Coords origin;
    origin.x = (max_x-GRID_WIDTH)/2;
    origin.y = (max_y-GRID_HEIGHT)/2;

    Coords spawn;
    spawn.x = max_x/2;
    spawn.y = origin.y;

    Piece p;
    p.position = spawn;
    //make L-piece
    memcpy(p.body, (char[4][3]) 
    {
        {0,0,0},
        {0,1,0},
        {0,1,0},
        {0,1,1}
    }, 4*3);

    int c;

    //tv_sec (seconds),
    //tv_usec (microseconds (1/1 000 000))
    struct timeval t,z; 

    gettimeofday(&t,NULL);
    while(c != 'q')
    {
        c = getch();

        //capture key strokes
        switch(c)
        {
            case KEY_LEFT:
                printw("left");
                break;
            case KEY_RIGHT:
                printw("right");
                break;
            case KEY_UP:
                printw("up");
                break;
            case KEY_DOWN:
                printw("down");
                break;
            default:
                break;
        }

        gettimeofday(&z,NULL);
        if( tick(&t,&z) )
        {
            gettimeofday(&t,NULL);
            drawgrid(origin, p);  //draw current grid state
            //move piece down and check for complete lines
        }

        refresh();
    }
    
    endwin();
    return 0;
}
