#include <ncurses.h>
#include <sys/time.h>

// In microseconds, equivalent to half a second
#define TICK_DURATION 500000

#define GRID_WIDTH 10
#define GRID_HEIGHT 20

typedef struct coords {
    int x;
    int y;
} coords;

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

void drawgrid(coords origin)
{
    for(int x = 0; x < GRID_WIDTH; x++)
    {
        for(int y = 0; y < GRID_HEIGHT; y++)
            mvprintw(origin.y+y,origin.x+x,".");
    }
}

int main(int argc, char *argv[])
{
    initscr();
    cbreak(); //disables line buffering
    noecho(); //disable getch() echo
    keypad(stdscr,TRUE); //accept keyboard input
    nodelay(stdscr,TRUE); // disable getch() blocking
    curs_set(0); //make cursor invisible

    int max_y = 0;
    int max_x = 0;

    getmaxyx(stdscr, max_y, max_x);

    coords origin;
    origin.x = (max_x-GRID_WIDTH)/2;
    origin.y = (max_y-GRID_HEIGHT)/2;

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
            drawgrid(origin);  //draw current grid state
            printw("tick!\n");
            //move piece down and check for complete lines
        }

        refresh();
    }
    
    endwin();
    return 0;
}
