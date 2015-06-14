//---------------------------------------------------------
//	Font!
//
//		©2013 Yuichiro Nakada
//---------------------------------------------------------
// http://xionchannel.blogspot.jp/2011/12/coco2d-advent-calendar-2011-2-cocos2d.html

#include <string.h>

#define MAX_STRING	70
#define IMAGE_FONT	"fontFixed.png"
#define IMAGE_FONT_P	"fontProportional.png"

struct FontRect {
	const char *c;
	unsigned short rect[4];
};
FontRect rect[] = {
"×", 11, 45, 6, 9,
"“", 14, 54, 4, 9,
"”", 14, 54, 4, 9,
"…", 22, 117, 6, 9,
"※", 5, 45, 6, 9,
"★", 41, 45, 6, 9,
"♥", 35, 45, 6, 9,
"♪", 54, 45, 8, 9,
"、", 6, 117, 4, 9,
"。", 10, 117, 4, 9,
"「", 14, 117, 4, 9,
"」", 18, 117, 4, 9,
"゛", 28, 122, 4, 4,
"゜", 32, 122, 4, 4,
"ァ", 7, 108, 6, 9,
"ア", 0, 63, 7, 9,
"ィ", 13, 108, 6, 9,
"イ", 7, 63, 6, 9,
"ゥ", 19, 108, 6, 9,
"ウ", 14, 63, 7, 9,
"ェ", 25, 108, 5, 9,
"エ", 21, 63, 7, 9,
"ォ", 30, 108, 6, 9,
"オ", 28, 63, 7, 9,
"カ", 35, 63, 7, 9,
"キ", 42, 63, 7, 9,
"ク", 49, 63, 7, 9,
"ケ", 56, 63, 7, 9,
"コ", 0, 72, 7, 9,
"サ", 7, 72, 7, 9,
"シ", 14, 72, 7, 9,
"ス", 21, 72, 7, 9,
"セ", 28, 72, 7, 9,
"ソ", 35, 72, 7, 9,
"タ", 42, 72, 7, 9,
"チ", 49, 72, 7, 9,
"ッ", 53, 108, 7, 9,
"ツ", 56, 72, 7, 9,
"テ", 0, 81, 7, 9,
"ト", 8, 81, 5, 9,
"ナ", 14, 81, 7, 9,
"ニ", 21, 81, 7, 9,
"ヌ", 28, 81, 7, 9,
"ネ", 35, 81, 7, 9,
"ノ", 42, 81, 6, 9,
"ハ", 49, 81, 7, 9,
"ヒ", 56, 81, 7, 9,
"フ", 0, 90, 7, 9,
"ヘ", 7, 90, 7, 9,
"ホ", 14, 90, 7, 9,
"マ", 21, 90, 7, 9,
"ミ", 28, 90, 7, 9,
"ム", 35, 90, 7, 9,
"メ", 42, 90, 7, 9,
"モ", 49, 90, 7, 9,
"ャ", 36, 108, 6, 9,
"ヤ", 56, 90, 7, 9,
"ュ", 42, 108, 6, 9,
"ユ", 0, 99, 7, 9,
"ョ", 48, 108, 5, 9,
"ヨ", 7, 99, 7, 9,
"ラ", 14, 99, 7, 9,
"リ", 21, 99, 7, 9,
"ル", 28, 99, 7, 9,
"レ", 36, 99, 6, 9,
"ロ", 42, 99, 7, 9,
"ワ", 49, 99, 7, 9,
"ヲ", 56, 99, 7, 9,
"ン", 0, 108, 7, 9,
"ー", 0, 117, 6, 9,
"＆", 17, 45, 7, 9,
"!", 6, 54, 3, 9,
"\"", 14, 54, 4, 9,
"#", 43, 54, 6, 9,
"%", 55, 54, 6, 9,
"'", 18, 54, 3, 9,
"(", 27, 45, 4, 9,
")", 31, 45, 4, 9,
"+", 37, 54, 6, 9,
",", 3, 54, 3, 9,
"-", 32, 54, 5, 9,
".", 0, 54, 3, 9,
"/", 49, 54, 6, 9,
"0", 0, 0, 5, 9,
"1", 5, 0, 5, 9,
"2", 10, 0, 5, 9,
"3", 15, 0, 5, 9,
"4", 20, 0, 5, 9,
"5", 25, 0, 5, 9,
"6", 30, 0, 5, 9,
"7", 35, 0, 5, 9,
"8", 40, 0, 5, 9,
"9", 45, 0, 5, 9,
":", 21, 54, 3, 9,
";", 24, 54, 3, 9,
"=", 27, 54, 5, 9,
"?", 9, 54, 5, 9,
"A", 50, 0, 5, 9,
"B", 55, 0, 5, 9,
"C", 0, 9, 5, 9,
"D", 5, 9, 5, 9,
"E", 10, 9, 5, 9,
"F", 15, 9, 5, 9,
"G", 20, 9, 5, 9,
"H", 25, 9, 5, 9,
"I", 30, 9, 3, 9,
"J", 33, 9, 5, 9,
"K", 38, 9, 5, 9,
"L", 43, 9, 5, 9,
"M", 48, 9, 7, 9,
"N", 55, 9, 6, 9,
"O", 0, 18, 5, 9,
"P", 5, 18, 5, 9,
"Q", 10, 18, 5, 9,
"R", 15, 18, 5, 9,
"S", 20, 18, 5, 9,
"T", 25, 18, 5, 9,
"U", 30, 18, 5, 9,
"V", 35, 18, 5, 9,
"W", 40, 18, 7, 9,
"X", 47, 18, 5, 9,
"Y", 52, 18, 5, 9,
"Z", 57, 18, 5, 9,
"`", 24, 45, 3, 9,
"a", 0, 27, 5, 9,
"b", 5, 27, 5, 9,
"c", 10, 27, 5, 9,
"d", 15, 27, 5, 9,
"e", 20, 27, 5, 9,
"f", 25, 27, 5, 9,
"g", 30, 27, 5, 9,
"h", 35, 27, 5, 9,
"i", 40, 27, 3, 9,
"j", 43, 27, 4, 9,
"k", 47, 27, 5, 9,
"l", 52, 27, 3, 9,
"m", 55, 27, 7, 9,
"n", 0, 36, 5, 9,
"o", 5, 36, 5, 9,
"p", 10, 36, 5, 9,
"q", 15, 36, 5, 9,
"r", 20, 36, 5, 9,
"s", 25, 36, 5, 9,
"t", 30, 36, 5, 9,
"u", 35, 36, 5, 9,
"v", 40, 36, 5, 9,
"w", 45, 36, 7, 9,
"x", 52, 36, 5, 9,
"y", 57, 36, 5, 9,
"z", 0, 45, 5, 9,
"~", 47, 45, 7, 9,
0, 0, 0, 0, 0
};

