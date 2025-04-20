//#include "Game_object.h"
//#include "Animation.h"
//
//Player::Player() {
//	IMAGE img_shadow_player;
//	POINT player_pos = { 500 , 500 };
//	Animation* anim_left;
//	Animation* anim_right;
//	bool move_up = false;
//	bool move_down = false;
//	bool move_left = false;
//	bool move_right = false;
//
//	loadimage(&img_shadow_player, _T("img/img/shadow_player.png"));
//	anim_left = new Animation(_T("img/img/player_left_%d.png"), 6, 45);
//	anim_right = new Animation(_T("img/img/player_right_%d.png"), 6, 45);
//}
//
//Player::~Player() {
//	delete anim_left;
//	delete anim_right;
//}
//
//void Player::ProcessEvent(ExMessage& msg) {
//	if (msg.message == WM_KEYDOWN) {
//		switch (msg.vkcode)
//		{
//		case 0x57://w
//			move_up = true;
//			break;
//		case 0x53://s
//			move_down = true;
//			break;
//		case 0x41://a
//			move_left = true;
//			break;
//		case 0x44://d
//			move_right = true;
//			break;
//		}
//	}
//
//	else if (msg.message == WM_KEYUP) {
//		switch (msg.vkcode)
//		{
//		case 0x57://w
//			move_up = false;
//			break;
//		case 0x53://s
//			move_down = false;
//			break;
//		case 0x41://a
//			move_left = false;
//			break;
//		case 0x44://d
//			move_right = false;
//			break;
//		}
//	}
//}
//
//void Player::Move() {
//	const int WINDOWS_HIGHT = 720;
//	const int WINDOWS_WIDTH = 1280;
//
//	int delta_x = move_right - move_left;
//	int delta_y = move_down - move_up;
//	double len_dir = sqrt(delta_x * delta_x + delta_y * delta_y);
//	if (len_dir != 0) {
//		double unit_vector_x = delta_x / len_dir;
//		double unit_vector_y = delta_y / len_dir;
//		player_pos.x += (int)(speed * unit_vector_x);
//		player_pos.y += (int)(speed * unit_vector_y);
//	}
//
//	if (player_pos.x < 0) player_pos.x = 0;
//	if (player_pos.y < 0) player_pos.y = 0;
//	if (player_pos.x > WINDOWS_WIDTH - PLAYER_WIDTH)
//		player_pos.x = WINDOWS_WIDTH - PLAYER_WIDTH;
//	if (player_pos.y > WINDOWS_HIGHT - PLAYER_HIGHT)
//		player_pos.y = WINDOWS_HIGHT - PLAYER_HIGHT;
//}
//
//void Player::Draw(int delta) {
//	int player_shadow_x = player_pos.x + (PLAYER_WIDTH - PLAYER_SHADOW_WIDTH) / 2;
//	int player_shadow_y = player_pos.y + 72;
//
//	static bool face_left = false;
//	int dir_x = move_left - move_right;
//	if (dir_x > 0) {
//		face_left = true;
//	}
//	else if (dir_x < 0) {
//		face_left = false;
//	}
//
//	putimage_alpha(player_shadow_x, player_shadow_y, &img_shadow_player);
//
//	if (face_left) {
//		anim_left->Play(player_pos.x, player_pos.y, delta);
//	}
//	else {
//		anim_right->Play(player_pos.x, player_pos.y, delta);
//	}
//}
//
//const POINT Player::GetPosition() {
//	return player_pos;
//}
//
//
