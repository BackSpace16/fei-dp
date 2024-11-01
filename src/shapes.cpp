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
    glDrawArrays(GL_TRIANGLES, 0, triangles.size() * 3);
    glBindVertexArray(0);
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::loadBufferData() {
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    std::vector<Vertex> vertexData;
    for (const Triangle& triangle : triangles) {
        for (size_t i = 0; i < 3; ++i) {
            vertexData.emplace_back(vertexPositions[triangle.vertices[i]], normals[triangle.vertices[i]][triangle.normals[i]]);
        }
    }
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
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
        if (comparePositions(vertexPositions[i], position))
            return i;
    }
    return vertexPositions.size();
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
size_t Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addVertex(const glm::vec3& position) {
    if (vertexCount == MAX_VERTICES)
        return vertexCount;

    //std::cout << "pridavam vrchol " << vertexCount << " " << position.x << " " << position.y << " " << position.z << " " << std::endl;
    vertexPositions[vertexCount] = position;
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

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
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
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addTriangle(const std::array<glm::vec3,3>& positions) {
    std::array<size_t, 3> triangleVertices;
    std::array<size_t, 3> triangleNormals;

    glm::vec3 normal = glm::normalize(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));

    size_t i = 0;
    for (const glm::vec3& position : positions) {
        size_t index = findVertex(position);
        if (index == MAX_VERTICES)
            index = addVertex(position);
        // TODO if index == MAX_VERTICES snazime sa pridat vrchol nad ramec array
        size_t normalIndex = addNormal(index, normal);

        triangleVertices[i] = index;
        triangleNormals[i] = normalIndex;
        i++;
    }
    triangles[triangleCount] = Triangle(triangleVertices, triangleNormals);
    triangleCount++;
}

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addTriangle(const std::array<size_t,3>& positionIndices) {
    std::array<glm::vec3,3> positions{ vertexPositions[positionIndices[0]],
                                        vertexPositions[positionIndices[1]],
                                        vertexPositions[positionIndices[2]] };

    glm::vec3 normal = glm::normalize(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));

    std::array<size_t, 3> triangleNormals;
    size_t i = 0;
    for (const size_t positionIndex : positionIndices) {
        size_t normalIndex = addNormal(positionIndex, normal);

        triangleNormals[i] = normalIndex;
        i++;
    }
    triangles[triangleCount] = Triangle(positionIndices, triangleNormals);
    triangleCount++;
}

// To iste len s dodanym vectorom, nie atribut triedy
/*void addTriangle(std::vector<Triangle>& triangleList, const std::array<glm::vec3,3>& positions) {
    glm::vec3 normal = glm::normalize(glm::cross(positions[1] - positions[0], positions[2] - positions[0]));
    triangleList.emplace_back(positions, normal);
}*/

template<size_t MAX_TRIANGLES, size_t MAX_VERTICES, size_t MAX_NORMALS>
void Mesh<MAX_TRIANGLES, MAX_VERTICES, MAX_NORMALS>::addGeometry(const std::span<glm::vec3>& vertices, const std::span<std::array<unsigned int, 3>>& indices) {
    for (const auto& triangle : indices)
        addTriangle({ vertices[triangle[0]], vertices[triangle[1]], vertices[triangle[2]] });
}

/*float calcSize(Vertex vertex, Vertex vertex2) {
    return sqrt(pow((vertex.position.x - vertex2.position.x),2)
            +pow((vertex.position.y - vertex2.position.y),2)
            +pow((vertex.position.z - vertex2.position.z),2));
}

void calcTriangles() {
    for(Triangle& triangle : triangles) {
        float a = calcSize(triangle.vertices[0], triangle.vertices[1]);
        float b = calcSize(triangle.vertices[1], triangle.vertices[2]);
        float c = calcSize(triangle.vertices[0], triangle.vertices[2]);
        std::cout << a << ", " << b << ", " << c << ", " << std::endl;
    }
}*/

//////// CUbe /////////

Cube::Cube(float size) {
    float halfsize = size / 2;

    std::array<glm::vec3,8> vertices{ 
        glm::vec3(-halfsize, -halfsize, -halfsize),
        glm::vec3(-halfsize,  halfsize, -halfsize),
        glm::vec3( halfsize, -halfsize, -halfsize),
        glm::vec3( halfsize,  halfsize, -halfsize),

        glm::vec3(-halfsize, -halfsize,  halfsize),
        glm::vec3(-halfsize,  halfsize,  halfsize),
        glm::vec3( halfsize, -halfsize,  halfsize),
        glm::vec3( halfsize,  halfsize,  halfsize)
    };

    std::array<std::array<unsigned int,3>,12> indices{{
        {1, 2, 0},
        {2, 1, 3},
        {6, 5, 4},
        {5, 6, 7},
        {2, 7, 6},
        {7, 2, 3},
        {5, 0, 4},
        {0, 5, 1},
        {7, 1, 5},
        {1, 7, 3},
        {0, 6, 4},
        {6, 0, 2}
    }};

    addGeometry(vertices, indices);
}

