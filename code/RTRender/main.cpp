// opengl_cube_macos.c
// Simple "Hello World" OpenGL program for macOS that displays a rotating colored cube
// Uses legacy OpenGL (immediate mode) + GLUT so it requires no extra loader libraries.
// Compile on macOS with:
//    clang -framework OpenGL -framework GLUT main.cpp -o opengl_cube
// Run with:
//    ./opengl_cube

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

static float angle = 0.0f; // rotation angle (degrees)

// Draw a unit cube centered at the origin using immediate mode, coloring each face.
static void draw_colored_cube(void) {
    glBegin(GL_QUADS);
    // +X face (right) - red
    glColor3f(1.0f, 0.0f, 0.0f);
    glNormal3f(1, 0, 0);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);

    // -X face (left) - green
    glColor3f(0.0f, 1.0f, 0.0f);
    glNormal3f(-1, 0, 0);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // +Y face (top) - blue
    glColor3f(0.0f, 0.0f, 1.0f);
    glNormal3f(0, 1, 0);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);

    // -Y face (bottom) - yellow
    glColor3f(1.0f, 1.0f, 0.0f);
    glNormal3f(0, -1, 0);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);

    // +Z face (front) - magenta
    glColor3f(1.0f, 0.0f, 1.0f);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f, -0.5f,  0.5f);
    glVertex3f( 0.5f,  0.5f,  0.5f);
    glVertex3f(-0.5f,  0.5f,  0.5f);

    // -Z face (back) - cyan
    glColor3f(0.0f, 1.0f, 1.0f);
    glNormal3f(0, 0, -1);
    glVertex3f( 0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f,  0.5f, -0.5f);
    glVertex3f( 0.5f,  0.5f, -0.5f);
    glEnd();
}

static void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Move back a bit and rotate the cube so it's visible
    glTranslatef(0.0f, 0.0f, -3.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);

    draw_colored_cube();

    glutSwapBuffers();
}

static void reshape(int w, int h) {
    if (h == 0) h = 1;
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Use a simple perspective projection
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

static void idle(void) {
    // Update rotation and request redraw
    angle += 0.4f;
    if (angle >= 360.0f) angle -= 360.0f;
    glutPostRedisplay();
}

static void keyboard(unsigned char key, int x, int y) {
    (void)x; (void)y;
    if (key == 27) { // ESC
        exit(0);
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200, 200);
    glutCreateWindow("OpenGL: Rotating Cube - macOS (GLUT)");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);

    // Nice perspective calculations
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
