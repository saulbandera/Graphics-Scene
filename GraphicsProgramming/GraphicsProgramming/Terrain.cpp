#include "ProjectIncludes.h"
#include "Terrain.h"

/////////////////   HELPER FUNCTIONS     ////////////////////
#define BUFFER_OFFSET(i) ((char*)NULL + (i))
inline float GetPercentage(float value, const float min, const float max) {
    value = glm::clamp(value, min, max);
    return (value - min) / (max - min);
}

inline int GetFileLength(std::istream& file) { // Get's the size of the file in bytes
    int pos = file.tellg();
    file.seekg(0, std::ios::end);
    int length = file.tellg();
    file.seekg(pos);
    return length;
}


// creating and deleting buffers
inline void DeleteVertexBuffer(GLuint& vboID) {
    if (vboID != 0) {
        glDeleteBuffers(1, &vboID);
        vboID = 0;
    }
}
inline void CreateVertexBuffer(GLuint& vboID) {
    DeleteVertexBuffer(vboID);
    glGenBuffers(1, &vboID);  
}


inline void DeleteTexture(GLuint& texID) {
    if (texID != 0) {
        glDeleteTextures(1, &texID);
        texID = 0;
    }
}
inline void CreateTexture(GLuint& texID) {
   // DeleteTexture(texID);
    glGenTextures(1, &texID);
}


/////////////////     GET HEIGHT VALUE     ///////////////////
inline float GetHeightValue(const unsigned char* data, unsigned char numBytes) {
     return (unsigned short)(data[1] << 8 | data[0]) / (float)0xffff;
}




///////////////     TERRAIN     //////////////////////

Terrain::Terrain(float heightScale, float blockScale)
    : m_GLVertexBuffer(0)
    , m_GLNormalBuffer(0)
    , m_GLTex0Buffer(0)
    , m_GLIndexBuffer(0)
    ,  m_InverseLocalToWorldMatrix(1)
    , m_HeightmapDimensions(0, 0)
    , m_fHeightScale(heightScale)
    , m_fBlockScale(blockScale)
{}
Terrain::~Terrain() {
    Terminate();
}




///////////////     TERMINATE     ////////////////////

void Terrain::Terminate() {
    DeleteVertexBuffer(m_GLVertexBuffer);
    DeleteVertexBuffer(m_GLNormalBuffer);
    DeleteVertexBuffer(m_GLTex0Buffer);
    DeleteVertexBuffer(m_GLIndexBuffer);
    DeleteTexture(texture);
}




//////////////////    LOAD TEXTURE    /////////////////////

bool Terrain::LoadTexture(const std::string& filename) {
    DeleteTexture(texture);
    texture = SOIL_load_OGL_texture(filename.c_str(), SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, 0);
    return (texture != 0);
}




///////////////////    LOAD HEIGHT MAP    //////////////////////


bool Terrain::LoadHeightmap(const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height) {
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);

    // checking that the file opened
    if (ifs.fail()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;}
    const unsigned int bytesPerPixel = bitsPerPixel / 8;
    const unsigned int expectedFileSize = (bytesPerPixel * width * height);
    const unsigned int fileSize = GetFileLength(ifs);
    //checking that the file is the correct size
    if (expectedFileSize != fileSize) {
        std::cerr << "Expected file size [" << expectedFileSize << " bytes] differs from actual file size [" << fileSize << " bytes]" << std::endl;
        return false;}
    unsigned char* heightMap = new unsigned char[fileSize]; // array of characters
    ifs.read((char*)heightMap, fileSize); // fill the heightmap with characters read from the RAW image file
    //check if we read the file properly
    if (ifs.fail()) {
        std::cerr << "An error occurred while reading from the height map file: " << filename << std::endl;
        ifs.close();
        delete[] heightMap;
        return false;}
    std::cout << "filesize: " << fileSize << "\n";
    std::cout << "bytesPerPixel: " << bytesPerPixel << "\n";
    ifs.close();
    unsigned int numVerts = width * height; // calculate how many vertices we have 
    std::cout << "numVerts: " << numVerts << "\n";
    m_PositionBuffer.resize(numVerts); // resize buffers according to the number of vertices
    m_NormalBuffer.resize(numVerts);
    m_Tex0Buffer.resize(numVerts);
    m_HeightmapDimensions = glm::uivec2(width, height); //set the terrain dimensions to size of the RAW image
    float terrainWidth = (width - 1) * m_fBlockScale; // Size of the terrain
    float terrainHeight = (height - 1) * m_fBlockScale; // Height of the terrain
    std::cout << "Width: " << terrainWidth << "    Height: " << terrainHeight << "\n";
    float halfTerrainWidth = terrainWidth * 0.5f;
    float halfTerrainHeight = terrainHeight * 0.5f;
    for (unsigned int j = 0; j < height; ++j) {
        for (unsigned i = 0; i < width; ++i) {
            unsigned int index = (j * width) + i;
            assert(index * bytesPerPixel < fileSize); 
           
            float heightValue =  GetHeightValue(&heightMap[index * bytesPerPixel], bytesPerPixel); // return a value between 0 and 1 that corresponds to the height

            float S = (i / (float)(width - 1));
            float T = (j / (float)(height - 1));
            float X = (S * terrainWidth) - halfTerrainWidth;
            float Y = heightValue * m_fHeightScale;
            float Z = (T * terrainHeight) - halfTerrainHeight;

            //populating the buffers with the values obtained from the RAW file 
            m_NormalBuffer[index] = glm::vec3(0); 
            m_PositionBuffer[index] = glm::vec3(X, Y, Z);
            m_Tex0Buffer[index] = glm::vec2(S, T);
        }
    }
    std::cout << "\n_______________________________________________________________________________________________________________________\n";
    std::cout << "Terrain has been loaded!" << std::endl;
    delete[] heightMap; // delete the array once we're done with it

    //generate buffers
    GenerateIndexBuffer();
    GenerateNormals();
    GenerateVertexBuffers();
    return true;
}




