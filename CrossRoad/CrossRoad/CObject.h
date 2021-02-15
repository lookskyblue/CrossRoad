#pragma once
#include <Windows.h>

typedef struct _tagRect {
	float left, top, right, bottom;
}RECTANGLE;

class Obj {
private:
	float speed;
	RECTANGLE tObj_rc;
public:
	Obj(float l, float t, float r, float b, float speed = 0) // 초기화 리스트 방법?
		:speed(speed)
	{
		tObj_rc.left = l;
		tObj_rc.top = t;
		tObj_rc.right = r;
		tObj_rc.bottom = b;
	}

	float GetL() const { return tObj_rc.left; }
	float GetT() const { return tObj_rc.top; }
	float GetR() const { return tObj_rc.right; }
	float GetB() const { return tObj_rc.bottom; }
	float GetSpeed() const { return speed; }
	
	void MoveX(float value){ tObj_rc.left += value; tObj_rc.right += value; }
	void MoveY(float value){ tObj_rc.top += value; tObj_rc.bottom += value; }
	void SetL(float value) { tObj_rc.left = value; }
	void SetT(float value) { tObj_rc.top = value; }
	void SetR(float value) { tObj_rc.right = value; }
	void SetB(float value) { tObj_rc.bottom = value; }

	virtual ~Obj() { 
	};
};

class Player : public Obj {
private:
	int nowStage;
public:
	Player(float l, float t, float r, float b, float speed = 500)
		:Obj(l, t, r, b, speed)
		, nowStage(1)
	{}

	Player() = default;
	int GetNowStage() { return nowStage; }
	void StageUp(int i) { nowStage += i; }
	virtual ~Player() {
	};
};

class Monster : public Obj {
private:
	int iDir;
public:
	Monster(float l, float t, float r, float b, int dir, float speed = 500)
		:Obj(l, t, r, b, speed)
		, iDir(dir)
	{}

	int GetDir() { return iDir; }
	void SetDir(int i) { iDir = i; }
};