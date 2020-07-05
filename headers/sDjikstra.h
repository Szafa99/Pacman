#ifndef sDjikstra_h
#define sDjikstra_h


#include "pawns.h"


class sDjikstra {
    friend class sEnemy;
    friend class sPlayer;
private:

    static sf::Clock algotime;
    static sf::CircleShape refpoint[2000];

    static bool nodeset;
    static std::vector<sf::Vector2f> nodecoords;
    bool** reached;
    float* shrt_way_node;
    bool* visited;
    float shortestconection[4];
    float** edge_length;
    float maxalogotime;
    std::ifstream readfromfile;


    sf::Vector2i enemydirection[4];
private:
    void init(sEnemy& object, int node);
    void setnodes();
    void find_edge_length(float radious, const sBoard* board);
    int get_nearest_node();
    bool colliding(sf::CircleShape& obiect, const sBoard&, sf::Vector2f dir, sf::Vector2f tolerance);
public:
    void djikstra(std::vector<sEnemy>& object, const sPlayer& pacman, const sBoard& board);
    void drawpath(sf::RenderWindow& window);

    sDjikstra();

};

#endif 
