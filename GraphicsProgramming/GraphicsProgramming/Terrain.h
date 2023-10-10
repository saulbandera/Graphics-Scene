#pragma once;
class Terrain
{
public:
    Terrain(float heightScale = 500.0f, float blockScale = 2.0f);
    virtual ~Terrain();

    void Terminate();
    bool LoadHeightmap(const std::string& filename, unsigned char bitsPerPixel, unsigned int width, unsigned int height); // load RAW file
    bool LoadTexture(const std::string& filename); // load texture
    void Render(); // render terrain
    typedef std::vector<glm::vec3>  PositionBuffer;
    
protected:
    void GenerateIndexBuffer();
    void GenerateNormals();
    void GenerateVertexBuffers(); // Generating the vertex buffer objects from the position, normal, and texture

    
private:
    //buffer objects 
    typedef std::vector<glm::vec3>  NormalBuffer;
    typedef std::vector<glm::vec2>  TexCoordBuffer;
    typedef std::vector<GLuint>     IndexBuffer;

    PositionBuffer  m_PositionBuffer;
    NormalBuffer    m_NormalBuffer;
    TexCoordBuffer  m_Tex0Buffer;
    IndexBuffer     m_IndexBuffer;

    GLuint  m_GLVertexBuffer;
    GLuint  m_GLNormalBuffer;
    GLuint  m_GLTex0Buffer;
    GLuint  m_GLIndexBuffer;

    GLuint texture;
    glm::mat4x4 m_InverseLocalToWorldMatrix;
    glm::uivec2 m_HeightmapDimensions; // The dimensions of the heightmap texture


    float   m_fHeightScale; // The height scale of the terrain
    float   m_fBlockScale; // The size scale of the terrain
};