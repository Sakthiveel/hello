#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>     // read(), usleep(), STDIN_FILENO
#include <termios.h>    // struct termios, tcgetattr, tcsetattr
#include <fcntl.h>      // fcntl(), F_SETFL, O_NONBLOCK
#include <stdlib.h>     // atexit()

struct termios orig_termios;

  struct GameMechanics
    {
        int gameCells[4][4];
        int rowsCount;
        int columnsCount;
    };

    struct Position {
        int row;
        int column;
    };
// 1. Restore normal terminal settings when the game exits
void disableRawMode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf("\033[?25h"); // Show the blinking cursor again
}

// 2. Switch terminal into real-time game mode
void enableRawMode() {
    tcgetattr(STDIN_FILENO, &orig_termios); // Save original settings
    atexit(disableRawMode);                 // Guarantee cleanup on exit

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);        // Turn off echo & Enter-buffering
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // Make keyboard reading non-blocking (don't freeze the game loop)
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    printf("\033[?25l"); // Hide the blinking cursor for a cleaner look
}

void showGameCells (struct GameMechanics gameMechanics) {
    for (int row = 0; row < gameMechanics.rowsCount;row++) {
        for (int col = 0; col < gameMechanics.columnsCount;col++) {
            printf("%d ",gameMechanics.gameCells[row][col]);
        }
        printf("\n");
    }
}

int main() {
    enableRawMode();

    int x = 10;// Player horizontal position
    char ch;
    printf("Game Started\n");
  
    

    struct GameMechanics gameMechanics = {
        .gameCells = { {0,0,0}, {0,0,0}, {0,0,0}},
        .rowsCount = 4, // todo: should we getting the size dynamically.
        .columnsCount = 4
    };

    struct Position position = {
        .row = 1,
        .column = 2
    };

    gameMechanics[position.row][position.column] = 1; // why the fuck this is not working

    showGameCells(gameMechanics);
    return 1;
  
    while (1) {
        // 3. Check if a key was pressed (returns 1 if a byte was read)
        if (read(STDIN_FILENO, &ch, 1) == 1) {
            if (ch == 'q') break;          // Quit game
            if (ch == 'a' && x > 0) x--;   // Move left
            if (ch == 'd' && x < 30) x++;  // Move right


        }
        printf("\033[2J");
        // 4. Draw the game frame
       // printf("\r");                      // Jump back to start of the line
        for(int i=0;i<3;i++) {
            printf("\nd"); 
            for (int i = 0; i < 50; i++) {
            if (i == x) printf("@");       // Draw player
            else printf(".");              // Draw empty track
        }
        }
        fflush(stdout);                    // Force terminal to draw immediately

        usleep(50000);                     // Sleep 50ms (~20 FPS) so CPU doesn't max out
    }

    printf("\nGame Over!\n");
    return 0;
}
