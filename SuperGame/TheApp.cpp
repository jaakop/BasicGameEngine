#include "TheApp.h"

TheApp::TheApp() :
    m_uVertexShader(0),
    m_uFragmentShader(0),
    m_uProgram(0),
    m_uTexture(0)
{
}

bool TheApp::OnCreate()
{
    auto renderer = GetOpenGLRenderer();
    m_uVertexShader = renderer->CreateVertexShaderFromFile("phongshader.vert");
    m_uFragmentShader = renderer->CreateFragmentShaderFromFile("phongshader.frag");
    m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
    m_uTexture = renderer->CreateTexture("texture.png");

    if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture) 
    {
        return false;
    }

    renderer->SetViewMatrix(
        glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)));
    renderer->SetProjectionMatrix(glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f));

    m_pSphere = std::make_shared<Geometry>();
    m_pSphere->GenSphere(glm::vec3(0.5f, 0.5f, 0.5f));

    return true;
}

void TheApp::OnDestroy()
{
    m_pSphere = nullptr;

    glDeleteTextures(1, &m_uTexture);
    glDeleteProgram(m_uProgram);
    glDeleteShader(m_uFragmentShader);
    glDeleteShader(m_uVertexShader);
}

void TheApp::OnUpdate(float frametime)
{
    Debug(std::string("FPS: ") + std::to_string(1.0f / frametime) + "\n");
}

void TheApp::OnDraw(IRenderer& renderer)
{
    renderer.Clear(0.14f, 0.14f, 0.14f, 1.0f);

    glUseProgram(m_uProgram);

    renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

    m_pSphere->SetAttribs(m_uProgram);

    // set the uniforms
    static float x = 0.0f;
    m_mModel = glm::rotate(glm::mat4(1.0f), x, glm::vec3(0.0f, 1.0f, 0.0f));
    //m_mModel[3][0] = x;
    x += 0.0001f;
    OpenGLRenderer::SetUniformMatrix4(m_uProgram, "modelMatrix", m_mModel);

    glm::mat4 mvp = renderer.GetProjectionMatrix() * renderer.GetViewMatrix() * m_mModel;
    OpenGLRenderer::SetUniformMatrix4(m_uProgram, "modelViewProjectionMatrix", mvp);

    m_pSphere->Draw(renderer);
}
