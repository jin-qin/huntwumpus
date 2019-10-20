#ifndef Knowledge_H
#define Knowledge_H

class Tile;

typedef Tile** Map;

class KnowledgeBase{
public:
    KnowledgeBase(int row_len, int col_len);
    void add_knowledge(int pos_x, int pos_y, int status);
    void create_knowledge(int x, int y);
    void analyze();
    void update_knowledge();


private:
    int m_row_len = 0, m_col_len = 0;
    Map m_map = 0;//Store information of board
    //m_pos_x and m_pos_y are used to store the position player has been
    int * m_pos_x;
    int * m_pos_y;
};


#endif