Icosahedron::Icosahedron(float radius) {
    createGeometry(radius);
    
    //calcTriangles();
}

void Icosahedron::createGeometry(float radius) {
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

        addTriangle({ northPos, upperFirstPos, upperSecondPos });
        addTriangle({ upperFirstPos, lowerFirstPos, upperSecondPos });
        addTriangle({ upperSecondPos, lowerFirstPos, lowerSecondPos });
        addTriangle({ lowerFirstPos, southPos, lowerSecondPos });

        uAngle += H_ANGLE;
        lAngle += H_ANGLE;
    }
}

template<size_t N_SUBDIVISION>
Icosphere<N_SUBDIVISION>::Icosphere(float radius) {
    createGeometry(radius);
    subdivideTriangles(radius);
    //calcTriangles();
}

template<size_t N_SUBDIVISION>
glm::vec3 Icosphere<N_SUBDIVISION>::updateRadius(const glm::vec3& position, const float radius) {
    float r = glm::length(position);              // radius
    float phi = atan2(position.y, position.x);   // azimuthal angle
    float theta  = acos(position.z / r);        // polar angle

    float x = radius * sin(theta) * cos(phi);
    float y = radius * sin(theta) * sin(phi);
    float z = radius * cos(theta);

    return glm::vec3{x,y,z};
}

template<size_t N_SUBDIVISION>
void Icosphere<N_SUBDIVISION>::subdivideTriangles(float radius) {
    //std::vector<Triangle> new_triangles;
    size_t end = this->triangleCount;
    for (size_t t = 0; t < end; t++) {
        //std::cout << t << std::endl;
        Triangle& triangle = this->triangles[t];

        size_t o0 = triangle.vertices[0];
        size_t o1 = triangle.vertices[1];
        size_t o2 = triangle.vertices[2];

        glm::vec3 o0p = this->vertexPositions[o0];
        glm::vec3 o1p = this->vertexPositions[o1];
        glm::vec3 o2p = this->vertexPositions[o2];

        glm::vec3 f = o0p;
        glm::vec3 l = o1p;
        glm::vec3 u = o2p;
        glm::vec3 uf, ul;

        for (size_t d = N_SUBDIVISION; d > 0; d--) {
            if (d == N_SUBDIVISION) {
                glm::vec3 p1 = updateRadius(f + (1.0f / d) * (l - f), radius);
                glm::vec3 p2 = updateRadius(f + (1.0f / d) * (u - f), radius);
                triangle.vertices[1] = this->findOrAddVertex(updateRadius(p1, radius));
                triangle.vertices[2] = this->findOrAddVertex(updateRadius(p2, radius));
                
                glm::vec3 normal = glm::normalize(glm::cross(p1 - f, p2 - f));                        
                this->normals[o0][triangle.normals[0]] = normal;
                triangle.normals[1] = this->addNormal(triangle.vertices[1], normal);
                triangle.normals[2] = this->addNormal(triangle.vertices[2], normal);
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


        /*
        glm::vec3 m1 = p0 + (1.0f / 2.0f) * (p1 - p0);
        glm::vec3 m2 = p0 + (1.0f / 2.0f) * (p2 - p0);
        glm::vec3 m3 = p1 + (1.0f / 2.0f) * (p2 - p1);

        m1 = updateRadius(m1, radius);
        m2 = updateRadius(m2, radius);
        m3 = updateRadius(m3, radius);

        size_t i1 = this->findOrAddVertex(m1);
        size_t i2 = this->findOrAddVertex(m2);
        size_t i3 = this->findOrAddVertex(m3);

        triangle.vertices[1] = i1;
        triangle.vertices[2] = i2;

        glm::vec3 normal = glm::normalize(glm::cross(m1 - p0, m2 - p0));

        this->normals[v0][triangle.normals[0]] = normal;
        size_t n1 = this->addNormal(i1,normal);
        size_t n2 = this->addNormal(i2,normal);
        triangle.normals[1] = n1;
        triangle.normals[2] = n2;

        //triangle.vertices[0].normal = normal;

        //this->vertexPositions[triangle.vertices[1]] = c1;

        //c1 = this->vertexPositions[triangle.vertices[1]];
        //std::cout << c1.x << ", " << c1.y << ", " << c1.z << std::endl;

        //this->vertexPositions[triangle.vertices[2]] = c2;

        //this->normals[triangle.vertices[0]][triangle.normals[0]] = normal;
        //this->normals[triangle.vertices[1]][triangle.normals[1]] = normal;
        //this->normals[triangle.vertices[2]][triangle.normals[2]] = normal;


        //this->addTriangle({v0,i1,i2});

        this->addTriangle({i1,i3,i2});
        this->addTriangle({i1,v1,i3});
        this->addTriangle({i2,i3,v2});
        */
    }
    //std::cout << this->getMaxTriangles() << "   " << this->getMaxVertices() << std::endl;
    //triangles.insert(triangles.end(), new_triangles.begin(), new_triangles.end());
}

template<size_t N_SUBDIVISION>
void Icosphere<N_SUBDIVISION>::createGeometry(float radius) {
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
