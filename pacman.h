#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
#include <array>
const unsigned int INF = 9999;
const sf::Vector2f windowsize = { 999.9f,829.9f };
const float borderthicknes = 25.f;


// nie jeste pewny preprocesora. Jakby nie dzialalo ,to to nizej
// SFML_STATIC:_MBCS;%(PreprocessorDefinitions)





class Board
{
    friend class Player;
    friend class Enemy;
    friend class Djikstra;


    sf::Texture AGHtexture;
    sf::Texture foodtexture;
    sf::RectangleShape border[36];
    sf::CircleShape points[500];
    float space;
    float border_thicknes;
    float border_thicknes2;
    float foodradious;
    sf::Color foodcolor;
    sf::Vector2f Lettersize;
    sf::Vector2f AGHcoord;


private:
    bool colliding(float radious, sf::Vector2f foodcoords);
public:

    void put_points_on_board();
    void drawing(sf::RenderWindow& window);
    Board(sf::Color color, float border_thicknes, float border_thicknes2, sf::Vector2f AGHcoord, sf::Vector2f lettersize, float space);
};



class Player
{
    friend class Djikstra;
protected:
    sf::Vector2i player_direction;
    sf::Vector2i blocked_direction;
    sf::Vector2i next_direction;
    bool tried_direction = false;

    float radious;
    float speed;
    sf::Texture player_texture;

    class Djikstra* djikstra;

protected:
    bool colliding(const sf::CircleShape& obiect, const class Board& board, sf::Vector2i dir);

public:
    sf::Clock time_since_tried_change_dir;
    sf::CircleShape player;
public:
    void keep_player_in_board(sf::CircleShape& Player);
    void control_moving(sf::CircleShape& obiect, Board& board);


public:
    Player(float radious, sf::Color player_color, sf::Vector2f start_position, float speed);

};



class Enemy:private Player
{
    friend class Djikstra;
private:
    std::ofstream writetofile;
    void findnodes();
    Board* board;
    sf::Color enemycolor;
    static unsigned int enemynr;
private:
    void control_moving(sf::CircleShape& obiect, Board& board, int node);
    void choose_enemy_direction(sf::CircleShape& enemy, Board& board, int node);

public:
    using Player::keep_player_in_board;
  
    using Player::player;
    //sf::CircleShape enemy;
    //void movingenemy(sf::CircleShape& pacman);
    void drawenemy(sf::RenderWindow& window, std::vector<Enemy>&);


    Enemy(float radious, sf::Color player_color, sf::Vector2f start_position, Board board, float speed);
};




class Djikstra{
    friend Enemy;
private:
//    Board* board;
   static sf::Clock algotime;
    static sf::CircleShape refpoint[2000];
    //int *memopath;
    static bool nodeset;
    static std::vector<sf::Vector2f> nodecoords;
    static int memopath[55];
    bool** reached;
    float* shrt_way_node;
    bool* visited;
    float shortestconection[4];
    float** edge_length;

    float maxalogotime;
    std::ifstream readfromfile;


    sf::Vector2i enemydirection[4];
private:

    void setnodes();
    void find_edge_length(float radious,const Board *board);
    int get_nearest_node();
    bool colliding( sf::CircleShape& obiect, const Board&,sf::Vector2f dir, sf::Vector2f tolerance);
public:
    void djikstra(const std::vector<Enemy>& object, const sf::CircleShape& pacman,const Board &board);
    void drawpath(sf::RenderWindow & window);

    Djikstra();

};
