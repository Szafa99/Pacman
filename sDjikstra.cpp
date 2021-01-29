#include "headers/sDjikstra.h"


sDjikstra::sDjikstra() :
    maxalogotime(0.1f)
{
    setnodes();
    algotime.restart();
}

// inits arrays of data needed for the djikstra algo
void sDjikstra::init(sEnemy& object, int node)
{
    for (int i = 0; i < nodecoords.size(); i++) {
        shrt_way_node[i] = INF;
        object.memopath[i] = i;
        visited[i] = false;
    }
}



void sDjikstra::djikstra(std::vector<sEnemy>& object, const sPlayer& pacman, const sBoard& board)
{
    for (int i = 0; i < nodecoords.size(); i++)
        refpoint[i].setFillColor(sf::Color::Transparent);


    nodecoords.push_back(pacman.player.getPosition()); // the nodes for those obiects are changing,therefore the algo has to create a new matrix with distances between nodes every time. Of course it could be optimazed, but for now it's enough
    int pacmannode = nodecoords.size() - 1;
    for (int i = 0; i < object.size(); i++)
        nodecoords.push_back(object[i].player.getPosition());




    if (algotime.getElapsedTime().asSeconds() > maxalogotime) {
        find_edge_length(object[2].player.getRadius(), &board); // this function is creating a matrix with the distances between the nodes. It's memoraized in the array named edge_length

        for (int z = 0; z < object.size(); z++)
        {
            if (object[z].change_target_time.getElapsedTime().asSeconds() > object[z].max_change_target_time || object[z].reached_target) {

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

// visited is a boolean array which tels us whether a node has been visited
// edge_length[nearestnode][j] != INF - makes sure that the node that we are seeking from is no considered
// shrt_way_node[j] > shrt_way_node[nearestnode] - checks wheter the current shortes parth is smaller, then then path to the considered node plus the distance from it
// to the node "we are in". 
//If those requirements are met the previous node is in the memopath overwirtien with the new node and the shortest path is updatet                  
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
        int temp = object[i].memopath[pacmannode + i + 1]; //the shortes path is memoraized ine the memopath array. Every element holds the value of it's parent 
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
        int node = object[i].memopath[pacmannode + i + 1];
        if (node == object[i].target)object[i].reached_target = true;
     
        object[i].control_moving(const_cast<sEnemy&>(object[i]).player, const_cast<sPlayer&>(pacman), const_cast<sBoard&>(board), node);
    }




    for (int i = 0; i < object.size() + 1; i++)
        nodecoords.pop_back();
}




void sDjikstra::setnodes() {
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

    shrt_way_node = new float[nodecoords.size() + 6];
    visited = new bool[nodecoords.size() + 6];
    for (int i = 0; i < nodecoords.size() + 6; i++)
        edge_length[i] = new float[nodecoords.size() + 6];


}





int sDjikstra::get_nearest_node()
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





bool sDjikstra::colliding(sf::CircleShape& obiect, const sBoard& board, sf::Vector2f dir, sf::Vector2f tolerance)
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





void sDjikstra::find_edge_length(float radious, const sBoard* board)
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



}




void sDjikstra::drawpath(sf::RenderWindow& window) {

    for (int i = 0; i <= nodecoords.size(); i++)
        window.draw(refpoint[i]);
}

