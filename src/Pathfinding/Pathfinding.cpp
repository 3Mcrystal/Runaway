#include "Pathfinding.h"
#include <conio.h>
#include <thread>
#include <chrono>
#include <queue>
#include <algorithm>
#include <limits>
#include <functional>

Pathfinding::Pathfinding()
{
}

Pathfinding::~Pathfinding()
{
}

//réarrange le chemin dans le bonne ordre
static std::vector<Node*> RebuildPath(const std::vector<std::vector<Node*>>& parent,Node* s, Node* t)
{
    std::vector<Node*> result;
    // à refaire
    for (Node* current = t; current != s; current = parent[current->GetX()][current->GetY()])
        result.push_back(current);

    std::reverse(result.begin(), result.end());

    return result;
}
// recherche en largeur
void Pathfinding::BFS(Grid& grid, Node* start, Node* target)
{
    attainable = true;
    path.clear();
    deplacement = 0;

    const int lines = grid.GetColSize();
    const int cols  = grid.GetRowSize();

    Node* s = grid[start->GetX()][start->GetY()];
    Node* t = grid[target->GetX()][target->GetY()];

	std::vector<std::vector<bool>>  visited   (lines, std::vector<bool>(cols, false));//Crée un tableau de tableau de booléens pour savoir si on a déjà visité une case // j'adore cette feature 
	std::vector<std::vector<Node*>> parent (lines, std::vector<Node*>(cols, nullptr));//Crée un tableau de tableau de pointeurs vers les parents pour reconstruire le chemin
    std::queue<Node*> toVisit;

    //direction
    const int dx[4] = { -1, 1, 0, 0 };
    const int dy[4] = { 0, 0, -1, 1 };

    toVisit.push(s);
    visited[s->GetX()][s->GetY()] = true;

    while (!toVisit.empty())
    {
        Node* current = toVisit.front();
        toVisit.pop();

        if (current == t)
            break;

        for (int d = 0; d < 4; d++)
        {
            int nx = current->GetX() + dx[d];
            int ny = current->GetY() + dy[d];

            if (nx < 0 || nx >= lines || ny < 0 || ny >= cols)
                continue; // hors limites

            if (grid[nx][ny]->GetIsWall())
                continue; // mur

            if (grid[nx][ny]->GetIsBlocked())
                continue; //case prise par agent

            if (visited[nx][ny])
                continue; // déjà vu

            visited[nx][ny] = true;
            parent[nx][ny] = current;
            toVisit.push(grid[nx][ny]);
        }
    }

    if (visited[t->GetX()][t->GetY()] == false)
    {
        attainable = false;
        return;
    }

    path = RebuildPath(parent, s, t);
    deplacement = static_cast<int>(path.size());
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Pathfinding::Djikstra(Grid& grid, Node* start, Node* target, bool canClimb)
{
	// réinitialisation
    attainable = true;
    path.clear();
    deplacement = 0;
    cost = 0;

    const int lines = grid.GetColSize();
    const int cols = grid.GetRowSize();

	constexpr int INFINITE_COST = std::numeric_limits<int>::max();//valeur maximale d'un int, pour représenter un coût infini

    Node* startNode = grid[start->GetX()][start->GetY()];
    Node* targetNode = grid[target->GetX()][target->GetY()];

    std::vector<std::vector<int>> bestCost(lines, std::vector<int>(cols, INFINITE_COST));

    std::vector<std::vector<Node*>> parent(lines, std::vector<Node*>(cols, nullptr));

    //la case au coût le plus faible sort en premier
    std::priority_queue<std::pair<int, Node*>, std::vector<std::pair<int, Node*>>, std::greater<std::pair<int, Node*>>> toVisit;

    // Déplacements possibles
    const int dirX[4] = { -1, 1, 0, 0 };
    const int dirY[4] = { 0, 0, -1, 1 };

    bestCost[startNode->GetX()][startNode->GetY()] = 0;
    toVisit.push({ 0, startNode });

    while (!toVisit.empty())
    {
		int currentCost = toVisit.top().first;
		Node* current = toVisit.top().second;
        toVisit.pop();

        //on a déjà trouvé un meilleur chemin vers cette case
        if (currentCost > bestCost[current->GetX()][current->GetY()])
            continue;

        if (current == targetNode)
            break;

        for (int i = 0; i < 4; i++)
        {
            const int nx = current->GetX() + dirX[i];
            const int ny = current->GetY() + dirY[i];

            
            if (nx < 0 || nx >= lines || ny < 0 || ny >= cols)// Hors de la grille
                continue;

            Node* neighbor = grid[nx][ny];

			if ((neighbor->GetIsWall() && !canClimb) || neighbor->GetIsBlocked())// Mur (sauf si on peut grimper) ou case occupée
                continue;

            int newCost = currentCost + neighbor->GetLevel() + 1;

            // Chemin plus court trouvé vers ce voisin : on le mémorise
            if (newCost < bestCost[nx][ny])
            {
                bestCost[nx][ny] = newCost;
                parent[nx][ny] = current;
                toVisit.push({ newCost, neighbor });
            }
        }
    }


    int targetCost = bestCost[targetNode->GetX()][targetNode->GetY()];
    if (targetCost == INFINITE_COST)
    {
        attainable = false;
        return;
    }

    cost = targetCost;
    path = RebuildPath(parent, startNode, targetNode);
    deplacement = static_cast<int>(path.size());
}