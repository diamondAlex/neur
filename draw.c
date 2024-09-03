#include <stdio.h>
#include <ncurses.h>
#define height 10
#define width 20
const int start_x = 1;
const int start_y = 1;

int data[height][width] = {{0}};
/*int n[height*width] = {{0}};*/
/*int v[height*width] = {{0}};*/

void drawSpot(WINDOW* win, int y, int x){
    mvwprintw(win, y, x, "0");
    /*mvwprintw(win, y-1, x, "0");*/
    /*mvwprintw(win, y+1, x, "0");*/
    /*mvwprintw(win, y, x+1, "0");*/
    /*mvwprintw(win, y, x-1, "0");*/
    data[y][x] = 1;
    /*if(y-1 >=0)*/
        /*data[y-1][x] = 1;*/
    /*if(y+1 < height)*/
        /*data[y+1][x] = 1;*/
    /*if(x-1 >=0)*/
        /*data[y][x-1] = 1;*/
    /*if(x+1 < width)*/
        /*data[y][x+1] = 1;*/
}

void convert_to_bitmap(int i, char *tag){
    char name[1000];
    sprintf(name, "images/test%i_%s.img",i,tag);
    FILE *file = fopen(name, "w");
    char num[6];
    for(int i = 0;i<height;i++){
        for(int j = 0;j<width;j++){
            sprintf(num, "%i", data[i][j]);
            fwrite(num, sizeof num[0], 1, file);
        }
    }
}

void printArr(){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            mvprintw(20+i,20+j,"%i",data[i][j]); 
        }
    }
}

void clear_window(WINDOW *win){
    for(int i=0;i<height;i++){
        for(int j=0;j<width;j++){
            data[i][j] = 0;
        }
    }
    werase(win);
}


void display(char *tag){
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
    int i=0;
    while(1){
        box(win, 0, 0);
        printArr();
        wrefresh(win);

        ch = getch();
        if(ch == 'q'){
            break;
        }
        if(ch == 'd'){
            clear_window(win);
        }
        if(ch == 's'){
            convert_to_bitmap(i, tag);
            clear_window(win);

            i++;
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


int main(int argc, char *argv[]) {
    if(argc == 1){
        printf("forgot the number input");
        return 1;
    }
    display(argv[1]);

    return 0;
}
