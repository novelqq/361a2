#include "game.h"
#include <time.h>
#include <stdlib.h>
#include <ostream>

Game::Game(){
    board.resize(10);
    for(int k = 0; k<10; k++){
        board[k].resize(20);
    }
    for(int y=0; y<20; y++){
        for(int x=0; x<10; x++){
            board[x][y].isOccupied = false;
            board[x][y].isCurrent = false;
        }
    }
}
Game::~Game(){
    board.clear();
}

void Game::restart(){
    board.resize(10);
    for(int k = 0; k<10; k++){
        board[k].resize(20);
    }
    for(int y=0; y<20; y++){
        for(int x=0; x<10; x++){
            board[x][y].isOccupied = false;
            board[x][y].isCurrent = false;
        }
    }
    game_over = false;
    spawnFruits();
}
bool Game::getGameOver(){
    return game_over; 
}
bool Game::getCurrent(int x, int y){
    return board[x][y].isCurrent;
}
bool Game::getOccupied(int x, int y){
    return board[x][y].isOccupied;
}
float Game::getRed(int x, int y){
    return board[x][y].red;
}
float Game::getGreen(int x, int y){
    return board[x][y].green;
}
float Game::getBlue(int x, int y){
    return board[x][y].blue;
}
float Game::getRGB(Color color, int RGB){
    switch(color){
        case purple:
            return colors[0][RGB];
        case red:
            return colors[1][RGB];
        case green:
            return colors[2][RGB];
        case yellow:
            return colors[3][RGB];
        case orange:
            return colors[4][RGB];
    }
    return -1;
}
void Game::setCurrent(int x, int y){
    clearPreviousFrame();
    curr_x = x;
    curr_y = y;
    updateActiveFruits();

}

void Game::spawnFruits(){
    int shape = rand() % 6;
    int rotation = rand() % 4;
    Color color;
    //int position = (rand() % 5) + 2; 
    for(int i = 0; i < 4; i++){
        color = static_cast<Color>(rand() % orange);
        curr_colors[i] = color;
    }
    curr_shape = shape;
    curr_rotation = rotation;
    // if(checkCollision(position, 17, rotation)){
    //     game_over = true;
    // }
    // else{
    //     curr_shape = shape;
    //     curr_rotation = rotation;
    //     curr_x = position;
    //     curr_y = 17;
    //     updateActiveFruits();
    // }


    
}
void Game::rotate(){
    int rotation;
    if(curr_rotation > 0){
        rotation = curr_rotation - 1; 
    }
    else{
        rotation = 3;
    }
    // if(!(checkCollision(curr_x, curr_y, rotation))){
        clearPreviousFrame();
        curr_rotation = rotation;
        updateActiveFruits();
    //}
}
void Game::translate(int direction){
    int new_x = curr_x + direction;
    if(!(checkCollision(new_x, curr_y, curr_rotation))){
        clearPreviousFrame();
        curr_x = new_x;
        updateActiveFruits();
    }

}
void Game::moveDown(){
    if(!(checkCollision(curr_x, curr_y - 1, curr_rotation))){
        clearPreviousFrame();
        curr_y -= 1;
        updateActiveFruits();
    }
    else{
        freezeCurrent();
        clearLines();
        spawnFruits();
    }
}
void Game::freezeCurrent(){
    board[curr_x][curr_y].isOccupied = true;
    board[curr_x][curr_y].isCurrent = false;
    board[curr_x][curr_y].red = getRGB(curr_colors[0], 0);
    board[curr_x][curr_y].green = getRGB(curr_colors[0], 1);
    board[curr_x][curr_y].blue = getRGB(curr_colors[0], 2);
    int k = 1;
    for(int i = 1; i < 6; i+=2){
        board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isOccupied = true;
        board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isCurrent = false;
        board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].red = getRGB(curr_colors[k], 0);
        board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].green = getRGB(curr_colors[k], 1);
        board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].blue = getRGB(curr_colors[k], 2);
        k+=1;
    }
}
void Game::clearLines(){
    checkRow();
    //checkFruits();
}

