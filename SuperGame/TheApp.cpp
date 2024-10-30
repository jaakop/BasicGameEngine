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
        glm::lookAt(glm::vec3(0.0f, 0.0f, 10.0f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f)));
    renderer->SetProjectionMatrix(glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f));
    
    m_pSphere = std::make_shared<Geometry>();
    m_pSphere->GenSphere(glm::vec3(0.5f, 0.5f, 0.5f));

    m_pMaterial = std::make_shared<Material>();
    m_pMaterial->m_cAmbient = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    m_pMaterial->m_cDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    //m_pMaterial->m_cEmissive = glm::vec4(0.01f, 0.0f, 0.0f, 1.0f);
    //m_pMaterial->m_fSpecularPower = 0;

    m_pSceneRoot = std::make_unique<Node>();

    for (size_t i = 0; i < 50; i++)
    {
        auto node = std::make_shared<GeometryNode>(m_pSphere, m_pMaterial);
        node->SetTexture(0, m_uTexture);

        node->SetPos(glm::vec3(glm::linearRand(-5.0f, 5.0f), glm::linearRand(0.0f, 5.0f), glm::linearRand(-20.0f, 0.0f)));

        node->SetVelocity(glm::vec3(glm::linearRand(-3.0f, 3.0f), glm::linearRand(-3.0f, 3.0f), glm::linearRand(-3.0f, 3.0f)));
        node->SetAcceleration(glm::vec3(0.0f,-9.81f,0.0f));

        m_pSceneRoot->AddNode(node);
    }

    return true;
}

void TheApp::OnDestroy()
{
    m_pSceneRoot = nullptr;
    m_pSphere = nullptr;

    glDeleteTextures(1, &m_uTexture);
    glDeleteProgram(m_uProgram);
    glDeleteShader(m_uFragmentShader);
    glDeleteShader(m_uVertexShader);
}

void TheApp::OnUpdate(float frametime)
{
    if (m_pSceneRoot)
    {
        m_pSceneRoot->Update(frametime);

        auto& nodes = m_pSceneRoot->GetNodes();
        for (auto& node : nodes) 
        {
            auto pos = node->GetPos();
            if (pos.y < -2.0f) 
            {
                pos.y = -2.0f;
                node->SetPos(pos);
                auto vel = node->GetVelocity();
                vel.y = -vel.y;
                node->SetVelocity(vel * 0.9f);
            }
        }
    }
}

void TheApp::OnDraw(IRenderer& renderer)
{
    renderer.Clear(0.14f, 0.14f, 0.14f, 1.0f);

    glUseProgram(m_uProgram);

    OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", -renderer.GetViewMatrix()[3]);

    const glm::vec3 lightDirection(glm::normalize(glm::vec3(1.0f, -0.25f, -1.0f)));
    OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);

    if (m_pSceneRoot) {
        m_pSceneRoot->Render(renderer, m_uProgram);
    }

}

void TheApp::OnScreenChanged(uint32_t widthPixels, uint32_t heightPixels)
{
    GetOpenGLRenderer()->SetProjectionMatrix(glm::perspective(0.61f, GetAspect(), 1.0f, 500.0f));
}

