#include "pch.h"
#include "mesh_addon.h"

void mesh_addon::CreateRectangleBlock(float halfwidth, float halfheight, float halfdepth, XMFLOAT3 color)
{
	Clear();
	const float w = halfwidth;
	const float h = halfheight;
	const float d = halfdepth;
	XMFLOAT3 p0 = { -w, -h, -d };
	XMFLOAT3 p1 = { w, -h, -d };
	XMFLOAT3 p2 = { w,  h, -d };
	XMFLOAT3 p3 = { -w,  h, -d };
	XMFLOAT3 p4 = { -w, -h,  d };
	XMFLOAT3 p5 = { w, -h,  d };
	XMFLOAT3 p6 = { w,  h,  d };
	XMFLOAT3 p7 = { -w,  h,  d };
	XMFLOAT2 tl = { 0.f, 0.f };					// top-left
	XMFLOAT2 tr = { 1.f, 0.f };					// top-right
	XMFLOAT2 br = { 1.f, 1.f };					// bottom-right
	XMFLOAT2 bl = { 0.f, 1.f };					// bottom-left
	vertices.reserve(vertices.size() + 36);
	AddFace(p0, p1, p2, p3, bl, br, tr, tl, color);	// -Z (back)
	AddFace(p4, p7, p6, p5, bl, tl, tr, br, color);	// +Z (front)
	AddFace(p1, p5, p6, p2, bl, br, tr, tl, color);	// +X (droite)
	AddFace(p4, p0, p3, p7, bl, br, tr, tl, color);	// -X (gauche)
	AddFace(p3, p2, p6, p7, bl, br, tr, tl, color);	// +Y (haut)
	AddFace(p4, p5, p1, p0, bl, br, tr, tl, color);	// -Y (bas)
	Optimize();
}
