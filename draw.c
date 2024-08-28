#include <ncurses.h>
#define height 20
#define width 40
const int start_x = 1;
const int start_y = 1;

int data[height][width] = {{0}};
int n[height*width] = {{0}};
int v[height*width] = {{0}};

void drawSpot(WINDOW* win, int y, int x){
    mvwprintw(win, y, x, "0");
    mvwprintw(win, y-1, x, "0");
    mvwprintw(win, y+1, x, "0");
    mvwprintw(win, y, x+1, "0");
    mvwprintw(win, y, x-1, "0");
    data[y][x] = 1;
    if(y-1 >=0)
        data[y-1][x] = 1;
    if(y+1 < height)
        data[y+1][x] = 1;
    if(x-1 >=0)
        data[y][x-1] = 1;
    if(x+1 < width)
        data[y][x+1] = 1;
}

void printArr(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            mvprintw(20+i,20+j,"%i",data[i][j]); 
        }
    }
}

void display(){
    // Initialize ncurses
    initscr();            // Start ncurses mode
    raw();             // Disable line buffering, pass all input directly
    noecho();             // Don't echo user input
    curs_set(FALSE);      // Hide the cursor
    keypad(stdscr, TRUE);
    mousemask(BUTTON1_PRESSED,NULL);


    WINDOW* win = newwin(height, width, start_y, start_x);

    refresh();
    wrefresh(win);

    MEVENT event;
    int ch;
    int pressed = 0;
    while(1){
        box(win, 0, 0);
        printArr();
        wrefresh(win);

        ch = getch();
        if(ch == 'q'){
            break;
        }
        if(ch == 'd'){
            werase(win);
        }
        if(ch == KEY_MOUSE){
            getmouse(&event);
            if(event.bstate & BUTTON1_PRESSED){
                int x = event.x;
                int y = event.y;
                if(x>start_x && x<(width + start_x) 
                    && y>start_y && y<(height+start_y)){
                    drawSpot(win,y-start_y,x-start_x);
                }
            }
        }
        wrefresh(win);
    }

    endwin();
}


int main() {
    /*display();*/

    return 0;
}
