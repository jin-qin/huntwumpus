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
    int row_len = 0, col_len = 0;
    Map map = 0;
};


#endif