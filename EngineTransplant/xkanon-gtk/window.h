/*  window.h
 *     �ᥤ�󡦥�����ɥ������
 *     ������ɥ��β������ѹ��ȡ��������åȤ����˴�Ϣ����᥽�åɤ����ޤäƤ���
 */
/*
 *
 *  Copyright (C) 2000-   Kazunori Ueno(JAGARL) <jagarl@creator.club.ne.jp>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
*/

#ifndef __KANON_WINDOW_H__
#define __KANON_WINDOW_H__

#include <gtk/gtk.h>

#include<vector>

#include "window_all.h"

#include "file.h"
#include "image_di.h"
#include "image_sys.h"
#include "image_cursor.h"
#include "anm.h"

/* gettext �� */
#  include <libintl.h>

/* AyuWindow �ξ��Ž��Ĥ����륦����ɥ� */
class PartWindow {
public:
	GtkWidget* wid; /* GtkDrawingArea */
	int x0, y0;
	bool is_realized;
	class AyuWindow* parent;
	GdkGC* gc;
	PartWindow(class AyuWindow* _p, int _x0, int _y0, int _w0, int _h0);
	~PartWindow();
	int WindowID(void);
	static gint exposeEvent(GdkEventExpose* event, gpointer pointer);
	static gint mapEvent(GdkEventAny*, gpointer);
};

class AyuWindow {
public:
	GtkWidget* wid;

	int is_initialized; // configure event ��������������ɥ�������������ȿ��ˤʤ�

	/* �ޥ����ξ��� */
	int mouse_x, mouse_y, mouse_clicked, mouse_key;
	int mouse_drawed; void* mouse_timer; void* retn_timer;

	/* �ƥ����ȼ���κ�ɸ */
	int text_x_first, text_x_end;
	int text_y_first, text_y_end;
	int text_x_pos, text_y_pos; // ����������Υƥ����Ȥν��ϰ���
	int text_first; // ���ߤΥƥ������������Ƭ����
	int char_width, char_height; // ��ʸ�����礭��
	int kinsoku_flag;
	int retn_x, retn_y;
	char drawed_text[1024*16];
	int text_pos; // ����������Υƥ����Ȥΰ���
	int text_window_type; // �ƥ����ȥ�����ɥ��ξ���
	int text_window_brightness;
	int text_window_x, text_window_y, text_window_width, text_window_height; /* �ƥ����ȥ�����ɥ��ΰ��� */

	/* select �Ѥ� pixmap */
	/* ʸ����Ĺ�� x char_height*7 ���礭�� */
	/* ɬ�פ˱����ơ�FreeType ��Ȥä�ʸ�������ǽ */
	GdkPixmap* select_pix;

	int assign_window_type;
	void AssignTextPixmap(int type);
	void FreeTextPixmap(void);
	void CalcTextGeom(int type);

	GdkGC* gc;
	PangoFontDescription* font;
	PangoLayout* font_layout;
	PangoContext* font_context;
	GdkColor fore_color, back_color;
	GIConv	iconv_euc;
	GIConv	iconv_sjis;
	void conv_euc(const char* from, char* to, int tosize = 1000);
	void conv_sjis(const char* from, char* to, int tosize = 1000);

	GdkColor white_color,black_color;

	GtkWidget* main_vbox; /* GtkVbox */
	GtkWidget* main; /* GtkFixed */
	GtkWidget* pix; /* GtkPixmap */
	int expose_flag;
	/* pixmap �ȼºݤΥ�����ɥ������Ƥ�Ʊ�����Ƥ��ʤ��Ȥ����� */
	/* ��������ǤϿ��Ȥʤ� */
	int not_synced_flag;
	void SyncPixmap(void);
	int redraw_x,redraw_y,redraw_width,redraw_height;

	/* �����̥⡼�� */
	int is_all_screen;
	Window_AllScreen all_screen;
	gulong enterEvent_handle;
	gulong leaveEvent_handle;

	/* �����ƥ�� PDT image . �ƥ����ȥ�����ɥ����ȡ��ޥ����������롢�꥿���󥫡�����ʤ� */
	SYSTEM_IMAGE* sys_im;
	/* pixmap �Υ������� */
	P_CURSOR* cursor;
	/* return cursor �ξ��� */
	int return_cursor_viewed;
	int return_cursor_type;

        // Gaiji stuffs
        DI_ImageMask* gaiji_pdt;
        int xcont, ycont;
        int xsize, ysize;

	// �����֡������ɡ���ư
	class AyuWin_Menu* menu_window;
	void hide_menu(void);
	void show_menu(void);

	// main window ���Ž��Ĥ������̥�����ɥ�
	std::vector<PartWindow*> parts;
	int MakePartWindow(int x, int y, int w, int h);
	void DeletePartWindow(int id);
private:

	// �ե����
	char* fontname;
	int fontsize;
	char* default_fontname;
	struct TextWinInfo* twinfo; // �Ȥꤢ�����ƥ����Ȱ��֤Ȥ�����¸��
	bool next_scroll;
	void ScrollupText(int lines); // �ƥ����Ȥ��� n �ԥ��������뤹��
public:
	/* ���Υ�����ɥ���°���륷���ƥ� */	
	AyuSys& local_system;

