#include <main.hpp>

Vertex::Vertex(const glm::vec3& pos, const glm::vec3& norm) : position{pos}, normal{norm} {}

bool Vertex::operator==(const Vertex& other) const {
    return position == other.position && normal == other.normal;
}

Triangle::Triangle(const std::array<size_t, 3>& positions, const std::array<size_t, 3>& normals)
    : vertices{ positions[0], positions[1], positions[2] }, normals{ normals[0], normals[1], normals[2] } {}
    
Triangle::Triangle() : vertices{ 0, 0, 0 }, normals{ 0, 0, 0 } {}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::getMaxVertices() {
    return MAX_VERTICES;
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::getMaxTriangles() {
    return MAX_TRIANGLES;
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::getMaxNormals() {
    return MAX_NORMALS;
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::loadBufferData() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Atribúty pozície a normály
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0); // position
    glEnableVertexAttribArray(0);

    // Vytvoríme EBO (element buffer)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

/*template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::smoothSurface() {
    std::array<std::array<glm::vec3, MAX_NORMALS>, MAX_VERTICES> averagedNormals = {}; // 

    for (size_t i = 0; i < MAX_VERTICES; ++i) {
        glm::vec3 sum(0.0f);

        size_t normalsCount = 0;
        for (size_t j = 0; j < MAX_NORMALS; ++j) {
            if (!comparePositions(normals[i][j], glm::vec3(0.0f)))
                normalsCount++;
            sum += normals[i][j];
        }
        //std::cout << normalsCount << std::endl;
        glm::vec3 average = sum / static_cast<float>(normalsCount);
        averagedNormals[i][0] = glm::normalize(average);
    }

    for (size_t i = 0; i < MAX_TRIANGLES; ++i) {
        triangles[i].normals = { 0, 0, 0 };
    }

    normals = averagedNormals;
}*/

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::drawInstances(size_t instanceCount) {
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instanceCount);
    glBindVertexArray(0);
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::loadBufferData(const std::vector<glm::mat4>& modelMatrices, const std::vector<glm::vec3>& colors) {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Nastavenie základných vertexov
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // Atribúty pozície a normály
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0); // position
    glEnableVertexAttribArray(0);
    // normal delete
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    //glEnableVertexAttribArray(1);

    // Vytvoríme EBO (element buffer)
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Nastavenie model matíc pre inštancie
    glGenBuffers(1, &modelMatrixVBO);
    glBindBuffer(GL_ARRAY_BUFFER, modelMatrixVBO);
    glBufferData(GL_ARRAY_BUFFER, modelMatrices.size() * sizeof(glm::mat4), modelMatrices.data(), GL_STATIC_DRAW);

    for (int i = 0; i < 4; i++) {
        glVertexAttribPointer(2 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * sizeof(glm::vec4)));
        glEnableVertexAttribArray(2 + i);
        glVertexAttribDivisor(2 + i, 1);  // Atribúty inštancií
    }

    // Nastavenie farieb pre inštancie
    glGenBuffers(1, &colorVBO);
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(6);
    glVertexAttribDivisor(6, 1);

    glBindVertexArray(0);  // Odpojenie VAO
}


