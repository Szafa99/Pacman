#include "pacman.h"



Player::Player(float radious, sf::Color player_color, sf::Vector2f start_position, float speed) :
    radious(radious),
    player(radious),
    speed(speed),
    huntmode(false),
    player_direction({ 0,0 }),
    blocked_direction({ 0,0 }),
    next_direction({ 0,0 }),
    last_direction({1,0}),
    djikstra(new Djikstra()),
    max_hunt_mode_time(8.f),
    obiectcolor(player_color),
    startposition(start_position)
{
    player_texture = new sf::Texture;
    if (!player_texture->loadFromFile("Pacman_HD.png")) 
        exit(0);
    
    player.setTexture(player_texture);
    // player.setFillColor(player_color);
    player.setPosition(start_position.x, start_position.y);
    player.setOrigin({ player.getLocalBounds().width / 2, player.getLocalBounds().height / 2 });
}




void Player::keep_player_in_board(sf::CircleShape& player)
{

    if (player.getPosition().x > 1000)player.setPosition(0.f, player.getPosition().y);
    if (player.getPosition().x < 0)player.setPosition(1000.0f, player.getPosition().y);
    if (player.getPosition().y > 800)player.setPosition(player.getPosition().x, 0.f);
    if (player.getPosition().y < 0)player.setPosition(player.getPosition().x, 800.0f);
}



//template <class type>
void Player::control_moving(sf::CircleShape& obiect, Board& board)
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

