#include <ncurses.h>

int main() {
    // Initialize ncurses
    initscr();            // Start ncurses mode
    cbreak();             // Disable line buffering, pass all input directly
    noecho();             // Don't echo user input
    curs_set(FALSE);      // Hide the cursor


    start_x = start_y = 20;

    /*mvprintw(0, 0, "test");*/
	WINDOW *my_win;
	int startx, starty, width, height;
   
	height = 3;
	width = 10;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	printw("Press F1 to exit");
	refresh();
	my_win = create_newwin(height, width, starty, startx);


    wrefresh(subwin);

    // Refresh the screen to show the message
    refresh();

    // Wait for user input
    getch();

    // End ncurses mode
    endwin();

    return 0;
}
