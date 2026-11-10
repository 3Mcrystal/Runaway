#include "pch.h"
#include "Resource.h"
#include "Utils.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

enum mState
{
	START,
	PLAY,
	PAUSE,
	END,
	COUNT
};

App::App()
{
	s_pApp = this;
	CPU_CALLBACK_START(OnStart);
	CPU_CALLBACK_UPDATE(OnUpdate);
	CPU_CALLBACK_EXIT(OnExit);
	CPU_CALLBACK_RENDER(OnRender);

	m_state = START;

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

	cpuEngine.GetCamera()->transform.pos = XMFLOAT3(-30.f, 30.f, 0.f);

	cpuEngine.GetCamera()->transform.LookAt(0.f,0.f,0.f, CPU_VEC3_RIGHT);
	
}

void App::OnUpdate()
{
	// Deltatime //

	float dt = cpuTime.delta;
	float time = cpuTime.total;

	// Handle Input //

	HandleInput();

	// Game over : nothing moves anymore, Escape still works
	if (m_state == END || m_state == START || m_state == PAUSE)
		return;

	if (m_pAgentManager != nullptr)
		m_pAgentManager->Update(dt);

	UpdateEntity(dt);

	for (Enemy* enemy : m_pEnemies)
	{
		if (enemy->IsTouchingPlayer())
		{
			m_state = END;
			SaveBestScore();
		}
	}

	CheckScore();

	m_DistanceFromBonus = Utils::Distance(m_pPlayer->GetTransform().pos, m_pBonus->GetTransform().pos);

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
		DrawInfoText();

		std::string text;
		XMFLOAT3 color;

		switch (m_state)
		{
		case END:
			color = { 1.f, 0.f, 0.f };
			text = "GAME OVER - score : " + CPU_STR(m_score) + " (Escape to quit)";
			cpuDevice.DrawText(&m_font, text.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), (int)(cpuDevice.GetHeight() * 0.5f), CPU_TEXT_CENTER, &color);
			break;
		case PAUSE:
			color = { 1.f, 0.f, 0.f };
			text = "Pause (Enter to continue) (Escape to quit)";
			cpuDevice.DrawText(&m_font, text.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), (int)(cpuDevice.GetHeight() * 0.5f), CPU_TEXT_CENTER, &color);
			break;
		case START:
			color = { 1.f, 0.f, 0.f };
			text = "Hello kind Sir (Enter to play) (Escape to quit)";
			cpuDevice.DrawText(&m_font, text.c_str(), (int)(cpuDevice.GetWidth() * 0.5f), (int)(cpuDevice.GetHeight() * 0.5f), CPU_TEXT_CENTER, &color);
			break;
		}
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
	info += CPU_STR(m_Bestscore) + " : meilleure score, ";
	info += CPU_STR(abs(static_cast<int>(m_DistanceFromBonus.x))) +" "+ CPU_STR(abs(static_cast<int>(m_DistanceFromBonus.y))) +" "+ CPU_STR(abs(static_cast<int>(m_DistanceFromBonus.z))) + " : distance from bonus, ";
	info += CPU_STR(stats.drawnTriangleCount) + " : triangles, ";
	info += CPU_STR(stats.clipEntityCount) + " : clipped entities\n";

#else

	std::string info = CPU_STR(cpuTime.fps) + " fps, ";
	info += CPU_STR(m_score) + " : score, ";
	info += CPU_STR(m_Bestscore) + " : meilleure score, ";
	info += CPU_STR(abs(static_cast<int>(m_DistanceFromBonus.x))) + " " + CPU_STR(abs(static_cast<int>(m_DistanceFromBonus.y))) + " " + CPU_STR(abs(static_cast<int>(m_DistanceFromBonus.z))) + " : distance from bonus";

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

bool App::FindFreeCell(int& row, int& col)
{
	Grid& grid = m_pBoard.GetGrid();

	int lines = grid.GetColSize();
	int columns = grid.GetRowSize();

	int bestRow = -1;
	int bestCol = -1;
	int bestDistance = lines + columns + 1;

	for (int i = 0; i < lines; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			if (grid[i][j]->GetIsWall() || grid[i][j]->GetIsBlocked())
				continue;

			int distance = abs(i - row) + abs(j - col);

			if (distance < bestDistance)
			{
				bestDistance = distance;
				bestRow = i;
				bestCol = j;
			}
		}
	}

	if (bestRow == -1)
		return false;

	row = bestRow;
	col = bestCol;
	return true;
}

