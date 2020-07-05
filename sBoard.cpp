#include "headers/sBoard.h"



sBoard::sBoard(sf::Color color, float border_thicknes, float AGH_thicknes2, sf::Vector2f AGHcoord, sf::Vector2f Lettersize, float space) :
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
    if (!AGHtexture.loadFromFile("images/binary.jpg")) { std::cout << "Failed to load texture from file "; exit(0); }
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




bool sBoard::colliding(float radious, sf::Vector2f pointcoord)
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





void sBoard::put_points_on_board()
{
    if (!foodtexture.loadFromFile("images/piwo.jpg")) { std::cout << "Failed to load texture from file (FOOD)"; exit(0); }
    sf::CircleShape* temp = new sf::CircleShape;

    for (float j = border_thicknes + space / 2; j < board_size.x - border_thicknes; j += space)
    {
        for (float i = border_thicknes + space / 2; i < board_size.y - border_thicknes; i += (space / 2 + border_thicknes / 2))
        {


            if (!colliding(foodradious - 0.01f, { j,i }))
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
            if (!colliding(foodradious - 0.01f, { j,y }))
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
    points[36]->scale(2, 2);
    points[137]->scale(2, 2);
    points[132]->scale(2, 2);
    points[154]->scale(2, 2);






}




void sBoard::drawing(sf::RenderWindow& window)
{


    for (int i = 0; i < points.size(); i++)
    {
        if (points[i] != nullptr)
            window.draw(*points[i]);
    }
    for (int i = 0; i < 36; i++)
    {
        window.draw(border[i]);
    }
}




