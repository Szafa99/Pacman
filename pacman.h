#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>

const unsigned int INF = 9999;
const sf::Vector2f board_size = { 999.9f,829.9f };
const sf::Vector2f windows_size = { 999.9f,1000.f };
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
    std::vector<sf::CircleShape*>points;
    
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
    friend Board;
   
    friend void drawenemy(sf::RenderWindow& window, std::vector<Enemy>& enemys, bool pacmanmode);
private:
    void collect_points(const Board&);
    sf::Clock hunt_mode_time;
    float max_hunt_mode_time;
protected:
    static int points; 
    sf::Vector2i player_direction;
    sf::Vector2i blocked_direction;
    sf::Vector2i next_direction;
    sf::Vector2i last_direction;
    bool tried_direction = false;
    float radious;
    float speed;
    sf::Texture *player_texture;
    class Djikstra* djikstra;
    sf::Color obiectcolor;
protected:
    bool colliding(const sf::CircleShape& obiect, const class Board& board, sf::Vector2i dir);
    bool objects_colliding(const sf::CircleShape& obiect, const sf::CircleShape& obiect2,float tolerance);
public:
    sf::CircleShape player;
    sf::Clock time_since_tried_change_dir;
    bool huntmode;
    bool is_dead;
    sf::Vector2f startposition;

  
public:
    int* get_pointer_to_points() { return &points; }

    void keep_player_in_board(sf::CircleShape& Player);
    void control_moving(sf::CircleShape& obiect, Board& board);
    void rotate(sf::CircleShape& obiect);
    Player(float radious, sf::Color player_color, sf::Vector2f start_position, float speed);
};



class Enemy :private Player
{
    friend class Djikstra;
   friend void drawenemy(sf::RenderWindow& window, std::vector<Enemy>& enemys,bool pacmanmode);
private:
    std::ofstream writetofile;
    Board* board;

    sf::Color pathcolor;
    static unsigned int enemynr;

    int memopath[55];
    int target;
    int homenode;
    bool reached_target;
    float max_change_target_time;
    sf::Clock change_target_time;
    sf::Texture *deadenemy;

    static bool chasemode;
    static sf::Clock chasetime;
    float max_chase_time;
    sf::Clock flash_time;

private:
    void findnodes();
    void control_moving(sf::CircleShape& obiect, Player& pacman, Board& board, int node);
    void choose_enemy_direction(sf::CircleShape& enemy, Board& board, int node);
    int gettarget(int);

public:
    using Player::keep_player_in_board;
    using Player::player;
    using Player::startposition;

    Enemy(float radious, sf::Color player_color, sf::Vector2f start_position, Board board, float speed);
};







class Djikstra {
    friend Enemy;
private:
    //    Board* board;
    static sf::Clock algotime;
    static sf::CircleShape refpoint[2000];
    //int *memopath;
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
    void init(Enemy& object, int node);
    void setnodes();
    void find_edge_length(float radious, const Board* board);
    int get_nearest_node();
    bool colliding(sf::CircleShape& obiect, const Board&, sf::Vector2f dir, sf::Vector2f tolerance);
public:
    void djikstra( std::vector<Enemy>& object, const Player& pacman, const Board& board);
    void drawpath(sf::RenderWindow& window);

    Djikstra();

};




class Menu
{
public:
    Menu(int *points,sf::Vector2f position);
    sf::Text pointinfo;
    sf::Font textfont;
    int *points;
    sf::Texture* heart;
    std::vector<sf::Sprite*>pacmanlives;
    void drawemenu(sf::RenderWindow& window,int* points);
};