void Player::rotate(sf::CircleShape& obiect) {
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




bool Player::colliding(const sf::CircleShape& obiect, const Board& board, sf::Vector2i direction)
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



bool Player::objects_colliding(const sf::CircleShape& obiect, const sf::CircleShape& obiect2,float tolerance) {
    if (abs(obiect.getPosition().x - obiect2.getPosition().x) < tolerance &&
        abs(obiect.getPosition().y - obiect2.getPosition().y) < tolerance)return true;
    else return false;
}


void Player::collect_points(const Board& board) {
    for (int i = 0; i < board.points.size(); i++) {
        if(board.points[i]!=nullptr)
        if ( objects_colliding(player,*board.points[i],2.f ) ) {
            if (board.points[i]->getScale().x==2) {
                huntmode = true;
                hunt_mode_time.restart();
            }
            delete board.points[i];
            const_cast<Board&>(board).points[i] = nullptr;
            points++;
        }
   }

    


    if (hunt_mode_time.getElapsedTime().asSeconds() > max_hunt_mode_time)
        huntmode = false;
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////




Board::Board(sf::Color color, float border_thicknes, float AGH_thicknes2, sf::Vector2f AGHcoord, sf::Vector2f Lettersize, float space) :
    border_thicknes(border_thicknes),
    border_thicknes2(AGH_thicknes2),
    space(space),
    Lettersize(Lettersize),
    foodradious(10.f),
    foodcolor(255, 253, 149),
    AGHcoord(AGHcoord)
{
    points.reserve(60);
    sf::CircleShape temp;
  
    float tolerance = 0.1f;
    border[0].setSize({ border_thicknes, board_size.y / 2.f - space / 2.f });  // LEFT-UPPER BORDER
    border[0].setPosition(0.f, 0.f);
    // nodecoord[0] = { border_thicknes / 2 + space / 2,border_thicknes / 2 + space / 2 }; // start node, going down in the y axis

    border[1].setSize({ border_thicknes,  board_size.y / 2.f - space / 2.f }); // LEFT-LOWER BORDER
    border[1].setPosition(0.f, board_size.y / 2.f + space / 2.f);

    // nodecoord[1] = { border_thicknes / 2 + space / 2,windowsize.y - space / 2 }; 
     //////////////////////////////////////////////////

    border[2].setSize({ board_size.x / 2.f - space / 2.f, border_thicknes + 0.2f });    // UPPER -LEFT BORDER
    border[2].setPosition(0.f, 0.f);
    //   nodecoord[2] = { windowsize.x / 2.f ,border_thicknes / 2 + space / 2 }; //going 1 further in x axiss

    border[3].setSize({ board_size.x / 2.f - space / 2.f, border_thicknes + 0.2f });    // UPPER-RIGHT BORDER
    border[3].setPosition(board_size.x / 2.f + space / 2.f, 0.f);
    //  nodecoord[4] = { windowsize.x - space / 2 ,space / 2 + border_thicknes / 2 };
      ///////////////////////////////////////////////////////

    border[4].setSize({ board_size.x / 2.f - space / 2.f, border_thicknes });   // DOWN-LEFT BORDER
    border[4].setPosition(0.f, board_size.y - border_thicknes);
    //  nodecoord[5] = {windowsize.x/2, windowsize.y - border_thicknes / 2 - space / 2};

    border[5].setSize({ board_size.x / 2.f - space / 2.f, border_thicknes });  //DOWN-LEFT BORDER
    border[5].setPosition(board_size.x / 2.f + space / 2.f, board_size.y - border_thicknes);
    //  nodecoord[6] = { windowsize.x-space/2, windowsize.y - border_thicknes / 2 - space / 2 };


      ////////////////////////////////////////////////////////

    border[6].setSize({ border_thicknes,  board_size.y / 2.f - space / 2.f }); //RIGHT-UPPER BORDER
    border[6].setPosition(board_size.x - border_thicknes, 0.f);
    //  nodecoord[7] = { windowsize.x - space / 2,windowsize.y / 2 };

    border[7].setSize({ border_thicknes,  board_size.y / 2.f - space / 2.f }); // RIGHT-LOWER BORDER
    border[7].setPosition(board_size.x - border_thicknes, board_size.y / 2.f + space / 2.f);





    ////////////////////////////AGH

    ///////////////////////////A
    border[8].setSize({ AGH_thicknes2,Lettersize.y + tolerance });                           // | od A
    border[8].setPosition(AGHcoord);                                               // | od A   

    border[9].setSize({ Lettersize.x, AGH_thicknes2 });                             //  -- od A 
    border[9].setPosition({ AGHcoord });                                               //  -- od A

    border[10].setSize({ AGH_thicknes2,Lettersize.y + tolerance });                         // prawe | OD A
    border[10].setPosition({ AGHcoord.x + Lettersize.x - AGH_thicknes2,AGHcoord.y });                 // prawe| od A

    border[11].setSize({ Lettersize.x, AGH_thicknes2 });                            // srodkowa kresa -- od A
    border[11].setPosition({ AGHcoord.x,AGHcoord.y + (Lettersize.y / 2) - space / 2 });             // srodkowa kreska -- od A    

    ////////////////// little borsers in the letter A

    float Athicknes = ((Lettersize.x - 2 * AGH_thicknes2) - 3 * space) / 2;

    border[28].setSize({ Athicknes, Lettersize.y / 2.f - space / 2 - AGH_thicknes2 + tolerance });
    border[28].setPosition({ AGHcoord.x + AGH_thicknes2 + space,AGHcoord.y + (Lettersize.y / 2.f) + space / 2 + AGH_thicknes2 });


    border[29].setSize({ Athicknes, Lettersize.y / 2.f - space / 2 - AGH_thicknes2 + tolerance });
    border[29].setPosition({ AGHcoord.x + AGH_thicknes2 + Athicknes + 2 * space,AGHcoord.y + (Lettersize.y / 2.f) + space / 2 + AGH_thicknes2 });
    /////////////////////////////////////////G  

    border[12].setSize({ Lettersize.x, AGH_thicknes2 });                            // GORNA kresa -- od G
    border[12].setPosition({ AGHcoord.x + Lettersize.x + space,AGHcoord.y });              //  GORNA kreska -- od G    

    border[13].setSize({ AGH_thicknes2,Lettersize.y + tolerance });                           // | od G
    border[13].setPosition({ AGHcoord.x + Lettersize.x + space, AGHcoord.y });       //  LEWA kreska | od G    


    border[14].setPosition({ AGHcoord.x + Lettersize.x + space, AGHcoord.y + Lettersize.y - AGH_thicknes2 });          //  DOLNA kreska -- od G    
    border[14].setSize({ Lettersize.x, AGH_thicknes2 + tolerance });                             // DOLNA kresa -- od G


    border[15].setSize({ AGH_thicknes2, Lettersize.y / 2.5f });                                   // prawe DOLNE | OD G
    border[15].setPosition({ AGHcoord.x + 2 * Lettersize.x + space - AGH_thicknes2,AGHcoord.y + Lettersize.y / 1.667f });                 // prawe DOLNE| od G


    border[16].setSize({ Lettersize.x - AGH_thicknes2 - space, AGH_thicknes2 });                             // srd kresa -- od G
    border[16].setPosition({ AGHcoord.x + Lettersize.x + 2 * space + AGH_thicknes2, AGHcoord.y + Lettersize.y / 1.667f - AGH_thicknes2 });          //  srd kreska -- od G  


                                                                                                                                                // litle borders around g | the higher ones
    float Gythicknes = (Lettersize.y / 1.667f - AGH_thicknes2 - AGH_thicknes2 - 2 * space);
    float Gythicknes2 = (Lettersize.y - AGH_thicknes2 - Lettersize.y / 1.667f - 2 * space);


    //     (Lettersize.y - AGH_thicknes2 -  Lettersize.y / 1.667f-2*space)

    border[30].setSize({ Lettersize.x - AGH_thicknes2 - space, Gythicknes });
    border[30].setPosition({ AGHcoord.x + Lettersize.x + 2 * space + AGH_thicknes2, AGHcoord.y + AGH_thicknes2 + space });


    // the lower one

    border[35].setSize({ Lettersize.x - 2 * AGH_thicknes2 - 2 * space, Gythicknes2 });
    border[35].setPosition({ AGHcoord.x + Lettersize.x + 2 * space + AGH_thicknes2, AGHcoord.y + Lettersize.y / 1.667f + space });



    ////////////////////////////// H

    border[17].setSize({ AGH_thicknes2,Lettersize.y + tolerance });                           //  LEWA | od H
    border[17].setPosition({ AGHcoord.x + 2 * Lettersize.x + 2 * space, AGHcoord.y });       //  LEWA kreska | od H

    border[18].setSize({ AGH_thicknes2, Lettersize.y + tolerance });                                        // prawe  | OD H
    border[18].setPosition({ AGHcoord.x + 3 * Lettersize.x + 2 * space - AGH_thicknes2,AGHcoord.y });                 // prawe | od H


    border[19].setSize({ Lettersize.x, AGH_thicknes2 + tolerance });                                                    // srd kresa -- od H
    border[19].setPosition({ AGHcoord.x + 2 * Lettersize.x + 2 * space,AGHcoord.y + Lettersize.y / 2.5f });              //  srd kreska -- od H   


    // little border around H - the highres ones
    float Hxthicknes = (Lettersize.x - 2 * AGH_thicknes2 - 3 * space) / 2;

    border[31].setSize({ Hxthicknes, Lettersize.y / 2.5f - space + tolerance });
    border[31].setPosition({ AGHcoord.x + Lettersize.x * 2 + 3 * space + AGH_thicknes2, AGHcoord.y });

    border[32].setSize({ Hxthicknes, Lettersize.y / 2.5f - space + tolerance });
    border[32].setPosition({ AGHcoord.x + Lettersize.x * 2 + 4 * space + AGH_thicknes2 + Hxthicknes, AGHcoord.y });

    // the lower ones
    border[33].setSize({ Hxthicknes, Lettersize.y - Lettersize.y / 2.5f - AGH_thicknes2 - space + tolerance });
    border[33].setPosition({ AGHcoord.x + Lettersize.x * 2 + 3 * space + AGH_thicknes2, AGHcoord.y + Lettersize.y / 2.5f + space + AGH_thicknes2 });

    border[34].setSize({ Hxthicknes, Lettersize.y - Lettersize.y / 2.5f - AGH_thicknes2 - space + tolerance });
    border[34].setPosition({ AGHcoord.x + Lettersize.x * 2 + 4 * space + AGH_thicknes2 + Hxthicknes, AGHcoord.y + Lettersize.y / 2.5f + space + AGH_thicknes2 });

    //////////////////////////////////////////////

     //   INTERN BORDERS  

        // The inner most down border

    float Innerythicknes = (board_size.y - border_thicknes - AGHcoord.y - Lettersize.y - 3 * space) / 2;

    border[20].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[20].setPosition({ AGHcoord.x,AGHcoord.y + Lettersize.y + 2 * space + Innerythicknes });

    border[21].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[21].setPosition({ board_size.x / 2 + space / 2, AGHcoord.y + Lettersize.y + 2 * space + Innerythicknes });




    // The inner litle higher down border
    border[22].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[22].setPosition({ Innerythicknes + space , AGHcoord.y + Lettersize.y + space });

    border[23].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[23].setPosition({ board_size.x / 2 + space / 2, AGHcoord.y + Lettersize.y + space });

    //////////////////////////////////////////////

    // The inner most high border
    border[24].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[24].setPosition({ Innerythicknes + space,AGHcoord.y - 2 * space - 2 * Innerythicknes });

    border[25].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[25].setPosition({ board_size.x / 2 + space / 2, AGHcoord.y - 2 * space - 2 * Innerythicknes });

    // The inner litle higher down border
    border[26].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[26].setPosition({ Innerythicknes + space, AGHcoord.y - space - Innerythicknes });

    border[27].setSize({ board_size.x / 2.f - Innerythicknes - space * 1.5f, Innerythicknes });
    border[27].setPosition({ board_size.x / 2.f + space / 2,AGHcoord.y - space - Innerythicknes });


    ////////////////////////////////////////////
    //////////////////////////////  POINTS



    ////////////////////////////////
    if (!AGHtexture.loadFromFile("binary.jpg")) { std::cout << "Failed to load texture from file "; exit(0); }
    for (int i = 0; i < 35; i++)
    {
        border[i].setFillColor(color);
    }

    //GREEN
    for (int i = 8; i <= 34; i++)
    {
        if (i == 12)i = 31;
        border[i].setFillColor(sf::Color(0, 105, 60));
        border[i].setTexture(&AGHtexture);
    }

    //BLACK
    for (int i = 12; i <= 29; i++)
    {
        if (i == 17)i = 28;
        border[i].setFillColor(sf::Color(255, 255, 255));
        border[i].setTexture(&AGHtexture);
    }

    //RED
    for (int i = 17; i <= 30; i++)
    {
        if (i == 20)i = 30;
        border[i].setFillColor(sf::Color(167, 25, 48));
        border[i].setTexture(&AGHtexture);
    }
    border[35].setFillColor(sf::Color(167, 25, 48));
    border[35].setTexture(&AGHtexture);


}




bool Board::colliding(float radious, sf::Vector2f pointcoord)
{

    for (int i = 0; i < 35; i++)
    {
        sf::Vector2f borderposition = { border[i].getPosition().x + border[i].getSize().x / 2, border[i].getPosition().y + border[i].getSize().y / 2 };
        sf::Vector2f bordersize = border[i].getSize();

        if (
            abs(borderposition.x - (pointcoord.x)) <= radious + bordersize.x / 2 + space / 4 &&
            abs(borderposition.y - (pointcoord.y)) <= radious + bordersize.y / 2 + space / 4
            )return true;
    }
    return false;

}





void Board::put_points_on_board()
{
    if (!foodtexture.loadFromFile("piwo.jpg")) { std::cout << "Failed to load texture from file (FOOD)"; exit(0); }
    sf::CircleShape *temp=new sf::CircleShape;

    for (float j = border_thicknes + space / 2; j < board_size.x - border_thicknes; j += space)
    {
        for (float i = border_thicknes + space / 2; i < board_size.y - border_thicknes; i += (space / 2 + border_thicknes / 2))
        {


            if (!colliding(foodradious-0.01f, { j,i }))
            {
                temp->setOrigin({ foodradious / 2.f,foodradious / 2.f });
                temp->setRadius(foodradious);
                temp->setFillColor(foodcolor);
                temp->setTexture(&foodtexture);
                temp->setPosition({ j,i });
                points.push_back(new sf::CircleShape(*temp));
                
            }
            if (i == space * 2.5f + 3 * border_thicknes)i += (Lettersize.y + space);
        }
        ///////////////////////////////////////////////////////////////////////////
        for (float y = space * 3.f + 3 * border_thicknes + border_thicknes2 / 2.f; y <= space * 3.f + 2.5f * border_thicknes + Lettersize.y; y += (space / 2 + border_thicknes / 2))
        {
            if (!colliding(foodradious-0.01f, { j,y }))
            {
                temp->setOrigin({ foodradious / 2.f,foodradious / 2.f });
                temp->setRadius(foodradious);
                temp->setFillColor(foodcolor);
                temp->setTexture(&foodtexture);
                temp->setPosition({ j,y });
                points.push_back(new sf::CircleShape(*temp));
             
            }
        }



    }


    for (float j = border_thicknes + space / 2; j < board_size.x - border_thicknes; j += space)
    {
        float i = Lettersize.y + space * 3.5f + 3 * border_thicknes + 0.2f;
        if (!colliding(foodradious - 0.01f, { j,i }))
        {
            temp->setOrigin({ foodradious / 2.f,foodradious / 2.f });
            temp->setRadius(foodradious);
            temp->setFillColor(foodcolor);
            temp->setTexture(&foodtexture);
            temp->setPosition({ j,i });
            points.push_back(new sf::CircleShape(*temp));
           
        }
    }

    //36
    points[36]->scale(2,2);
    points[137]->scale(2,2);
    points[132]->scale(2,2);
    points[154]->scale(2,2);






}




void Board::drawing(sf::RenderWindow& window)
{


    for (int i = 0; i < points.size(); i++)
    {
        if(points[i]!=nullptr)
        window.draw(*points[i]);
    }
    for (int i = 0; i < 36; i++)
    {
        window.draw(border[i]);
    }
}




/////////////////////////////////////////////////////ENEMY

///////////////////////////////////////////////////////////////////////////////////////////////


Enemy::Enemy(float radious, sf::Color player_color, sf::Vector2f start_position, Board board, float speed) :
    Player::Player(radious, player_color, start_position, speed),
    board(new Board(board)),
    target(0),
    max_change_target_time(5.f),
    reached_target(true),
    max_chase_time(10.f),
    homenode(17),
    pathcolor(player_color),
    deadenemy(new sf::Texture())
{
    for (int i = 0; i < 55;i++)
        memopath[i] = i;

    if (!player_texture->loadFromFile("enemy"+std::to_string(enemynr)+".png")) 
        exit(0);
    
    if (!deadenemy->loadFromFile("deadenemy.png"))exit(0);
    obiectcolor = player.getFillColor();
    player.setTexture(player_texture);
    player.setTextureRect(sf::IntRect{0,0,300,559});
    time_since_tried_change_dir.restart();
    enemynr++;
}




void Enemy::choose_enemy_direction(sf::CircleShape& enemy, Board& board, int node)
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



void Enemy::control_moving(sf::CircleShape& obiect, Player& pacman, Board& board, int node)
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

    if (pacman.huntmode && objects_colliding(this->player, pacman.player,20.f))
    {
        pacman.points += 20;
        is_dead = true;
        this->player.setTexture(deadenemy);
        target = homenode;
    }
    else if(!pacman.huntmode && objects_colliding(this->player, pacman.player,20.f)) pacman.is_dead=true;

    obiect.move({ speed * player_direction.x, speed * player_direction.y });
}



int Enemy::gettarget(int pacmannode) {
    
    if (is_dead)
        if (reached_target) { 
            is_dead = false; 
            player.setTexture(player_texture);
    }
        else return target;

    
    if (Player::points % 20 < 5 && points>20) {
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








void Enemy::findnodes()
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

                (!Player::colliding(refpoint[nodes], *board, { 1,0 }) || !Player::colliding(refpoint[nodes], *board, { -1,0 })) &&
                (!Player::colliding(refpoint[nodes], *board, { 0,1 }) || !Player::colliding(refpoint[nodes], *board, { 0,-1 }))
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


////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////Djikstra


Djikstra::Djikstra() :
    maxalogotime(0.1f)
{
    setnodes();
    algotime.restart();
}


void Djikstra::init(Enemy& object, int node)
{
    for (int i = 0; i < nodecoords.size(); i++) {
        shrt_way_node[i] = INF;
        object.memopath[i] = i;
        visited[i] = false;
    }
}



void Djikstra::djikstra( std::vector<Enemy>& object, const Player& pacman, const Board& board)
{
    for (int i = 0; i < nodecoords.size(); i++)
        refpoint[i].setFillColor(sf::Color::Transparent);


    nodecoords.push_back(pacman.player.getPosition()); // the nodes for those obiects are changing,therefore the algo has to create a new matrix with distances between nodes every time. Of course it could be optimazed, but for now it's enough
    int pacmannode = nodecoords.size()-1;
    for (int i = 0; i < object.size(); i++)
        nodecoords.push_back(object[i].player.getPosition());



     
    if (algotime.getElapsedTime().asSeconds() > maxalogotime) {
        find_edge_length(object[2].player.getRadius(), &board); // this function is creating a matrix with the distances between the nodes. It's memoraized in the array named edge_length
        
        for (int z = 0; z < object.size(); z++)
        {
            if (object[z].change_target_time.getElapsedTime().asSeconds() > object[z].max_change_target_time || object[z].reached_target){ 
          
                object[z].target = object[z].gettarget(pacmannode);
                object[z].change_target_time.restart();
                object[z].reached_target = false;
            }
            init(object[z], object[z].target);

            object[z].memopath[object[z].target] = object[z].target; // the starting node is the node of the hunter - the enemy
            shrt_way_node[object[z].target] = 0;



            for (int i = 0; i < nodecoords.size(); i++) { // for every node seeking the nearest unvisited node
                int nearestnode = get_nearest_node();
                visited[nearestnode] = true;

                for (int j = 0; j < nodecoords.size(); j++) //then the algo is checking if there is a shoreter path to reach the neighbours of the nearestnode 
                {

                    if (!visited[j] && edge_length[nearestnode][j] != INF && shrt_way_node[j] > shrt_way_node[nearestnode] + edge_length[nearestnode][j]) {
                        object[z].memopath[j] = nearestnode;
                        shrt_way_node[j] = shrt_way_node[nearestnode] + edge_length[nearestnode][j];
                    }
                }

            }
        }
            algotime.restart();
    }

    for (int i = 0; i < object.size(); i++) {
        int temp = object[i].memopath[pacmannode+i+1]; //the shortes path is memoraized ine the memopath array. Every element holds the value of it's parent 
        int target = object[i].target;
        int count = 0;    



        while (temp != target)
        {
            count++;
            if (count > 40)break; // just making sure that the algo will not get stuck in the loop

            refpoint[temp].setFillColor(object[i].pathcolor);

            temp = object[i].memopath[temp];
        }
        count = 0;
        int node = object[i].memopath[pacmannode+i+1];
        if (node == object[i].target)object[i].reached_target = true;
       /* while (node > pacmannode) {
            node = memopath[node];
            if (count++ > 40)break;
        }
       */
        object[i].control_moving(const_cast<Enemy&>(object[i]).player, const_cast<Player&>(pacman), const_cast<Board&>(board), node);
    }




    for (int i = 0; i < object.size() + 1; i++)
        nodecoords.pop_back();
}




void Djikstra::setnodes() {
    // loading node coordinates from Nodecoord file
    if (!nodeset) {
        std::ifstream readfromfile("Nodecoord");
        float x, y;
        int count = 0;
        while (readfromfile >> x >> y) {
            nodecoords.push_back({ (float)x,(float)y });
            refpoint[count].setPosition({ (float)x,(float)y });
            refpoint[count].setRadius(24.8f);
            refpoint[count].setOrigin({ refpoint[count].getLocalBounds().width / 2, refpoint[count].getLocalBounds().height / 2 });
            refpoint[count++].setFillColor(sf::Color::Red);
        }
        nodeset = true;
        readfromfile.close();
    }

    edge_length = new float* [nodecoords.size() + 6];
    //  memopath = new int [nodecoords.size() +5];
    shrt_way_node = new float[nodecoords.size() + 6];
    visited = new bool[nodecoords.size() + 6];
    for (int i = 0; i < nodecoords.size() + 6; i++)
        edge_length[i] = new float[nodecoords.size() + 6];


}





int Djikstra::get_nearest_node()
{
    float mindist = INF;
    int node = 0;
    for (int i = 0; i < nodecoords.size(); i++) {
        if (!visited[i] && shrt_way_node[i] < mindist)
        {
            mindist = shrt_way_node[i];
            node = i;
        }

    }

    return node;

}





bool Djikstra::colliding(sf::CircleShape& obiect, const Board& board, sf::Vector2f dir, sf::Vector2f tolerance)
{

    float radious = obiect.getRadius();
    sf::Vector2f playerposition = obiect.getPosition();
    for (int i = 0; i < 36; i++)
    {
        sf::Vector2f borderposition = { board.border[i].getPosition().x + board.border[i].getSize().x / 2, board.border[i].getPosition().y + board.border[i].getSize().y / 2 };
        sf::Vector2f bordersize = board.border[i].getSize();

        float x = abs(borderposition.x - playerposition.x + dir.x);
        float y = abs(borderposition.y - playerposition.y + dir.y);
        float difx = radious + bordersize.x / 2;
        float dify = radious + bordersize.y / 2;
        if (
            abs(borderposition.x - playerposition.x + dir.x) + tolerance.x < radious + bordersize.x / 2 &&
            abs(borderposition.y - playerposition.y + dir.y) + tolerance.y < radious + bordersize.y / 2
            ) {
            if (x <= tolerance.x || y <= tolerance.y) {
                obiect.move(-dir.x * x, -dir.y * y);
            }
            return true;
        }
    }
    return false;

}





void Djikstra::find_edge_length(float radious, const Board* board)
{
    sf::Vector2f tolerance2 = { 0.3,0.3 };
    sf::Vector2f tolerance1 = { 0.3,0.3 };
    sf::CircleShape temp;
    temp.setRadius(radious);
    temp.setOrigin(temp.getLocalBounds().width / 2, temp.getLocalBounds().height / 2);


    for (int j = 0; j < nodecoords.size(); j++) {
        sf::Vector2u nodenr = { INF,INF };
        sf::Vector2u nodenr2 = { INF,INF };
        shortestconection[0] = INF;
        shortestconection[1] = INF;
        shortestconection[2] = INF;
        shortestconection[3] = INF;
        sf::Vector2f objectpos = nodecoords[j];
        for (int i = 0; i < nodecoords.size(); i++) {
            edge_length[j][i] = INF;




            if (abs(nodecoords[i].x - objectpos.x) <= 2 && objectpos.y < nodecoords[i].y && abs(objectpos.y - nodecoords[i].y) <= shortestconection[0])
            {
                temp.setPosition(objectpos.x, objectpos.y + radious);
                if (!colliding(temp, *board, { 0,0 }, tolerance1)) {  // avoiding setting conections between nodes seperated by walls
                    shortestconection[0] = abs(objectpos.y - nodecoords[i].y);
                    nodenr.y = i;
                }
            }

            if (abs(nodecoords[i].x - objectpos.x) <= 2 && objectpos.y > nodecoords[i].y && abs(objectpos.y - nodecoords[i].y) <= shortestconection[1])
            {
                temp.setPosition(objectpos.x, objectpos.y - radious);
                if (!colliding(temp, *board, { 0,0 }, tolerance1)) {
                    shortestconection[1] = abs(objectpos.y - nodecoords[i].y);
                    nodenr2.y = i;
                }
            }


            if (abs(nodecoords[i].y - objectpos.y) <= 2 && objectpos.x > nodecoords[i].x && abs(objectpos.x - nodecoords[i].x) <= shortestconection[2])
            {
                temp.setPosition(objectpos.x - radious, objectpos.y);
                if (!colliding(temp, *board, { 0,0 }, tolerance2)) {
                    shortestconection[2] = abs(objectpos.x - nodecoords[i].x);
                    nodenr.x = i;
                }

            }
            if (abs(nodecoords[i].y - objectpos.y) <= 2 && objectpos.x < nodecoords[i].x && abs(objectpos.x - nodecoords[i].x) <= shortestconection[3])
            {
                temp.setPosition(objectpos.x + radious, objectpos.y);

                if (!colliding(temp, *board, { 0,0 }, tolerance2)) {
                    shortestconection[3] = abs(objectpos.x - nodecoords[i].x);
                    nodenr2.x = i;
                }

            }


        }
        if (nodenr.y != INF) edge_length[j][nodenr.y] = shortestconection[0];
        if (nodenr2.y != INF) edge_length[j][nodenr2.y] = shortestconection[1];
        if (nodenr.x != INF) edge_length[j][nodenr.x] = shortestconection[2];
        if (nodenr2.x != INF) edge_length[j][nodenr2.x] = shortestconection[3];
        edge_length[j][j] = 0;
    }


    /* for (int i = 0; i < nodecoords.size(); i++) {
         for (int j = 0; j < nodecoords.size(); j++) {
             if (j == 0)std::cout << i << " |   ";

             if(edge_length[i][j]==INF)
             std::cout << 11.f<< " ";
              else std::cout << edge_length[i][j]<< " ";

         }
         std::cout << std::endl;
     }
     std::cout << "\n\n";
     int z = 0;
     for (int i = 0; i < 3; i++) {
         for (int j = 0; j < 3; j++) {
             if (j == 0)std::cout << i << " |   ";

             std::cout << "(" << nodecoords[z].x << "," << nodecoords[z].y << ") ";
             z++;
         }
         std::cout << std::endl;*/
         //}


}




void Djikstra::drawpath(sf::RenderWindow& window) {

    for (int i = 0; i <= nodecoords.size(); i++)
        window.draw(refpoint[i]);
}



Menu::Menu(int* points, sf::Vector2f position) :
    points(new int(*points)),
    heart(new sf::Texture())
{
    textfont.loadFromFile("menu_font.ttf");
    pointinfo.setFont(textfont);
    pointinfo.setPosition(position);
    pointinfo.setString("Your score: " + std::to_string(*points));
    pointinfo.setCharacterSize(50);
    pointinfo.setFillColor(sf::Color::Green);
    if (!heart->loadFromFile("heart.png"))exit(0);
    for (int i = 0; i < 3; i++) {
        sf::Sprite* temp = new sf::Sprite();
        temp->setTexture(*heart);
        temp->setPosition({ board_size.x - 150+50*i,position.y });
        temp->setScale(0.1f,0.1f);
        pacmanlives.push_back(temp);
    }
}


void Menu::drawemenu(sf::RenderWindow& window,int *points) {
    pointinfo.setString("Your score: " + std::to_string(*points));
    window.draw(pointinfo);
    for (int i = 0; i < pacmanlives.size(); i++) {
        if (pacmanlives[i] != nullptr)
        window.draw(*pacmanlives[i]);
    }
}