#include "SampleScene.h"

int GetRandomNumber(int min, int max)
{
    static std::random_device rd;

    static std::mt19937 gen(rd());

    std::uniform_int_distribution<> dis(min, max);

    return dis(gen);
}

#define maybe GetRandomNumber(0,1)

void SampleScene::OnInitialize()
{
    GenerateMaze();

    const float gridW = mGrid.GetRowSize() * CELL_SIZE;
    const float gridH = mGrid.GetColSize() * CELL_SIZE;

    mOrigin = { (GetWindowWidth() - gridW) * 0.5f, (GetWindowHeight() - gridH) * 0.5f };

    mpPlayerNode = mGrid[0][1];
    mpCursorNode = mGrid[0][1];

    mpPlayer = CreateEntity<DummyEntity>(10.f, Color::Cyan);
    mpPlayer->SetPosition(NodeCenter(mpPlayerNode));
}

void SampleScene::OnUpdate()
{
    HandleInput();
    UpdateMovement();
    DrawGrid();

    Debug::DrawText({ 10.f, 10.f },
        "ZQSD : deplacer le curseur - Entree : lancer le pathfinding", Color::White);

    if (mReachable == false)
    {
        Debug::DrawText({ 10.f, 40.f }, "Inatteignable", Color::Orange);
    }
    else if (mpPlayerNode->GetX() == mpCursorNode->GetX() && mpPlayerNode->GetY() == mpCursorNode->GetY())
    {
        Debug::DrawText({ 10.0f, 60.0f }, "C'est atteint", Color::Cyan);
    }
    else if (mIsMoving)
    {
        std::string text = "Deplacements restants : " + std::to_string(mPath.size() - mPathIndex);
        Debug::DrawText({ 10.f, 40.f }, text.c_str(), Color::White);
    }
}

void SampleScene::HandleInput()
{
    InputManager* input = InputManager::Get();

    if (input->IsKeyDown(SDLK_Z)) MoveCursor(-1, 0);
    if (input->IsKeyDown(SDLK_S)) MoveCursor(1, 0);
    if (input->IsKeyDown(SDLK_Q)) MoveCursor(0, -1);
    if (input->IsKeyDown(SDLK_D)) MoveCursor(0, 1);

    if (input->IsKeyDown(SDLK_RETURN) && mIsMoving == false)
        StartPathfinding();

    if (input->IsKeyDown(SDLK_ESCAPE))
        exit(0);

    if (input->IsMouseDown(SDL_BUTTON_LEFT))
    {
        Vector2f mouse = input->GetMousePosition();
        int col = static_cast<int>(std::floor((mouse.x - mOrigin.x) / CELL_SIZE));
        int row = static_cast<int>(std::floor((mouse.y - mOrigin.y) / CELL_SIZE));

        if (IsInGrid(row, col))
            mpCursorNode = mGrid[row][col];
    }

    if (input->IsMouseDown(SDL_BUTTON_RIGHT))
    {
        Vector2f mouse = input->GetMousePosition();
        int col = static_cast<int>(std::floor((mouse.x - mOrigin.x) / CELL_SIZE));
        int row = static_cast<int>(std::floor((mouse.y - mOrigin.y) / CELL_SIZE));

        if (IsInGrid(row, col))
        {
            if (mGrid[row][col]->GetIsWall())
            {
                mGrid[row][col]->SetIsWall(false);
            }
            else
            {
                mGrid[row][col]->SetIsWall(true);
            }
        }
            
    }
}

void SampleScene::MoveCursor(int dRow, int dCol)
{
    int row = mpCursorNode->GetX() + dRow;
    int col = mpCursorNode->GetY() + dCol;

    if (IsInGrid(row, col))
        mpCursorNode = mGrid[row][col];
}

bool SampleScene::IsInGrid(int row, int col)
{
    return row >= 0 && row < mGrid.GetColSize() && col >= 0 && col < mGrid.GetRowSize();
}

void SampleScene::StartPathfinding()
{
    pathfind.BFS(mGrid, mpPlayerNode, mpCursorNode);   // ou pathfind->Djikstra(mGrid, ...)
    mReachable = pathfind.GetAttainable();
    mPath = pathfind.GetPath();                        // on récupère le chemin calculé

    if (mReachable && mPath.empty() == false)
    {
        mPathIndex = 0;
        mStepTimer = 0.f;
        mIsMoving = true;
    }
}

void SampleScene::ResetVisited()
{
    for (int i = 0; i < mGrid.GetColSize(); i++)
    {
        for (int j = 0; j < mGrid.GetRowSize(); j++)
            mGrid[i][j]->SetIsVisited(false);
    }
}

void SampleScene::UpdateMovement()
{
    if (mIsMoving == false)
        return;

    mStepTimer += GetDeltaTime();

    if (mStepTimer < STEP_DELAY)
        return;

    mStepTimer -= STEP_DELAY;

    mpPlayerNode = mPath[mPathIndex];
    mPathIndex++;
    mpPlayer->SetPosition(NodeCenter(mpPlayerNode));

    if (mPathIndex >= mPath.size())
    {
        mIsMoving = false;
        mPath.clear();
        mPathIndex = 0;
    }
}

