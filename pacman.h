#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>
const unsigned int INF = 9999;
const sf::Vector2f windowsize = { 999.9f,829.9f };
const float borderthicknes = 25.f;


// nie jeste pewny preprocesora. Jakby nie dzialalo ,to to nizej
// SFML_STATIC:_MBCS;%(PreprocessorDefinitions)





class Player
{
protected:
    sf::Vector2i player_direction;
    float radious;
    float speed;
    sf::Texture player_texture;
    sf::Vector2i blocked_direction;
    sf::Vector2i next_direction;
    bool tried_direction = false;

protected:
    bool colliding(const sf::CircleShape& obiect, const class Board& board, sf::Vector2i dir);

public:
    sf::Clock time_since_tried_change_dir;
    sf::CircleShape player;
public:
    void keep_player_in_board(sf::CircleShape& Player);
    void control_moving(sf::CircleShape&, Board& board);


public:
    Player(float radious, sf::Color player_color, sf::Vector2f start_position, float speed);

};


class Board
{
    friend Player;
    friend class Enemy;


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






class Enemy:private Player
{
private:
    std::vector<sf::Vector2f> nodecoords;
    sf::CircleShape refpoint[2000];
    //int *memopath;
    int memopath[55];
    float *shrt_way_node;
    bool* visited;
    float shortestconection[4];
    float** edge_length;

    std::ofstream writetofile;
    std::ifstream readfromfile;

    Board* board;
    sf::Vector2i enemydirection[4];
private:
    void setnodes();
    void find_edge_length();
    int get_nearest_node();
    void choose_enemy_direction(sf::CircleShape& pacman, Board& board);
    bool colliding(const sf::CircleShape& obiect, const Board&, sf::Vector2f tolerance);
    void findnodes();
public:
    void djikstra(const sf::CircleShape* object, const sf::CircleShape& pacman);
    using Player::keep_player_in_board;
    sf::CircleShape enemy[4];
    void movingenemy(sf::CircleShape& pacman);
    void drawenemy(sf::RenderWindow& window);


    Enemy(float radious, sf::Color player_color, sf::Vector2f start_position, Board board, float speed);
};