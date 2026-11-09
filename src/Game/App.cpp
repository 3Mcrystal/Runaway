#include "pch.h"
#include "Resource.h"

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
	//Resources
	m_font.Create(cpuDevice.GetHeight() <= 512 ? 14 : 28);

	LoadResources();

	cpuEngine.GetCamera()->transform.pos = XMFLOAT3(-20.f, 50.f, 0.f);

	cpuEngine.GetCamera()->transform.LookAt(0.f,0.f,0.f, CPU_VEC3_UP);
	
}

void App::OnUpdate()
{
	// Deltatime //

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	// Handle Input //

	HandleInput();

	if (m_pAgentManager != nullptr)
		m_pAgentManager->Update(dt);

	UpdateEntity(dt);

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
		//XMFLOAT3 tint = { 0.f, 0.f, 0.5f };
		//cpuDevice.DrawText(&m_font, TextMenu.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), (int)(cpuDevice.GetHeight() * 0.5f), CPU_TEXT_CENTER, &tint);

		DrawInfoText();
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
			it = m_entityToUpdate.erase(it);
		else
		{
			e->Update(dt);
			++it;
		}
	}
}

bool App::FindFreeCell(int& row, int& col, int direction)
{
	Grid& grid = m_pBoard.GetGrid();

	int lines = grid.GetColSize();
	int columns = grid.GetRowSize();

	for (int i = 0; i < lines * columns; i++)
	{
		int index = (row * columns + col + i * direction + lines * columns) % (lines * columns);
		Node* node = grid[index / columns][index % columns];

		if (node->GetIsWall() == false && node->GetIsBlocked() == false)
		{
			row = index / columns;
			col = index % columns;
			return true;
		}
	}
	return false;
}

void App::CreateEnemy(int row, int col, bool chase)
{
	if (FindFreeCell(row, col, -1) == false)
		return;

	Agent* agent = m_pAgentManager->CreateAgent(row, col);

	if (agent == nullptr)
		return;

	Enemy* enemy = new Enemy;
	enemy->SetAgent(agent);

	if (chase)
	{
		enemy->SetTarget(m_pPlayer);
	}
	else
	{
		//patrol start cell to first free cell of the top left corner (temp) 
		int destRow = 1;
		int destCol = 1;

		if (FindFreeCell(destRow, destCol, 1))
		{
			agent->SetPatrol(true);
			agent->SetDestination(destRow, destCol);
		}
	}

	m_pEnemies.push_back(enemy);
	m_entityToUpdate.push_back(enemy);
}

void App::HandleInput()
{
	if (cpuInput.IsBackPressed())
		cpuEngine.Quit();

	if (cpuInput.vi.IsKeyPressed(90))
	{
		m_pPlayer->SetNewDir({ 1,0,0 });
		m_pPlayer->SetChangeDirection(true);
	}

	if (cpuInput.vi.IsKeyPressed(83))
	{
		m_pPlayer->SetNewDir({ -1,0,0 });
		m_pPlayer->SetChangeDirection(true);
	}

	if (cpuInput.vi.IsKeyPressed(68))
	{
		m_pPlayer->SetNewDir({ 0,0,-1 });
		m_pPlayer->SetChangeDirection(true);
	}

	if (cpuInput.vi.IsKeyPressed(81))
	{
		m_pPlayer->SetNewDir({ 0,0,1 });
		m_pPlayer->SetChangeDirection(true);
	}
}

void App::LoadResources()
{
	m_pPlayer = new Player;
	//m_pBonus = new Bonus;
	Board& m_pBoard = Board::GetInstance();
	m_pBoard.GenerateMaze(*m_pPlayer);

	const float gridW = m_pBoard.GetGrid().GetRowSize() * CELL_SIZE;
	const float gridH = m_pBoard.GetGrid().GetColSize() * CELL_SIZE;

	mOrigin = { (0 - gridW) * 0.5f, (0 - gridH) * 0.5f ,(0)};

	m_entityToUpdate.push_back(m_pPlayer);
	//m_entityToUpdate.push_back(m_pBonus);

	//Enemies 2 chase, 1 patrols (test/temp)
	m_pAgentManager = new AgentManager(m_pBoard.GetGrid());

	//Last row / col are walls,
	//search  backward
	const int lastRow = m_pBoard.GetGrid().GetColSize() - 1;
	const int lastCol = m_pBoard.GetGrid().GetRowSize() - 1;

	CreateEnemy(lastRow, 1, false);
	CreateEnemy(lastRow, lastCol, true);
	CreateEnemy(1, lastCol, true);
	
	m_pBoard.Init();

	for (BaseEntity* e : m_entityToUpdate)
		e->Init();
}

void App::ReleaseRessources()
{
	for (Enemy* enemy : m_pEnemies)
		delete enemy;

	m_pEnemies.clear();

	delete m_pAgentManager;
	m_pAgentManager = nullptr;

	delete m_pPlayer;
	delete m_pBonus;
	m_pBoard.DestroyInstance();
}

void App::MyPixelShader(cpu_ps_io& io)
{
	io.color = io.p.color;
}