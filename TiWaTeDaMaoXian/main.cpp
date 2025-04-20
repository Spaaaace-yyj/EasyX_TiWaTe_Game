#include<iostream>
#include<easyx.h>
#include<string>
#include<vector>
#include<math.h>
#include<string.h>
using namespace std;

const int WINDOWS_HIGHT = 720;
const int WINDOWS_WIDTH = 1280;

IMAGE img_bk;

//alpha通道绘图
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib , "MSIMG32.LIB")



inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER , 0 , 255 , AC_SRC_ALPHA });
}

class Bullet {

public:
	POINT Bullet_position;
public:
	Bullet() = default;
	~Bullet() = default;

	void Draw() const {
		setlinecolor(RGB(255, 241, 30));
		setfillcolor(RGB(21, 255, 245));
		fillcircle(Bullet_position.x, Bullet_position.y, RADIUS);
	}


private:
	
	const int RADIUS = 8;
};


class Atlas {
public:
	Atlas(LPCTSTR path, int num){

		TCHAR path_file[256];
		for (size_t i = 0; i < num; i++) {

			_stprintf_s(path_file, path, i);//初始化

			IMAGE* frame = new IMAGE();
			loadimage(frame, path_file);
			frame_list.push_back(frame);
		}
	}
	~Atlas() {

	}

public:
	vector<IMAGE*> frame_list;
};


//动画加载类封装(TMD代码从上往下运行，如果有调用，要写前面，屮，调了老子半天)
class Animation {

public:
	Animation(Atlas *atlas, int interval) { // 动画图片路径 ， 帧数量 ，帧间隔
		interval_ms = interval;
		this->atlas = atlas;
	}

	~Animation() = default;

	void Play(int x, int y, int delta) {
		timer += delta;
		if (timer >= interval_ms) {
			index_frame = (index_frame + 1) % atlas->frame_list.size();
			timer = 0;
		}
		putimage_alpha(x, y, atlas->frame_list[index_frame]);
	}

private:
	int timer = 0; //动画计时器
	int index_frame = 0; //动画帧索引
	int interval_ms = 0;
	Atlas *atlas;

};

Atlas* enemy_left_atlas;
Atlas* enemy_right_atlas;
Atlas* player_left_atlas;
Atlas* player_right_atlas;

class Player {

public:
	Player() {
		loadimage(&img_shadow_player, _T("img/img/shadow_player.png"));
		anim_left = new Animation(player_left_atlas, 45);
		anim_right = new Animation(player_right_atlas, 45);
	}
	~Player() {
		delete anim_left;
		delete anim_right;
	}

	void ProcessEvent(ExMessage &msg) {
		if (msg.message == WM_KEYDOWN) {
			//cout << "yes";
			switch (msg.vkcode)
			{
			case 0x57://w
				move_up = true;
				break;
			case 0x53://s
				move_down = true;
				break;
			case 0x41://a
				move_left = true;
				break;
			case 0x44://d
				move_right = true;
				break;
			}
		}

		else if (msg.message == WM_KEYUP) {
			switch (msg.vkcode)
			{
			case 0x57://w
				move_up = false;
				break;
			case 0x53://s
				move_down = false;
				break;
			case 0x41://a
				move_left = false;
				break;
			case 0x44://d
				move_right = false;
				break;
			}
		}
	}

	void Move() {

		int delta_x = move_right - move_left;
		int delta_y = move_down - move_up;
		double len_dir = sqrt(delta_x * delta_x + delta_y * delta_y);
		if (len_dir != 0) {
			double unit_vector_x = delta_x / len_dir;
			double unit_vector_y = delta_y / len_dir;
			player_pos.x += (int)(speed * unit_vector_x);
			player_pos.y += (int)(speed * unit_vector_y);
		}

		if (player_pos.x < 0) player_pos.x = 0;
		if (player_pos.y < 0) player_pos.y = 0;
		if (player_pos.x > WINDOWS_WIDTH - PLAYER_WIDTH)
			player_pos.x = WINDOWS_WIDTH - PLAYER_WIDTH;
		if (player_pos.y > WINDOWS_HIGHT - PLAYER_HIGHT)
			player_pos.y = WINDOWS_HIGHT - PLAYER_HIGHT;

	}

	void Draw(int delta) {
		int player_shadow_x = player_pos.x + (PLAYER_WIDTH - PLAYER_SHADOW_WIDTH) / 2;
		int player_shadow_y = player_pos.y + 72;

		static bool face_left = false;
		int dir_x = move_left - move_right;
		if (dir_x > 0) {
			face_left = true;
		}
		else if (dir_x < 0) {
			face_left = false;
		}

		putimage_alpha(player_shadow_x, player_shadow_y, &img_shadow_player);

		if (face_left) {
			anim_left->Play(player_pos.x, player_pos.y, delta);
		}
		else {
			anim_right->Play(player_pos.x, player_pos.y, delta);
		}
	}

