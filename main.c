#define _DEFAULT_SOURCE
#include <stdio.h>
#include <unistd.h>  // read(), usleep(), STDIN_FILENO
#include <termios.h> // struct termios, tcgetattr, tcsetattr
#include <fcntl.h>   // fcntl(), F_SETFL, O_NONBLOCK
#include <stdlib.h>  // atexit()

struct termios orig_termios;

struct GameMechanics
{
    int gameCells[5][5];
    int rowsCount; // these needs to be const, immutable once initialized.s
    int columnsCount;
};

struct Position
{
    int row;
    int column;
};
// 1. Restore normal terminal settings when the game exits
void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
    printf("\033[?25h"); // Show the blinking cursor again
}

// 2. Switch terminal into real-time game mode
void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &orig_termios); // Save original settings
    atexit(disableRawMode);                 // Guarantee cleanup on exit

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Turn off echo & Enter-buffering
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // Make keyboard reading non-blocking (don't freeze the game loop)
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    printf("\033[?25l"); // Hide the blinking cursor for a cleaner look
}

void showGameCells(struct GameMechanics gameMechanics)
{
    for (int row = 0; row < gameMechanics.rowsCount; row++)
    {
        for (int col = 0; col < gameMechanics.columnsCount; col++)
        {
            printf("%d ", gameMechanics.gameCells[row][col]);
        }
        printf("\n");
    }
}

int main()
{
    enableRawMode();

    int x = 10; // Player horizontal position
    char ch;
    printf("Game Started\n");

    struct GameMechanics gameMechanics = {
        .gameCells = {{0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}},
        .rowsCount = 5, // todo: should we getting the size dynamically.
        .columnsCount = 5};

    struct Position position = {
        .row = 1,
        .column = 2};

    gameMechanics.gameCells[position.row - 1][position.column - 1] = 1; // why the fuck this is not working

    // showGameCells(gameMechanics);

    while (1)
    {
        // if (position.column < gameMechanics.columnsCount)
        // {
        //     gameMechanics.gameCells[position.row - 1][position.column - 1] = 0;
        //     gameMechanics.gameCells[position.row - 1][position.column] = 1;
        //     position.column++;
        // }
        // 3. Check if a key was pressed (returns 1 if a byte was read)
        if (read(STDIN_FILENO, &ch, 1) == 1)
        {
            if (ch == 'q')
                break; // Quit game
            if (ch == 'w')
            {
                if (position.row > 1)
                {
                    gameMechanics.gameCells[position.row - 1][position.column - 1] = 0;
                    gameMechanics.gameCells[position.row - 2][position.column - 1] = 1;
                    position.row--;
                }
            }
            if (ch == 's')
            {
                if (position.row < gameMechanics.rowsCount)
                {
                    gameMechanics.gameCells[position.row - 1][position.column - 1] = 0;
                    gameMechanics.gameCells[position.row][position.column - 1] = 1;
                    position.row++;
                }
            }
            if (ch == 'a')
            {
                if (position.column > 1)
                {
                    gameMechanics.gameCells[position.row - 1][position.column - 1] = 0;
                    gameMechanics.gameCells[position.row - 1][position.column - 2] = 1;
                    position.column--;
                }
            } // Move left
            if (ch == 'd')
            {
                if (position.column < gameMechanics.columnsCount)
                {
                    gameMechanics.gameCells[position.row - 1][position.column - 1] = 0;
                    gameMechanics.gameCells[position.row - 1][position.column] = 1;
                    position.column++;
                }
            } // Move right
        }
        printf("\033[2J");
        // 4. Draw the game frame
        showGameCells(gameMechanics);

        fflush(stdout); // Force terminal to draw immediately

        usleep(50000); // Sleep 50ms (~20 FPS) so CPU doesn't max out
    }

    printf("\nGame Over!\n");
    return 0;
}