template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
bool Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::comparePositions(const glm::vec3& position1, const glm::vec3& position2) {
    const float tolerance = 0.0001f;
    return (fabs(position1.x - position2.x) < tolerance) &&
        (fabs(position1.y - position2.y) < tolerance) &&
        (fabs(position1.z - position2.z) < tolerance);
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::findVertex(const glm::vec3& position) {
    for (size_t i = 0; i < vertexCount; ++i) {
        if (comparePositions(vertices[i], position))
            return i;
    }
    return vertices.size();
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addVertex(const glm::vec3& position) {
    if (vertexCount == MAX_VERTICES)
        return vertexCount;

    //std::cout << "pridavam vrchol " << vertexCount << " " << position.x << " " << position.y << " " << position.z << " " << std::endl;
    vertices[vertexCount] = position;
    return vertexCount++;
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::findOrAddVertex(const glm::vec3& position) {
    size_t index = findVertex(position);
    if(index == MAX_VERTICES)
        return addVertex(position);  // TODO check if its added (not == max_vertices)
    else 
        return index;
}

/*template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addNormal(const size_t index, const glm::vec3& newNormal) {
    for (size_t i = 0; i < MAX_NORMALS; ++i) {  //glm::vec3& normal : normals[index]
        if (normals[index][i] == newNormal)
            return i;
        else if (normals[index][i] == glm::vec3(0,0,0)) {
            //std::cout << "pridavam normal na poziciu " << i << std::endl;
            normals[index][i] = newNormal;
            return i;
        }
    }
    return MAX_NORMALS;
}*/

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addTriangle(const std::array<glm::vec3,3>& positions) {

    size_t i = 0;
    for (const glm::vec3& position : positions) {
        // REFACTOR find or add vertex
        unsigned int index = findVertex(position);
        if (index == MAX_VERTICES)
            index = addVertex(position);
        // TODO if index == MAX_VERTICES snazime sa pridat vrchol nad ramec array

        indices[triangleCount*3+i] = index;
        ++i;
    }
    triangleCount++;
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addTriangle(const std::array<unsigned int,3>& positionIndices) {
    size_t i = 0;
    for (const unsigned int index : positionIndices) {
        indices[triangleCount*3+i] = index;
        ++i;
    }
    triangleCount++;
}

/*template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addGeometry(const std::span<glm::vec3>& vertices, const std::span<std::array<unsigned int, 3>>& indices) {
    for (const auto& triangle : indices)
        addTriangle({ vertices[triangle[0]], vertices[triangle[1]], vertices[triangle[2]] });
}*/


Icosphere::Icosphere(size_t subdivision, float radius) : N_SUBDIVISION{subdivision} {
    createGeometry(radius);
    subdivideTriangles(radius);
}

void Icosphere::createGeometry(float radius) {
    float z, xy;
    float uAngle = -M_PI / 2 - H_ANGLE / 2;  // -126 deg (234 deg = 90 + 2*72 )
    float lAngle = -M_PI / 2;               // -90 deg  (270 deg)

    glm::vec3 northPos{0,0,radius};
    glm::vec3 southPos{0,0,-radius};

    for(int i = 1; i <= 5; ++i) {
        z  = radius * sinf(V_ANGLE);
        xy = radius * cosf(V_ANGLE);

        glm::vec3 upperFirstPos{xy * cosf(uAngle), xy * sinf(uAngle), z};
        glm::vec3 upperSecondPos{xy * cosf(uAngle+H_ANGLE), xy * sinf(uAngle+H_ANGLE), z};

        glm::vec3 lowerFirstPos{xy * cosf(lAngle), xy * sinf(lAngle), -z};
        glm::vec3 lowerSecondPos{xy * cosf(lAngle+H_ANGLE), xy * sinf(lAngle+H_ANGLE), -z};

        this->addTriangle({ northPos, upperFirstPos, upperSecondPos });
        this->addTriangle({ upperFirstPos, lowerFirstPos, upperSecondPos });
        this->addTriangle({ upperSecondPos, lowerFirstPos, lowerSecondPos });
        this->addTriangle({ lowerFirstPos, southPos, lowerSecondPos });

        uAngle += H_ANGLE;
        lAngle += H_ANGLE;
    }
}

glm::vec3 Icosphere::updateRadius(const glm::vec3& position, const float radius) {
    float r = glm::length(position);              // radius
    float phi = atan2(position.y, position.x);   // azimuthal angle
    float theta  = acos(position.z / r);        // polar angle

    float x = radius * sin(theta) * cos(phi);
    float y = radius * sin(theta) * sin(phi);
    float z = radius * cos(theta);

    return glm::vec3{x,y,z};
}

void Icosphere::subdivideTriangles(float radius) {
    //std::vector<Triangle> new_triangles;
    size_t end = this->triangleCount;
    for (size_t t = 0; t < end; t++) {
        //std::cout << t << std::endl;

        unsigned int o0 = this->indices[3*t];
        unsigned int o1 = this->indices[3*t+1];
        unsigned int o2 = this->indices[3*t+2];

        glm::vec3 o0p = this->vertices[o0];
        glm::vec3 o1p = this->vertices[o1];
        glm::vec3 o2p = this->vertices[o2];

        glm::vec3 f = o0p;
        glm::vec3 l = o1p;
        glm::vec3 u = o2p;
        glm::vec3 uf, ul;

        for (size_t d = N_SUBDIVISION; d > 0; d--) {
            if (d == N_SUBDIVISION) {
                glm::vec3 p1 = updateRadius(f + (1.0f / d) * (l - f), radius);
                glm::vec3 p2 = updateRadius(f + (1.0f / d) * (u - f), radius);
                this->indices[3*t+1] = this->findOrAddVertex(updateRadius(p1, radius));
                this->indices[3*t+2] = this->findOrAddVertex(updateRadius(p2, radius));
            }
            else {
                glm::vec3 p1 = f + (1.0f / d) * (l - f);

                glm::vec3 p2 = f + (1.0f / d) * (u - f);

                if (this->comparePositions(p2, o2p)) {
                    //std::cout << p2.x << ", " << p2.y << ", " << p2.z << std::endl;
                    //std::cout << o2p.x << ", " << o2p.y << ", " << o2p.z << std::endl;
                    this->addTriangle({this->findOrAddVertex(updateRadius(f, radius)), this->findOrAddVertex(updateRadius(p1, radius)), o2});
                }
                else {
                    this->addTriangle({updateRadius(f, radius), updateRadius(p1, radius), updateRadius(p2, radius)});
                }
                

            }
            uf = f + (1.0f / d) * (u - f);
            ul = l + (1.0f / d) * (u - l);
            for (size_t i = 1; i < d; i++) {
                this->addTriangle({updateRadius(uf + (static_cast<float>(i - 1) / (d - 1)) * (ul - uf), radius), 
                                    updateRadius(f + (static_cast<float>(i) / d) * (l - f), radius),
                                    updateRadius(uf + (static_cast<float>(i) / (d - 1)) * (ul - uf), radius)});
                this->addTriangle({updateRadius(f + (static_cast<float>(i) / d) * (l - f), radius), 
                                    updateRadius(f + (static_cast<float>(i + 1) / d) * (l - f), radius),
                                    updateRadius(uf + (static_cast<float>(i) / (d - 1)) * (ul - uf), radius)});
            }
            f = uf;
            l = ul;
        }
    }
}
