#ifndef DISPLAY_H
#define DISPLAY_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "singleton.h"

class Display : public Singleton
{
    public:
        static Display& get();

        void close();

        GLFWwindow* getWindow() { return m_window; }

        int getWidth() { return m_width; }
        int getHeight() { return m_height; }

        void swapBuffers();

        bool shouldClose();

    private:
        Display();

        GLFWwindow* m_window;

        int m_width = 1024;
        int m_height = 1024;
};

#endif /* ifndef DISPLAY_H */
