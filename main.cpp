#include <GL/glut.h>
#include <math.h> 
#include "game.h"
#include <time.h>
#include <iostream>

// angle of rotation for the camera direction
float angle = 0.0;
float lower_arm_angle = 0.0;
float upper_arm_angle = 0.0;

// XZ position of the camera
float camera[3] = {0.0f, 20.0f, 30.0f};
float r = sqrt(15*15 + 30*30);

#define lower_arm 1
#define upper_arm 2
#define base 3

Game game;

int counter = 10;

void drawGridCube(){
    glColor3f(1.0f, 1.0f, 1.0f);
    glutWireCube(1.0f);
}
void drawGrid(){
    glPushMatrix();
    glTranslatef(-5.0f, 2.0f, 0);
    
	for(int y = 0; y < 20; y++){
        glPushMatrix();
		for(int x=0; x < 10; x++) {
			glTranslatef(1.0f, 0, 0);
            if(game.getOccupied(x, y) ^ game.getCurrent(x, y)){
                glColor3f(game.getRed(x, y), game.getGreen(x, y), game.getBlue(x, y));
                glutSolidCube(1.0f);
            }
            if(game.getCurrent(x, y) && game.getOccupied(x, y)){
                glColor3f(0.7f, 0.7f, 0.7f);
                glutSolidCube(1.0f);
            }
			drawGridCube();
		}
        glPopMatrix();
        glTranslatef(0, 1.0f, 0);
    }
    glPopMatrix();

}
void drawGrabbedBlock(){
    GLfloat matrix[16];
            glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
            std::cout << "_____________________________________________" << std::endl;
            std::cout << matrix[0] << " " << matrix[1] << " " << matrix[2] << " " << matrix[3] << std::endl <<
                         matrix[4] << " " << matrix[5] << " " << matrix[6] << " " << matrix[7] << std::endl <<
                         matrix[8] << " " << matrix[9] << " " << matrix[10] << " " << matrix[11] << std::endl <<
                         matrix[12] << " " << matrix[13] << " " << matrix[14] << " " << matrix[15] << std::endl;
            std::cout << "_____________________________________________" << std::endl;
            
            int curr_x = (int) round(matrix[12]);
            int curr_y = (int) round(matrix[13]);

            //std::cout << curr_x << " " << curr_y << std::endl;
            game.setCurrent((curr_x + 5), curr_y + 8);
            // glPushMatrix();	
            //     //glTranslatef(2, 2, 0);
            //     for(int y=-2; y<2; y++){
            //         glPushMatrix();
            //         for(int x = -2; x<2; x++){
            //             glTranslatef(1.0f, 0, 0);
            //             glColor3f(0.7f, 0.7f, 0.7f);
            //             if()
            //             glutSolidCube(1.0f);
            //         }
            //         glPopMatrix();
            //         glTranslatef(0, 1.0f, 0);
            //     }
            // glPopMatrix();
}
void drawArm(){
    glPushMatrix();
        glTranslatef(-10,4,0);
        glRotatef(lower_arm_angle,0,0,1);
        glTranslatef(10,-4,0);
        glPushMatrix();
            glCallList(lower_arm);
        glPopMatrix();

        glPushMatrix();
            glTranslatef(-10,14,0);
            glRotatef(upper_arm_angle,0,0,1);	
            glTranslatef(10,-14,0);
            glPushMatrix();
                glCallList(upper_arm);
            glPopMatrix();
             //draw attached block
            //get current matrix
            glTranslatef(-10, 26, 0);
            glRotatef(-upper_arm_angle,0,0,1);	
            glRotatef(-lower_arm_angle,0,0,1);
            //glTranslatef(10, -24, 0);
            drawGrabbedBlock();
        glPopMatrix();
    glPopMatrix();
}
void drawBase(){
    glPushMatrix();
        glTranslatef(-10, 2, 0);
        glScalef(7, 4, 7);
        glColor3ub(255, 178, 102);
        glutSolidCube(1);
    glPopMatrix();
}
void structLowerArm(){
    glNewList(lower_arm, GL_COMPILE);
	glTranslatef(-10,9,0.0);
	glScalef(1,10,1);
	glColor3ub(255,255,255);
	glutSolidCube(1.0);
    glEndList();
}
void structUpperArm(){
    glNewList(upper_arm, GL_COMPILE);	
	glTranslatef(-10, 19,0.0);
	glScalef(1,10.0,1.0);
	glColor3ub(255,0,0);
	glutSolidCube(1.0);
	
	glScalef(.5,1.0,1.0);
	glTranslatef(0.0,0.4,0.0);
	glutSolidCube(.5);
	
	glTranslatef(0.0,0.25,0.0);
	glutSolidCube(.3);
	glEndList();

}
void initStructures(){
    structLowerArm();
    structUpperArm();
}
void text(){
    glRasterPos2f(-1, 21.7);
    char text[32];
    sprintf(text, "Dropping in:%d", counter);
    glColor3f(1, 1, 1);
    for(int i = 0; text[i] != '\0'; i++)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);

}

