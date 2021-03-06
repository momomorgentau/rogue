#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <windows.h>
using namespace std;


enum Algorithm
{
	Anahori,
	Divide
};

//ハイパーパラメータ

//描画用の待機時間
int h_mSecond = 2000;
//部屋の幅の最小値
int h_minWidth = 6;
//部屋の高さの最小値
int h_minHeight = 6;
//部屋数の最大値
int h_maxRoomNum = 10;
//アルゴリズムの指定
int h_Algorithm = Anahori;
//マップの幅（穴掘り法では奇数を指定）
int h_Width = 33;
//マップの高さ（穴掘り法では奇数を指定）
int h_Height = 33;

//※穴掘り法を再帰関数にて実装しているため、幅高さが大きいとスタックオーバーフローを起こす
//繰り返しにて実装した方が無難？(私の環境では30*30程度でオーバーフローします)

//マップのクラス
class Map
{
public:
	int _width; //幅
	int _height; //高さ
	int _outOfRange = -1;
	vector<vector<int>> _ground;
	vector<vector<int>> _divideArea; //区間分割(領域)
	vector<vector<int>> _divideLine; //区間分割（線）
	vector<vector<int>> _divideRoom; //区間分割（部屋）


	enum mapValue
	{
		Wall,
		Route,
		Chara
	};

	enum mapDir
	{
		Vertical,
		Horizontal

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
				_ground[i][j] = Wall;
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
	//要素の取得
	int Get(int x, int y)
	{
		if (IsOutOfRange(x, y)) return _outOfRange;
		return _ground[y][x];
	}

	//要素の書き換え
	void Set(int x, int y, int v)
	{
		if (IsOutOfRange(x, y)) return;
		_ground[y][x] = v;
	}

	//範囲内を全て書き換える
	void AllSet(int left, int top, int right, int bottom, int v)
	{
		for (int i = top; i <= bottom; ++i)
		{
			for (int j = left; j <= right; ++j)
			{
				Set(j, i, Route);
			}
		}
	}

	//区間分割
	//上下左右の座標と部屋番号を取得
	void InsertDivideArea(int left, int top, int right, int bottom, int id)
	{
		_divideArea.emplace_back();
		int s = _divideArea.size();
		_divideArea[s - 1].emplace_back(left);
		_divideArea[s - 1].emplace_back(top);
		_divideArea[s - 1].emplace_back(right);
		_divideArea[s - 1].emplace_back(bottom);
		_divideArea[s - 1].emplace_back(id);
	}

	//分割する線
	//始点のxy座標、終点のxy座標、方向 0-> ver 1->hor
	void InsertDivideLine(int sx, int sy, int tx, int ty, int dir)
	{
		_divideLine.emplace_back();
		int s = _divideLine.size();
		_divideLine[s - 1].emplace_back(sx);
		_divideLine[s - 1].emplace_back(sy);
		_divideLine[s - 1].emplace_back(tx);
		_divideLine[s - 1].emplace_back(ty);
		_divideLine[s - 1].emplace_back(dir);

	}

	//部屋を作成
	void InsertDivideRoom(int left, int top, int right, int bottom)
	{
		_divideRoom.emplace_back();
		int s = _divideRoom.size();
		_divideRoom[s - 1].emplace_back(left);
		_divideRoom[s - 1].emplace_back(top);
		_divideRoom[s - 1].emplace_back(right);
		_divideRoom[s - 1].emplace_back(bottom);
	}

	//マップを表示
	void Show()
	{
		system("cls");

		for (auto i : _ground)
		{
			for (int j : i)
			{
				if (j == Wall) cout << "■";
				else cout << "　";
			}
			cout << endl;
		}

		//描画用に少し待つ
		Sleep(h_mSecond);

	}
};


//乱数を取得
int GetRandomInt(int s, int t)
{
	mt19937 mt{ random_device{}() };
	uniform_int_distribution<int> dist(s, t);
	return dist(mt);
}




//区間を分割する。
void AreaDivide(Map& map)
{
	int lowerWidth = 0;
	int upperWidth = map._width - 1;
	int lowerHeight = 0;
	int upperHeight = map._height - 1;
	int roomId = 0;

	bool isVertical;
	if (GetRandomInt(0, 1) & 1) isVertical = true;
	else isVertical = false;


	while (1)
	{
		//分割できない条件
		if (upperWidth - lowerWidth < 2 * h_minWidth + 4) break;
		if (upperHeight - lowerHeight < 2 * h_minHeight + 4) break;
		//部屋が指定した数に達したら
		if (roomId >= h_maxRoomNum) break;

		//この座標において分割線を引く
		int dividePoint;

		//縦に分割
		if (isVertical)
		{
			dividePoint = GetRandomInt(lowerWidth + 2 + h_minWidth, upperWidth - h_minWidth - 2);
			if ((upperWidth + lowerWidth) / 2 < dividePoint)
			{
				map.InsertDivideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1, roomId);
				upperWidth = dividePoint - 1;
			}
			else
			{
				map.InsertDivideArea(lowerWidth + 1, lowerHeight + 1, dividePoint - 2, upperHeight - 1, roomId);
				lowerWidth = dividePoint + 1;
			}
			map.InsertDivideLine(dividePoint, lowerHeight + 1, dividePoint, upperHeight - 1, map.Vertical);
			isVertical = false;
		}
		//横に分割
		else
		{
			dividePoint = GetRandomInt(lowerHeight + h_minHeight + 2, upperHeight - h_minHeight - 2);
			if ((upperHeight + lowerHeight) / 2 < dividePoint)
			{
				map.InsertDivideArea(lowerWidth + 1, dividePoint + 2, upperWidth - 1, upperHeight - 1, roomId);
				upperHeight = dividePoint - 1;
			}
			else
			{
				map.InsertDivideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2, roomId);
				lowerHeight = dividePoint + 1;
			}
			map.InsertDivideLine(lowerWidth + 1, dividePoint, upperWidth - 1, dividePoint, map.Horizontal);

			isVertical = true;
		}
		++roomId;
	}
}

