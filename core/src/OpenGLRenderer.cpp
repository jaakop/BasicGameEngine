#include "../include/OpenGLRenderer.h"

OpenGLRenderer::OpenGLRenderer() :
	m_Context(nullptr),
	m_RC(nullptr)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
	if (m_Context) 
	{
		wglMakeCurrent(m_Context, nullptr);
		wglDeleteContext(m_RC);
		::ReleaseDC(IApplication::GetApp()->GetWindow(), m_Context);
		m_Context = nullptr;
	}
}

bool OpenGLRenderer::Create()
{
	m_Context = ::GetDC(IApplication::GetApp()->GetWindow());
	uint32_t flags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		flags,
		PFD_TYPE_RGBA,	// the kind of framebuffer
		32,				// Color depth
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		24,				// Depth buffer bits
		8,				// Stencil buffer bits
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	int pixelFormat = ::ChoosePixelFormat(m_Context, &pfd);
	if (pixelFormat == 0) {
		return false;
	}
	if (!::SetPixelFormat(m_Context, pixelFormat, &pfd)) {
		return false;
	}

	m_RC = wglCreateContext(m_Context);
	wglMakeCurrent(m_Context, m_RC);

	SetDefaultSettings();

	return true;
}

void OpenGLRenderer::Flip()
{
	glFlush();
	SwapBuffers(m_Context);
}

void OpenGLRenderer::Clear(float r, float g, float b, float a, float depth, int32_t stencil)
{
	//glClearDepthf(depth);
	glClearStencil(stencil);

	glClearColor(r, b, g, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void OpenGLRenderer::SetViewport(const glm::ivec4& area)
{
	glViewport(area.x, area.y, area.z, area.w);
}

bool OpenGLRenderer::SetTexture(uint32_t program, uint32_t texture, uint32_t slot, const std::string_view& uniforName)
{
	return false;
}

bool OpenGLRenderer::SetDefaultSettings()
{
	glStencilMask(0);
	glDisable(GL_SCISSOR_TEST);
	
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH_FUNC);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_STENCIL_TEST);

	glClearDepth(1.0f);
	glClearStencil(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//glBlendColor(1.0f, 1.0f, 1.0f, 1.0f);

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	glCullFace(GL_CCW);

	return true;
}