void renderScene(void) {

	// Clear Color and Depth Buffers

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    text();
	// Reset transformations
	glLoadIdentity();
    
    gluLookAt(
        camera[0], camera[1], camera[2], /* look from camera XYZ */ 
        0.0f, 10.0f,  0.0f,
        0.0f, 1.0f, 0.0f); /* positive Y up vector */ 

    //glRotatef(angle, 0.0f, 1.0f, 0.0f);/* orbit the Y axis */

    //Draw Grid boxes
    drawGrid();

    //Draw arm
    
    drawArm();
    drawBase();
	glutSwapBuffers();
}


void changeSize(int w, int h){
    if(h == 0){
        h = 1;
    }
    float ratio = 1.0*w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45, ratio, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

//timer function that moves pieces down every 500ms 
void timer(int id){
    if(game.getGameOver()){
        counter = 10;
        game.restart();
        glutPostRedisplay();
        
    }
    if(id == 0){ //beginning of game
        game.spawnFruits();
        glutPostRedisplay();
    }
    if(counter ==0){
        counter = 10;
        game.dropFruit();
        glutPostRedisplay();
        
    }
    counter -= 1;
    glutPostRedisplay();
    
    glutTimerFunc(1000, timer, id+1);
}

//take keyboard input to move block
void transform(int key, int x, int y){

    int mod;
    switch(key){
        case GLUT_KEY_UP:
            game.rotate();
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT:
            mod = glutGetModifiers();
            if(mod == GLUT_ACTIVE_CTRL){
                angle -= 50.0f;
                camera[0] = r*sin(angle);
                camera[2] = r*cos(angle);
                break;
            }
            //game.translate(-1);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT:
            mod = glutGetModifiers();
            if(mod == GLUT_ACTIVE_CTRL){
                angle += 50.01f;
                camera[0] = r*sin(angle);
                camera[2] = r*cos(angle);
			    break;
            }
            //game.translate(1);
            glutPostRedisplay();
            break;
        // case GLUT_KEY_DOWN:
        //     //game.moveDown();
        //     game.dropFruit();
        //     glutPostRedisplay();
        //     break;


    }
}


void keyboard(unsigned char key, int x, int y){
    switch(key){
        case 'q':
            exit(0);
        case 'r':
            counter = 10;
            game.restart();
            break;
        case 'a':
            lower_arm_angle+=2.0f;
            if (lower_arm_angle>90.0f) lower_arm_angle=90.0f;
            break;
        case 'd':
            lower_arm_angle-=2.0f;
            if (lower_arm_angle<-90.0f) lower_arm_angle=-90.0f;
            break;
        case 's':
            upper_arm_angle-=2.0f;
            if (upper_arm_angle<-180.0f) upper_arm_angle=-180.0f;
            break;
        case 'w':
            upper_arm_angle+=2.0f;
            if (upper_arm_angle>180.0f) upper_arm_angle=180.0f;
            break;
        case 32:
            game.dropFruit();
            game.spawnFruits();
            glutPostRedisplay();
            break;
    }
}
int main(int argc, char **argv){
    srand(time(NULL));

    //init GLUT and create window
    glutInit(&argc, argv);
    glutInitWindowPosition(100, 500);
    glutInitWindowSize(1920, 1080);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("3D Tetris");
    initStructures();
    //register callbacks
    glutTimerFunc(1000, timer, 0);
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    
    glutSpecialFunc(transform);
    glutKeyboardFunc(keyboard);
    glEnable(GL_DEPTH_TEST);

    //enter GLUT event processing cycle
    glutMainLoop();

    return 1; 
}
