//#include "Animation.h"
//#include<iostream>
//#include<easyx.h>
//#include<string>
//#include<vector>
//#include<math.h>
//
////alpha draw picture
//#pragma comment(lib , "MSIMG32.LIB")
//
//inline void putimage_alpha(int x, int y, IMAGE* img) {
//	int w = img->getwidth();
//	int h = img->getheight();
//	AlphaBlend(GetImageHDC(NULL), x, y, w, h,
//		GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER , 0 , 255 , AC_SRC_ALPHA });
//}
//
////Animation class
//Animation::Animation(LPCTSTR path, int num, int interval) {
//	interval_ms = interval;
//
//	TCHAR path_file[256];
//	for (size_t i = 0; i < num; i++) {
//
//		_stprintf_s(path_file, path, i);//初始化?
//
//		IMAGE* frame = new IMAGE();
//		loadimage(frame, path_file);
//		frame_list.push_back(frame);
//	}
//}
//
//Animation::~Animation() {
//	for (int i = 0; i <= frame_list.size(); i++) {
//		delete frame_list[i];//释放内存
//	}
//}
////Play Animation
//void Animation::Play(int x, int y, int delta) {
//	timer += delta;
//	if (timer >= interval_ms) {
//		index_frame = (index_frame + 1) % frame_list.size();
//		timer = 0;
//	}
//	putimage_alpha(x, y, frame_list[index_frame]);
//}