//分割した区間に小さい部屋を入れる。
void CreateRoom(Map& map)
{
	for (auto v : map._divideArea)
	{
		int left = v[0];
		int top = v[1];
		int right = v[2];
		int bottom = v[3];

		int roomLeft = GetRandomInt(left, right - h_minWidth + 1);
		int roomRight = GetRandomInt(roomLeft + h_minWidth - 1, right);
		int roomTop = GetRandomInt(top, bottom - h_minHeight + 1);
		int roomBottom = GetRandomInt(roomTop + h_minHeight - 1, bottom);

		map.InsertDivideRoom(roomLeft, roomTop, roomRight, roomBottom);
	}
}

//部屋から通路を伸ばす
//通路はマップに書き込む
void CreateRoute(Map& map)
{
	int cnt = 0;
	int size = map._divideLine.size() - 1;

	for (auto v : map._divideLine)
	{
		//通路数は部屋数+1であるため、最後の通路は無視する
		if (cnt >= size) break;

		int sx = v[0];
		int sy = v[1];
		int tx = v[2];
		int ty = v[3];
		int dir = v[4];

		//部屋情報の取得
		auto roomBefore = map._divideRoom[cnt];
		auto roomAfter = map._divideRoom[cnt + 1];

		int leftBefore = roomBefore[0];
		int topBefore = roomBefore[1];
		int rightBefore = roomBefore[2];
		int bottomBefore = roomBefore[3];

		int leftAfter = roomAfter[0];
		int topAfter = roomAfter[1];
		int rightAfter = roomAfter[2];
		int bottomAfter = roomAfter[3];

		//縦に線が引かれている場合
		if (dir == map.Vertical)
		{
			//部屋のどこから線を伸ばすのかを決定。
			int routeBefore = GetRandomInt(topBefore + 1, bottomBefore - 1);
			int routeAfter = GetRandomInt(topAfter + 1, bottomAfter - 1);
			//線を挟んで2つの部屋がどちらにあるかで場合分け。
			if (leftBefore < leftAfter)
			{
				map.AllSet(rightBefore, routeBefore, sx, routeBefore, map.Route);
				map.AllSet(sx, routeAfter, leftAfter, routeAfter, map.Route);
			}
			else
			{
				map.AllSet(rightAfter, routeAfter, sx, routeAfter, map.Route);
				map.AllSet(sx, routeBefore, leftBefore, routeBefore, map.Route);
			}
			//伸ばした線の間を結ぶ
			if (routeBefore > routeAfter) swap(routeBefore, routeAfter);
			map.AllSet(sx, routeBefore, sx, routeAfter, map.Route);

		}
		else
		{
			int routeBefore = GetRandomInt(leftBefore + 1, rightBefore - 1);
			int routeAfter = GetRandomInt(leftAfter + 1, rightAfter - 1);

			if (topBefore < topAfter)
			{
				map.AllSet(routeBefore, bottomBefore, routeBefore, sy, map.Route);
				map.AllSet(routeAfter, sy, routeAfter, topAfter, map.Route);
			}
			else
			{
				map.AllSet(routeAfter, bottomAfter, routeAfter, sy, map.Route);
				map.AllSet(routeBefore, sy, routeBefore, topBefore, map.Route);
			}
			if (routeBefore > routeAfter) swap(routeBefore, routeAfter);
			map.AllSet(routeBefore, sy, routeAfter, sy, map.Route);

		}
		++cnt;
	}
}


//穴掘りアルゴリズム
void Dig(Map& map, int x, int y)
{

	map.Set(x, y, map.Route);


	int moveX[4] = { 0,0,-1,1 };
	int moveY[4] = { 1,-1,0,0 };
	vector<int> act = { 0,1,2,3 };

	//行動順をランダムにする
	random_device get_rand_dev;
	mt19937 get_rand_mt(get_rand_dev());
	shuffle(act.begin(), act.end(), get_rand_mt);


	for (auto a : act)
	{
		int dx = moveX[a];
		int dy = moveY[a];

		if (map.Get(x + dx * 2, y + dy * 2) == map.Wall)
		{
			map.Set(x + dx, y + dy, map.Route);
			Dig(map, x + dx * 2, y + dy * 2);
		}
	}

}
//穴掘り開始
void DigStart(int width, int height, int sx, int sy)
{
	Map map(width, height);
	int xStart = sx;
	int yStart = sy;
	Dig(map, xStart, yStart);
	map.Show();
}

//分割法開始
void DivideStart(int width, int height)
{
	Map map(width, height);

	//マップを区画に分ける（書き込まない。区画の四隅の座標を取得。）
	AreaDivide(map);
	//区画に収まる部屋を用意。（書き込まない、部屋の四隅の座標を取得）
	CreateRoom(map);
	//通路を伸ばす。通路はマップに書き込む
	CreateRoute(map);

	for (auto a : map._divideRoom)
	{
		map.AllSet(a[0], a[1], a[2], a[3], map.Route);

	}
	map.Show();
}

int main()
{
	if (h_Algorithm == Anahori)
	{
		int startX = GetRandomInt(0, h_Width - 2);
		int startY = GetRandomInt(0, h_Height - 2);
		if (startY % 2 == 0) ++startY;
		if (startX % 2 == 0) ++startX;
		DigStart(h_Width, h_Height, startX, startY);
	}
	else if (h_Algorithm == Divide)
	{
		DivideStart(h_Width, h_Height);
	}
	return 0;
}