	const POINT GetPosition() { 
		return player_pos;
	}
	
public:
	const int PLAYER_HIGHT = 80;
	const int PLAYER_WIDTH = 80;

private:
	const int speed = 7;
	
	const int PLAYER_SHADOW_HIGHT = 20;
	const int PLAYER_SHADOW_WIDTH = 32;

private:
	IMAGE img_shadow_player;
	POINT player_pos = { 500 , 500 };
	Animation* anim_left;
	Animation* anim_right;
	bool move_up = false;
	bool move_down = false;
	bool move_left = false;
	bool move_right = false;
};

class Enemy {

public:
	Enemy() {
		loadimage(&img_shadow_Enemy, _T("img/img/shadow_enemy.png"));
		anim_left = new Animation(enemy_left_atlas, 45);
		anim_right = new Animation(enemy_right_atlas, 45);

		SpawnEdge edge = (SpawnEdge)(rand() % 4);
		switch (edge) {
		case SpawnEdge::UP:
			position.x = rand() % WINDOWS_WIDTH;
			position.y = -ENEMY_HIGHT;
			break;
		case SpawnEdge::DOWN:
			position.x = rand() % WINDOWS_WIDTH;
			position.y = WINDOWS_HIGHT + 1;
			break;
		case SpawnEdge::RIGHT:
			position.x = WINDOWS_WIDTH + 1;
			position.y = rand() % WINDOWS_HIGHT;
			break;
		case SpawnEdge::LEFT:
			position.x = -ENEMY_WIDTH - 1;
			position.y = rand() % WINDOWS_HIGHT;
			break;
		}
	}

	enum class SpawnEdge {
		UP = 0,
		DOWN,
		LEFT,
		RIGHT,
	};

	void Move(Player& player) {
		//cout << "move function is run" << endl;
		const POINT& player_pos = player.GetPosition();
		//printf("player_pos.x:%d || player_pos.y:%d\n", player_pos.x, player_pos.y);
		int dir_x = player_pos.x - position.x;
		int dir_y = player_pos.y - position.y;
		if (dir_x > 0) {
			facing_left = false;
		}
		else if (dir_x < 0) {
			facing_left = true;
		}
		double len_dir = sqrt(dir_x * dir_x + dir_y * dir_y);
		if (len_dir != 0) {
			double norm_x = dir_x / len_dir;
			double norm_y = dir_y / len_dir;
			position.x += (int)(speed * norm_x);
			position.y += (int)(speed * norm_y);
		}
		//cout << "position.x = " << position.x << "|| position.y = " << position.y << endl;
	}

	void Draw(int delta) {
		//cout << "Draw function is run" << endl;
		int enemy_shadow_x = position.x + (ENEMY_WIDTH - ENEMY_SHADOW_WIDTH) / 2;
		int enemy_shadow_y = position.y + ENEMY_HIGHT - 25;
		putimage_alpha(enemy_shadow_x, enemy_shadow_y, &img_shadow_Enemy);
		if (facing_left) {
			anim_left->Play(position.x, position.y, delta);
		}
		else {
			anim_right->Play(position.x, position.y, delta);
		}
	}

	bool CheckBulletCollition(const Bullet& bullet) {
		if (bullet.Bullet_position.x >= position.x && bullet.Bullet_position.x <= position.x + ENEMY_WIDTH
			&& bullet.Bullet_position.y >= position.y && bullet.Bullet_position.y <= position.y + ENEMY_HIGHT) {
			live = false;
			mciSendString(_T("play hit from 0"), NULL, 0, NULL);
			return true;
		}
		else {
			return false;
		}
	}

	bool CheckAlive() {
		return live;
	}

	bool CheckPlayer(Player& player) {
		POINT check_enemy_position = { position.x + ENEMY_WIDTH / 2, position.y + ENEMY_HIGHT / 2 };
		POINT player_box_point_1 = { player.GetPosition().x, player.GetPosition().y };
		POINT player_box_point_2 = { player.GetPosition().x + player.PLAYER_WIDTH, player.GetPosition().y + player.PLAYER_HIGHT };
		if (check_enemy_position.x >= player_box_point_1.x && check_enemy_position.x <= player_box_point_2.x
			&& check_enemy_position.y >= player_box_point_1.y && check_enemy_position.y <= player_box_point_2.y) {
			return true;
		}
		else {
			return false;
		}
	}

