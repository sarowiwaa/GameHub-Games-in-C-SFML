#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

using namespace std;

const int SIZE = 3;
char board[SIZE][SIZE];  // Tic-tac-toe board

// Function to initialize the game board
void initializeBoard() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

// Function to print the game board
void printBoard() {
    cout << "-------------\n";
    for (int i = 0; i < SIZE; i++) {
        cout << "| ";
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j] << " | ";
        }
        cout << "\n-------------\n";
    }
}

// Function to check if there are any empty spaces left on the board
bool isMovesLeft() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                return true;
            }
        }
    }
    return false;
}

// Function to evaluate the board and return a score
int evaluate(char player, char opponent) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][0] != ' ') {
            if (board[i][0] == player) return 10;
            if (board[i][0] == opponent) return -10;
        }
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[0][i] != ' ') {
            if (board[0][i] == player) return 10;
            if (board[0][i] == opponent) return -10;
        }
    }

    // Check diagonals
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[0][0] != ' ') {
        if (board[0][0] == player) return 10;
        if (board[0][0] == opponent) return -10;
    }

    if (board[0][2] == board[1][1] && board[1][1] == board[2][0] && board[0][2] != ' ') {
        if (board[0][2] == player) return 10;
        if (board[0][2] == opponent) return -10;
    }

    return 0;  // No winner yet
}

// Minimax algorithm to determine the best move for AI
int minimax(bool isMax, char player, char opponent, int depth) {
    int score = evaluate(player, opponent);

    // If the game is won or lost, return the score
    if (score == 10) return score - depth;  // Prioritize faster wins
    if (score == -10) return score + depth;  // Prioritize slower losses

    // If no moves left and no winner, it's a draw
    if (!isMovesLeft()) return 0;

    if (isMax) {
        int best = numeric_limits<int>::min();
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = player;  // AI's move
                    best = max(best, minimax(false, player, opponent, depth + 1));
                    board[i][j] = ' ';  // Undo move
                }
            }
        }
        return best;
    } else {
        int best = numeric_limits<int>::max();
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = opponent;  // Human's move
                    best = min(best, minimax(true, player, opponent, depth + 1));
                    board[i][j] = ' ';  // Undo move
                }
            }
        }
        return best;
    }
}

// Function to find the best move using the Minimax algorithm
pair<int, int> findBestMove(char player, char opponent) {
    int bestVal = numeric_limits<int>::min();
pair<int, int> bestMove = make_pair(-1, -1);


    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = player;
                int moveVal = minimax(false, player, opponent, 0);
                board[i][j] = ' ';  // Undo move
                if (moveVal > bestVal) {
                    bestMove = {i, j};
                    bestVal = moveVal;
                }
            }
        }
    }

    return bestMove;
}

// Random AI move
pair<int, int> findRandomMove() {
    srand(time(0));
    int row, col;
    do {
        row = rand() % SIZE;
        col = rand() % SIZE;
    } while (board[row][col] != ' ');
    return {row, col};
}

// Function to check for winner
bool checkWin(char player) {
    int score = evaluate(player, player == 'X' ? 'O' : 'X');
    return score == 10 || score == -10;
}

// Main game function
int main() {
    initializeBoard();
    char player, opponent, currentPlayer;
    int choice;

    cout << "Welcome to Tic-Tac-Toe\n";
    cout << "Do you want to be X or O? ";
    cin >> player;
    opponent = (player == 'X') ? 'O' : 'X';

    cout << "Choose AI for the opponent:\n1. Minimax AI\n2. Random AI\n";
    cin >> choice;

    currentPlayer = 'X';  // 'X' always starts

    while (isMovesLeft()) {
        printBoard();
        if (currentPlayer == player) {
            // Player's move
            int row, col;
            cout << "Your turn. Enter row and column (0-2): ";
            cin >> row >> col;

            if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || board[row][col] != ' ') {
                cout << "Invalid move! Try again.\n";
                continue;
            }
            board[row][col] = player;
        } else {
            // AI's move
            pair<int, int> aiMove;
            if (choice == 1) {
                aiMove = findBestMove(opponent, player);  // Minimax AI
            } else {
                aiMove = findRandomMove();  // Random AI
            }
            board[aiMove.first][aiMove.second] = opponent;
            cout << "AI played at (" << aiMove.first << ", " << aiMove.second << ")\n";
        }

        // Check for winner
        if (checkWin(currentPlayer)) {
            printBoard();
            cout << currentPlayer << " wins!\n";
            break;
        }

        // Switch turns
        currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
    }

    if (!checkWin('X') && !checkWin('O')) {
        printBoard();
        cout << "It's a draw!\n";
    }

    return 0;
}
