#include <iostream>
#include <limits>
#include <string>

using namespace std;

enum Turn {
    PlayerOne,
    PlayerTwo
};

// UI
void show_intro();

void load_progress();

// Game
void get_turn(Turn *current);

bool make_move(int y, int x, char playerChar);

bool player_wins(char playerChar);

bool is_draw();

void get_position(int *input, char from);

void start_game();

void reset_game();

/**
 * Blank character used as no data or input
 */
const char blank = '_';

/**
 * Array of 3x3 to store player data
 */
char tic[3][3]{};

/**
 * Default global turn will which keep on alternating every new game.
 * 
 * Ex: 1st Game = PlayerOne
 *     2nd Game = PlayerTwo
 *     3rd Game = PlayerOne
 * 
 * So that game remain fair
 */
Turn def_turn = Turn::PlayerTwo;

int main() {
    start_game();
    return 0;
}

void flush_input() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void show_intro() {
    system("cls");
    cout << "Let's play Tic-Tac-Toe !\n\nHere is the layout...\n";

    // Show an empty guide array
    char alp[] = {'A', 'B', 'C'};
    for (int i = 0; i < 3; i++) {
        if (i == 0) {
            cout << "    1    2    3  \n";
        }
        cout << alp[i] << " ";
        for (int j = 0; j < 3; j++) {
            cout << "|_" << blank << "_|";
        }
        cout << endl;
    }

    cout << "\nIf you wish to place your move at the center, you must enter\n2b\nThis means your move will be placed "
         << "at 2 column B row.\n\nPlayer 1: X\nPlayer 2: O\n\nLet's play !!\n"
         << endl;

    load_progress();
}

/**
 * Load/show players' progress
 */
void load_progress() {
    for (auto &i : tic) {
        for (auto &j : i) cout << "|_" << j << "_|";
        cout << endl;
    }
    cout << endl;
}

void start_game() {
    // init empty tics (player data)
    for (auto &i : tic) {
        for (auto &j : i) j = blank;
    }

    // switch turns every new game
    get_turn(&def_turn);

    // First turn will be default turn
    Turn player_turn = def_turn;
    char player_char = blank;

    while (!player_wins(player_char)) {
    loop:
        show_intro();
        if (player_turn == Turn::PlayerOne) {
            player_char = 'X';
            cout << "Player One turn - X" << endl
                 << endl;
        }
        if (player_turn == Turn::PlayerTwo) {
            player_char = 'O';
            cout << "Player Two turn - O" << endl
                 << endl;
        }

    input:
        string in;
        cout << "Enter your move:";
        cin >> in;
        flush_input();
        if (in.length() > 2) {
            cout << "Invalid length" << endl
                 << endl;
            goto input;
        }
        if (!(in[0] >= '1' && in[0] <= '3')) {
            cout << "Number not in range" << endl
                 << endl;
            goto input;
        }
        if (!(in[1] >= 'a' && in[1] <= 'c') && !(in[1] >= 'A' && in[1] <= 'C')) {
            cout << "Character not in range" << endl
                 << endl;
            goto input;
        }
        int x = -1;
        int y = -1;
        get_position(&x, in[0]);
        get_position(&y, in[1]);

        if (!make_move(y, x, player_char)) {
            goto loop;
        }

        if (player_wins(player_char)) {
            show_intro();
            if (player_turn == Turn::PlayerOne) {
                cout << "Player One [X] WINS !" << endl
                     << endl;
            }
            if (player_turn == Turn::PlayerTwo) {
                cout << "Player Two [O] WINS !" << endl
                     << endl;
            }
            break;
        }

        if (is_draw()) {
            cout << "It's Draw !" << endl;
            break;
        }

        get_turn(&player_turn);
    }

    reset_game();
}

void reset_game() {
    string ans;
    cout << "Do you want to restart ? [y/n]:";
    cin >> ans;
    flush_input();

    if (ans[0] == 'n' || ans[0] == 'N') {
        exit(0);
    } else {
        start_game();
    }
}

bool make_move(int y, int x, char player_char) {
    if (tic[y][x] != blank) {
        return false;
    }
    tic[y][x] = player_char;
    return true;
}

void get_turn(Turn *current) {
    if (*current == Turn::PlayerTwo) {
        *current = Turn::PlayerOne;
    } else {
        *current = Turn::PlayerTwo;
    }
}

void get_index(int *in, const char arr[], char from) {
    for (int i = 0; i < 3; i++) {
        if (arr[i] == from) {
            *in = i;
            break;
        }
    }
}

void get_position(int *input, char from) {
    if (from >= 'a' && from <= 'c') {
        get_index(input, new char[3]{'a', 'b', 'c'}, from);
    } else if (from >= 'A' && from <= 'C') {
        get_index(input, new char[3]{'A', 'B', 'C'}, from);
    } else {
        get_index(input, new char[3]{'1', '2', '3'}, from);
    }
}

bool player_wins(char player_char) {
    if (player_char == blank) {
        return false;
    }

    /*
     * Layout:
     * [0, 0][0, 1][0, 2]
     * [1, 0][1, 1][1, 2]
     * [2, 0][2, 1][2, 2]
     */

    // Horizontal checks
    for (auto &i : tic) {
        if (i[0] == player_char && i[1] == player_char && i[2] == player_char) {
            return true;
        }
    }

    // Vertical checks
    for (int i = 0; i < 3; i++) {
        if (tic[0][i] == player_char && tic[1][i] == player_char && tic[2][i] == player_char) {
            return true;
        }
    }

    // Diagonals check
    return tic[0][0] == player_char && tic[1][1] == player_char && tic[2][2] == player_char ||
           tic[0][2] == player_char && tic[1][1] == player_char && tic[2][0] == player_char;
}

bool is_draw() {
    int count = 0;
    for (auto &i : tic) {
        for (char j : i) {
            if (j != blank) {
                count++;
            }
        }
    }
    return count == 9;
}
