#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <random>
#include <windows.h>
using namespace std;

//�n�C�p�[�p�����[�^
int h_cnt = 0;
int h_mSecond = 2000;
int h_minWidth = 6;
int h_minHeight = 6;
int h_maxRoomNum = 10;

class Map
{
public:
	int _width; //��
	int _height; //����
	int _outOfRange = -1;
	vector<vector<int>> _ground;
	vector<vector<int>> _divideArea; //��ԕ���(�̈�)
	vector<vector<int>> _divideLine; //��ԕ����i���j
	vector<vector<int>> _divideRoom; //��ԕ����i�����j


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

	//�R���X�g���N�^
	Map(int width, int height)
	{
		_width = width;
		_height = height;

		//�}�b�v�̏�����
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

	//�̈�O���ǂ����̔���
	bool IsOutOfRange(int x, int y)
	{
		if (x <= -1 || _width <= x) return true;
		if (y <= -1 || _height <= y) return true;
		return false;
	}
	//�v�f�̎擾
	int Get(int x, int y)
	{
		if (IsOutOfRange(x, y)) return _outOfRange;
		return _ground[y][x];
	}

	//�v�f�̏�������
	void Set(int x, int y, int v)
	{
		if (IsOutOfRange(x, y)) return;
		_ground[y][x] = v;
	}

	//�͈͓���S�ď���������
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

	//��ԕ���
	//�㉺���E�̍��W�ƕ����ԍ����擾
	void InsertDevideArea(int left, int top, int right, int bottom,int id)
	{
		_divideArea.emplace_back();
		int s = _divideArea.size();
		_divideArea[s - 1].emplace_back(left);
		_divideArea[s - 1].emplace_back(top);
		_divideArea[s - 1].emplace_back(right);
		_divideArea[s - 1].emplace_back(bottom);
		_divideArea[s - 1].emplace_back(id);
	}

	//���������
	//�n�_��xy���W�A�I�_��xy���W�A���� 0-> ver 1->hor
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

	//�������쐬
	void InsertDivideRoom(int left, int top, int right, int bottom)
	{
		_divideRoom.emplace_back();
		int s = _divideRoom.size();
		_divideRoom[s - 1].emplace_back(left);
		_divideRoom[s - 1].emplace_back(top);
		_divideRoom[s - 1].emplace_back(right);
		_divideRoom[s - 1].emplace_back(bottom);
	}

	//�}�b�v��\��
	void Show()
	{
		//system("cls");

		for (auto i : _ground)
		{
			for (int j : i)
			{
				if (j == Wall) cout << "��";
				else cout << "�@";
			}
			cout << endl;
		}

		//�`��p�ɏ����҂�
		Sleep(h_mSecond);

	}
};


//�������擾
int GetRandomInt(int s, int t)
{
	mt19937 mt{ random_device{}() };
	uniform_int_distribution<int> dist(s, t);
	return dist(mt);
}




//��Ԃ𕪊�����B
void AreaDivide(Map& map)
{
	int lowerWidth = 0;
	int upperWidth = map._width-1;
	int lowerHeight = 0;
	int upperHeight = map._height-1;
	int roomId = 0;

	bool isVertical;
	if (GetRandomInt(0, 1) & 1) isVertical = true;
	else isVertical = false;
	/*
	if (upperWidth >= upperHeight) isVertical = true;
	else isVertical = false;
	*/
	while (1)
	{
		//�����ł��Ȃ�����
		if (upperWidth - lowerWidth < 2 * h_minWidth + 4) break;
		if (upperHeight - lowerHeight < 2 * h_minHeight + 4) break;
		//�������w�肵�����ɒB������
		if (roomId >= h_maxRoomNum) break;

		//���̍��W�ɂ����ĕ�����������
		int dividePoint;

		//�c�ɕ���
		if (isVertical)
		{
			dividePoint = GetRandomInt(lowerWidth + 2 + h_minWidth, upperWidth - h_minWidth - 2);
			if ((upperWidth + lowerWidth) / 2 < dividePoint)
			{
				map.InsertDevideArea(dividePoint + 2, lowerHeight + 1, upperWidth - 1, upperHeight - 1,roomId);
				upperWidth = dividePoint-1;
			}
			else
			{
				map.InsertDevideArea(lowerWidth + 1, lowerHeight + 1, dividePoint - 2, upperHeight - 1,roomId);
				lowerWidth = dividePoint+1;
			}
			map.InsertDivideLine(dividePoint,lowerHeight+1,dividePoint,upperHeight-1, map.Vertical);
			isVertical = false;
		}
		//���ɕ���
		else
		{
			dividePoint = GetRandomInt(lowerHeight +h_minHeight + 2, upperHeight - h_minHeight - 2);
			if ((upperHeight + lowerHeight) / 2 < dividePoint)
			{
				map.InsertDevideArea(lowerWidth+1, dividePoint + 2, upperWidth - 1, upperHeight - 1,roomId);
				upperHeight = dividePoint-1;
			}
			else
			{
				map.InsertDevideArea(lowerWidth + 1, lowerHeight + 1, upperWidth - 1, dividePoint - 2,roomId);
				lowerHeight = dividePoint+1;
			}
			map.InsertDivideLine(lowerWidth+1, dividePoint, upperWidth -1, dividePoint, map.Horizontal);

			isVertical = true;
		}
		++roomId;
	}
}

//����������Ԃɏ���������������B
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
		int roomBottom = GetRandomInt(roomTop+h_minHeight-1,bottom);

