#include <ncurses.h>
#include <sys/time.h>

/* In microseconds, equivalent to half a second */
#define TICK_DURATION 500000 
#define FIELD_ROWS 22
#define FIELD_COLS 12

/*
 * Tests if game tick has elapsed
 * Returns true if delta between t1 and t2 is equal to or longer than TICK_DURATION, false otherwise
 */
bool tick( struct timeval *t1, struct timeval *t2)
{
    long start = t1->tv_usec;
    long end = 0;
    int sec_delta = t2->tv_sec - t1->tv_sec;

    /* Add 'sec_delta' seconds to 'end' if t1 and t2 are not the same second */
    if( sec_delta != 0 )
        end += sec_delta * 1000000; 

    end += t2->tv_usec;

    if( (end - start) > TICK_DURATION)
        return true;
    else
        return false;
    
}

int main(int argc, char *argv[])
{
    char c;
    int x,y; /* to be used as loop indexes */
    /* tv_sec (seconds), */
    /* tv_usec (microseconds (1/1 000 000)) */
    struct timeval t,z; 
    char field[FIELD_COLS][FIELD_ROWS] = {  /* tetris game field */
        {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','.','*'},
        {'*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*','*'},
    };

    initscr();
    cbreak(); /* disables line buffering */
    noecho(); /* disable getch() echo */
    keypad(stdscr,TRUE); /* accept keyboard input */
    nodelay(stdscr,TRUE); /* disable getch() blocking */

    gettimeofday(&t,NULL);
    while(c != 'q')
    {
        c = getch();

        /* print field */
        for( x=0 ; x<FIELD_COLS ; x++ )
        {
            for( y=0 ; y<FIELD_ROWS ; y++ )
            {
                move(x,y);
                printw("%c",field[x][y]);
            }
        }

        /* capture key strokes */
        if(c != ERR)
            printw("do thing!\n");

        gettimeofday(&z,NULL);
        if( tick(&t,&z) )
        {
            gettimeofday(&t,NULL);
            /* printw("tick!\n"); */
            /* move piece down and check for complete lines */
        }

        refresh();
    }
    
    endwin();
    return 0;
}
