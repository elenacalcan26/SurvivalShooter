#include "SurvivalShooter/createObjects2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"

using namespace std;

Mesh* createObjects2D::CreateSquare(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* createObjects2D::CreateCircle(const std::string& name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    vector<VertexFormat> vertices;
    vector<unsigned int> indices;

    vertices.push_back(VertexFormat(glm::vec3(0), glm::vec3(1, 1, 1), color));

    // 40 nr triunghiuri
    for (int i = 0; i < NUM_TRIANGLES; i++) {
        float theta = TWO_PI * i / NUM_TRIANGLES;

        // raza = length
        vertices.push_back(VertexFormat(glm::vec3(cos(theta) * length, sin(theta) * length, 0), color));
        indices.push_back(i);
    }

    indices.push_back(40);
    indices.push_back(1);

    Mesh* circle = new Mesh(name);
    circle->InitFromData(vertices, indices);
    circle->SetDrawMode(GL_TRIANGLE_FAN);

    return circle;
}

Mesh* createObjects2D::CreateHealthBar(const std::string& name, glm::vec3 leftBottomCorner, float health, glm::vec3 color, bool fill)
{

    glm::vec3 corner = leftBottomCorner;
    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(glm::vec3(0, 50, 0), color),
        VertexFormat(glm::vec3(health, 50, 0), color),
        VertexFormat(glm::vec3(health, 0, 0), color)
    };

    std::vector<unsigned int> indices = { 0 , 1, 2, 3 };

    Mesh* bar = new Mesh(name);

    if (!fill)
    {
        bar->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        indices.push_back(0);
        indices.push_back(2);
    }

    bar->InitFromData(vertices, indices);
    return bar;
}
