#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

// Global variable to hold the original puzzle.
// This array will be filled once and then used to check which cells are original.
int original[9][9], grid_no;
bool isVisualized = false;

void read_problem(int grid[9][9]);
bool isValid(int grid[9][9], int r, int c, int num);
bool solve(int grid[9][9], int r, int c);
void print_grid(int grid[9][9], int cur_r, int cur_c);
void print_header();
void print_start();
void credits();

int main(void)
{
    // Adding background music in a loop.
    mciSendString("open \"music.mp3\" type mpegvideo alias mymusic", NULL, 0, NULL);
    mciSendString("play mymusic repeat", NULL, 0, NULL);

    print_start();

    int grid[9][9];
    int i, j, choice;

    // Read the problem from file.
    read_problem(grid);

    // Print the initial puzzle (without color coding).
top:
    system("cls");
    print_header();
    printf("\n\n************This is Your Problem #%d*******************\n", grid_no);

    print_grid(grid, -1, -1);

    printf("\n*************Menu:************\n");
    printf("\n You have Following Choices:\n");
    printf("1) Try Yourself!!!! \n2) See Solution Directly \n3) See Visualization of the Algorithm\n");
    printf("Enter a choice[1/2/3]: ");
    scanf("%d", &choice);

    switch (choice)
    {
    case 1:
        system("cls");
        // Reprint header art for consistency
        print_header();
        printf("\n\n*****TRY DOING IT BY YOURSELF*******\n");

        print_grid(grid, -1, -1);

        int c_1;
        printf("\nPress 1 to Giveup and Return To Menu!! Or Press 2 to Exit!!: ");
        scanf("%d", &c_1);
        if (c_1 == 1)
            goto top;
        else
        {
            credits();
        }
        break;
    case 2:
        system("cls");
        print_header();
        printf("\n\n*******YOUR SOLVED %d SODUKU IS************\n", grid_no);
        solve(grid, 0, 0);
        print_grid(grid, -1, -1);

        credits();
        break;
    case 3:
        // For visualization, the current cell is highlighted in yellow.
        isVisualized = true;
        solve(grid, 0, 0);
        credits();
        break;
    default:
        break;
    }

    // Pause before exiting.
    printf("\n\n\nPress any key to exit...");
    getch();
    return 0;
}

void read_problem(int grid[9][9])
{
    srand(time(0));
    int rand_problem_no = (rand() % 50); // Generate a random number between 0 and 49.
    grid_no = rand_problem_no;
    int i, j;
    FILE *file;
    file = fopen("sudoku.txt", "r");
    char line[15]; // Temporary string to store each line.

    if (file)
    {
        // Skip lines before the randomly chosen problem.
        for (i = 0; i < rand_problem_no * 10 + 1; i++)
        {
            fgets(line, sizeof(line), file);
        }
        // Read the 9 lines of the problem.
        for (i = 0; i < 9; i++)
        {
            fgets(line, sizeof(line), file);
            for (j = 0; j < 9; j++)
            {
                grid[i][j] = line[j] - '0';
            }
        }
        fclose(file);
    }
    else
    {
        printf("Error: could not open sudoku.txt\n");
        exit(EXIT_FAILURE);
    }
    // Copy the initial grid into the global 'original' array.
    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            original[i][j] = grid[i][j];
        }
    }
}

void print_grid(int grid[9][9], int cur_r, int cur_c)
{
    int i, j;
    if (isVisualized)
        system("cls"); // Clear the screen

    for (i = 0; i < 9; i++)
    {
        for (j = 0; j < 9; j++)
        {
            if (i == cur_r && j == cur_c)
            {
                // Highlight current cell in yellow
                printf("\033[1;33m%d\033[0m ", grid[i][j]);
            }
            else if (original[i][j] != 0)
            {
                // Original puzzle numbers in green
                printf("\033[1;32m%d\033[0m ", grid[i][j]);
            }
            else
            {
                // Solver-filled numbers in red
                printf("\033[1;31m%d\033[0m ", grid[i][j]);
            }
        }
        printf("\n");
    }
    if (isVisualized)
        Sleep(500); // Pause for visualization
}

bool isValid(int grid[9][9], int r, int c, int num)
{
    int i, j;
    for (i = 0; i < 9; i++)
    {
        if (grid[r][i] == num)
            return false;
    }
    for (i = 0; i < 9; i++)
    {
        if (grid[i][c] == num)
            return false;
    }
    int startRow = r - r % 3, startCol = c - c % 3;
    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 3; j++)
        {
            if (grid[i + startRow][j + startCol] == num)
                return false;
        }
    }
    return true;
}

bool solve(int grid[9][9], int r, int c)
{
    if (r == 9)
        return true;
    if (c == 9)
        return solve(grid, r + 1, 0);
    if (grid[r][c] != 0)
        return solve(grid, r, c + 1);

    for (int num = 1; num <= 9; num++)
    {
        if (isValid(grid, r, c, num))
        {
            grid[r][c] = num;
            if (isVisualized)
                print_grid(grid, r, c); // Visualize current assignment

            if (solve(grid, r, c + 1))
                return true;

            grid[r][c] = 0; // Backtrack
            if (isVisualized)
                print_grid(grid, r, c);
        }
    }
    return false;
}

