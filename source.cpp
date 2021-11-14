#include <iostream>
#include "resources/consolelib.h"
#include "resources/Position2D.h"
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <cstdlib>

#define width 118
#define height 28
using namespace std;

map<string,bool> wallCollider2D; 
vector<string> wallColliderArray;
vector<Position2D> snake;
vector<string> menuOption = {"       EASY        ", "      NORMAL       ","       HARD        ", "      MASTER       ","       EXIT        "};
                
int timeDelay,indexMenu;
Position2D direction(2,0);
Position2D food(0,0);
int score;

int Menu();
int MenuOption();
void RenderTitleGame(int,int);
int PlayGame();
bool FoodRender();
int GetHighScore(int);
void SnakeRender();
void InitSnake();
void Move();
bool SnakeCollisionEnter2D();
bool SnakeEatFood();
void InitWall();
void ReadWallFromFile(string);
void UpdateScore();
bool AddLengthSnake();
void RenderScore();
bool SnakeEatSnake();

int main(){
    ShowCur(false);
    Menu();
    return 0;
}
int Menu(){
    indexMenu = 0;
   
    while (true){
        
        MenuOption();
        if (indexMenu==menuOption.size()-1) exit(-1);
        else {
            switch (indexMenu) {
                case 0:
                    ReadWallFromFile("easy");
                    break;
                case 1:
                    ReadWallFromFile("normal");
                    break;
                case 2:
                    ReadWallFromFile("hard");
                    break;
                case 3:
                    ReadWallFromFile("master");
                    break;
            }
            PlayGame();
        }
        RenderScore();
    }
}
void RenderScore(){
    textcolor(15);
    system("cls");
    gotoXY(47,16+ menuOption.size());
    cout << "Score: ";
    textcolor(3);
    cout << score ;
    textcolor(15);
    cout << " | High Score: ";
    textcolor(4);
    cout << GetHighScore(score);
}
void IndexChoice(byte color,byte color1){
    textcolor(16*color+color1);
    gotoXY(50, 8+indexMenu*2);
    cout << menuOption.at(indexMenu);
}
int MenuOption(){
    ShowCur(false);
    RenderTitleGame(50,5);
    int i = 0;
    IndexChoice(7,16);
    for (auto item : menuOption){
        gotoXY(50,8+i*2);
        cout << item;
        i++;
    }
    gotoXY(46,26);
    textcolor(7);
    cout << "Copyright by nguyenphuc1040";
    IndexChoice(5,15);
    while (true){
        int input = inputKey();
        switch (input){
            case 72:
            case 87:
            case 119:
                IndexChoice(7,16);
                if (indexMenu==0) indexMenu = menuOption.size()-1;  
                    else indexMenu--;
                IndexChoice(5,15);
                break;
            case 80:
            case 83:
            case 115: 
                IndexChoice(7,16);
                if (indexMenu==menuOption.size()-1) indexMenu = 0;
                    else indexMenu++;
                IndexChoice(5,15);
                break;
        }
        if (input==13){
            break;
        }
    }
     textcolor(15);
}
void RenderTitleGame(int x, int y){
    gotoXY(x,y);
    textcolor(15);
    cout << " A HUNTING ";
    textcolor(4);
    cout << "SNAKE ";

}
int PlayGame(){
    ShowCur(false);
    direction.SetXY(2,0);
    snake = {Position2D(13,4), Position2D(11,4), Position2D(9,4), Position2D(7,4),Position2D(5,4),Position2D(3,4)};
    textcolor(15);
    system("cls");
    RenderTitleGame(50,1);
    timeDelay = 150; // time delay processor at the same time speed of snake
    score = -1;
    UpdateScore();
    InitWall();
    InitSnake();
    FoodRender();
    while (true){
        Move();
        if (SnakeEatFood()) {
            FoodRender();
            UpdateScore();
            AddLengthSnake();
        }
        if (SnakeCollisionEnter2D()){
            return false;
        }
        if (SnakeEatSnake()){
            return false;
        }
    }
}
void UpdateScore(){
    score ++;
    timeDelay -= timeDelay>40 ? 5 : 0;
    gotoXY(4,1); 
    textcolor(15);
    cout << "Score: " << score;
}
void ReadWallFromFile(string x){
    ifstream fi("graphics/map"+x+".txt");
    wallColliderArray.clear();
    while (true){
        string line;
        getline(fi,line);
        if (fi.eof()) break;
        wallColliderArray.push_back(line);
    }
    fi.close();
}
void InitWall(){
    // Height 29 Width 118?
    
    wallCollider2D.clear();
    for (int y=0; y<wallColliderArray.size(); y++){
        for (int x=0; x<wallColliderArray[y].length(); x++){
            if (wallColliderArray[y][x]=='1') {
                textcolor(8*16);
                gotoXY(x,y);
                cout << " "; // Print Wall
                wallCollider2D[to_string(x)+";"+to_string(y)] = true; // Wall Collider 
            } else
            if (wallColliderArray[y][x]!='0') {
                textcolor(6);
                gotoXY(x,y);
                cout << wallColliderArray[y][x];
            }
        }
    }
}
int GetHighScore(int score){
    ifstream fi("storage/highscore.txt");
    int bestScore;
    fi >> bestScore;
    fi.close();
    if (bestScore<score) {
        ofstream fo("storage/highscore.txt");
        fo << score;
        fo.close();
        return score;
    }
    return bestScore;
}

