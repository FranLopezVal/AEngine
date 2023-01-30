#ifndef VEC2_H
#define VEC2_H

#define V2(X,Y) Vector2(X,Y)

#include "Libs/imgui.h"

class Vector2
{

public:

	int x, y;

	Vector2()
	{
		x = 10; y = 10;
	}
	Vector2(int _x, int _y) :x(_x), y(_y) {};
	Vector2(const Vector2& v)
	{
		x = v.x;
		y = v.y;
	};
	~Vector2() = default;

	static ImVec2 StaticToIgV2(Vector2& v)
	{
		return ImVec2(v.x, v.y);
	}

	ImVec2 toIgV2() const
	{
		return ImVec2(x, y);
	}

	static Vector2 StaticfromIgV2(ImVec2& i)
	{
		return Vector2(i.x, i.y);
		
	}

	void fromIgV2(ImVec2& i)
	{
		x = i.x;
		y = i.y;
	}


	Vector2 operator + (const Vector2& v2)
	{
		return Vector2(x + v2.x, y + v2.y);
	};
	Vector2 operator - (const Vector2& v2)
	{
		return Vector2(x - v2.x, y - v2.y);
	};
	Vector2 operator * (const int v2)
	{
		return Vector2(x * v2, y * v2);
	};
	Vector2 operator * (const Vector2& v2)
	{
		return Vector2(x * v2.x, y * v2.y);
	};

	
private:

};


#endif // !VEC2_H