class FontTex {
private:
	ckScr *m_scr;
	ckSprt sprt, sprtP, sprtE;
	short n[3];

	short time[MAX_STRING]/*, timeP[MAX_STRING]*/;
	char type[MAX_STRING];	// エフェクトタイプ

public:
	FontTex() {
		m_scr = ckDrawMgr::newScreen(ckID::genID());
		m_scr->setClearMode(false, true);
		m_scr->setPerspective(false);
		m_scr->moveLast();

		sprt.init(MAX_STRING, m_scr->getID());
		//sprt.init(1, ckDrawMgr::DEFAULT_2D_SCREEN_ID);
		sprt.setTextureID(ckID_("fontFixed.png"));
		sprt.setBlendMode(ckDraw::BLEND_HALF, true);

		/*for (int i=0; i<MAX_STRING; i++) {
			sprt.dataPos(i).set(-ITEM_WIDTH-SCREEN_WIDTH/2, 0);
			sprt.setDataSize(i, ITEM_WIDTH, ITEM_HEIGHT);
			sprt.setDataUV(i, 0.0f, 0.0f, 1.0/16, 1.0/5);
		}*/

		sprtP.init(MAX_STRING, m_scr->getID());
		sprtP.setTextureID(ckID_(IMAGE_FONT_P));
		sprtP.setBlendMode(ckDraw::BLEND_HALF, true);

		sprtE.init(MAX_STRING, m_scr->getID());
		sprtE.setTextureID(ckID_(IMAGE_FONT));
		sprtE.setBlendMode(ckDraw::BLEND_HALF, true);
	}
	~FontTex() {
		ckDrawMgr::deleteScreen(m_scr->getID());
	}

