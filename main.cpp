#include "GL/freeglut.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>
#include "vecmath.h"
using namespace std;

// Globals
int colorswith = 0, COLORNUMBER=4;
float light_lr = 1.0, light_ud = 1.0;
GLfloat rotationAngle = 0.0;
bool isRotating = false;
GLint list_id;
// This is the list of points (3D vectors)
vector<Vector3f> vecv;

// This is the list of normals (also 3D vectors)
vector<Vector3f> vecn;

// This is the list of faces (indices into vecv and vecn)
vector<vector<unsigned> > vecf;


// You will need more global variables to implement color and position changes
    // Here are some colors you might use - feel free to add more DONE
GLfloat diffColors[4][4] = { {1.0, 0.1, 0.0, 1.0},
                             {1.0, 1.0, 1.0, 1.0},
                             {0.1, 0.1, 0.1, 1.0},
                             {0.5, 0.5, 0.5, 1.0} };
GLfloat currColor[4] = { 1.0, 0.1, 0.0, 1.0 };
float zoomWeight = 0.0;
bool leftButton = false;
float startX, startY, rotationX, rotationY;
// These are convenience functions which allow us to call OpenGL 
// methods on Vec3d objects
inline void glVertex(const Vector3f &a) 
{ glVertex3fv(a); }

inline void glNormal(const Vector3f &a) 
{ glNormal3fv(a); }


// This function is called whenever a "Normal" key press is received.
void keyboardFunc( unsigned char key, int x, int y ){
    switch ( key )
    {
    case 27: // Escape key
        exit(0);
        break;
    case 'c':
        // add code to change color here DONE
        colorswith = (colorswith + 1) % COLORNUMBER;
        
        break;
    case 'r':
        isRotating = !isRotating;
        

        break;

    default:
        cout << "Unhandled key press " << key << "." << endl;        
    }

	// this will refresh the screen so that the user sees the color change
    glutPostRedisplay();
}

// This function is called whenever a "Special" key press is received.
// Right now, it's handling the arrow keys.
void specialFunc( int key, int x, int y )
{
    switch ( key )
    {
    case GLUT_KEY_UP:
        // add code to change light position DONE
        light_ud += 0.5;
		break;
    case GLUT_KEY_DOWN:
        // add code to change light position DONE
        light_ud -= 0.5;
		break;
    case GLUT_KEY_LEFT:
        // add code to change light position DONE
        light_lr -= 0.5;
		break;
    case GLUT_KEY_RIGHT:
        // add code to change light position DONE
        light_lr += 0.5;
		break;
    }

	// this will refresh the screen so that the user sees the light position
    glutPostRedisplay();
}




void parseFace(std::vector<unsigned>& face, std::string s) {
    size_t pos = 0;

    while ((pos = s.find('/')) != std::string::npos)
    {


        face.push_back(std::stoi(s.substr(0, pos)));
        s.erase(0, pos + 1);
    }

    face.push_back(std::stoi(s));
}


void loadInput() {
    // load the OBJ file here DONE
    const int MAX_BUFFER_SIZE = 128;
    char buffer[MAX_BUFFER_SIZE];
    while (cin.getline(buffer, MAX_BUFFER_SIZE))
    {
        stringstream ss(buffer);
        string s;

        ss >> s;
        if (s == "v")
        {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];

            vecv.push_back(v);
        }

        if (s == "vn")
        {
            Vector3f v;
            ss >> v[0] >> v[1] >> v[2];
            vecn.push_back(v);
        }

        if (s == "f")
        {
            vector<unsigned> face;
            string group;
            while (ss >> group)
            {
                parseFace(face, group);
            }

            vecf.push_back(face);
        }
    }
}

void drawObject(){
    glPushMatrix();
   
    for (auto face : vecf)
    {
        glBegin(GL_TRIANGLES);
        glNormal3d(vecn[face[2] - 1][0], vecn[face[2] - 1][1], vecn[face[2] - 1][2]);
        glVertex3d(vecv[face[0] - 1][0], vecv[face[0] - 1][1], vecv[face[0] - 1][2]);
        glNormal3d(vecn[face[5] - 1][0], vecn[face[5] - 1][1], vecn[face[5] - 1][2]);
        glVertex3d(vecv[face[3] - 1][0], vecv[face[3] - 1][1], vecv[face[3] - 1][2]);
        glNormal3d(vecn[face[8] - 1][0], vecn[face[8] - 1][1], vecn[face[8] - 1][2]);
        glVertex3d(vecv[face[6] - 1][0], vecv[face[6] - 1][1], vecv[face[6] - 1][2]);
        glEnd();
    }
    glPopMatrix();
}

void init()
{
    list_id = glGenLists(1);
    
    glNewList(list_id, GL_COMPILE);
    drawObject();
    glEndList();

}




