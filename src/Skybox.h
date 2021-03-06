/*
 * Copyright (c) 2013-2014 Daniel Kirchner
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE ANDNONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef SKYBOX_H
#define SKYBOX_H

#include "common.h"
#include "Texture.h"
#include "ShaderProgram.h"

/** Sky box.
 * This class renders a sky box for the scene.
 */
class Skybox
{
public:
    /** Constructor.
     */
	Skybox (void);
    /** Destructor.
     */
    ~Skybox (void);
    /** Render.
     * Renders the skybox.
     * \param envmap Environment map to use on the sky box.
     */
    void Render (const Texture &envmap);
private:
    /** Shader program.
     * Shader program for displaying the sky box.
    */
    ShaderProgram program;

    union {
        struct {
            /** Vertex buffer.
             *  OpenGL buffer object storing the vertices of the sky box.
             */
            GLuint vertexbuffer;
            /** Normal buffer.
             *  OpenGL buffer object storing the normals of the sky box.
             */
            GLuint normalbuffer;
            /** Texture coordinate buffer.
             *  OpenGL buffer object storing the texture coordinates of the sky box.
             */
            GLuint texcoordbuffer;
            /** Index buffer.
             * OpenGL buffer object storing the vertex indices.
             */
            GLuint indexbuffer;
        };
        /** Buffer objects.
         * The buffer objects are stored in a union, so that it is possible
         * to create/delete all buffer objects with a single OpenGL call.
         */
        GLuint buffers[4];
    };
    /** Vertex array object.
     * OpenGL vertex array object used to store information about the layout and location
     * of the vertex attributes.
     */
    GLuint vertexarray;
};

#endif /* SKYBOX_H */
