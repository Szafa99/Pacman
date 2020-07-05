#ifndef pawns
#define pawns

#include "sBoard.h"
#include "sDjikstra.h"


class sPlayer
{
    friend class sDjikstra;
    friend class sBoard;

    friend void drawenemy(sf::RenderWindow& window, std::vector<class sEnemy>& enemys, bool pacmanmode);
private:
    void collect_points(const sBoard&);
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
    sf::Texture* player_texture;
    sDjikstra* djikstra;
    sf::Color obiectcolor;
protected:
    bool colliding(const sf::CircleShape& obiect, const class sBoard& board, sf::Vector2i dir);
    bool objects_colliding(const sf::CircleShape& obiect, const sf::CircleShape& obiect2, float tolerance);
public:
    sf::CircleShape player;
    sf::Clock time_since_tried_change_dir;
    bool huntmode;
    bool is_dead;
    sf::Vector2f startposition;


public:
    int* get_pointer_to_points() { return &points; }

    void keep_player_in_board(sf::CircleShape& Player);
    void control_moving(sf::CircleShape& obiect, sBoard& board);
    void rotate(sf::CircleShape& obiect);
    sPlayer(float radious, sf::Color player_color, sf::Vector2f start_position, float speed);
};



class sEnemy :private sPlayer
{
    friend class sDjikstra;
    friend void drawenemy(sf::RenderWindow& window, std::vector<sEnemy>& enemys, bool pacmanmode);
private:
    std::ofstream writetofile;
    sBoard* board;

    sf::Color pathcolor;
    static unsigned int enemynr;

    int memopath[55];
    int homenode;
    int target;
    bool reached_target;
    float max_change_target_time;
    sf::Clock change_target_time;

    sf::Texture* deadenemy;
    static bool chasemode;
    static sf::Clock chasetime;
    float max_chase_time;
    sf::Clock flash_time;

private:
    void findnodes();
    void control_moving(sf::CircleShape& obiect, sPlayer& pacman, sBoard& board, int node);
    void choose_enemy_direction(sf::CircleShape& enemy, sBoard& board, int node);
    int gettarget(int);

public:
    using sPlayer::keep_player_in_board;
    using sPlayer::player;
    using sPlayer::startposition;

    sEnemy(float radious, sf::Color player_color, sf::Vector2f start_position, sBoard board, float speed);
};

#endif