// This function is responsible for displaying the object.
void drawScene(void)
{
    int i;

    // Clear the rendering window
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate the image
    glMatrixMode( GL_MODELVIEW );  // Current matrix affects objects positions
    glLoadIdentity();              // Initialize to the identity
    glTranslatef(0.0f, 0.0f, zoomWeight);
    // Position the camera at [0,0,5], looking at [0,0,0],
    // with [0,1,0] as the up direction.
    gluLookAt(0.0, 0.0, 5.0,
              0.0, 0.0, 0.0,
              0.0, 1.0, 0.0);

    // Set material properties of object


	// Here we use the first color entry as the diffuse color
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, currColor);

	// Define specular color and shininess
    GLfloat specColor[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat shininess[] = {100.0};

	// Note that the specular color and shininess can stay constant
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specColor);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
  
    // Set light properties

    // Light color (RGBA)
    GLfloat Lt0diff[] = {1.0,1.0,1.0,1.0};
    // Light position
	GLfloat Lt0pos[] = {light_lr, light_ud, 5.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_DIFFUSE, Lt0diff);
    glLightfv(GL_LIGHT0, GL_POSITION, Lt0pos);

	// This GLUT method draws a teapot.  You should replace DONE
	// it with code which draws the object you loaded.
	//glutSolidTeapot(1.0);
    glRotatef(rotationAngle, 0.f, 1.f, 0.f);//�� ������ r

    glRotatef(rotationX, 0.0f, 1.0f, 0.0f);//�� �����
    glRotatef(rotationY, 1.0f, 0.0f, 0.0f);

    //drawObject(); //������� ���������
   
    glCallList(list_id); //��������� � �������������� OpenGL display lists(��� 2)
    // Dump the image to the screen.
    glutSwapBuffers();


}

// Initialize OpenGL's rendering modes
void initRendering()
{
    glEnable(GL_DEPTH_TEST);   // Depth testing must be turned on
    glEnable(GL_LIGHTING);     // Enable lighting calculations
    glEnable(GL_LIGHT0);       // Turn on light #0.
}

// Called when the window is resized
// w, h - width and height of the window in pixels.
void reshapeFunc(int w, int h)
{
    // Always use the largest square viewport possible
    if (w > h) {
        glViewport((w - h) / 2, 0, h, h);
    } else {
        glViewport(0, (h - w) / 2, w, w);
    }

    // Set up a perspective view, with square aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // 50 degree fov, uniform aspect ratio, near = 1, far = 100
    gluPerspective(50.0, 1.0, 1.0, 100.0);
}



void changeColor() {
    float delta = 0.01, change=0.01;

    for (int i = 0; i < 4; i++) {
        if (fabs(currColor[i] - diffColors[colorswith][i]) > delta) {
            if (currColor[i] < diffColors[colorswith][i])
                currColor[i] += change;
            else
                currColor[i] -= change;
        }
    }
}

void update(int)//��������� � �����������
{
    if (isRotating) rotationAngle += 2;
    changeColor();
   

    glutPostRedisplay();
    glutTimerFunc(15, update, 0);
}


void mouseWheelFunc(int button, int dir, int x, int y){
    if (dir > 0) zoomWeight+=0.1;
    
    else zoomWeight-=0.1;
    
}
void mouseFunc(int button, int state, int x, int y)
{
    startX = x, startY = y;

    leftButton = (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN);
}

void MotionFunc(int x, int y)
{
    if (leftButton)
    {
        rotationX = (x - startX) ;
        rotationY = (startY - y) ;
    }
}

// Main routine.
// Set up OpenGL, define the callbacks and start the main loop
int main( int argc, char** argv )
{
    loadInput();

    glutInit(&argc,argv);

    // We're going to animate it, so double buffer 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );

    // Initial parameters for window position and size
    glutInitWindowPosition( 60, 60 );
    glutInitWindowSize( 360, 360 );
    glutCreateWindow("Assignment 0");

    // Initialize OpenGL parameters.
    initRendering();
    init();
    // Set up callback functions for key presses
    glutKeyboardFunc(keyboardFunc); // Handles "normal" ascii symbols
    glutSpecialFunc(specialFunc);   // Handles "special" keyboard keys
    glutMouseFunc(mouseFunc);
    glutMouseWheelFunc(mouseWheelFunc);
    glutMotionFunc(MotionFunc);
     // Set up the callback function for resizing windows
    glutReshapeFunc( reshapeFunc );
    
    // Call this whenever window needs redrawing
    glutDisplayFunc( drawScene );
    glutTimerFunc(15, update, 0);
    

    // Start the main loop.  glutMainLoop never returns.
    glutMainLoop( );

    return 0;	// This line is never reached.
}
