//---------------------------------------------------------
//	BerryLab
//
//		©2013 Yuichiro Nakada
//---------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <catcake_main.h>
#include "font.h"

// パラメータ
int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;
#define SCREEN_HZ		30

#define CAMERA_WIDTH		640
#define CAMERA_HEIGHT		480

// 画像
#if defined(__ANDROID__)
#define RESOURCE_PATH		"./sdcard/berry/"
#else
#define RESOURCE_PATH		"./assets/"
#endif
#define IMAGE_ICON		"camera-rotate.png"


class Task : public ckTask
{
public:
	Task();
	~Task();

private:
	virtual void onUpdate();
	//virtual void onMessage(ckID msg_id, ckMsg<4>& msg);
	void (Task::*Scene)();
	void SceneUpdate();

	ckScr *m_scr;
	ckSprt m_sprt;		// フレームバッファ・スプライト
	static ckTex *m_tex;	// フレームバッファ・テクスチャ
	static u8* image;	// フレームバッファ・アドレス

	r32 m_cur_mouse_x;
	r32 m_cur_mouse_y;
	r32 m_last_mouse_x;
	r32 m_last_mouse_y;
};
u8* Task::image;
ckTex *Task::m_tex;
void Task::onUpdate()
{
	(this->*Scene)();
}
Task::Task() : ckTask(ORDER_ZERO)
{
	//ckSndMgr::openSoundDevice(ckSndMgr::CHANNEL_NUM_STEREO, ckSndMgr::SAMPLE_RATE_44KHZ, 50);

	// フレームバッファ
	m_tex = ckDrawMgr::newTexture(ckID::genID(), CAMERA_WIDTH, CAMERA_HEIGHT, ckTex::FORMAT_RGBA);
//	m_tex = ckDrawMgr::newTexture(ckID::genID(), CAMERA_WIDTH, CAMERA_HEIGHT, ckTex::FORMAT_RGBA, image, CAMERA_WIDTH*CAMERA_HEIGHT*4);

	m_sprt.init(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
	m_sprt.setTextureID(m_tex->getID());
	m_sprt.setBlendMode(ckDraw::BLEND_HALF, true);
	m_sprt.dataPos(0).set(0, 0);	// 真ん中・スプライトの中央の座標
	m_sprt.setDataSize(0, SCREEN_WIDTH, SCREEN_HEIGHT);	// 拡大
//	m_sprt.setDataSize(0, CAMERA_WIDTH, CAMERA_HEIGHT);	// 原寸
	m_sprt.setDataUV(0, 0.0f, 0.0f, 1, 1);

	image = static_cast<u8*>(m_tex->beginEditImage());
	m_tex->endEditImage();

	m_cur_mouse_x = m_last_mouse_x = 0.0f;
	m_cur_mouse_y = m_last_mouse_y = 0.0f;


	Scene = &Task::SceneUpdate;


	ckSysMgr::printf("Constructor!");
}
Task::~Task()
{
	ckSysMgr::printf("Destructor!");
	//ckSndMgr::closeSoundDevice();
}
void Task::SceneUpdate()
{
	m_last_mouse_x = m_cur_mouse_x;
	m_last_mouse_y = m_cur_mouse_y;

	ckScr* scr = ckDrawMgr::getScreen(ckDrawMgr::DEFAULT_3D_SCREEN_ID);

	if (ckKeyMgr::isPressed(ckKeyMgr::KEY_LBUTTON)) {
		m_cur_mouse_x = m_last_mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
		m_cur_mouse_y = m_last_mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());
	}

	if (ckKeyMgr::isOn(ckKeyMgr::KEY_LBUTTON)) {
		m_cur_mouse_x = scr->framebufferXToScreenX(ckKeyMgr::getMouseX());
		m_cur_mouse_y = scr->framebufferYToScreenY(ckKeyMgr::getMouseY());
	}
	/*if ((m_cur_mouse_x - m_last_mouse_x) > 20) {
		type--;
		if (type<0) type = 0;
	}
	if ((m_cur_mouse_x - m_last_mouse_x) < -20) {
		type++;
		if (type>=iLabP_num) type = iLabP_num-1;
	}*/

	// 文字列表示
	ckDbgMgr::drawString(SCREEN_WIDTH/2-120, SCREEN_HEIGHT/2, ckCol(255, 192, 192, 192), /*size*/2, "%.2f FPS", ckTaskMgr::getCurFPS());
	ckDbgMgr::drawString(-SCREEN_WIDTH/2, SCREEN_HEIGHT/2-20, ckCol(255, 192, 192, 192), 2, "(%f,%f)", m_cur_mouse_x, m_cur_mouse_y);
	ckDbgMgr::drawString(-SCREEN_WIDTH/2, SCREEN_HEIGHT/2-40, ckCol(255, 192, 192, 192), 2, "(%f,%f)", m_last_mouse_x, m_last_mouse_y);

	image = static_cast<u8*>(m_tex->beginEditImage());
//	iLabP((int*)image, 640, s_camera_yuv, CAMERA_WIDTH, CAMERA_HEIGHT, type);
	m_tex->endEditImage();

	// 終了
	if (ckKeyMgr::isPressed(ckKeyMgr::KEY_ESCAPE)) {
		ckEndCatcake();
	}
}

ckMain()
{
#if defined(__ANDROID__)
	SCREEN_WIDTH = ckSysMgr::getFramebufferWidth();
	SCREEN_HEIGHT = ckSysMgr::getFramebufferHeight();
	ckSysMgr::printf("Start! %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
#endif
	ckCreateCatcake("BerryLab!", SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_HZ);

	ckResMgr::loadResource(RESOURCE_PATH IMAGE_FONT, true);
	ckResMgr::loadResource(RESOURCE_PATH IMAGE_FONT_P, true);
	ckMath::srand(static_cast<u32>(ckSysMgr::getUsecTime()));

	ckNewTask(Task);

	ckStartCatcake();
#if ! defined(__ANDROID__)
	ckDestroyCatcake();
#endif
}