void InitSnake(){
    textcolor(7*16);
    for (Position2D position : snake){
        gotoXY(position.x, position.y);
        cout <<"  " ;
    }
    textcolor(4*16);
    gotoXY(snake.at(0).x,snake.at(0).y);
    cout << "  ";
}
void Move(){
    int key = inputKey();
    switch (key){
        // up
        case 72:
        case 87:
        case 119: 
            if (direction.y == 0){
                direction.SetXY(0,-1);
            }
            break;
        // down
        case 80:
        case 83:
        case 115: 
            if (direction.y == 0){
                direction.SetXY(0,1);
            }
            break;
        // left
        case 75:
        case 97:
        case 65: 
            if (direction.x == 0){
                direction.SetXY(-2,0);
            }
            break;
        // right
        case 77:
        case 100:
        case 68: 
            if (direction.x == 0){
                direction.SetXY(2,0);
            }
            break;
    }
    SnakeRender();
}
void SnakeRender(){
    Sleep(timeDelay);
    // destroy tail snake
    textcolor(0*16);
    gotoXY(snake.back().x,snake.back().y);
    cout << "  ";

    // move body snake
    for (int i=snake.size()-1; i>0; i--){
        snake[i] = snake[i-1];
    }

    // render body snake
    textcolor(7*16);
    for (Position2D position : snake){
        gotoXY(position.x, position.y);
        cout << "  ";
    }

    // move header snake
    snake[0] += direction;

    // render header snake
    textcolor(4*16);
    gotoXY(snake.front().x, snake.front().y);
    cout << "  ";
}
bool AddLengthSnake(){
    snake.push_back(snake.back());
    snake.push_back(snake.back());
    return true;
}
bool SnakeCollisionEnter2D(){
    if (wallCollider2D[to_string(snake.front().x) + ";" + to_string(snake.front().y)]){
        return true;
    }
    return false;
}

bool FoodRender(){
    while (true){
        // x = 3 -> 116 y = 4 -> 27
        int x = rand() % 114 + 3;
        int y = rand() % 24 + 4;
        if (x%2==0) x++;
        Position2D xy(x,y);
        bool checkPositionSpawn = false;
        for (Position2D position : snake){
            if (xy==position || wallCollider2D[to_string(x)+";"+to_string(y)]) {
                checkPositionSpawn = true;
                break;
            }
        }
        if (!checkPositionSpawn) {
            food.SetXY(x,y);
            gotoXY(x,y);
            textcolor(5*16);
            cout << "  ";
            return true;
        }
    }
}

bool SnakeEatFood(){
    if (snake.front() == food ) return true;
    return false;
}

bool SnakeEatSnake(){
    for (int i=1; i<snake.size(); i++){
        if (snake.at(i) == snake.front()) return true;
    }
    return false;
}