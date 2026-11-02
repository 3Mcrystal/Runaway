#pragma once
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
	void HandleInput();

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

	int m_score = 0;
};