void App::CreateEnemy(int row, int col, int destRow, int destCol)
{
	if (FindFreeCell(row, col) == false || FindFreeCell(destRow, destCol) == false)
		return;

	Agent* agent = m_pAgentManager->CreateAgent(row, col);

	if (agent == nullptr)
		return;

	Enemy* enemy = new Enemy;
	enemy->SetAgent(agent);
	enemy->SetTarget(m_pPlayer);
	enemy->SetPatrolRoute(destRow, destCol);

	m_pEnemies.push_back(enemy);
	m_entityToUpdate.push_back(enemy);
}

void App::HandleInput()
{
	if (cpuInput.IsBackPressed())
	{
		if (m_state == PAUSE)
		{
			SaveBestScore();
			cpuEngine.Quit();
		}
		if (m_state == PLAY)
		{
			m_state = PAUSE;
		}
		if (m_state == END)
		{
			cpuEngine.Quit();
		}
		if (m_state == START)
		{
			cpuEngine.Quit();
		}
	}

	if (cpuInput.vi.IsKeyPressed(13))
	{
		if (m_state == START)
			m_state = PLAY;

		if(m_state == PAUSE)
			m_state = PLAY;
	}

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

void App::CheckScore()
{
	Board& m_pBoard = Board::GetInstance();
	std::pair<std::pair<int, int>, float> player = { m_pPlayer->WorldToGrid(m_pPlayer->GetTransform().pos), m_pBoard.GetStandHeight(m_pPlayer->WorldToGrid(m_pPlayer->GetTransform().pos).first,m_pPlayer->WorldToGrid(m_pPlayer->GetTransform().pos).second)};
	std::pair<std::pair<int, int>, float> bonus  = { m_pBonus->WorldToGrid(m_pBonus->GetTransform().pos),  m_pBoard.GetStandHeight(m_pBonus->WorldToGrid(m_pBonus->GetTransform().pos).first,m_pBonus->WorldToGrid(m_pBonus->GetTransform().pos).second)};
	if (player == bonus)
	{
		m_score = m_score + 1;
		int row = Utils::GetRandomNumber(2, m_pBoard.GetGrid().GetColSize() - 2);
		int col = Utils::GetRandomNumber(2, m_pBoard.GetGrid().GetRowSize() - 2);
		m_pBonus->GetTransform().pos = m_pBonus->GridToWorld(row, col);
		m_pBonus->GetTransform().pos.y = m_pBoard.GetStandHeight(row, col);
		m_pBonus->GetParticleEmmiter()->pos = m_pBonus->GetTransform().pos;
	}
}

void App::SaveBestScore()
{
	if (m_score <= m_Bestscore)
		return;

	m_Bestscore = m_score;
	std::ofstream file("../../res/Game/score.txt");
	if (file.is_open())
		file << m_Bestscore;
}

void App::LoadResources()
{
	m_pPlayer = new Player;
	m_pBonus = new Bonus;
	Board& m_pBoard = Board::GetInstance();

	m_pBoard.GenerateMaze(*m_pPlayer);

	const float gridW = m_pBoard.GetGrid().GetRowSize() * CELL_SIZE;
	const float gridH = m_pBoard.GetGrid().GetColSize() * CELL_SIZE;

	mOrigin = { (0 - gridW) * 0.5f, (0 - gridH) * 0.5f ,(0)};

	m_entityToUpdate.push_back(m_pPlayer);
	m_entityToUpdate.push_back(m_pBonus);
	m_pAgentManager = new AgentManager(m_pBoard.GetGrid());

	const int lastRow = m_pBoard.GetGrid().GetColSize() - 1;
	const int lastCol = m_pBoard.GetGrid().GetRowSize() - 1;

	CreateEnemy(lastRow, 1, 1, 1);
	CreateEnemy(lastRow, lastCol, 1, lastCol / 2);
	CreateEnemy(1, lastCol, lastRow / 2, 1);
	
	m_pBoard.Init();

	for (BaseEntity* e : m_entityToUpdate)
		e->Init();

	std::ifstream file("../../res/Game/score.txt");
	if (file.is_open())
	{
		file >> m_Bestscore;
		file.close();
	}
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