	GtkMenu* CreateSubMenu(char* name, int deal, /*const SigC::Slot1<void, int>*/ void* func);
	void CreateMenu(void); // menubar ��Ĥ���
	void UpdateMenu(char** save_titles); // menu bar ��Ĥ���
	void SetMenuTitle(char* title);
	void SetMenuBacklogMode(int mode); // mode �ˤ��碌�� backlog �Υ�˥塼������
		// mode == 1 �� backlog mode
	void DisplaySync(void); // X Server �Ȥ�Ʊ������
	GdkImage* image;
	GdkImage* image_with_text; // �ƥ����ȥ�����ɥ������褷�����֤Υ�����ɥ�
	GdkImage* image_without_text; // �ƥ����ȥ�����ɥ������褷�Ƥʤ����֤Υ�����ɥ�
	DI_Image* di_image_text;
	DI_ImageMask* di_image_icon;
	DI_Image* di_image_icon_back;
	enum ICON_STATE {ICON_NODRAW=0, ICON_AUTO = 16, ICON_FAST = 32, ICON_DRAW=256} icon_state;
	/* �������᡼���� GdkImage ���Ѵ����뤿��Υ��顼�ơ��֥� */
	unsigned int r_table[256];
	unsigned int g_table[256];
	unsigned int b_table[256];
	bool is_translation_required;
	GdkPixmap* pix_image;

	void InitText(void); // �ƥ����ȴط��ν����
	void SetFont(char* fontname); // �ե���Ȥ�����
	void SetFontSize(int size); // �ե���Ȥ��礭��������
	void DrawTextWindow(int window_type=1, int brightness=-1); // �ƥ����ȥ�����ɥ������褹��
			// window_type == -1 �� DeleteTextWindow
			// window_type == 0 �ǥ�����ɥ��Ȥʤ�
	int CheckBacklogButton(void); /* �ޥ������Хå������ܥ����ˤ��뤫��Ĵ�٤� */
	void SetDrawedText(char* str);
	void DeleteTextWindow(int* old_window_type=0, int* old_br = 0); // �ƥ����ȥ�����ɥ���õ��
	void DrawReturnCursor(void); // �꥿���󡦥��������ɽ��
	void DrawReturnCursor(int type); // �꥿���󡦥��������ɽ��
	void DeleteReturnCursor(void); // �꥿���󡦥��������õ�
	void DrawText(char* str); // �ƥ����Ȥ򤫤�
	int DrawTextEnd(int end_flag); // �ƥ��������褬����ä�����
			// end_flag != 0 �ǡ�������λ�����롣
        void LoadGaijiTable(void);
        void DrawGaiji(int index, int flag);
	void DrawOneChar(int flag = 0);
	void DrawUpdate(void);
	void DrawCurrentText(void);
	int DrawUpdateText(int pos);
	void DeleteText(void); // �ƥ����Ȥ�ä�
	int SelectItem(TextAttribute* textlist, int deal, int select_type); // �ƥ����Ȥ�����
	
	void DeleteMouse(void);
	void DrawMouse(void);

	// �����̥⡼��
	bool IsAllScreen(void);
	void ToAllScreen(void);
	void ToNoAllScreen(void);
	void SetMenuScreenmode(int mode);

	// �ޥ������٥�Ȥθ��Τ�ͭ��/��ͭ���ˤ���
        void MaskPointerEvent(void);
        void UnmaskPointerEvent(void);

	/* �ޥ�����������ե饰�ξ���
	** bit 0(1)     : ������å�
	** bit 1(2)     : ������å�
	** bit 2(4)     : CTRL ����������
	** bit 3(8)     : ������å���
	** bit 4(16)    : ������å���
	** bit 5(32)    : ��������ξ�
	** bit 6(64)    : ��������β�
	** bit 7(128)   : ESC (�Хå�����������)
	** bit 8(256)   : ESC(2) (�Хå������βս꤫�饲���೫��)
	** bit 9(512)   : ��������κ�
	** bit 10(1024) : ��������α�
	** bit 11(2048) : �ۥ�����ξ�
	** bit 12(4096) : �ۥ�����β�
	*/
	// �Ƕ�Υޥ�����������ξ������֤�
	void GetMouseState(int& x, int& y, int& clicked, int& now_click) {
		x = mouse_x; y = mouse_y;
		if (mouse_x == DELETED_MOUSE_X) {
			x = -1; y = -1;
		}
		clicked = -1; now_click = -1;
		if (mouse_clicked & 4) { clicked = 2;
		} else if (mouse_clicked & 1) { clicked = 0;
		} else if (mouse_clicked & 2) { clicked = 1;
		} else if (mouse_clicked & 2048) { clicked = 3;
		} else if (mouse_clicked & 4096) { clicked = 4;
		}
		if (mouse_clicked & 4) { now_click = 2;
		} else if (mouse_clicked&8 || mouse_clicked&1) { now_click = 0;
		} else if (mouse_clicked&16|| mouse_clicked&2) { now_click = 1;
		}
		return;
	}
	void ClearMouseState(void) {
		mouse_clicked &= ~(1|2|2048|4096);
	}
	void SetMouseMode(int is_key_use);
	void MouseClick(int button) { /* �ޥ����ܥ���򲡤������֤ˤ��� */
		if (button == 0) mouse_clicked |= 1;
		else if (button == 2) mouse_clicked |= 2;
	}
	// �Ƕ�Υ������륭���ξ������֤�
	void GetKeyCursorInfo(int& left, int& right, int& up, int& down, int& esc) {
		left=right=up=down=esc=0;
		if (mouse_clicked & (32|64|128|256|512|1024)) {
			if (mouse_clicked & 32) up=1;
			if (mouse_clicked & 64) down=1;
			if (mouse_clicked & 128) esc=1;
			if (mouse_clicked & 256) esc=2;
			if (mouse_clicked & 512) left=1;
			if (mouse_clicked & 1024) right=1;
			mouse_clicked &= ~(32|64|128|256|512|1024);
		}
	}
	// ��˥塼����ΥХå����������������Ϥȸ��ʤ�
	void EnterBacklog(void) {
		mouse_clicked |= 128;
	}
	void LeaveBacklog(void) {
		mouse_clicked |= 128;
	}
	void ContinueBacklog(void) {
		mouse_clicked |= 256;
	}