void print_header()
{
    printf(" \033[36m_______  __   __  ______   _______  ___   _  __   __    _______  _______  ___      __   __  _______  ______   \n");
    printf("|       ||  | |  ||      | |       ||   | | ||  | |  |  |       ||       ||   |    |  | |  ||       ||    _ |  \n");
    printf("|  _____||  | |  ||  _    ||   _   ||   |_| ||  | |  |  |  _____||   _   ||   |    |  |_|  ||    ___||   | ||  \n");
    printf("| |_____ |  |_|  || | |   ||  | |  ||      _||  |_|  |  | |_____ |  | |  ||   |    |       ||   |___ |   |_||_ \n");
    printf("|_____  ||       || |_|   ||  |_|  ||     |_ |       |  |_____  ||  |_|  ||   |___ |       ||    ___||    __  | \n");
    printf(" _____| ||       ||       ||       ||    _  ||       |   _____| ||       ||       | |     | |   |___ |   |  | | \n");
    printf("|_______||_______||______| |_______||___| |_||_______|  |_______||_______||_______|  |___|  |_______||___|  |_|\033[0m\n");
}

void print_start()
{
    printf(" _     _  _______  ___      _______  _______  __   __  _______    _______  _______    _______  __   __  _______     _______  _______  __   __  _______  __  \n");
    printf("| | _ | ||       ||   |    |       ||       ||  |_|  ||       |  |       ||       |  |       ||  | |  ||       |   |       ||   _   ||  |_|  ||       ||  | \n");
    printf("| || || ||    ___||   |    |       ||   _   ||       ||    ___|  |_     _||   _   |  |_     _||  |_|  ||    ___|   |    ___||  |_|  ||       ||    ___||  | \n");
    printf("|       ||   |___ |   |    |       ||  | |  ||       ||   |___     |   |  |  | |  |    |   |  |       ||   |___    |   | __ |       ||       ||   |___ |  | \n");
    printf("|       ||    ___||   |___ |      _||  |_|  ||       ||    ___|    |   |  |  |_|  |    |   |  |       ||    ___|   |   ||  ||       ||       ||    ___||__| \n");
    printf("|   _   ||   |___ |       ||     |_ |       || ||_|| ||   |___     |   |  |       |    |   |  |   _   ||   |___    |   |_| ||   _   || ||_|| ||   |___  __  \n");
    printf("|__| |__||_______||_______||_______||_______||_|   |_||_______|    |___|  |_______|    |___|  |__| |__||_______|   |_______||__| |__||_|   |_||_______||__| \n");
    printf("\n\n\n*********PRESS ANY KEY to START THE GAME!!!!***************\n");
    getch();
    system("cls");

    // Header text (ASCII art)
    print_header();
    printf("\033[1;31m______    __   __  ___      _______  _______  __   __   __  \n");
    printf("|    _ |  |  | |  ||   |    |       ||       ||  | |  | |  | \n");
    printf("|   | ||  |  | |  ||   |    |    ___||  _____||  | |  | |  | \n");
    printf("|   |_||_ |  |_|  ||   |    |   |___ | |_____ |  | |  | |  | \n");
    printf("|    __  ||       ||   |___ |    ___||_____  ||__| |__| |__| \n");
    printf("|   |  | ||       ||       ||   |___  _____| | __   __   __  \n");
    printf("|___|  |_||_______||_______||_______||_______||__| |__| |__| \033[0m \n\n");

    printf("1. Grid Structure:\n");
    printf("   - Sudoku consists of a 9x9 grid, divided into nine 3x3 subgrids.\n\n");

    printf("2. Number Placement:\n");
    printf("   - The goal is to fill the entire grid with numbers from 1 to 9.\n\n");

    printf("3. Row Rule:\n");
    printf("   - Each row must contain the numbers 1 to 9, without repetition.\n\n");

    printf("4. Column Rule:\n");
    printf("   - Each column must also contain the numbers 1 to 9, without repetition.\n\n");

    printf("5. Subgrid (Box) Rule:\n");
    printf("   - Each of the nine 3x3 subgrids must contain the numbers 1 to 9, without repetition.\n\n");

    printf("6. Starting Clues:\n");
    printf("   - Some numbers are pre-filled in the grid as clues.\n");
    printf("   - These numbers cannot be changed.\n\n");

    printf("7. Logical Deduction:\n");
    printf("   - The puzzle is solved using logic; no guessing is required.\n\n");

    printf("8. Uniqueness:\n");
    printf("   - A valid Sudoku puzzle has only one unique solution.\n\n");

    printf("\n\n\n*********PRESS ANY KEY to CONTINUE!!!!***************\n");
    getch();
    system("cls");
}

void credits()
{
    // exiting the program
    printf("\n\n\tThanks for playing Sudoku Solver\n");
    Sleep(1000);
    printf("\tCreated with <3 by:\n");
    Sleep(1000);
    printf("\n\tSUMIRAN SUBEDI\t\t(081BEI043)\n");
    Sleep(1000);
    printf("\tPRASHIL LAMICHHANE \t(081BEI024)\n");
    Sleep(1000);
    printf("\tDHIRAJ KUNWAR\t\t(081BEI015)\n");
    Sleep(1000);
    printf("\tSAURAB ADHIKARI\t\t(081BEI037)\n");
    Sleep(3000);
}