	// 指定のキャラクターの範囲を返す(Nora Games)
	void getRectWithASCII(char ascii, float *r) {
		char c;
		if (ascii>='0' && ascii<='9') {
			c = ascii-'0'+16;
		} else if (ascii>='A' && ascii<='Z') {
			c = ascii-'A'+32+1;
		} else if (ascii>='a' && ascii<='z') {
			c = ascii-'a'+64+1;
		} else if (ascii>='!' && ascii<='/') {
			c = ascii-'!'+1;
		} else if (ascii>=':' && ascii<='@') {
			c = ascii-':'+26;
		} else if (ascii>='[' && ascii<='`') {
			c = ascii-'['+59;
		} else if (ascii>='{' && ascii<=0x7f) {
			c = ascii-'{'+91;
		} else {
			c = 0;	// スペースに置き換える
		}
		float x = 16*(c%16)/256.0;
		float y = 16*(c/16)/192.0;
		//printf("%d %f,%f\n",c, x, y);
		r[0] = x;
		r[1] = y;
		r[2] = x+16/256.0;
		r[3] = y+16/192.0;
		return;
	}
	// 指定のキャラクターの範囲を返す(88tHg3)
	/*void getRectWithASCII(char ascii, float *r) {
		char c;
		if (ascii>='0' && ascii<='9') {
			c = ascii-'0'+16;
		} else if (ascii>='A' && ascii<='Z') {
			c = ascii-'A'+32+1;
		} else if (ascii>='a' && ascii<='z') {
			c = ascii-'a'+64+1;
		} else if (ascii>='!' && ascii<='/') {
			c = ascii-'!'+1;
		} else if (ascii>=':' && ascii<='@') {
			c = ascii-':'+26;
		} else if (ascii>='[' && ascii<='`') {
			c = ascii-'['+59;
		} else if (ascii>='{' && ascii<=0x7f) {
			c = ascii-'{'+91;
		} else {
			c = 0;	// スペースに置き換える
		}
		float x = 8*(c%32)/256.0;
		float y = 8*(c/32)/24.0;
		//printf("%d %f,%f\n",c, x, y);
		r[0] = x;
		r[1] = y;
		r[2] = x+8/256.0;
		r[3] = y+8/24.0;
		return;
	}*/
	// 指定のキャラクターの範囲を返す
	/*void getRectWithASCII(char ascii, float *r) {
		char c;
		if (ascii>='0' && ascii<='9') {
			c = ascii-'0';
		}
		else if (ascii>='A' && ascii<='Z') {
			c = ascii-'A'+10;
		}
		else if (ascii>='a' && ascii<='z') {
			c = ascii-'a'+10;
		}
		else if (ascii=='!') {	//!
			c = ('Z'-'A'+10)+1;
		}
		else if (ascii=='?') {	//?
			c = ('Z'-'A'+10)+2;
		}
		else if (ascii=='.') {	//.
			c = ('Z'-'A'+10)+3;
		}
		else if (ascii==',') {	//,
			c = ('Z'-'A'+10)+4;
		}
		else if (ascii=='\'') {	//'
			c = ('Z'-'A'+10)+5;
		}
		else if (ascii=='/') {	//slash
			c = ('Z'-'A'+10)+6;
		}
		else if (ascii=='"') {	//"
			c = ('Z'-'A'+10)+7;
		}
		else {
			c = ('Z'-'A'+10)+8;	// スペースに置き換える
		}

		float x = 8*(c%8)/64.0;
		float y = 8*(c/8)/64.0;
		r[0] = x;
		r[1] = y;
		r[2] = x+8/64.0;
		r[3] = y+8/64.0;
		return;
	}*/
	void DrawString(r32 x, r32 y, char *s, r32 sx=16, r32 sy=16) {
		//int i=0;
		while (*s) {
			if (*s != ' ') {
				float r[4];
				getRectWithASCII(*s, r);
				sprt.dataPos(/*i*/n[0]).set(x, y);
				sprt.setDataSize(/*i*/n[0], /*16*/sx, /*16*/sy);
				sprt.setDataUV(/*i*/n[0], r[0], r[1], r[2], r[3]);
				//i++;
				n[0]++;
			}
			s++;
			x+=/*16*/sx;
		}
		//for (; i<MAX_STRING; i++) sprt.dataPos(i).set(-1000, 0);
	}
	void DrawStringCenter(r32 y, char *s, r32 sx=16, r32 sy=16) {
		//DrawString(sx/2 - strlen(s)*16/2, y, s);
		DrawString(-sx/2.0*strlen(s), y, s, sx, sy);
	}
	void DrawStringRight(r32 x, r32 y, char *s, r32 sx=16, r32 sy=16) {
		DrawString(x-sx*strlen(s), y, s);
	}
	void clear() {
		n[0]=0;
		n[1]=0;
		n[2]=0;
		for (int i=0; i<MAX_STRING; i++) sprt.dataPos(i).set(-1000, 0);
		for (int i=0; i<MAX_STRING; i++) sprtP.dataPos(i).set(-1000, 0);
	}

