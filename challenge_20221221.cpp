#include <string>
#include <iostream>
#include <fstream>
#include <map>

using namespace std;

map<string, struct Monkey> monkeys;
map<string, string> dependencies;

struct Monkey
{
    void setVal (long long val) {
        this->val = val;
        op = '=';
    }

    void setOp (const string &a1, const string &a2, char op) {
        this->a1 = a1;
        this->a2 = a2;
        this->op = op;
    }

    long long solve () {
        if (op == '=')
            return val;
        else
        {
            long long l1 = monkeys[a1].solve();
            long long l2 = monkeys[a2].solve();

            if (op == '+') return l1 + l2;
            if (op == '-') return l1 - l2;
            if (op == '*') return l1 * l2;
            if (op == '/') return l1 / l2;
        }

        cout << "invalid operation" << endl;
        exit(1);
    }

    long long       val;
    std::string     a1, a2;
    char            op;
};

void load (const std::string fname) {    
    ifstream    ifs (fname);
    string      s, a1, a2, name;
    Monkey     *m;

    while ( getline(ifs, s) ) {
        name = s.substr(0, 4);
        size_t op_pos = s.find_first_of("+-*/");

        if ( op_pos == string::npos ) {
            monkeys[name].setVal ( stoll( s.substr(6) ) );
        } else {
            a1 = s.substr(6, 4);
            a2 = s.substr(13, 4);
            
            monkeys[name].setOp ( a1, a2, s[11] );

            dependencies[a1] = name;
            dependencies[a2] = name;
        }
    }
}

void tellWhatRootYells() {
    long long val = monkeys["root"].solve();
    std::cout << "PART I \nROOT YELLS: " << val << endl << endl;
}

void construct( Monkey &tgt, const string &tn, const Monkey &dep, const string &dn ) {
    if ( dep.a1 == tn ) {
        if ( dep.op == '+' ) tgt.setOp ( dn, dep.a2, '-' );
        else if ( dep.op == '-' ) tgt.setOp ( dn, dep.a2, '+' );
        else if ( dep.op == '*' ) tgt.setOp ( dn, dep.a2, '/' );
        else if ( dep.op == '/' ) tgt.setOp ( dn, dep.a2, '*' );
    } else {
        if ( dep.op == '+' ) tgt.setOp ( dn, dep.a1, '-' );
        else if ( dep.op == '-' ) tgt.setOp ( dep.a1, dn, '-' );
        else if ( dep.op == '*' ) tgt.setOp ( dn, dep.a1, '/' );
        else if ( dep.op == '/' ) tgt.setOp ( dep.a2, dn, '/' );
    }
}

void solveHumanFor () {
    string pdn, dn = "humn";
    Monkey *mp = &monkeys["humn"];
    Monkey *m = NULL;

    while ( true ) {
        pdn =  dn;
        dn  =  dependencies[pdn];
        m   = &monkeys[dn];

        if ( dn == "root" ) {            
            Monkey *root = &monkeys["root"];

            if ( root->a1 == pdn )
                mp->setVal( monkeys[root->a2].solve() );
            else
                mp->setVal( monkeys[root->a1].solve() );

            break;
        }

        construct ( *mp, pdn, *m, dn );
        mp = m;
    }    

    m = &monkeys["humn"];
    cout << "\nPART II \nThe HUMAN should YELL: " << m->solve() << endl;
}


int main () {

    load ("challenge_20221221.txt");

    tellWhatRootYells();
    solveHumanFor();

    return 0;
}