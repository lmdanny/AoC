#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define BUFFER_SZ 6144

struct tpl_t
{
    long long val;
    int pos;
    tpl_t *next;
    tpl_t *prev;
};

tpl_t msg[BUFFER_SZ];
int msg_sz = 0;

void putLeftOf(tpl_t *tp, tpl_t *lo) {
    tpl_t tmp = *tp;

    (*lo->prev).next = tp;
    tp->prev = lo->prev;
    tp->next = lo;
    lo->prev = tp;

    (*tmp.next).prev = tmp.prev;
    (*tmp.prev).next = tmp.next;
}

void move(tpl_t *tp, long long adv) {
    tpl_t *lp = tp;

    adv = adv % (msg_sz-1);

    if (adv < 0) {
        for (int i=0; i<abs(adv);i++) 
            lp = lp->prev;
    }
    else if (adv > 0) {
        for (int i=0; i<=abs(adv);i++) 
            lp = lp->next;
    }
    else
        return;

    putLeftOf(tp, lp);
}

tpl_t *advance (tpl_t *start, int adv) {

    for (; adv > 0; adv--)
        start = start->next;

    return start;
}

void printResult ( const string &heading )
{
    long long a, b, c;
    tpl_t *tp = msg;

    cout << heading << endl;

    do {
        if ( tp->val == 0 ) {

            tp = advance(tp, 1000);
            a = tp->val;

            tp = advance(tp, 1000);
            b = tp->val;

            tp = advance(tp, 1000);
            c = tp->val;

            printf ("1000th : %lld\n", a);
            printf ("2000th : %lld\n", b);
            printf ("3000th : %lld\n", c);
            printf ("ANSWER: %lld\n\n", a + b + c);

            break;
        }

        tp = tp->next; 
    } while (tp != msg);

}

void load (const string &fname, long long key) {
    int d;

    msg_sz = 0;

    ifstream ifs (fname);

    while ( ifs >> d ) {
        msg[msg_sz].pos = msg_sz;
        msg[msg_sz].val = ( d * key );

        msg[msg_sz].next = &msg[msg_sz+1];
        msg[msg_sz+1].prev = &msg[msg_sz];

        msg_sz += 1;

        if (msg_sz > BUFFER_SZ)
        {
            cout << "ERROR: Not enough buffer space" << endl;
            exit(-1);
        }
    }

    msg[msg_sz-1].next = &msg[0];
    msg[0].prev = &msg[msg_sz-1];
}

void solveOnce () {
    for (int i = 0; i < msg_sz; i++)
        move(&msg[i], msg[i].val);
}

int main() {

    // PART I
    load("challenge_20221220.txt", 1);
    solveOnce();
    printResult ( "--Part I" );

    // PART II
    load("challenge_20221220.txt", 811589153);

    for (int i=0; i<10; i++)
        solveOnce();

    printResult ( "--Part II" );

    return 0;
}
