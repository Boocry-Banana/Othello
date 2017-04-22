#include "../lib/situation.h"
#include <windows.h>
situation::situation()
{
    //ctor
}
situation::situation(vector< vector <char> > board, char col, int depth, int x, int y)
:game(board), m_col(col), m_depth(depth), m_predecessor(NULL), m_x(x), m_y(y)
{
    vector <int> buff1={500,-150, 30,10,10,30,-150, 500}; //source de l'heuristique: Universit� de Valenciennes
    m_heuristique.push_back(buff1);
    vector <int> buff2={-150, -250, 0,0,0,0,-250,-150};
    m_heuristique.push_back(buff2);
    vector <int> buff3={30,0,1,2,2,1,0,30};
    m_heuristique.push_back(buff3);
    vector <int> buff4={10,0,2,16,16,2,0,10};
    m_heuristique.push_back(buff4);
    m_heuristique.push_back(buff4);
    m_heuristique.push_back(buff3);
    m_heuristique.push_back(buff2);
    m_heuristique.push_back(buff1);



}
situation::~situation()
{
    for (unsigned int i=0; i<m_succesors.size(); i++) delete m_succesors[i];
}

void situation::get_moves()
{
    m_moves.clear();
    vector < vector <int> > buff;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
        {
            if(m_board[i][j]=='e')
            {

                if( (buff=valid_move(i,j,m_col)).size())
                    {
                    m_moves.push_back(buff);
                    m_moves[m_moves.size()-1].push_back(m_moves[m_moves.size()-1][0]);
                    m_moves[m_moves.size()-1][0]={i,j};
                }
            }
        }
    }

}

int situation::is_end()
{
    get_moves();
    if(!m_moves.size())
    {
        return 1;
    }
    else return 0;
}

void situation::get_all_succesors()
{
    vector < vector <char> > board_save=m_board;
    if(is_end()) /*do nothing */;
    else
    {
        for(unsigned int i=0; i<m_moves.size(); i++)
        {
            m_board=board_save;
            for (unsigned int j=0; j<m_moves[i].size(); j++)
            {
                Place(m_moves[i][j][0], m_moves[i][j][1], m_col);
            }

            m_succesors.push_back(new situation(m_board, (m_col== 'w' ? 'b' : 'w'), m_depth+1, m_moves[i][0][0], m_moves[i][0][1]));
            m_board=board_save;
        }
    }
}

int situation::heuristique()
{

    get_moves();
    int value=0;
    int val_cases=0;
    int pions_joues=0;
    int pions_col=0;
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            if(m_board[i][j]!='e')
            {
                pions_joues++;
                if(m_board[i][j]==m_col)
                {
                    pions_col++;
                    val_cases+=m_heuristique[i][j];
                }
            }
        }
        if(pions_joues>=50)
                value= 90*pions_col + 10*m_moves.size() + 20 * val_cases;
                else if(pions_joues >=20)
                    value= 10*pions_col + 20*m_moves.size() + 90 * val_cases;
                else value= 10*pions_col + 80*m_moves.size() + 30*val_cases;

    return value;
}

vector <int> situation::assess(char ia_col)
{
    vector <int> result={0,0,(ia_col==m_col? MIN_HEURISTIC : MAX_HEURISTIC)};
    vector <int> buff;

    get_all_succesors();

    if (m_depth!=SEARCH_DEPTH)
    {
    for (unsigned int i=0; i<m_succesors.size(); i++)
    {
        m_succesors[i]->display(8*(m_succesors[i]->m_depth>4 ? (int)(m_succesors[i]->m_depth/4):0),(m_succesors[i]->m_depth)*30);

        buff=m_succesors[i]->assess(ia_col);
        if(ia_col == m_col){
        if(result[2] <= buff[2])
        {
            result=buff;
        }}
        else if(result[2] >= buff[2])
        {
            result=buff;

        }

    }
    if(m_depth!=0) result={m_x,m_y,result[2]};
    }
    else
    {
        result={m_x,m_y,heuristique()};
        return result;

    }

    return result;
}

vector <int> situation::assess2(char ia_col)
{
    vector <int> result={0,0, MIN_HEURISTIC};
    vector <int> buff;
    situation* buffer=NULL;
    get_all_succesors();
    int test=0;
    if (m_depth!=SEARCH_DEPTH)
    {

    for (unsigned int i=0; i<m_succesors.size(); i++)
    {
        m_succesors[i]->display(8*(m_succesors[i]->m_depth>4 ? (int)(m_succesors[i]->m_depth/4):0),(m_succesors[i]->m_depth)*30);

        if(ia_col==m_col){
                buff=m_succesors[i]->assess(ia_col);
        if(result[2] <= buff[2])
        {
            result=buff;
        }

        }
        else
            {
                if(test){if(m_succesors[i]->heuristique() >= buffer->heuristique()) buffer=m_succesors[i];
                }
                else buffer=m_succesors[i];
            }
    }
    if(ia_col!=m_col) return buffer->assess2(ia_col);
    if(m_depth!=0) result={m_x,m_y,result[2]};

    }
    else
    {
        result={m_x,m_y,heuristique()};
        return result;

    }

    return result;
}