	void DrawEString(r32 x, r32 y, char *s, int t=-1) {
		if (n[2]>=MAX_STRING-10) n[2]=0;
		while (*s) {
			if (*s != ' ') {
				float r[4];
				getRectWithASCII(*s, r);
				sprtE.dataPos(n[2]).set(x, y);
				sprtE.setDataSize(n[2], 16, 16);
				sprtE.setDataUV(n[2], r[0], r[1], r[2], r[3]);
				time[n[2]] = t;
				n[2]++;
			}
			s++;
			x+=16;
		}
	}
	void effect() {
		for (int i=0; i<MAX_STRING; i++) {
			time[i]--;
			sprtE.dataPos(i).y -= 1;
			//sprtE.dataCol(i) -= ckCol(1, 0, 0, 0);
			if (time[i]==0) sprtE.dataPos(i).set(-1000, 0);
		}
	}

	unsigned short *getRectWithString(char *c, char **s) {
		FontRect *r = rect;
		(*s)++;
		while (r->c) {
			int n = strlen(r->c);
			if (!strncmp(r->c, c, n)) {
				(*s)+=n-1;
				return r->rect;
			}
			r++;
		}
		return 0;
	}
	void DrawPString(r32 x, r32 y, char *s) {
		while (*s) {
			unsigned short *r = getRectWithString(s, &s);
			//s++;
			if (!r) {
				x+=16;
			} else {
				//printf("(%d,%d) +%d +%d %d %d\n", x, y, r[2], r[3], r[0], r[1]);
				sprtP.dataPos(n[1]).set(x, y);
				sprtP.setDataSize(n[1], r[2]*2, r[3]*2);
				sprtP.setDataUV(n[1], r[0]/128.0, r[1]/128.0, (r[0]+r[2])/128.0, (r[1]+r[3])/128.0);
				n[1]++;
				x+=r[2]*2;
			}
		}
	}
	void DrawPStringCenter(r32 y, char *s) {
		r32 x = 0;
		char *p = s;
		int num = n[1];
		while (*s) {
			unsigned short *r = getRectWithString(s, &s);
			if (!r) {
				x+=16;
			} else {
				sprtP.dataPos(n[1]).set(x, y);
				sprtP.setDataSize(n[1], r[2]*2, r[3]*2);
				sprtP.setDataUV(n[1], r[0]/128.0, r[1]/128.0, (r[0]+r[2])/128.0, (r[1]+r[3])/128.0);
				n[1]++;
				x+=r[2]*2;
			}
		}
		x /= 2;
		while (*p) {
			sprtP.dataPos(num++).x -= x;
			p++;
		}
	}
};