Vector2f SampleScene::NodeCenter(Node* node)
{
    Vector2f center = { mOrigin.x + (node->GetY() + 0.5f) * CELL_SIZE ,mOrigin.y + (node->GetX() + 0.5f) * CELL_SIZE };
    return center;
}

void SampleScene::DrawGrid()
{
    const int lines = mGrid.GetColSize();
    const int rows = mGrid.GetRowSize();
    Color gridColor{ 0, 0, 0 };

    for (int i = 0; i <= lines; i++)
    {
        float y = mOrigin.y + i * CELL_SIZE;
        Debug::DrawLine({ mOrigin.x, y }, { mOrigin.x + rows * CELL_SIZE, y }, gridColor);
    }
    for (int j = 0; j <= rows; j++)
    {
        float x = mOrigin.x + j * CELL_SIZE;
        Debug::DrawLine({ x, mOrigin.y }, { x, mOrigin.y + lines * CELL_SIZE }, gridColor);
    }

    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            Node* node = mGrid[i][j];

            if (node->GetIsWall())
                DrawSquare(NodeCenter(node), CELL_SIZE - 2.f, Color::Black, true);
        }
    }

    if (mIsMoving)
    {
        for (size_t k = mPathIndex; k < mPath.size(); k++)
            Debug::DrawCircle(NodeCenter(mPath[k]), 3.f, Color::Red, true);
    }

    Color red{ 255, 0, 0 };

    if (mpCursorNode == mpPlayerNode)
        DrawSquare(NodeCenter(mpCursorNode), 26.f, red, false);
    else
        DrawSquare(NodeCenter(mpCursorNode), 15.f, red, true);
}

void SampleScene::DrawSquare(Vector2f center, float size, const Color& color, bool filled)
{
    const float h = size * 0.5f;

    if (filled)
    {
        for (float y = -h; y <= h; y += 1.f)
            Debug::DrawLine({ center.x - h, center.y + y }, { center.x + h, center.y + y }, color);
    }
    else
    {
        Debug::DrawLine({ center.x - h, center.y - h }, { center.x + h, center.y - h }, color);
        Debug::DrawLine({ center.x + h, center.y - h }, { center.x + h, center.y + h }, color);
        Debug::DrawLine({ center.x + h, center.y + h }, { center.x - h, center.y + h }, color);
        Debug::DrawLine({ center.x - h, center.y + h }, { center.x - h, center.y - h }, color);
    }
}

void SampleScene::addEntranceAndExit() {


    for (int x = 1; x < mGrid.GetRowSize(); x += 2) 
    { 
        if (mGrid[1][x]->GetIsWall() == false) 
        {
            mGrid[0][x]->SetIsWall(false);
            mpPlayerNode = mGrid[0][x];
            mpCursorNode = mGrid[0][x];
            break;
        }
    }

    std::vector<int> validExits;
    for (int x = 1; x < mGrid.GetRowSize(); x += 2) {
        if (mGrid[mGrid.GetColSize() - 2][x]->GetIsWall() == false)
        {
            validExits.push_back(x);
        }
    }

    if (!validExits.empty()) {
        int exitIndex = validExits[m_RNG() % validExits.size()];
        mGrid[mGrid.GetColSize() - 2][exitIndex]->SetIsWall(false);
    }
}

void SampleScene::CarveMaze(int x, int y) 
{
    static const int dx[] = { 0,  0, -2,  2 };
    static const int dy[] = { -2, 2,  0,  0 };

    static std::vector<int> dirs = { 0, 1, 2, 3 };

    std::shuffle(dirs.begin(), dirs.end(), SampleScene::m_RNG);

    mGrid[y][x]->SetIsWall(false);

    auto isBounds = [](int ax, int bound)
        {
        return (ax > 0) && (ax < bound - 1);
        };

    for (int dir : dirs)
    {
        int nx = x + dx[dir];
        int ny = y + dy[dir];

        if (isBounds(ny, mGrid.GetColSize()) && isBounds(nx, mGrid.GetRowSize()) && mGrid[ny][nx]->GetIsWall())
        {
            mGrid[y + dy[dir] / 2][x + dx[dir] / 2]->SetIsWall(false);

            CarveMaze(nx, ny);
        }
    }
}

void SampleScene::GenerateMaze()
{

    if (mGrid.GetColSize() <= 0 || mGrid.GetRowSize() <= 0)
    {
        std::cout << "Set the maze dimensions first!\n";
        return;
    }

    m_RNG.seed(std::random_device{}());

    for (int i = 0; i < mGrid.GetColSize();i++)
    {
        for (int j = 0; j < mGrid.GetRowSize();j++)
        {
            mGrid[i][j]->SetIsWall(true);
        }
    }

    CarveMaze(1, 1);

    addEntranceAndExit();
}
