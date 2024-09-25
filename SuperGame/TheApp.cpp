#include "TheApp.h"

TheApp::TheApp()
{
}

bool TheApp::OnCreate()
{
    return true;
}

void TheApp::OnDestroy()
{
}

void TheApp::OnUpdate(float frametime)
{
    Debug(std::string("FPS: ") + std::to_string(1.0f / frametime) + "\n");
}

void TheApp::OnDraw(IRenderer& renderer)
{
    renderer.Clear(1.0f, 0.0f, 0.0f, 1.0f);
}
