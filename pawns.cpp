#include "headers/pawns.h"


sPlayer::sPlayer(float radious, sf::Color player_color, sf::Vector2f start_position, float speed) :
    radious(radious),
    player(radious),
    speed(speed),
    huntmode(false),
    player_direction({ 0,0 }),
    blocked_direction({ 0,0 }),
    next_direction({ 0,0 }),
    last_direction({ 1,0 }),
    djikstra(new sDjikstra()),
    max_hunt_mode_time(8.f),
    obiectcolor(player_color),
    startposition(start_position)
{
    player_texture = new sf::Texture;
    if (!player_texture->loadFromFile("images/Pacman_HD.png"))
        exit(0);

    player.setTexture(player_texture);
    // player.setFillColor(player_color);
    player.setPosition(start_position.x, start_position.y);
    player.setOrigin({ player.getLocalBounds().width / 2, player.getLocalBounds().height / 2 });
}




void sPlayer::keep_player_in_board(sf::CircleShape& player)
{

    if (player.getPosition().x > 1000)player.setPosition(0.f, player.getPosition().y);
    if (player.getPosition().x < 0)player.setPosition(1000.0f, player.getPosition().y);
    if (player.getPosition().y > 800)player.setPosition(player.getPosition().x, 0.f);
    if (player.getPosition().y < 0)player.setPosition(player.getPosition().x, 800.0f);
}



