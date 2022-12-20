#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define BUFFER_SZ 6144

typedef struct 
{
    long long val;
    int pos;
} tpl_t;

tpl_t msg[BUFFER_SZ];
tpl_t *msg_order[BUFFER_SZ];
int msg_sz = 0;

void swap (int a, int b) {
    tpl_t tmp = msg[a];

    msg[a] = msg[b];
    msg[b] = tmp;

    msg_order[ msg[a].pos ] = &msg[a];
    msg_order[ msg[b].pos ] = &msg[b];
}

void move (int pos, long long adv) {
    int npos;
    int dir = adv < 0 ? -1 : 1;

    adv = adv % (msg_sz-1);

    for ( int i = 0 ; i < abs(adv) ; i++ ) {        

        npos = ( pos + dir );
        if (npos < 0) npos = msg_sz-1; 
        if (npos >= msg_sz) npos = 0;

        swap (pos, npos);

        pos = npos;
    }
}

void printResult ( const string &heading )
{
    cout << heading << endl;

    long long a, b, c;
    
    for (int i = 0; i < msg_sz; i++) {
        if ( msg[i].val == 0 ) {

            a = msg[(i+1000) % msg_sz].val;
            b = msg[(i+2000) % msg_sz].val;
            c = msg[(i+3000) % msg_sz].val;

            printf ("1000th : %lld\n", a);
            printf ("2000th : %lld\n", b);
            printf ("3000th : %lld\n", c);
            printf ("ANSWER: %lld\n\n", a + b + c);
            break;
        }
    }
}

void load (const string &fname) {
    int d;

    msg_sz = 0;

    ifstream ifs (fname);

    while ( ifs >> d ) {
        msg[msg_sz].pos = msg_sz;
        msg[msg_sz].val = d;
        msg_order[msg_sz] = &msg[msg_sz];

        msg_sz += 1;

        if (msg_sz > BUFFER_SZ)
        {
            cout << "ERROR: Not enough buffer space" << endl;
            exit(-1);
        }
    }
}

void solveOnce () {
    for (int i = 0; i < msg_sz; i++) {
        int pos = (msg_order[i] - msg);
        move(pos, msg_order[i]->val);
    }
}

int main() {

    // PART I
    load("challenge_20221220.txt");

    solveOnce();

    printResult ( "--Part I" );

    
    // PART II
    load("challenge_20221220.txt");

    for (int i = 0; i < msg_sz; i++)
        msg[i].val *= 811589153;

    for (int i=0; i<10; i++)
        solveOnce();

    printResult ( "--Part II" );

    return 0;
}
