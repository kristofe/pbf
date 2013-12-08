#include "common.h"
#include "Simulation.h"

/** \file main.cpp
 * Main source file.
 * The source file that contains the main entry point and performs initialization and cleanup.
 */

/** GLFW window.
 * The GLFW window.
 */
GLFWwindow *window = NULL;
/** Simulation class.
 * The global Simulation object.
 */
Simulation *simulation = NULL;

/** GLFW error callback.
 * Outputs error messages from GLFW.
 * \param error error id
 * \param msg error message
 */
void glfwErrorCallback (int error, const char *msg)
{
    std::cerr << "GLFW error: " << msg << std::endl;
}

/** Cursor position.
 * Stores the last known cursor position. Used to calculate relative cursor movement.
 */
glm::dvec2 cursor;

/** Mouse movement callback.
 * GLFW callback for mouse movement events. Passes the event to the Simulation class.
 * \param window GLFW window that produced the event
 * \param x new x coordinate of the cursor
 * \param y new y coordinate of the cursor
 */
void OnMouseMove (GLFWwindow *window, double x, double y)
{
    Simulation *simulation = reinterpret_cast<Simulation*> (glfwGetWindowUserPointer (window));
    simulation->OnMouseMove (x - cursor.x, y - cursor.y);
    cursor.x = x; cursor.y = y;
}

/** Window resize callback.
 * GLFW callback for window resize events. Passes the event to the Simulation class.
 * \param window GLFW window that was resized
 * \param width new window width
 * \param height new window height
 */
void OnResize (GLFWwindow *window, int width, int height)
{
    Simulation *simulation = reinterpret_cast<Simulation*> (glfwGetWindowUserPointer (window));
    simulation->Resize (width, height);
}

/** Inialization.
 * Perform general initialization tasks.
 */
void initialize (void)
{
    // set GLFW error callback
    glfwSetErrorCallback (glfwErrorCallback);

    // specify parameters for the opengl context
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_SAMPLES, 4);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // open a window and an OpenGL context
    window = glfwCreateWindow (1280, 720, "PBF", NULL, NULL);
    if (window == NULL)
        throw std::runtime_error ("Cannot open window.");
    glfwMakeContextCurrent (window);

    // get OpenGL entry points
    glcorewInit ((glcorewGetProcAddressCallback) glfwGetProcAddress);

    // create the simulation class
    simulation = new Simulation ();

    // setup event callbacks
    glfwSetWindowUserPointer (window, simulation);
    glfwGetCursorPos (window, &cursor.x, &cursor.y);
    glfwSetCursorPosCallback (window, OnMouseMove);
    glfwSetFramebufferSizeCallback (window, OnResize);

    // notify the Simulation class of the initial window dimensions
    int width, height;
    glfwGetFramebufferSize (window, &width, &height);
    simulation->Resize (width, height);
}

/** Cleanup.
 */
void cleanup (void)
{
    if (simulation != NULL)
        delete simulation;
    if (window != NULL)
        glfwDestroyWindow (window);
    glfwTerminate ();
}

/** Main.
 * Main entry point.
 * \param argc number of arguments
 * \param argv argument array
 * \returns error code
 */
int main (int argc, char *argv[])
{
    // initialize GLFW
    if (!glfwInit ())
    {
        std::cerr << "Cannot initialize GLFW." << std::endl;
        return -1;
    }

    try {
        // initialization
        initialize ();

        // simulation loop
        while (!glfwWindowShouldClose (window))
        {
            simulation->Frame ();

            glfwSwapBuffers (window);
            glfwPollEvents ();
        }

        // cleanup
        cleanup ();
        return 0;
    } catch (std::exception &e) {
        cleanup ();
        std::cerr << "Exception: " << e.what () << std::endl;
        return -1;
    }
}