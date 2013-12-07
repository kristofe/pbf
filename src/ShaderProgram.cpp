#include "ShaderProgram.h"

ShaderProgram::ShaderProgram (void) : program (glCreateProgram ())
{
}

ShaderProgram::~ShaderProgram (void)
{
    glDeleteProgram (program);
}

void ShaderProgram::CompileShader (GLenum type, const std::string &filename)
{
    GLuint shader;

    // load shader source into memory
    std::vector<char> data;
    GLint length;
    {
        std::ifstream f (filename.c_str (), std::ios_base::in);
        if (!f.is_open ())
            throw std::runtime_error (std::string ("Cannot load shader: ") + filename);

        f.seekg (0, std::ios_base::end);
        length = f.tellg ();
        f.seekg (0, std::ios_base::beg);

        data.resize (length);
        f.read (&data[0], length);

        if (f.bad () || f.fail ())
            throw std::runtime_error (std::string ("Cannot load shader: ") + filename);
    }

    // create a shader, specify the source and attempt to compile it
    shader = glCreateShader (type);
    const GLchar *src = (const GLchar*) &data[0];
    glShaderSource (shader, 1, &src, &length);
    glCompileShader (shader);

    // check the compilation status and throw the error log as exception on failure
    GLint status;
    glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &length);
        data.resize (length);
        glGetShaderInfoLog (shader, length, NULL, &data[0]);
        glDeleteShader (shader);
        throw std::runtime_error (std::string ("Cannot compile shader \"") + filename + "\":"
                + std::string (&data[0], length));
    }

    // attach the shader object to the program
    glAttachShader (program, shader);
    // delete the shader object (it is internally stored as long as the program is not deleted)
    glDeleteShader (shader);
}

void ShaderProgram::Link (void)
{
    // attempt to link the program
    glLinkProgram (program);

    // check for errors and throw the error log as exception on failure
    GLint status;
    glGetProgramiv (program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        GLint length;
        std::vector<char> log;
        glGetProgramiv (program, GL_INFO_LOG_LENGTH, &length);
        log.resize (length);
        glGetProgramInfoLog (program, length, NULL, &log[0]);
        throw std::runtime_error (std::string ("Failed to link shader program: ") + std::string (&log[0], length));
    }
}

void ShaderProgram::Use (void) const
{
    glUseProgram (program);
}

GLint ShaderProgram::GetUniformLocation (const char *name) const
{
    return glGetUniformLocation (program, name);
}
