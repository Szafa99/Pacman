#include "pacman.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(windowsize.x, windowsize.y), "Pacman 4.0", sf::Style::Default);
    sf::Event evnt;


   
    Player pacman(24.8f, sf::Color(), { 50.f,50.f }, 0.3f); // jak zmienisz predkosc na 0.15 to zwieksz promien o 0.1
    Board board(sf::Color(0, 51, 98), borderthicknes, 40.f, { 75.f,225.f }, { 250.f,380.f }, 50.f);
    Enemy enemy(24.8f, sf::Color::Green, { 50.f, 300.f },board, 0.3f);
  
   board.put_points_on_board();
   sf::Texture lol;
   sf::Clock time;
   sf::Clock time2;

   int count = 0;
   while (window.isOpen())
    {
       count++;
        while (window.pollEvent(evnt)) {
            if (evnt.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        if (time2.getElapsedTime().asSeconds() > 0.4f) {
          //  enemy.djikstra(enemy.enemy[0], pacman.player);
            time2.restart();
        }


            pacman.control_moving(pacman.player,board);
            pacman.keep_player_in_board(pacman.player);
            pacman.control_moving(pacman.player,board);
            pacman.keep_player_in_board(pacman.player);
            
            enemy.keep_player_in_board(enemy.enemy[0]);
            enemy.movingenemy(pacman.player);

            
            
            
            window.clear();
            board.drawing(window);

            enemy.drawenemy(window);
            
            window.draw(pacman.player);
            window.display();

                    if (count > 1000) 
                    {
                 //   std::cout << "FPS " << count/time.getElapsedTime().asSeconds()<<std::endl;
                        time.restart();
                        count = 0;
                    }
    }
    return 0;
}