		map.InsertDivideRoom(roomLeft,roomTop,roomRight,roomBottom);
	}
}

//��������ʘH��L�΂�
void CreateRoute(Map& map)
{
	int cnt = 0;
	int size = map._divideLine.size()-1;
	for (auto v : map._divideRoom) 
	{
		for (auto i : v) cout << i << " ";
		cout << endl;
	}
	cout << size << endl;
	for (auto v : map._divideLine) 
	{
		if (cnt >= size) break;
		cout << cnt << endl;
		int sx = v[0];
		int sy = v[1];
		int tx = v[2];
		int ty = v[3];
		int dir = v[4];

		auto roomBefore = map._divideRoom[cnt];
		auto roomAfter = map._divideRoom[cnt+1];
		cout << roomAfter[0] << " " << roomAfter[1] << " " << roomAfter[2] << " " << roomAfter[3] << endl;

		int leftBefore = roomBefore[0];
		int topBefore = roomBefore[1];
		int rightBefore = roomBefore[2];
		int bottomBefore = roomBefore[3];

		int leftAfter = roomAfter[0];
		int topAfter = roomAfter[1];
		int rightAfter = roomAfter[2];
		int bottomAfter = roomAfter[3];
		/*
		if (dir == map.Vertical) 
		{
			int routeBefore = GetRandomInt(topBefore + 1, bottomBefore - 1);
			int routeAfter = GetRandomInt(topAfter + 1, bottomAfter - 1);

			if (leftBefore > leftAfter) swap(leftBefore,leftAfter);
			if (routeBefore > routeAfter) swap(routeBefore, routeAfter);
			map.AllSet(leftBefore, routeBefore, sx, routeBefore,map.Route);
			map.AllSet(sx, routeAfter, leftAfter, routeAfter, map.Route);
			map.AllSet(sx, routeBefore, sx,routeAfter,map.Route);
			
		}
		else 
		{
			int routeBefore = GetRandomInt(leftBefore + 1, rightBefore - 1);
			int routeAfter = GetRandomInt(leftAfter + 1, rightAfter - 1);
			
			if (topBefore > topAfter) swap(topBefore, topAfter);
			if (routeBefore > routeAfter) swap(routeBefore, routeAfter);
			map.AllSet(routeBefore, bottomBefore, routeBefore, sy, map.Route);
			map.AllSet(routeAfter, sy, routeAfter,topAfter ,map.Route);
			map.AllSet(routeBefore, sy, routeAfter, sy, map.Route);
		}
		*/
		++cnt;

	}
}


//���@��A���S���Y��
void Dig(Map& map, int x, int y)
{

	map.Set(x, y, map.Route);


	int moveX[4] = { 0,0,-1,1 };
	int moveY[4] = { 1,-1,0,0 };
	vector<int> act = { 0,1,2,3 };

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
			map.Show();
		}
	}

}
//���@��J�n
void DigStart(int width, int height, int sx, int sy)
{
	Map map(width, height);
	int xStart = sx;
	int yStart = sy;
	Dig(map, xStart, yStart);
	map.Show();
}
//�����@�J�n
void DivideStart(int width, int height, int sx, int sy)
{
	Map map(width, height);
	int xStart = sx;
	int yStart = sy;
	//Dig(map, xStart, yStart);

	AreaDivide(map);
	CreateRoom(map);
	CreateRoute(map);

//	for (auto a : map._divideArea)
//	{
//		map.AllSet(a[0], a[1], a[2], a[3], map.Route);
//	}

	
	for (auto a : map._divideLine)
	{
		map.AllSet(a[0], a[1], a[2], a[3], map.Route);

	}
	
	

	for (auto a : map._divideRoom)
	{
		map.AllSet(a[0], a[1], a[2], a[3], map.Route);

	}
	//map.AllSet(1, 2, 10, 12, map.Route);
	map.Show();
}

int main()
{
	//�ċA�֐����������邽�߁A���ƍ�����傫������ƁA�X�^�b�N�I�[�o�[�t���[�ɂ�菈������~����B
	//���[�v�ŏ�������������H
	//���A�����͊
	//��
	const int width = 80;
	//����
	const int height = 50;

	mt19937 mt{ random_device{}() };
	uniform_int_distribution<int> distW(0, width - 2);
	uniform_int_distribution<int> distH(0, height - 2);

	int startX = distW(mt);
	int startY = distH(mt);

	if (startX % 2 == 0) ++startX;
	if (startY % 2 == 0) ++startY;

	//DigStart(width, height, startX, startY);
	DivideStart(width, height, startX, startY);


	return 0;
}