//template <class type>
void sPlayer::control_moving(sf::CircleShape& obiect, sBoard& board)
{
    if (time_since_tried_change_dir.getElapsedTime().asSeconds() > 1.f) {
        tried_direction = false;
        next_direction = player_direction;
    }
    if (tried_direction && !colliding(obiect, board, next_direction))
    {
        obiect.move({ speed * next_direction.x, speed * next_direction.y });
        tried_direction = false;
        return;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        next_direction = { -1,0 };

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        next_direction = { 1,0 };

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        next_direction = { 0,-1 };

    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        next_direction = { 0,1 };
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        next_direction = { 0,0 };




    if (colliding(obiect, board, next_direction))
    {
        if (colliding(obiect, board, { player_direction }))
            player_direction = { 0,0 };
        else if (time_since_tried_change_dir.getElapsedTime().asSeconds() > 1.f)
        {
            blocked_direction = next_direction;

            tried_direction = true;
            time_since_tried_change_dir.restart();
        }

    }
    else player_direction = next_direction;
    rotate(obiect);


    obiect.move({ speed * player_direction.x, speed * player_direction.y });
    collect_points(board);

}

void sPlayer::rotate(sf::CircleShape& obiect) {
    if (last_direction.x * player_direction.x == -1) {
        obiect.rotate(180);
        last_direction = player_direction;
    }
    if (last_direction.y * player_direction.y == -1) {
        obiect.rotate(180);
        last_direction = player_direction;
    }
    if (last_direction.x == -1 && player_direction.y == 1 || last_direction.y == -1 && player_direction.x == -1 ||
        last_direction.x == 1 && player_direction.y == -1 || last_direction.y == 1 && player_direction.x == 1)
    {
        obiect.rotate(-90);
        last_direction = player_direction;
    }
    if (last_direction.x == -1 && player_direction.y == -1 || last_direction.y == -1 && player_direction.x == 1 ||
        last_direction.x == 1 && player_direction.y == 1 || last_direction.y == 1 && player_direction.x == -1)
    {
        obiect.rotate(90);
        last_direction = player_direction;
    }

}




bool sPlayer::colliding(const sf::CircleShape& obiect, const sBoard& board, sf::Vector2i direction)
{

    float radious = obiect.getRadius();
    sf::Vector2f playerposition = obiect.getPosition();
    for (int i = 0; i < 36; i++)
    {
        sf::Vector2f borderposition = { board.border[i].getPosition().x + board.border[i].getSize().x / 2, board.border[i].getPosition().y + board.border[i].getSize().y / 2 };
        sf::Vector2f bordersize = board.border[i].getSize();

        if (
            abs(borderposition.x - (playerposition.x + direction.x * this->speed)) < radious + bordersize.x / 2 &&
            abs(borderposition.y - (playerposition.y + direction.y * this->speed)) < radious + bordersize.y / 2
            ) {
            float x = abs(borderposition.x - (playerposition.x + direction.x * this->speed));
            float y = abs(borderposition.y - (playerposition.y + direction.y * this->speed));
            float difx = radious + bordersize.x / 2;
            float dify = radious + bordersize.y / 2;
            return true;
        }
    }
    return false;

}



bool sPlayer::objects_colliding(const sf::CircleShape& obiect, const sf::CircleShape& obiect2, float tolerance) {
    if (abs(obiect.getPosition().x - obiect2.getPosition().x) < tolerance &&
        abs(obiect.getPosition().y - obiect2.getPosition().y) < tolerance)return true;
    else return false;
}


void sPlayer::collect_points(const sBoard& board) {
    for (int i = 0; i < board.points.size(); i++) {
        if (board.points[i] != nullptr)
            if (objects_colliding(player, *board.points[i], 2.f)) {
                if (board.points[i]->getScale().x == 2) {
                    huntmode = true;
                    hunt_mode_time.restart();
                }
                delete board.points[i];
                const_cast<sBoard&>(board).points[i] = nullptr;
                points++;
            }
    }




    if (hunt_mode_time.getElapsedTime().asSeconds() > max_hunt_mode_time)
        huntmode = false;
}



sEnemy::sEnemy(float radious, sf::Color player_color, sf::Vector2f start_position, sBoard board, float speed) :
    sPlayer::sPlayer(radious, player_color, start_position, speed),
    board(new sBoard(board)),
    target(0),
    max_change_target_time(5.f),
    reached_target(true),
    max_chase_time(10.f),
    homenode(17),
    pathcolor(player_color),
    deadenemy(new sf::Texture())
{
    for (int i = 0; i < 55; i++)
        memopath[i] = i;

    if (!player_texture->loadFromFile("images/enemy" + std::to_string(enemynr) + ".png"))
        exit(0);

    if (!deadenemy->loadFromFile("images/deadenemy.png"))exit(0);
    obiectcolor = player.getFillColor();
    player.setTexture(player_texture);
    player.setTextureRect(sf::IntRect{ 0,0,300,559 });
    time_since_tried_change_dir.restart();
    enemynr++;
}




void sEnemy::choose_enemy_direction(sf::CircleShape& enemy, sBoard& board, int node)
{


    int count = 0;
    sf::Vector2f enemypos = enemy.getPosition();
    int parent = node;
    sf::Vector2f nextpos = djikstra->nodecoords[parent];
    while (abs(nextpos.x - enemypos.x) < 1.f && abs(nextpos.y - enemypos.y) < 1.f) {

        nextpos = djikstra->nodecoords[memopath[parent]];
        parent = memopath[parent];
        if (count++ > 20) {
            break;
        }
    }
    sf::Vector2f diffrence = { enemypos.x - nextpos.x ,enemypos.y - nextpos.y };
    if (abs(diffrence.x) >= abs(diffrence.y))
    {
        if (enemypos.x > nextpos.x && !djikstra->colliding(player, board, { -speed,0 }, { 0.3,0.3 }))next_direction = { -1,0 };
        else if (!djikstra->colliding(player, board, { speed,0 }, { 0.3,0.3 }))next_direction = { 1,0 };
    }
    else
    {
        if (enemypos.y > nextpos.y && !djikstra->colliding(player, board, { 0, -speed }, { 0.3,0.3 }))next_direction = { 0, -1 };
        else if (enemypos.y < nextpos.y && !djikstra->colliding(player, board, { 0,speed }, { 0.3,0.3 }))next_direction = { 0, 1 };
    }

    keep_player_in_board(enemy);


}



void sEnemy::control_moving(sf::CircleShape& obiect, sPlayer& pacman, sBoard& board, int node)
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        if (chasemode)chasemode = false;
        else chasemode = true;


    if (time_since_tried_change_dir.getElapsedTime().asSeconds() > 1.f) {
        tried_direction = false;
        next_direction = player_direction;
    }
    if (tried_direction && !colliding(obiect, board, next_direction))
    {
        obiect.move({ speed * next_direction.x, speed * next_direction.y });
        tried_direction = false;
        return;
    }


    choose_enemy_direction(obiect, board, node);


    if (colliding(obiect, board, next_direction))
    {
        if (colliding(obiect, board, { player_direction })) {
            player_direction = { 0,0 };
        }
        else if (time_since_tried_change_dir.getElapsedTime().asSeconds() > 1.f)
        {
            blocked_direction = next_direction;

            tried_direction = true;
            time_since_tried_change_dir.restart();
        }

    }
    else player_direction = next_direction;

    if (pacman.huntmode && objects_colliding(this->player, pacman.player, 20.f))
    {
        pacman.points += 20;
        is_dead = true;
        this->player.setTexture(deadenemy);
        target = homenode;
    }
    else if (!pacman.huntmode && objects_colliding(this->player, pacman.player, 20.f)) pacman.is_dead = true;

    obiect.move({ speed * player_direction.x, speed * player_direction.y });
}



