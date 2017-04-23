
#include <iostream>
#include <vector>
#include "console.h"
#include "time.hpp"
#define SEARCH_DEPTH 5
#define MIN_HEURISTIC -10000000
#define MAX_HEURISTIC 1000000
using namespace std;

class IA
{
    public:
        IA(char col='w');
        virtual ~IA();
        void clear_moves();
        void add_right_move(int x, int y, int nb);
        vector <int> play(int mode=0, vector < vector <char> > board={{}} );
        void get_heuristics (int x, int y, vector < vector<int> > plateau);
        ///Couleur de l'IA
        char m_col;
        ///tableau des mouvements possibles
        vector < vector<int> > m_right_moves;
    private:


};

