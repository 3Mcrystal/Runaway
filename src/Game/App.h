#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Board.h"
#include "Bonus.h"
//#include "Serializer.h"

#include <AgentManager.h>
#include <vector>

class App
{
public:

	App();
	virtual ~App();

	static App& GetInstance() { return *s_pApp; }

	void OnStart();
	void OnUpdate();
	void OnExit();
	void OnRender(int pass);

	void DrawInfoText();

	//Ressource
	void LoadResources();
	void ReleaseRessources();

	static void MyPixelShader(cpu_ps_io& io);

private:

	inline static App* s_pApp = nullptr;

	void UpdateEntity(float dt);
	void CreateEnemy(int row, int col, int destRow, int destCol);
	bool FindFreeCell(int& row, int& col);
	void HandleInput();
	void CheckScore();
	void SaveBestScore();

private: // Debug //

#ifdef _DEBUG

	bool m_debugDisplay = false;
	bool m_debugToolDisplay = false;
	bool m_debugBoxRenderer = false;

#endif // _DEBUG
private:

	cpu_font m_font;

	cpu_rt* m_rts[1];

	//Shader
	cpu_material m_base_material;

	std::vector<BaseEntity*> m_entityToUpdate;

	Board& m_pBoard = Board::GetInstance();

	Player* m_pPlayer = nullptr;
	Bonus* m_pBonus = nullptr;
	AgentManager* m_pAgentManager = nullptr;
	std::vector<Enemy*> m_pEnemies;

	XMFLOAT3 mOrigin = { 0.f, 0.f ,0.f};

	XMFLOAT3 m_DistanceFromBonus = { 0.f,0.f,0.f };

	//Serializer m_Serializer;

	int m_score = 0;
	int m_Bestscore;

	int m_state;

};