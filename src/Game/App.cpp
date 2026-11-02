#include "pch.h"

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);

	m_rts[0] = cpuEngine.CreateRT();
}

App::~App()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void App::OnStart()
{
	// Resources
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);

	LoadResources();
}

void App::OnUpdate()
{
	// Deltatime //

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	UpdateEntity(dt);

	// Handle Input //

	HandleInput();

	// // // // // //

}

void App::OnExit()
{
	ReleaseRessources();
}

void App::OnRender(int pass)
{
	cpu_ray ray;
	cpuEngine.GetCursorRay(ray);

	XMFLOAT2 cursor;
	cpuEngine.GetCursor(cursor);

	switch (pass)
	{
	case CPU_PASS_PARTICLE_BEGIN:
	{
		// Blur particles
		//cpuEngine.SetRT(m_rts[0]);
		//cpuEngine.ClearColor();
		break;
	}
	case CPU_PASS_PARTICLE_END:
	{
		// Blur particles
		//cpuEngine.Blur(10);
		//cpuEngine.SetMainRT();
		//cpuEngine.AlphaBlend(m_rts[0]);
		break;
	}
	case CPU_PASS_UI_END:
	{

		std::string TextMenu = "GG You Win";
		TextMenu += "\nPress Escape to Main Menu";
		XMFLOAT3 tint = { 0.f, 0.f, 0.5f };
		cpuDevice.DrawText(&m_font, TextMenu.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), (int)(cpuDevice.GetHeight() * 0.5f), CPU_TEXT_CENTER, &tint);

		// // // // // // // //
		break;
	}
	}
}

void App::DrawInfoText()
{
#ifdef _DEBUG

	cpu_stats& stats = *cpuEngine.GetStats();
	std::string info = CPU_STR(cpuTime.fps) + " fps, ";
	info += CPU_STR(m_score) + " : score, ";
	info += CPU_STR(stats.drawnTriangleCount) + " : triangles, ";
	info += CPU_STR(stats.clipEntityCount) + " : clipped entities\n";

#else

	std::string info = CPU_STR(cpuTime.fps) + " fps, ";
	info += CPU_STR(m_score) + " : score ";

#endif // _DEBUG

	XMFLOAT3 tint = { 0.f, 0.f, 0.5f };
	cpuDevice.DrawText(&m_font, info.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), 10, CPU_TEXT_CENTER, &tint);
}

void App::UpdateEntity(float dt)
{
	for (auto it = m_entityToUpdate.begin(); it != m_entityToUpdate.end();)
	{
		BaseEntity* e = *it;
		if (e->GetEntity()->dead)
			m_entityToUpdate.erase(it);
		else
		{
			e->Update(dt);
			++it;
		}
	}
}

void App::HandleInput()
{
	
}

void App::LoadResources()
{

}

void App::ReleaseRessources()
{

}

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}