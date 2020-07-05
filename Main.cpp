#include "headers/sBoard.h"
#include "headers/pawns.h"
#include "headers/sMenu.h"
#include "headers/sDjikstra.h"

std::vector<sf::Vector2f> sDjikstra::nodecoords;
sf::CircleShape sDjikstra::refpoint[2000];
bool sDjikstra::nodeset = false;
sf::Clock sDjikstra::algotime;
bool sEnemy::chasemode(false);
unsigned int sEnemy::enemynr;
int sPlayer::points;
sf::Clock sEnemy::chasetime;

void drawenemy(sf::RenderWindow& window, std::vector<class sEnemy>& enemys, bool pacmanmode) {
    for (int i = 0; i < enemys.size(); i++) {
        if (pacmanmode && enemys[i].flash_time.getElapsedTime().asSeconds() > 0.2f) {
            if (enemys[i].player.getFillColor() == sf::Color{ 6, 209, 121 })enemys[i].player.setFillColor({ 90, 94, 92 });
            else if (enemys[i].player.getFillColor() == sf::Color{ 90, 94, 92 })enemys[i].player.setFillColor({ 6, 209, 121 });
            else enemys[i].player.setFillColor({ 6, 209, 121 });
            enemys[i].flash_time.restart();
        }
        else if (!pacmanmode) 
            enemys[i].player.setFillColor(enemys[i].obiectcolor);
        
        if (!pacmanmode)enemys[i].flash_time.restart();
        window.draw(enemys[i].player);
    }
}

void freeze() {
    sf::Clock freeze;
    freeze.restart();
    while (freeze.getElapsedTime().asSeconds()<1.5f)
    { }
}

void setdefault(std::vector<sEnemy>& enemys, sPlayer& pacman) {
    for (auto i = 0; i < enemys.size(); i++)
        enemys[i].player.setPosition(enemys[i].startposition);
    pacman.player.setPosition(pacman.startposition);
    pacman.is_dead = false;
    freeze();
}


bool lives_left(sMenu &menu){
    
        for (int i = 0; i < menu.pacmanlives.size(); i++)
            if (menu.pacmanlives[i] != nullptr) {
                delete menu.pacmanlives[i];
                menu.pacmanlives[i] = nullptr;
     if (menu.pacmanlives[2] == nullptr) return false;
     else return true;
    }
}


int main()
{
    sf::RenderWindow window(sf::VideoMode(windows_size.x,windows_size.y), "Pacman 4.0", sf::Style::Close);
    sf::Event evnt;
    sf::Texture playertexture;


    sPlayer pacman(24.8f, sf::Color(), { 200.f,50.f }, 0.3f); 
    sBoard board(sf::Color(0, 51, 98), borderthicknes, 40.f, { 75.f,225.f }, { 250.f,380.f }, 50.f);
    sDjikstra djikstra;
    std::vector<sEnemy> enemy;
    enemy.push_back({ 24.8f, sf::Color::Red, { 50.f, 200.f }, board, 0.3f });
    enemy.push_back({ 24.8f, {235, 52, 235}, { 50.f, 300.f }, board, 0.3f });
    enemy.push_back({ 24.8f, {52, 214, 235}, { 50.f, 400.f }, board, 0.3f });
    enemy.push_back({ 24.8f, {235, 162, 52}, { 50.f, 500.f }, board, 0.3f });
    
    board.put_points_on_board();
    
    sMenu menu(pacman.get_pointer_to_points(),{100.f,board_size.y+20.f});

    sf::Clock time;

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



        pacman.control_moving(pacman.player, board);
        pacman.keep_player_in_board(pacman.player);
        if (pacman.huntmode) {
            pacman.control_moving(pacman.player, board);
            pacman.keep_player_in_board(pacman.player);
        }
        djikstra.djikstra(enemy, pacman, board);




        board.drawing(window);
      
        //  djikstra.drawpath(window); uncomment in order to visualize djikstra path

        drawenemy(window, enemy, pacman.huntmode);

        window.draw(pacman.player);
        menu.drawemenu(window,pacman.get_pointer_to_points());

        window.display();
        if (count > 1000)
        {
            std::cout << "FPS " << count / time.getElapsedTime().asSeconds() << std::endl;
            time.restart();
            count = 0;
        }

        if (pacman.is_dead)
            if (lives_left(menu))setdefault(enemy, pacman);
            else break;
        window.clear();
        
    }
    return 0;
}