int sEnemy::gettarget(int pacmannode) {

    if (is_dead)
        if (reached_target) {
            is_dead = false;
            player.setTexture(player_texture);
        }
        else return target;


    if (sPlayer::points % 20 < 5 && points > 20) {
        chasemode = true;
        chasetime.restart();
    }
    else if (chasetime.getElapsedTime().asSeconds() > max_chase_time)
        chasemode = false;

    if (chasemode) {
        return pacmannode;
    }
    else return (rand() % 30 + rand() % 20);
}








void sEnemy::findnodes()
{
    // Was to lazy to insert all nodecoords manually, so I created a fuction for that, the coordinates are wrtitten to a file. 
    int nodes = 0;
    std::vector<sf::CircleShape> refpoint;
    refpoint.push_back(sf::CircleShape{ 0,0 });
    writetofile.open("Nodecoord", std::ofstream::trunc);
    if (!writetofile.is_open())exit(0);
    for (float i = 0; i < board_size.y - speed; i += 0.5f)
    {
        for (float j = 0; j < board_size.x - speed; j += 0.5f)
        {

            refpoint[nodes].setPosition({ j,i });
            refpoint[nodes].setRadius(radious);
            refpoint[nodes].setOrigin({ refpoint[nodes].getLocalBounds().width / 2, refpoint[nodes].getLocalBounds().height / 2 });
            if (j > board->AGHcoord.x && j<board->AGHcoord.x + board->Lettersize.x && i >board->AGHcoord.y && i < board->AGHcoord.y + board->Lettersize.y / 2.f + board->space / 2.f)
                continue;
            if (

                (!sPlayer::colliding(refpoint[nodes], *board, { 1,0 }) || !sPlayer::colliding(refpoint[nodes], *board, { -1,0 })) &&
                (!sPlayer::colliding(refpoint[nodes], *board, { 0,1 }) || !sPlayer::colliding(refpoint[nodes], *board, { 0,-1 }))
                )
            {
                nodes++;
                refpoint.push_back(sf::CircleShape{ 0,0 });
            }
        }
    }
    for (int i = 0; i < nodes; i++)
    {
        for (int j = 0; j < nodes; j++)
        {
            if ((abs(refpoint[j].getPosition().x - refpoint[i].getPosition().x) <= 1.f && abs(refpoint[j].getPosition().y - refpoint[i].getPosition().y) <= 1.f) && j != i && refpoint[i].getPosition().x != -10) {
                refpoint[j].setPosition({ -10,-10 });
            }
        }
        if (refpoint[i].getPosition().x != -10)
            writetofile << refpoint[i].getPosition().x << " " << refpoint[i].getPosition().y << "\n";
    }
    writetofile.close();


    exit(0);
}