	~Enemy() {
		delete anim_left;
		delete anim_right;
	}

private:
	const int ENEMY_HIGHT = 80;
	const int ENEMY_WIDTH = 80;
	const int ENEMY_SHADOW_WIDTH = 48;
	const int ENEMY_SHADOW_HIGHT = 30;

private:
	POINT position = { 0 , 0 };
	IMAGE img_shadow_Enemy;
	Animation* anim_left;
	Animation* anim_right;
	bool facing_left = false;
	const int speed = 3;
	bool live = true;
};

void UpdateBullet(vector<Bullet>& bullet, Player& player) {
	const double RADIAL_SPEED = 0.0030;
	const double TANGENT_SPEED = 0.0055;
	double radian_interval = 2 * 3.14159 / bullet.size();
	POINT player_position = player.GetPosition();
	double radius = 100 + 25 * sin(GetTickCount() * RADIAL_SPEED);
	for (int i = 0; i < bullet.size(); i++) {
		double radian = GetTickCount() * TANGENT_SPEED + radian_interval * i;
		bullet[i].Bullet_position.x = player_position.x + player.PLAYER_WIDTH / 2 + (int)(radius * sin(radian));
		bullet[i].Bullet_position.y = player_position.y + player.PLAYER_HIGHT / 2 + (int)(radius * cos(radian));
	}
}

void TryGenerateEnemy(vector<Enemy*> &enemy_list) {
	const int Generate_Interval = 20;
	static int counter = 0;
	if ((counter++) % Generate_Interval == 0) {
		enemy_list.push_back(new Enemy());
	}
}

int main() {
	int counter = 0;
	ExMessage key;
	bool running = true;

	enemy_left_atlas = new Atlas(_T("img/img/enemy_left_%d.png"), 6);
	enemy_right_atlas = new Atlas(_T("img/img/enemy_right_%d.png"), 6);
	player_left_atlas = new Atlas(_T("img/img/player_left_%d.png"), 6);
	player_right_atlas = new Atlas(_T("img/img/player_right_%d.png"), 6);

	loadimage(&img_bk, _T("img/img/background.png"));
	Player Pai_Men;
	vector<Enemy*> enemy_list;
	vector<Bullet> bullet(3);

	initgraph(1280, 720, NULL);
	BeginBatchDraw();

	//音乐加载与播放
	mciSendString(_T("open E:\\Code\\easyX\\TiWaTeDaMaoXian\\TiWaTeDaMaoXian\\img\\mus\\bgm.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("Open E:\\Code\\easyX\\TiWaTeDaMaoXian\\TiWaTeDaMaoXian\\img\\mus\\hit.wav alias hit"), NULL, 0, NULL);

	mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);

	//主循环
	while (running) {
		int start_time = GetTickCount();
		TryGenerateEnemy(enemy_list);

		while (peekmessage(&key)) {
			Pai_Men.ProcessEvent(key);
		}
		Pai_Men.Move();
		UpdateBullet(bullet, Pai_Men);
		
		for (int i = 0; i < enemy_list.size(); i++) {
			enemy_list[i]->Move(Pai_Men);
			for(Bullet bullet : bullet){
				enemy_list[i]->CheckBulletCollition(bullet);
				if (enemy_list[i]->CheckBulletCollition(bullet)) {
					counter++;
				}
			}
			if (!enemy_list[i]->CheckAlive()) {
				auto itt = enemy_list.begin() + i;
				enemy_list.erase(itt);
				continue;
			}
		}

		for (Enemy* enemy : enemy_list) {
			if (enemy->CheckPlayer(Pai_Men)) {
				MessageBox(GetHWnd(), _T("此处插入战败动画：超棒的特效，宏大的战败场景QWQ"), _T("游戏结束"), MB_OK);
				running = false;
				return 0;
				//break;
			}
		}

		//渲染
		cout << counter << endl;
		string score = "counter = " + to_string(counter);
		cleardevice();
		putimage(0, 0, &img_bk);
		Pai_Men.Draw(1000 / 144);
		for (Enemy* enemy : enemy_list) {
			enemy->Draw(1000 / 144);
		}

		for (Bullet bullet : bullet) {
			bullet.Draw();
		}

		FlushBatchDraw();

		int end_time = GetTickCount();
		int detla_time = end_time - start_time;
		if (detla_time < 1000 / 60) {
			Sleep(1000 / 60 - detla_time);
		}

	}
	cout << "Game Over";

	delete enemy_left_atlas;
	delete enemy_right_atlas;
	delete player_left_atlas;
	delete player_right_atlas;

	EndBatchDraw();
	return 0;
}