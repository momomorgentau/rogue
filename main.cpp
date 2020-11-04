#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
using namespace std;

class Map
{
public :
	int _width; //幅
	int _height; //高さ
	int outOfRange = -1;
	vector<vector<int>> _ground;


	enum mapValue
	{
		Wall,
		Route,
		Chara
	};

	//コンストラクタ
	Map(int width, int height)
	{
		_width = width;
		_height = height;

		//マップの初期化
		_ground.resize(_height);

		for (auto v : _ground) v.resize(_width);
		for (int i = 0; i < _height; ++i)
		{
			_ground[i].resize(_width);
		}
		for (int i = 0; i < _height; ++i) 
		{
			for (int j = 0; j < _width; ++j)
			{
				_ground[i][j] = Route;
			}
		}
	}

	//領域外かどうかの判定
	bool IsOutOfRange(int x, int y) 
	{
		if (x <= -1 || _width <= x) return true;
		if (y <= -1 || _height <= y) return true;
		return false;
	}

	//マップを表示
	void Show()
	{
		system("cls");
		
		for (auto i : _ground)
		{
			for (int j : i) cout << j;
			cout << endl;
		}
		
	}
};


void Dig(Map map, int x,int y) 
{
	//if (map.IsOutOfRange(x, y)) return;
	int moveX[4] = { 0,0,-1,1};
	int moveY[4] = {1,-1,0,0};
	int rand[4] = { 0,1,2,3 };
	random_shuffle(rand, rand + 4);
	for (int i = 0; i < 4; ++i) cout << rand[i] << endl;
}

void DigStart()
{
	Map map(10,10);
	int xStart = 2;
	int yStart = 4;

	Dig(map, xStart, yStart);
	map.Show();
	cout << "hoge" << endl;
}


int main() 
{
	DigStart();

	return 0;
}