#pragma once

struct Pos
{
	int x;
	int y;
};

struct TargetSprite 
{
	int index;
	class Sprite* sprite;
	HTREEITEM	treeKey;
};

struct SpriteBinaryFileHeader 
{
	char fileName[256];
	int resourceType;
	int  spriteCount;
};

struct SpriteBinaryFileData
{
	D2D1_RECT_F		rect;
	D2D1_POINT_2F	pivotPos;
};