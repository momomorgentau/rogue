#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

class Map
{
public :
	int _width; //��
	int _height; //����
	int outOfRange = -1;
	vector<vector<int>> _ground;


	enum mapValue
	{
		Wall,
		Route,
		Chara
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

		for (auto v : _ground) for (int i : v) i = Wall;
	}

	//�}�b�v��\��
	void show()
	{
		system("cls");
		
		for (auto i : _ground)
		{
			for (int j : i) cout << j;
			cout << endl;
		}
		
	}
};


int main() 
{
	Map map(10, 10);
	map.show();
	map.show();

	return 0;
}