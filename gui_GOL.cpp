#include <GL/freeglut.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <ctime>
#include <vector>
using namespace std;

#define SIZE 140
#define FPS 40    

vector<vector<int>> curr(SIZE + 1, vector<int>(SIZE + 1, 0));
vector<vector<int>> nxt(SIZE + 1, vector<int>(SIZE + 1, 0));
vector<pair<int, int>> moves = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 1}};

// RULES
// Rule 1: Any live cell with fewer than two live neighbors dies (underpopulation)
// Rule 2: Any live cell with two or three live neighbors lives on to the nxt generation
// Rule 3: Any live cell with more than three live neighbors dies (overpopulation)
// Rule 4: Any dead cell with exactly three live neighbors becomes a live cell (reproduction)

bool safe(int i, int j) {
    if (i > 0 && j > 0 && i < SIZE && j < SIZE) {
        return true;
    }
    return false;
}

bool checkalive(int i, int j) {
    int count = 0;

    for (auto &move: moves) {
        int nx = i + move.first;
        int ny = j + move.second;
        if (safe(nx, ny)) {
            if (curr[nx][ny] == 1) {
                count++;
            }
        }
    }
    if (curr[i][j] == 1 && count < 2) {
        return false;
    }
    if (curr[i][j] == 1 && (count == 2 || count == 3)) {
        return true;
    }
    if (count > 3) {
        return false;
    }
    if (curr[i][j] == 0 && count == 3) {
        return true;
    }
    return false;
}

void square(int x, int y, char R, char G, char B) {
    glColor3ub(R, G, B); 
    glRectd(x, y, x + 1, y + 1);  
}

void draw() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (nxt[i][j] == 1) {
                square(i, j, 255, 255, 255); 
            } else {
                square(i, j, 0, 0, 0); 
            }
        }
    }
}

void updateMatrix() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (checkalive(i, j)) {
                nxt[i][j] = 1;
            } else {
                nxt[i][j] = 0;
            }
        }
    }
    curr = nxt; 
}

void display() {
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    draw(); 
    glutSwapBuffers();
}

void timer_func(int) {
    updateMatrix();  
    glutPostRedisplay();
    glutTimerFunc(1000 / FPS, timer_func, 0); 
}

void reshape_callback(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, SIZE, 0.0, SIZE, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

void initGrid() {
    srand(time(0));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            curr[i][j] = (rand() % 10 == 0) ? 1 : 0;  
        }
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Game of Life with OpenGL");
    initGrid();
    glutDisplayFunc(display);  
    glutReshapeFunc(reshape_callback); 
    glutTimerFunc(0, timer_func, 0);
    glutMainLoop();
    return 0;
}