void Game::checkRow(){
    bool full;
    for(int y=0; y<20; y++){
        full = true;
        for(int x=0; x<10; x++){
            if(!(board[x][y].isOccupied)){
                full = false;
                break;
            }
        }
        if(full){
            deleteRow(y);
            y--;
        }
    }
}
void Game::deleteRow(int y){
    for(int k = y; k<19; k++){
        for(int x = 0; x<10; x++){
            board[x][k].isOccupied = board[x][k+1].isOccupied;
            board[x][k].isCurrent = board[x][k+1].isCurrent;
            board[x][k].red = board[x][k+1].red;
            board[x][k].green = board[x][k+1].green;
            board[x][k].blue = board[x][k+1].blue;
        }
    }
    for(int x = 0; x<10; x++){
        board[x][19].isOccupied = false;
        board[x][19].isCurrent = false;
    }
}
void Game::clearPreviousFrame(){
    if(curr_x < 10 && curr_x > -1 && curr_y < 20 && curr_y > -1){
        //board[curr_x][curr_y].isOccupied = false;
        board[curr_x][curr_y].isCurrent = false;
    }
    for(int i = 1; i < 6; i+=2){
        if(curr_x+shapes[curr_shape][curr_rotation][i-1] < 10 && 
            curr_x+shapes[curr_shape][curr_rotation][i-1] > -1 &&
            curr_y+shapes[curr_shape][curr_rotation][i] < 20 &&
            curr_y+shapes[curr_shape][curr_rotation][i] > -1){
                //board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isOccupied = false;
                board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isCurrent = false;
            }
    }
}
bool Game::checkCollision(int x, int y, int rotation){
        int xpos[4] = {x, 
        x+shapes[curr_shape][rotation][0], 
        x+shapes[curr_shape][rotation][2], 
        x+shapes[curr_shape][rotation][4]
    };
    int ypos[4] = {y,
        y+shapes[curr_shape][rotation][1],
        y+shapes[curr_shape][rotation][3],
        y+shapes[curr_shape][rotation][5]
    };
    for(int i=0; i<4; i++){
        if(xpos[i] > 9 || xpos[i] < 0 || ypos[i] > 19 || ypos[i] < 0){
            return true;
        }
        if(board[xpos[i]][ypos[i]].isOccupied){
            return true;
        }
    }
    return false;
    
}
void Game::updateActiveFruits(){
    //board[curr_x][curr_y].isOccupied = true;
    if(curr_x < 10 && curr_x > -1 && curr_y < 20 && curr_y > -1){
        board[curr_x][curr_y].isCurrent = true;
        if(!board[curr_x][curr_y].isOccupied){
            board[curr_x][curr_y].red = getRGB(curr_colors[0], 0);
            board[curr_x][curr_y].green = getRGB(curr_colors[0], 1);
            board[curr_x][curr_y].blue = getRGB(curr_colors[0], 2);
        }
    }
    int k = 1;
    for(int i = 1; i < 6; i+=2){
        if(curr_x+shapes[curr_shape][curr_rotation][i-1] < 10 && 
            curr_x+shapes[curr_shape][curr_rotation][i-1] > -1 &&
            curr_y+shapes[curr_shape][curr_rotation][i] < 20 &&
            curr_y+shapes[curr_shape][curr_rotation][i] > -1){
                //board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isOccupied = true;
                board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isCurrent = true;
                if(!board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].isOccupied){
                    board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].red = getRGB(curr_colors[k], 0);
                    board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].green = getRGB(curr_colors[k], 1);
                    board[curr_x+shapes[curr_shape][curr_rotation][i-1]][curr_y+shapes[curr_shape][curr_rotation][i]].blue = getRGB(curr_colors[k], 2);  
                }
        }
        k+=1;
    }
}
void Game::checkFruit(int x, int y){
    //check below
    if((y-2)>-1){
        if(board[x][y].green == board[x][y-1].green && board[x][y-2].green == board[x][y].green &&
            board[x][y-1].isOccupied && board[x][y-2].isOccupied){
            deleteFruit(x, y, x, y-1, x, y-2);
        }
    }
    //check above
    if((y+2)<20){
        if(board[x][y].green == board[x][y+1].green && board[x][y+2].green == board[x][y].green &&
            board[x][y+1].isOccupied && board[x][y+2].isOccupied){
            deleteFruit(x, y+2, x, y+1, x, y);
        }
    }
    //check forward
    if((x+2)<10){
        if(board[x][y].green == board[x+1][y].green && board[x+2][y].green == board[x][y].green &&
            board[x+1][y].isOccupied && board[x+2][y].isOccupied){
            deleteFruit(x, y, x+1, y, x+2, y);
        }
    }
    
    //check backward
    if((x-2)>(-1)){
        if(board[x][y].green == board[x-1][y].green && board[x-2][y].green == board[x][y].green &&
            board[x-1][y].isOccupied && board[x-2][y].isOccupied){
            deleteFruit(x, y, x-1, y, x-2, y);
        }
    }

    //check diag right up
    if((x+2)<10 && (y-2)>(-1)){
        if(board[x][y].green == board[x+1][y-1].green && board[x+2][y-2].green == board[x][y].green &&
            board[x+1][y-1].isOccupied && board[x+2][y-2].isOccupied){
            deleteFruit(x, y, x+1, y-1, x+2, y-2);
        }
    }
    //check diag right down
    if((x+2)<10 && (y+2)<20){
        if(board[x][y].green == board[x+1][y+1].green && board[x+2][y+2].green == board[x][y].green &&
            board[x+1][y+1].isOccupied && board[x+2][y+2].isOccupied){
            deleteFruit(x, y, x+1, y+1, x-2, y+2);
        }
    }
    //check diag left up
    if((x-2)>(-1) && (y-2)>(-1)){
        if(board[x][y].green == board[x-1][y-1].green && board[x-2][y-2].green == board[x][y].green &&
            board[x-1][y-1].isOccupied && board[x-2][y-2].isOccupied){
            deleteFruit(x, y, x-1, y-1, x-2, y-2);
        }
    }
    //check diag left down
    if((x-2)>(-1) && (y+2)<20){
        if(board[x][y].green == board[x-1][y+1].green && board[x-2][y+2].green == board[x][y].green &&
            board[x-1][y+1].isOccupied && board[x-2][y+2].isOccupied){
            deleteFruit(x, y, x-1, y+1, x-2, y+2);
        }
    }
} 
//checks for consecutive fruit blocks
void Game::checkFruits(){
    for(int y = 0; y<20; y++){
        for(int x = 0; x<10; x++){
            if(board[x][y].isOccupied && !(board[x][y].isCurrent)){
                checkFruit(x, y);
            }
        }
    }
}

//deletes the consecutive fruit and moves the fruit above down
void Game::deleteFruit(int x1, int y1, int x2, int y2, int x3, int y3){
    if(x1 == x2){
        for(int k = y1; k>3; k--){
            shiftColumn(x1, k, 3);
        }
    }
    else{
        for(int k = y1; k>0; k--){
            shiftColumn(x1, k, 1);
        }
        for(int k = y2; k>0; k--){
            shiftColumn(x2, k, 1);
        }
        for(int k = y3; k>0; k--){
            shiftColumn(x3, k, 1);
        }

    }
}
//move tiles down above the selected tile
void Game::shiftColumn(int x, int k, int diff){
    board[x][k].isOccupied = board[x][k-diff].isOccupied;
    board[x][k].isCurrent = board[x][k-diff].isCurrent;
    board[x][k].red = board[x][k-diff].red;
    board[x][k].green = board[x][k-diff].green;
    board[x][k].blue = board[x][k-diff].blue;
}

void Game::dropFruit(){
    if(!(checkCollision(curr_x, curr_y, curr_rotation))){
        freezeCurrent();
        spawnFruits();
    }
    else{
        game_over = true;
    }
}