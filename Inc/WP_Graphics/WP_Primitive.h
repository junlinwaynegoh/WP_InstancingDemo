#ifndef WP_PRIMITIVE_H
#define WP_PRIMITIVE_H

#include <glm.hpp>
#include <GL/glew.h>

typedef struct Vertex
{
    glm::vec2 pos;
    glm::vec3 col;
    glm::vec2 texCoord;
} Vertex;

namespace Primitives
{

    struct Primitive
    {
        GLuint vertexArray;
        GLuint indicesSize;
        GLuint vertexBuffer;
        GLuint elementBuffer;

        static Primitive triangle;
        static Primitive square;

        static void CompilePrimitives();


        static Primitive CompileVertices(const Vertex* _vert, GLuint _numberVert, const GLuint* _indices, GLuint _numberIndices);

        static void CleanPrimitives();
        void CleanPrimitive();
    };


}

#endif