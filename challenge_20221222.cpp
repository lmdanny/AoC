/*
 *  CHALLENGE 2022-12-22 (Little late upload, having COVID f**)
 *  Puzzle source: https://adventofcode.com/
 *  Solution: Danny van der Meeren <danny@illogic.nl>
 *  License: Free to use for any non-commercial and educational purposes without limitations
 */

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum Dir {
    Right = 0, Down, Left, Up
};

struct Node
{
    char c;

    Node *up;
    Node *down;
    Node *left;
    Node *right;

    int x, y;
    
    int mhDistance (Node *other) {
        return ( abs(other->x - x) + abs(other->y - y) );
    }

    Node *move (Dir &dir, int steps, Node *prev=NULL) {
        
        steps--;

        if (c == '#') {
            return prev;
        }            

        /* Change rotation..? */
        if( prev != NULL && mhDistance(prev) > 1) {
            if ( up == prev ) dir = Down;
            else if (down == prev) dir = Up;
            else if (left == prev) dir = Right;
            else if (right == prev) dir = Left;                
        }

        c = ">v<^"[dir]; /* trace */

        if (steps < 0)
            return this;

        switch (dir) {
            case Left:
                return left->move(dir, steps, this);
            case Right:
                return right->move(dir, steps, this);
            case Up:
                return up->move(dir, steps, this);
            case Down:
                return down->move(dir, steps, this);
        }

        return NULL;
    }
};

#define ROWS 201
#define COLS 151

Node map[ROWS][COLS];

void printMap () {
    for (int y=0; y<ROWS; y++) {
        cout << "[";
        for (int x=0; x<COLS; x++) {
            cout << map[y][x].c;
        }
        cout << "]" << endl; 
    }
    cout << endl;
}

struct Humanoid {
    Dir dir;  
    Node *current;

    void startJourney () {
        int x;
        dir = Right;

        for ( x=0; x<COLS; x++)
            if ( map[0][x].c == '.' ) break;

        current = &map[0][x];
    }

    void followPath (const string &path) {
        size_t pos = 0;

        while ( pos != string::npos && pos < path.size() ) {
            char c = path[pos];
            if ( c == 'R' || c == 'L' ) {
                rotate(c);
                pos++;
            } else {
                current = current->move (dir, atoi ( &path[pos] ), current );
                pos = path.find_first_of( "LR", pos+1 );
            }
        }
    }

    void rotate (char to) {
        if (to == 'R')
            dir = (Dir)((dir + 1) % 4);
        else 
            dir = (Dir)((dir + 3) % 4);
    }

    void whereami () {
        printf ("I am at (%d, %d) facing %d \n", current->x + 1, current->y + 1, dir);
        printf ("puzzle input: %d \n", (1000* (current->y + 1)) + (4 * (current->x + 1)) + dir );
    }
};


void loadMap () {
    ifstream ifs("challenge_20221222_map.txt");
    string s;
    int y = 0;
    size_t first, last;

    while (getline(ifs, s)) {
        const char *str = s.c_str();

        for (int x = 0; x< s.size(); x++) {
            Node *n = &map[y][x];
            n->x = x;
            n->y = y;
            n->c = str[x];

            /* only for Part-I */
            first = s.find_first_not_of(' ');
            last  = s.find_last_of(".#");
            map[y][first].left = &map[y][last];
            map[y][last].right = &map[y][first];
            /* END only for Part-I */
        }
        y++;
    }

    /* only for Part-I */
    for ( int x=0; x<COLS; x++ ) {
        for ( first=0; first<ROWS; first++ ) 
            if ( map[first][x].c != ' ' ) break;
        for ( last=first; last<ROWS; last++ ) 
            if ( map[last][x].c == ' ' ) break;            

        map[first][x].up = &map[last-1][x];
        map[last-1][x].down = &map[first][x];
    }
    /* END only for Part-I */
}

void loadPath ( string &path ) {
    ifstream ifs("challenge_20221222_route.txt");
    getline(ifs, path);
}

void initMap () {
    for (int y=0; y<ROWS; y++)
        for (int x=0; x<COLS; x++)
            map[y][x].c = ' ';
    
    for (int y=0; y<ROWS-1; y++) {
        for (int x=0; x<COLS-1; x++) {
            map[y][x].right = &map[y][x+1];
            map[y][x+1].left = &map[y][x];
            map[y][x].down = &map[y+1][x];
            map[y+1][x].up = &map[y][x];
        }
    }
}


void foldMap () {
    int i;

    for (i=0; i<50; i++)
    {
        /* 1, 3 */
        map[49][100+i].down = &map[50+i][99];
        map[50+i][99].right = &map[49][100+i];

        /* 3, 5 */
        map[50+i][50].left = &map[100][i];
        map[100][i].up = &map[50+i][50];

        /* 4, 6 */
        map[149][50+i].down = &map[150+i][49];
        map[150+i][49].right = &map[149][50+i];

        /* 2, 5 */
        map[i][50].left = &map[149-i][0];
        map[149-i][0].left = &map[i][50];

        /* 6, 1 */
        map[199][i].down = &map[0][100+i];
        map[0][100+i].up = &map[199][i];

        /* 6, 2 */
        map[150+i][0].left = &map[0][50+i];
        map[0][50+i].up = &map[150+i][0];

        /* 1, 4 */
        map[i][149].right = &map[149-i][99];
        map[149-i][99].right = &map[i][149];
    }
}


int main () {
    string path;

    initMap();
    loadMap();
    loadPath(path);

    Humanoid me; // << ME!

    /* PART I */
    me.startJourney();
    me.followPath(path);
    me.whereami();

    /* PART II */
    initMap();
    loadMap();
    foldMap();
    me.startJourney();
    me.followPath(path);
    me.whereami();

    printMap();
}