	GdkWindow* main_window;
	// ���ߤ� Window �����Ƥ����褹��
	void Draw(void);
	void Draw(int x1, int y1 ,int x2, int y2);
	void TranslateImage(int x1, int y1, int x2, int y2);
	// ���ꤵ�줿 sel �ǥᥤ�󥦥���ɥ������褹��
	void DrawImage(GdkImage* image, SEL_STRUCT* sel);
	AyuWindow(AyuSys& sys);
	~AyuWindow();
	// configure: ���ޤ��ޤʤ�Τν����
	gint configure_event_impl(GdkEventConfigure* p1);
	// destroy
	static gboolean destroyEvent(GtkWidget* w, GdkEventAny *event, gpointer pointer);
	static gint configureEvent(GtkWidget* w, GdkEventConfigure* p1, gpointer pointer);
	// main ��ǤΥޥ����������������
	static gboolean motionNotify(GtkWidget* w, GdkEventMotion *event, gpointer pointer);
	static gboolean enterNotify(GtkWidget* w, GdkEventCrossing *event, gpointer pointer);
	static gboolean leaveNotify(GtkWidget* w, GdkEventCrossing *event, gpointer pointer);
	static gboolean nullNotify(GtkWidget* w, GdkEventCrossing *event, gpointer pointer);
	static gboolean exposeEvent(GtkWidget* w, GdkEventExpose *event, gpointer pointer);
	static gboolean keyEvent(GtkWidget* w, GdkEventKey *event, gpointer pointer);
	static gboolean buttonEvent(GtkWidget* w, GdkEventButton *event, gpointer pointer);
	static gboolean scrollEvent(GtkWidget* w, GdkEventScroll *event, gpointer pointer);
	static gboolean focusEvent(GtkWidget* w, GdkEventFocus *event, gpointer pointer);


	gint motion_notify_impl(GdkEventMotion* event);
	gint enter_notify_impl(GdkEventCrossing* event);
	gint leave_notify_impl(GdkEventCrossing* event);
	gint expose_notify_impl(GdkEventExpose* event);
	// �ޥ����䥭���ܡ��ɤ򲡤��Τθ���
	gint button_event_impl(GdkEventButton* event);
	gint scroll_event_impl(GdkEventScroll* event);
	gint key_event_impl(GdkEventKey* event);
	gint focus_event_impl(GdkEventFocus* event);
	// �꥿���󡦥�����������褹��
	void TimerCall(void);
	void ChangeRetnCursor(void);

	int IsInitialized(void){return (is_initialized != 0); }

	void PopupLoadMenu(int button); // �ǡ����Υ����ɡ�������ɥ���ɽ������
	void PopupMenu(int button); // popup menu ��ɽ������
	// �ݥåץ��åץ�˥塼��ɽ��
	void ChangeMenuTextFast(void); // ��˥塼�Ρ�������פ�ͭ����̵���ˤ���
	void ShowMenuItem(const char* items, int active); // ��˥塼��ɽ�����롦���ʤ�

	// ̾������
	int OpenNameDialog(AyuSys::NameInfo* names, int list_deal);
	NameSubEntry* OpenNameEntry(int x, int y, int width, int height, const COLOR_TABLE& fore_color, const COLOR_TABLE& back_color);

	// ���̤��ɤ餹
	void Shake(int x, int y);
	// ���̤��֤�
	void BlinkWindow(int c1, int c2, int c3, int wait_time, int count);

	// avi �����Ѥ� window ID �򵢤�
	int GetWindowID(void);

	// icon ɽ���ط�
	void DeleteIconRegion(void);
	void CheckIconRegion(void);
	bool PressIconRegion(void); /* icon �� press ���줿�� true */
};

#endif