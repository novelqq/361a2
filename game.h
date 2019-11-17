#include <vector>

//represents a space on the grid
class Space{
    public:
        bool isOccupied; //true if occupied by fruit
        bool isCurrent; //true if part of current moving piece
        float red; //rgb values
        float green;
        float blue;
};



enum Color {purple, red, green, yellow, orange};

//hardcoded displacement from origin for each shape and rotation's 3 fruit blocks (no need to store origin)
const int shapes[6][4][6] = 
{
    {
        {-2, 0, -1, 0, 1, 0}, //I shape 
        {0, -2, 0, -1, 0, 1},
        {2, 0, 1, 0, -1, 0},
        {0, 2, 0, 1, 0, -1}
    },
    {
        {-1, -1, 0, -1, 1, 0}, //S shape
        {1, -1, 1, 0, 0, 1},
        {1, 1, 0, 1, -1, 0},
        {-1, 1, -1, 0, 0, -1}
    },
    {
        {-1, 1, 0, 1, 1, 0}, //reverse S shape
        {-1, -1, -1, 0, 0, 1},
        {1, -1, 0, -1, -1, 0},
        {1, 1, 1, 0, 0, -1}
    },
    {
        {-1, -1, -1, 0, 1, 0}, //L shape
        {1, -1, 0, -1, 0, 1},
        {1, 1, 1, 0, -1, 0},
        {-1, 1, 0, 1, 0, -1}
    },
    {
        {-1, 1, -1, 0, 1, 0}, //reverse L shape
        {-1, -1, 0, -1, 0, 1},
        {1, -1, 1, 0, -1, 0},
        {1, 1, 0, 1, 0, -1}
    },
    {
        {-1, 0, 0, -1, 1, 0}, //T shape
        {0, -1, 1, 0, 0, 1},
        {1, 0, 0, 1, -1, 0},
        {0, 1, -1, 0, 0, -1}
    }
};
//class to represent the game as operated on a 10x20 grid
class Game{
    public:
        Game();
        ~Game();

        std::vector< std::vector<Space> > getBoard();
        bool getGameOver();
        bool getOccupied(int x, int y);
        bool getCurrent(int x, int y);
        float getRed(int x, int y);
        float getGreen(int x, int y);
        float getBlue(int x, int y);
        void spawnFruits();
        void rotate();
        void translate(int direction);
        void moveDown();
        void setCurrent(int x, int y);
        void dropFruit();
        void restart();

    private:
        std::vector< std::vector<Space> > board; 
        int curr_shape;
        int curr_rotation;
        int curr_x;
        int curr_y;
        Color curr_colors[4]; 
        bool game_over = false;

        bool checkCollision(int x, int y, int rotation);
        void updateActiveFruits();
        void clearPreviousFrame();
        float getRGB(Color color, int RGB);
        void freezeCurrent();
        void clearLines();
        void checkRow();
        void deleteRow(int y);
        void checkFruits();
        void checkFruit(int x, int y);
        void deleteFruit(int x1, int y1, int x2, int y2, int x3, int y3);
        void shiftColumn(int x, int k, int diff);

        //fruit types
        const float colors[5][3] = 
        {
            {0.5, 0.0, 0.5}, //purple
            {1.0, 0.01, 0.0}, //red
            {0.0, 0.99, 0.0}, //green
            {1.0, 1.1, 0.0}, //yellow
            {1.0, 0.5, 0.0} //orange
        };

};