//////////////////////////|     GENERATE INDEX BUFFER     |//////////////////////////

void Terrain::GenerateIndexBuffer() {
    if (m_HeightmapDimensions.x < 2 || m_HeightmapDimensions.y < 2) {
        return;  // terrain hasn't been loaded
    }
    const unsigned int terrainWidth = m_HeightmapDimensions.x;
    const unsigned int terrainHeight = m_HeightmapDimensions.y;
    const unsigned int numTriangles = (terrainWidth - 1) * (terrainHeight - 1) * 2;     // 2 triangles for every quad of the terrain 
    m_IndexBuffer.resize(numTriangles * 3);   // 3 indices for each triangle in terrain 
    unsigned int index = 0; // Index in the index buffer
    for (unsigned int j = 0; j < (terrainHeight - 1); ++j) {
        for (unsigned int i = 0; i < (terrainWidth - 1); ++i) {
            int vertexIndex = (j * terrainWidth) + i;
            // Top triangle 
            m_IndexBuffer[index++] = vertexIndex;                          
            m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;       
            m_IndexBuffer[index++] = vertexIndex + 1;                       
            // Bottom triangle 
            m_IndexBuffer[index++] = vertexIndex;                           
            m_IndexBuffer[index++] = vertexIndex + terrainWidth;           
            m_IndexBuffer[index++] = vertexIndex + terrainWidth + 1;        
        }
    }
}

//////////////////////////|     GENERATE NORMALS     |//////////////////////////
void Terrain::GenerateNormals() {
    for (unsigned int i = 0; i < m_IndexBuffer.size(); i += 3) {
        //calculate normals
        glm::vec3 v0 = m_PositionBuffer[m_IndexBuffer[i + 0]];
        glm::vec3 v1 = m_PositionBuffer[m_IndexBuffer[i + 1]];
        glm::vec3 v2 = m_PositionBuffer[m_IndexBuffer[i + 2]];
        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));
        m_NormalBuffer[m_IndexBuffer[i + 0]] += normal;
        m_NormalBuffer[m_IndexBuffer[i + 1]] += normal;
        m_NormalBuffer[m_IndexBuffer[i + 2]] += normal;
    }
    const glm::vec3 UP(0.0f, 1.0f, 0.0f);
    for (unsigned int i = 0; i < m_NormalBuffer.size(); ++i) {
        m_NormalBuffer[i] = glm::normalize(m_NormalBuffer[i]);
    }
}




//////////////////////////|     GENERATE VERTEX BUFFERS     |//////////////////////////

void Terrain::GenerateVertexBuffers() {
    //generate the buffer object ID's
    CreateVertexBuffer(m_GLVertexBuffer);
    CreateVertexBuffer(m_GLNormalBuffer);
    CreateVertexBuffer(m_GLTex0Buffer);
    CreateVertexBuffer(m_GLIndexBuffer);

    // Copy data into the vertex buffer objects
    glBindBuffer(GL_ARRAY_BUFFER, m_GLVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_PositionBuffer.size(), &(m_PositionBuffer[0]), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_GLNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * m_NormalBuffer.size(), &(m_NormalBuffer[0]), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_GLTex0Buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * m_Tex0Buffer.size(), &(m_Tex0Buffer[0]), GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_IndexBuffer.size(), &(m_IndexBuffer[0]), GL_DYNAMIC_DRAW);

    
}



//////////////////////////|     RENDER     |//////////////////////////

void Terrain::Render() {

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
        // Texturing Stage 
        glActiveTexture(GL_TEXTURE0);
        glMatrixMode(GL_TEXTURE);

        glPushMatrix();
            glScalef(32.0f, 32.0f, 1.0f);
            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, texture);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, m_GLTex0Buffer);
            glTexCoordPointer(2, GL_FLOAT, 0, BUFFER_OFFSET(0));


            // specifying data 
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            glBindBuffer(GL_ARRAY_BUFFER, m_GLVertexBuffer);
            glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));
            glBindBuffer(GL_ARRAY_BUFFER, m_GLNormalBuffer);
            glNormalPointer(GL_FLOAT, 0, BUFFER_OFFSET(0));


            // rendering vertices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_GLIndexBuffer); 
            glDrawElements(GL_TRIANGLES, m_IndexBuffer.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0)); 


            // dereferencing
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glDisableClientState(GL_NORMAL_ARRAY);
            glDisableClientState(GL_VERTEX_ARRAY);
            glActiveTexture(GL_TEXTURE0); 
        glPopMatrix();

        glClientActiveTexture(GL_TEXTURE0);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

}



