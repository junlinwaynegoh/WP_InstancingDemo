#include <WP_Graphics/WP_Primitive.h>
#include <GL/glew.h>

namespace Primitives
{

    Primitive Primitive::triangle;
    Primitive Primitive::square;

    Primitive Primitive::CompileVertices(const Vertex* _vert, GLuint _numberVert, const GLuint* _indices, GLuint _numberIndices)
    {

        GLuint vertex_array;
        glGenVertexArrays(1, &vertex_array);
        glBindVertexArray(vertex_array);

        GLuint vertex_buffer, elementBufferObject;
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, _numberVert * sizeof(Vertex),
            _vert, GL_STATIC_DRAW);

        glGenBuffers(1, &elementBufferObject);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * _numberIndices,
            _indices, GL_STATIC_DRAW);

        GLuint vposbindLocation = 0;
        GLuint vcolbindLocation = 1;
        GLuint vtexbindLocation = 2;

        glEnableVertexAttribArray(vposbindLocation);
        glVertexAttribPointer(vposbindLocation, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, pos));

        glEnableVertexAttribArray(vcolbindLocation);
        glVertexAttribPointer(vcolbindLocation, 3, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, col));

        glEnableVertexAttribArray(vtexbindLocation);
        glVertexAttribPointer(vtexbindLocation, 2, GL_FLOAT, GL_FALSE,
            sizeof(Vertex), (void*)offsetof(Vertex, texCoord));



        Primitive primitive;
        primitive.vertexArray = vertex_array;
        primitive.vertexBuffer = vertex_buffer;
        primitive.elementBuffer = elementBufferObject;
        primitive.indicesSize = _numberIndices;

        return primitive;
    }

    void Primitive::CompilePrimitives()
    {
        const Vertex TriangleVertices[3] =
        {
            { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f },{0,1}},
            { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f },{1,0} },
            { {   0.f,  0.6f }, { 0.f, 0.f, 1.f },{0,0} }
        };
        const Vertex SquareVertices[4] =
        {
            { { -1.f,   -1.f}  , { 1.f,0.f,0.f} , {0,1}},
            { { 1.f ,   -1.f}  , { 0.f,1.f,0.f} , {1,1}},
            { { 1.f ,   1.f}   , { 0.f,0.f,1.f} , {1,0}},
            { { -1.f,   1.f}   , { 1.f,0.f,0.f} , {0,0}}
        };

        const GLuint triangleIndices[3] = {
            0,1,2
        };

        const GLuint squareIndices[6] = {
            0, 1, 2,
            2, 3, 0
        };

        triangle = CompileVertices(TriangleVertices, 3, triangleIndices, 3);
        square = CompileVertices(SquareVertices, 4, squareIndices,6);

    }

    void Primitive::CleanPrimitive()
    {
        glDeleteBuffers(1, &vertexBuffer);
        glDeleteBuffers(1, &elementBuffer);
        glDeleteVertexArrays(1, &vertexArray);
    }

    void Primitive::CleanPrimitives()
    {
        triangle.CleanPrimitive();
        square.CleanPrimitive();
    }
}