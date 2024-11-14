#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

class Game2048 {
private:
    vector<vector<int>> b;
    int size;
    bool moved;

    void addRandomTile() {
        vector<pair<int, int>> emptyCells;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (b[i][j] == 0) {
                    emptyCells.push_back({i, j});
                }
            }
        }
        if (!emptyCells.empty()) {
            int index = rand() % emptyCells.size();
            b[emptyCells[index].first][emptyCells[index].second] = (rand() % 10 == 0) ? 4 : 2;
        }
    }

    void rotateb() {
        vector<vector<int>> rotated(size, vector<int>(size, 0));
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                rotated[i][j] = b[size - j - 1][i];
            }
        }
        b = rotated;
    }

    void slideLeft() {
        for (int i = 0; i < size; i++) {
            vector<int> nr(size, 0);
            int a = 0;
            for (int j = 0; j < size; j++) {
                if (b[i][j] != 0) {
                    nr[a++] = b[i][j];
                }
            }
            for (int j = 0; j < size - 1; j++) {
                if (nr[j] == nr[j + 1] && nr[j] != 0) {
                    nr[j] *= 2;
                    nr[j + 1] = 0;
                    moved = true;
                }
            }
            a = 0;
            for (int j = 0; j < size; j++) {
                if (nr[j] != 0) {
                    b[i][a++] = nr[j];
                } else {
                    b[i][a++] = 0;
                }
            }
        }
    }

    bool canMove() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (b[i][j] == 0) return true;
                if (j < size - 1 && b[i][j] == b[i][j + 1]) return true;
                if (i < size - 1 && b[i][j] == b[i + 1][j]) return true;
            }
        }
        return false;
    }

public:
    Game2048(int s = 4) : size(s), b(s, vector<int>(s, 0)) {
        srand(static_cast<unsigned>(time(0)));
        addRandomTile();
        addRandomTile();
    }

    void displayb() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << setw(5) << b[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }

    void moveLeft() {
        moved = false;
        slideLeft();
        if (moved) {
            addRandomTile();
        }
    }

    void moveRight() {
        rotateb();
        rotateb();
        moveLeft();
        rotateb();
        rotateb();
    }

    void moveUp() {
        rotateb();
        rotateb();
        rotateb();
        moveLeft();
        rotateb();
    }

    void moveDown() {
        rotateb();
        moveLeft();
        rotateb();
        rotateb();
        rotateb();
    }

    bool isGameOver() {
        return !canMove();
    }
};

int main() {
    Game2048 game;
    char move;
    while (true) {
        game.displayb();
        if (game.isGameOver()) {
            cout << "Game Over!" << endl;
            break;
        }
        cout << "Enter move (w = up, s = down, a = left, d = right): ";
        cin >> move;
        switch (move) {
            case 'w': game.moveUp(); break;
            case 's': game.moveDown(); break;
            case 'a': game.moveLeft(); break;
            case 'd': game.moveRight(); break;
            default: cout << "Incid move!" << endl; break;
        }
    }
    return 0;
}
