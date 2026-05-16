/*
 * File: main.cpp
 * Project: Personal Finance Tracker v4.0 (Mint Edition)
 * Style: Dark Mint + Green + Cream Accent, large font
 * Compile:
 * g++ main.cpp -o main.exe -I./raylib-6.0_win64_mingw-w64/include -L./raylib-6.0_win64_mingw-w64/lib -lraylib -lopengl32 -lgdi32 -lwinmm
 */

#include "raylib.h"
#include "structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==========================================
   KÍCH THƯỚC CỬA SỔ & NHẬP LIỆU
   ========================================== */
#define SW 1100
#define SH 700
#define MAX_INPUT 64

/* ==========================================
   BẢNG MÀU MỚI: DARK MINT - GREEN - CREAM
   Dựa trên bảng màu bạn cung cấp (image_2.png)
   ========================================== */
// `#5D7B6F`: Xanh rêu đậm - Màu nền chính
#define C_BG        (Color){93,  123, 111, 255} 

// `#A4C3A2`: Xanh bạc hà vừa - Màu nền panel/thẻ mặc định
#define C_PANEL     (Color){164, 195, 162, 255} 
#define C_CARD      (Color){164, 195, 162, 255} 

// `#B0D4B8`: Xanh bạc hà nhạt - Màu hover/nhấn mạnh nhẹ
#define C_CARD2     (Color){176, 212, 184, 255} 

// `#EAE7D6`: Màu kem nhạt - Màu văn bản chính, điểm nhấn
#define C_WHITE     (Color){234, 231, 214, 255} 
#define C_WHITE2    (Color){234, 231, 214, 255} 

// `#D7F9FA`: Xanh băng cực nhạt - Màu accent phụ, văn bản nhấn
#define C_AQUA      (Color){215, 249, 250, 255} 

// Định nghĩa lại các màu chức năng dựa trên bảng màu mới
#define C_GRAY      (Color){120, 110, 140, 255}   /* Xám placeholder */
#define C_GREEN     (Color){80,  230, 160, 255}   /* Xanh lá (thu) - Giữ nguyên cho rõ ràng */
#define C_RED       (Color){255, 90,  90,  255}   /* Đỏ (chi) - Giữ nguyên cho rõ ràng */
#define C_BORDER    (Color){93,  123, 111, 255}   /* Màu viền */
#define C_TEXT_LABEL C_WHITE                     /* Màu nhãn chữ to */
#define C_TEXT_INPUT C_AQUA                      /* Màu chữ nhập liệu */

/* ==========================================
   ĐỊNH NGHĨA KÍCH THƯỚC CHỮ LỚN
   (Giải quyết vấn đề chữ "nhỏ" bạn đã tô vàng)
   ========================================== */
#define FS_TITLE_MAIN 52   /* Tiêu đề chính trang chủ */
#define FS_TOPBAR_TITLE 32 /* Tiêu đề thanh TopBar, nút Back */
#define FS_LABEL_INPUT 24  /* Nhãn cho ô nhập liệu (Date, Category, Amount, Type) */
#define FS_TEXT_INPUT 26   /* Chữ người dùng nhập trong ô */
#define FS_BUTTON      22  /* Chữ trong nút bấm */
#define FS_CARD_VAL    18  /* Chữ trong StatCard */

/* ==========================================
   LINKED LIST (DANH SÁCH LIÊN KẾT)
   Quản lý cấu trúc dữ liệu lưu các giao dịch
   ========================================== */
Node* createNode(Transaction t) {
    Node* n = (Node*)malloc(sizeof(Node));
    if (!n) return NULL;
    n->data = t; n->next = NULL; return n;
}
void appendNode(Node** head, Node* n) {
    if (!n) return;
    if (!*head) { *head = n; return; }
    Node* t = *head; 
    while (t->next) t = t->next; 
    t->next = n;
}
void freeAll(Node** head) {
    Node* c = *head, *nx;
    while (c) { nx = c->next; free(c); c = nx; }
    *head = NULL;
}
int countNodes(Node* h) { int c = 0; while(h) { c++; h = h->next; } return c; }
int getMaxId(Node* h) { int m = 0; while(h) { if(h->data.id > m) m = h->data.id; h = h->next; } return m; }
long long calcTotal(Node* h, int type) {
    if (!h) return 0;
    return ((h->data.type == type) ? h->data.amount : 0) + calcTotal(h->next, type);
}

/* ==========================================
   FILE I/O (ĐỌC/GHI FILE)
   Lưu trữ dữ liệu vĩnh viễn
   ========================================== */
void loadFromFile(Node** head) {
    FILE* f = fopen("data.txt", "r"); if (!f) return;
    Transaction t;
    while (fscanf(f, "%d %14s %49s %lld %d", &t.id, t.date, t.category, &t.amount, &t.type) == 5)
        appendNode(head, createNode(t));
    fclose(f);
}
void saveToFile(Node* head) {
    FILE* f = fopen("data.txt", "w"); if (!f) return;
    while (head) {
        fprintf(f, "%d %s %s %lld %d\n", head->data.id, head->data.date,
                head->data.category, head->data.amount, head->data.type);
        head = head->next;
    }
    fclose(f);
}

/* ==========================================
   SCREENS (CÁC MÀN HÌNH)
   Enum đại diện cho trạng thái hiện tại của ứng dụng
   ========================================== */
typedef enum { SCR_MENU, SCR_LIST, SCR_ADD, SCR_SEARCH, SCR_STATS, SCR_DELETE } Screen;

/* ==========================================
   UI HELPERS (CÁC HÀM VẼ GIAO DIỆN PHỤ TRỢ)
   Đã cập nhật màu sắc và kích thước chữ lớn
   ========================================== */
void GlowRect(float x, float y, float w, float h, float r) {
    DrawRectangleRounded({x - 2, y - 2, w + 4, h + 4}, r, 8, C_CARD2);
    DrawRectangleRounded({x, y, w, h}, r, 8, C_BG);
    DrawRectangleRoundedLinesEx({x, y, w, h}, r, 8, 1.5f, C_WHITE);
}

bool Btn(float x, float y, float w, float h, const char* lbl, bool primary = true) {
    Rectangle r = {x, y, w, h};
    bool hov = CheckCollisionPointRec(GetMousePosition(), r);
    // `#B0D4B8` (nhạt) cho primary hover, `#A4C3A2` (vừa) cho mặc định
    Color bg   = primary ? (hov ? C_CARD2 : C_PANEL) : (hov ? C_CARD2 : C_CARD);
    // `#215, 249, 250` (xanh băng) cho chữ nhấn, `#234, 231, 214` (kem) cho mặc định
    Color tc   = primary ? (hov ? C_AQUA : C_WHITE) : (hov ? C_AQUA : C_WHITE2);
    Color bord = primary ? C_WHITE : C_BORDER;

    DrawRectangleRounded({x + 3, y + 3, w, h}, 0.3f, 8, {0, 0, 0, 100});
    DrawRectangleRounded(r, 0.3f, 8, bg);
    DrawRectangleRoundedLinesEx(r, 0.3f, 8, hov ? 2.0f : 1.5f, bord);

    int fs = FS_BUTTON, fw = MeasureText(lbl, fs);
    DrawText(lbl, (int)(x + (w - fw) / 2), (int)(y + (h - fs) / 2), fs, tc);
    
    return hov && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

/* Ô nhập liệu - Đã tăng cỡ chữ cho nhãn và chữ nhập */
void InputBox(float x, float y, float w, float h, const char* lbl, char* buf, bool focused) {
    // Tăng tiêu đề từ 14 lên 24 (FS_LABEL_INPUT), đẩy lên cao một chút (y-24)
    DrawText(lbl, (int)x, (int)(y - 24), FS_LABEL_INPUT, focused ? C_AQUA : C_WHITE); 
    
    Rectangle r = {x, y, w, h};
    DrawRectangleRounded(r, 0.2f, 6, C_PANEL);
    DrawRectangleRoundedLinesEx(r, 0.2f, 6, focused ? 2.0f : 1.0f, focused ? C_WHITE : C_BORDER);
    if (focused) DrawRectangleRounded({x, y, w, h}, 0.2f, 6, (Color){176, 212, 184, 10});
    
    // Tăng chữ nhập liệu từ 18 lên 26 (FS_TEXT_INPUT), đổi màu chữ nhập thành xanh băng nhạt (C_TEXT_INPUT)
    DrawText(buf, (int)(x + 12), (int)(y + (h - 22) / 2), FS_TEXT_INPUT, C_TEXT_INPUT); 
    
    if (focused && (int)(GetTime() * 2) % 2 == 0) {
        int tw = MeasureText(buf, FS_TEXT_INPUT);
        DrawText("|", (int)(x + 12 + tw), (int)(y + (h - 22) / 2), FS_TEXT_INPUT, C_AQUA);
    }
}

void TypeText(char* buf, int maxLen) {
    int k = GetCharPressed();
    while (k > 0) {
        int l = strlen(buf);
        if (k >= 32 && l < maxLen - 1) {
            buf[l] = (char)k; 
            buf[l + 1] = '\0';
        }
        k = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && strlen(buf) > 0) buf[strlen(buf) - 1] = '\0';
}

void TopBar(const char* title, Screen* scr) {
    // Gradient xanh rêu đậm-bạc hà cho TopBar
    for (int i = 0; i < 70; i++) {
        float t = (float)i / 70;
        Color c = {(unsigned char)(93 + t * 71), (unsigned char)(123 + t * 72), (unsigned char)(111 + t * 51), 255};
        DrawRectangle(0, i, SW, 1, c);
    }
    DrawRectangle(0, 68, SW, 3, C_CARD2);
    DrawRectangle(0, 69, SW, 1, C_WHITE);

    int fs = FS_TOPBAR_TITLE;
    DrawText(title, 32, 18, fs, C_WHITE);
    DrawCircle(20, 35, 5, C_WHITE);

    if (Btn(SW - 155, 15, 120, 40, "< Back", false)) *scr = SCR_MENU;
}

void StatCard(float x, float y, float w, float h, const char* title, const char* val, Color vc) {
    DrawRectangleRounded({x + 3, y + 3, w, h}, 0.25f, 8, {0, 0, 0, 80});
    DrawRectangleRounded({x, y, w, h}, 0.25f, 8, C_CARD);
    DrawRectangleRounded({x + 1, y + 1, w - 2, 6}, 0.5f, 4, vc);
    DrawRectangleRoundedLinesEx({x, y, w, h}, 0.25f, 8, 1.0f, C_BORDER);

    int tw = MeasureText(title, 12); DrawText(title, (int)(x + (w - tw) / 2), (int)(y + 16), 12, C_GRAY);
    int vw = MeasureText(val, FS_CARD_VAL);   DrawText(val,   (int)(x + (w - vw) / 2), (int)(y + 38), FS_CARD_VAL, vc);
}

/* ==========================================
   SCREENS RENDERING (VẼ CÁC MÀN HÌNH CHÍNH)
   ========================================== */
void DrawMenu(Screen* scr) {
    // Gradient xanh rêu trang trí cho nền
    for (int y = 0; y < SH; y++) {
        float t = (float)y / SH;
        Color c = {(unsigned char)(93), (unsigned char)(123), (unsigned char)(111 + (int)(t * 6)), 255};
        DrawRectangle(0, y, SW, 1, c);
    }

    DrawCircle(SW - 80, 80, 120, (Color){164, 195, 162, 8});
    DrawCircle(SW - 80, 80, 80,  (Color){164, 195, 162, 10});
    DrawCircle(80, SH - 80, 100, (Color){164, 195, 162, 8});
    DrawCircle(SW / 2, SH / 2, 200, (Color){164, 195, 162, 5});

    DrawRectangle(0, 115, SW, 1, C_BORDER);
    DrawRectangle(0, 116, SW, 1, (Color){234, 231, 214, 40});

    const char* t1 = "PERSONAL FINANCE";
    const char* t2 = "TRACKER";
    int tw1 = MeasureText(t1, FS_TITLE_MAIN), tw2 = MeasureText(t2, FS_TITLE_MAIN-14);
    DrawText(t1, (SW - tw1) / 2, 18, FS_TITLE_MAIN, C_WHITE);
    DrawText(t2, (SW - tw2) / 2, 76, FS_TITLE_MAIN-14, C_AQUA);

    const char* sub = "Personal Finance Tracker  v4.0 (Mint Edition)";
    int sw = MeasureText(sub, 15);
    DrawText(sub, (SW - sw) / 2, 122, 15, C_GRAY);

    struct { const char* lbl; } items[] = {
        {"  +   Add Transaction"},
        {"  =   View Transactions"},
        {"  ?   Search"},
        {"  #   Statistics"},
        {"  -   Delete Transaction"},
    };
    Screen tgts[] = {SCR_ADD, SCR_LIST, SCR_SEARCH, SCR_STATS, SCR_DELETE};

    int bw = 380, bh = 58, gap = 14;
    int totalH = 5 * bh + 4 * gap;
    int sy = (SH - totalH) / 2 + 40, bx = (SW - bw) / 2;

    for (int i = 0; i < 5; i++) {
        int by = sy + i * (bh + gap);
        bool isPrimary = (i == 0 || i == 4);
        if (Btn((float)bx, (float)by, (float)bw, (float)bh, items[i].lbl, isPrimary))
            *scr = tgts[i];
    }

    const char* ft = "ESC to Exit  |  Auto-saves on close";
    DrawText(ft, (SW - MeasureText(ft, 13)) / 2, SH - 30, 13, C_GRAY);
}

void DrawList(Screen* scr, Node* head, int* sy) {
    TopBar("TRANSACTION LIST", scr);

    DrawRectangle(20, 74, SW - 40, 32, C_CARD);
    DrawRectangle(20, 74, 4, 32, C_AQUA);
    DrawText("ID",       38,  83, 14, C_WHITE2);
    DrawText("Date",     90,  83, 14, C_WHITE2);
    DrawText("Category", 220, 83, 14, C_WHITE2);
    DrawText("Amount",   530, 83, 14, C_WHITE2);
    DrawText("Type",     840, 83, 14, C_WHITE2);

    *sy -= (int)GetMouseWheelMove() * 34;
    if (*sy < 0) *sy = 0;

    int rowH = 40, startY = 108;
    
    BeginScissorMode(20, startY, SW - 40, SH - startY - 35);
    Node* cur = head; int idx = 0;
    while (cur) {
        int ry = startY + idx * rowH - *sy;
        
        if (ry > startY - rowH && ry < SH) {
            Color bg = (idx % 2 == 0) ? C_BG : C_PANEL;
            DrawRectangle(20, ry, SW - 40, rowH - 1, bg);

            Color tc = (cur->data.type == 1) ? C_AQUA : C_WHITE;
            DrawRectangle(20, ry, 4, rowH - 1, tc);

            char idS[10], amS[32];
            sprintf(idS, "%d", cur->data.id);
            sprintf(amS, "%lld VND", cur->data.amount);

            DrawText(idS,                38,  ry + 11, 16, C_WHITE2);
            DrawText(cur->data.date,     90,  ry + 11, 16, C_WHITE2);
            DrawText(cur->data.category, 220, ry + 11, 16, C_WHITE2);
            DrawText(amS,                530, ry + 11, 16, tc);

            const char* ts = (cur->data.type == 1) ? "INCOME" : "EXPENSE";
            // Badge xanh bạc hà nhạt với chữ kem nhấn mạnh
            DrawRectangleRounded({840.f, (float)(ry + 8), 70, 24}, 0.4f, 6, C_CARD2);
            int tbw = MeasureText(ts, 14);
            DrawText(ts, 840 + (70 - tbw) / 2, ry + 11, 14, C_WHITE);
        }
        cur = cur->next; idx++;
    }
    EndScissorMode();

    if (!head) {
        const char* e = "No transactions found!";
        DrawText(e, (SW - MeasureText(e, 22)) / 2, SH / 2, 22, C_GRAY);
    }
    char cs[64]; sprintf(cs, "Total: %d transactions", countNodes(head));
    DrawText(cs, 30, SH - 26, 13, C_GRAY);
}

struct AddForm { char date[MAX_INPUT], cat[MAX_INPUT], amt[MAX_INPUT], typ[MAX_INPUT]; int focus; char msg[128]; Color msgCol; };

/* ĐÃ KHẮC PHỤC LỖI ĐÈ BỐ CỤC Ô AMOUNT NHƯ TRONG IMAGE_1.PNG */
/* BẰNG CÁCH SỬ DỤNG MẢNG TỌA ĐỘ Y TĨNH CHO 4 Ô NHẬP LIỆU ĐỂ CHỪA CHỖ CHO HÀNG QUICK SELECT */
void DrawAdd(Screen* scr, Node** head, AddForm* f) {
    TopBar("ADD NEW TRANSACTION", scr);

    float fx = 160, fw = 700, fh = 46;
    
    // ĐÃ CHỈNH SỬA LẠI TỌA ĐỘ Y: Đẩy Amount và Type xuống sâu hơn để nhãn không chạm vào nút
    float y_pos[] = {
        108.0f,          // [0] Date
        193.0f,          // [1] Category 
        330.0f,          // [2] Amount (Đẩy từ 305 xuống 330)
        415.0f           // [3] Type (Đẩy từ 390 xuống 415)
    };

    const char* lbls[] = {"Date (DD/MM/YYYY):", "Category:", "Amount (VND):", "Type (1=Income / 0=Expense):"};
    char* bufs[] = {f->date, f->cat, f->amt, f->typ};

    for (int i = 0; i < 4; i++) {
        bool focused = (f->focus == i);
        InputBox(fx, y_pos[i], fw, fh, lbls[i], bufs[i], focused);
        Rectangle r = {fx, y_pos[i], fw, fh};
        if (CheckCollisionPointRec(GetMousePosition(), r) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) f->focus = i;
    }
    
    if (f->focus >= 0 && f->focus < 4) TypeText(bufs[f->focus], MAX_INPUT);
    if (IsKeyPressed(KEY_TAB)) f->focus = (f->focus + 1) % 4;

    /* Đổi C_GRAY thành C_WHITE để chữ "Quick select:" dễ đọc hơn trên nền Mint */
    const char* cats[] = {"Food", "Living", "Shopping", "Salary", "Transport"};
    DrawText("Quick select:", (int)fx, (int)(y_pos[1] + 55), FS_LABEL_INPUT-6, C_WHITE); 
    
    for (int i = 0; i < 5; i++)
        if (Btn(fx + i * 142, y_pos[1] + 78, 136, 32, cats[i], false))
            strncpy(f->cat, cats[i], MAX_INPUT - 1);

    /* Nút Submit tự động đẩy lùi xuống theo y_pos[3] */
    if (Btn(fx + fw / 2 - 130, y_pos[3] + 80, 260, 54, "  ADD TRANSACTION", true)) {
        if (!strlen(f->date) || !strlen(f->cat) || !strlen(f->amt) || !strlen(f->typ)) {
            strcpy(f->msg, "Please fill in all fields!"); f->msgCol = C_RED;
        } else {
            long long amt = atoll(f->amt); int tp = atoi(f->typ);
            if (amt <= 0) { strcpy(f->msg, "Amount must be > 0!"); f->msgCol = C_RED; }
            else if (tp != 0 && tp != 1) { strcpy(f->msg, "Type must be 0 or 1!"); f->msgCol = C_RED; }
            else {
                Transaction t; t.id = getMaxId(*head) + 1; t.amount = amt; t.type = tp;
                strncpy(t.date, f->date, MAX_DATE_LEN - 1); t.date[MAX_DATE_LEN - 1] = '\0';
                strncpy(t.category, f->cat, MAX_CAT_LEN - 1); t.category[MAX_CAT_LEN - 1] = '\0';
                
                appendNode(head, createNode(t));
                
                memset(f->date, 0, MAX_INPUT); memset(f->cat, 0, MAX_INPUT);
                memset(f->amt, 0, MAX_INPUT);  memset(f->typ, 0, MAX_INPUT);
                f->focus = 0;
                
                sprintf(f->msg, "Added ID [%d]  %s  %lld VND  %s", t.id, t.category, t.amount, t.type == 1 ? "INCOME" : "EXPENSE");
                f->msgCol = C_AQUA; 
            }
        }
    }

    if (strlen(f->msg)) {
        int mw = MeasureText(f->msg, 16);
        DrawRectangleRounded({(float)(SW - mw - 40) / 2, (float)(y_pos[3] + 148), (float)(mw + 40), 30}, 0.3f, 6, (Color){0, 0, 0, 120});
        DrawText(f->msg, (SW - mw) / 2, y_pos[3] + 154, 16, f->msgCol);
    }
}

struct SearchSt { char kw[MAX_INPUT]; bool searched; };

void DrawSearch(Screen* scr, Node* head, SearchSt* s) {
    TopBar("SEARCH TRANSACTIONS", scr);
    InputBox(150, 82, 580, 46, "Enter category keyword:", s->kw, true);
    TypeText(s->kw, MAX_INPUT);
    
    if (Btn(748, 82, 130, 46, "  Search", true) || IsKeyPressed(KEY_ENTER)) s->searched = true;
    if (!s->searched) return;

    DrawRectangle(20, 148, SW - 40, 30, C_CARD);
    DrawRectangle(20, 148, 4, 30, C_AQUA);
    DrawText("ID", 38, 156, 13, C_WHITE2); DrawText("Date", 90, 156, 13, C_WHITE2);
    DrawText("Category", 220, 156, 13, C_WHITE2); DrawText("Amount", 530, 156, 13, C_WHITE2); DrawText("Type", 840, 156, 13, C_WHITE2);

    Node* cur = head; int found = 0, y = 180, rH = 36;
    while (cur && y < SH - 40) {
        if (strstr(cur->data.category, s->kw)) {
            Color bg = (found % 2 == 0) ? C_BG : C_PANEL;
            DrawRectangle(20, y, SW - 40, rH - 1, bg);
            Color tc = (cur->data.type == 1) ? C_AQUA : C_WHITE;
            DrawRectangle(20, y, 4, rH - 1, tc);
            
            char idS[10], amS[32];
            sprintf(idS, "%d", cur->data.id); sprintf(amS, "%lld VND", cur->data.amount);
            DrawText(idS, 38, y + 10, 15, C_WHITE2); DrawText(cur->data.date, 90, y + 10, 15, C_WHITE2);
            DrawText(cur->data.category, 220, y + 10, 15, C_WHITE2); DrawText(amS, 530, y + 10, 15, tc);
            DrawText(cur->data.type == 1 ? "INCOME" : "EXPENSE", 840, y + 10, 15, tc);
            y += rH; found++;
        }
        cur = cur->next;
    }
    
    if (!found && strlen(s->kw)) {
        char m[128]; sprintf(m, "No results for: '%s'", s->kw);
        DrawText(m, (SW - MeasureText(m, 18)) / 2, y + 30, 18, C_GRAY);
    } else if (found) {
        char m[64]; sprintf(m, "Found %d results", found);
        DrawText(m, 30, SH - 26, 13, C_GRAY);
    }
}

void DrawStats(Screen* scr, Node* head) {
    TopBar("FINANCIAL STATISTICS", scr);

    long long inc = calcTotal(head, 1), exp = calcTotal(head, 0), bal = inc - exp;
    int tot = countNodes(head);
    char sI[40], sE[40], sB[40], sT[20];
    sprintf(sI, "%lld VND", inc); sprintf(sE, "%lld VND", exp);
    sprintf(sB, "%lld VND", bal); sprintf(sT, "%d", tot);

    // Chuyển thẻ StatCard sang màu xanh rêu-bạc hà
    StatCard(20,  78, 255, 90, "TOTAL INCOME",  sI, C_WHITE);
    StatCard(295, 78, 255, 90, "TOTAL EXPENSE", sE, C_BG);
    StatCard(570, 78, 255, 90, "BALANCE",       sB, bal >= 0 ? C_WHITE : C_BG);
    StatCard(845, 78, 230, 90, "TOTAL TRANS",   sT, C_WHITE);

    // Barchart với màu rêu-bạc hà đậm-nhạt
    DrawRectangle(20, 182, SW - 40, 250, C_CARD);
    DrawRectangle(20, 182, 4, 250, C_WHITE);
    DrawText("INCOME - EXPENSE CHART", 44, 192, 17, C_WHITE);

    long long mx = (inc > exp) ? inc : exp;
    if (mx > 0) {
        int bMaxH = 180, bW = 150, bY = 408;
        DrawRectangle(80, bY, 740, 2, C_BORDER); 

        auto drawBar = [&](int bx, long long val, Color col, const char* lbl, const char* valStr) {
            int h = (int)((float)val / mx * bMaxH);
            for (int i = 0; i < h; i++) {
                float t = (float)i / h;
                Color c = {col.r, col.g, (unsigned char)(col.b + t * 40), (unsigned char)(180 + t * 75)};
                DrawRectangle(bx, bY - i, bW, 1, c);
            }
            DrawRectangle(bx, bY - h, bW, 4, (Color){col.r, col.g, col.b, 200});
            int lw = MeasureText(lbl, 14); DrawText(lbl, bx + (bW - lw) / 2, bY + 8, 14, col);
            int vw = MeasureText(valStr, 12); DrawText(valStr, bx + (bW - vw) / 2, bY - h - 20, 12, col);
        };

        drawBar(100, inc, C_BG, "INCOME", sI);
        drawBar(380, exp, C_PANEL,   "EXPENSE", sE);
        if (bal > 0) drawBar(660, bal, C_WHITE, "BALANCE", sB);
    } else {
        DrawText("No data available", (SW - MeasureText("No data available", 18)) / 2, 290, 18, C_GRAY);
    }

    // Cảnh báo sử dụng màu bạc hà rêu đậm-nhạt nhấn mạnh
    if (bal < 0) {
        DrawRectangle(20, SH - 56, SW - 40, 42, {93, 20, 20, 220});
        DrawRectangle(20, SH - 56, 4, 42, C_BG);
        const char* w = "WARNING: Expenses exceed income!";
        DrawText(w, (SW - MeasureText(w, 18)) / 2, SH - 44, 18, C_BG);
    } else if (tot > 0) {
        DrawRectangle(20, SH - 56, SW - 40, 42, {164, 195, 162, 220});
        DrawRectangle(20, SH - 56, 4, 42, C_WHITE);
        const char* ok = "Finances stable - Keep it up!";
        DrawText(ok, (SW - MeasureText(ok, 18)) / 2, SH - 44, 18, C_WHITE);
    }
}

struct DelSt { char id[MAX_INPUT]; char msg[128]; Color msgCol; };

void DrawDelete(Screen* scr, Node** head, DelSt* d) {
    TopBar("DELETE TRANSACTION", scr);
    
    // 1. ĐẨY TỌA ĐỘ Y XUỐNG (Từ 88 xuống 120) ĐỂ KHÔNG ĐÈ LÊN TOPBAR
    InputBox(330, 120, 380, 46, "Enter Transaction ID to delete:", d->id, true);
    TypeText(d->id, MAX_INPUT);
    
    // 2. ĐẨY NÚT BẤM XUỐNG TƯƠNG ỨNG (Từ 150 xuống 190)
    if (Btn(425, 190, 210, 48, "  DELETE", true)) {
        int delId = atoi(d->id);
        if (delId <= 0) { strcpy(d->msg, "Invalid ID!"); d->msgCol = C_RED; }
        else {
            Node* cur = *head, *prev = NULL; bool found = false;
            while (cur) {
                if (cur->data.id == delId) {
                    if (prev) prev->next = cur->next; 
                    else *head = cur->next; 
                    free(cur); found = true; break;
                }
                prev = cur; cur = cur->next;
            }
            if (found) { sprintf(d->msg, "Successfully deleted ID [%d]!", delId); d->msgCol = C_AQUA; memset(d->id, 0, MAX_INPUT); }
            else { sprintf(d->msg, "ID [%d] not found!", delId); d->msgCol = C_RED; }
        }
    }
    
    // Đẩy dòng thông báo báo lỗi/thành công xuống 255
    if (strlen(d->msg)) { int mw = MeasureText(d->msg, 17); DrawText(d->msg, (SW - mw) / 2, 255, 17, d->msgCol); }

    // 3. ĐỔI MÀU LABEL VÀ ĐẨY XUỐNG (Đổi C_GRAY thành C_WHITE cho rõ)
    DrawText("Current transactions:", 36, 280, 18, C_WHITE);
    
    Node* cur = *head; 
    int y = 310, rH = 32; // Đẩy danh sách bắt đầu từ Y=310
    
    while (cur && y < SH - 30) {
        // 4. SỬA LỖI TÀNG HÌNH: Dùng C_AQUA (Xanh băng) cho INCOME, C_WHITE (Kem) cho EXPENSE
        Color c = (cur->data.type == 1) ? C_AQUA : C_WHITE;
        
        DrawRectangle(28, y, SW - 56, rH - 2, {164, 195, 162, 18});
        DrawRectangle(28, y, 3, rH - 2, c);
        
        char row[160];
        sprintf(row, "[%d]  %s  |  %s  |  %lld VND  |  %s",
                cur->data.id, cur->data.date, cur->data.category,
                cur->data.amount, cur->data.type == 1 ? "INCOME" : "EXPENSE");
        
        DrawText(row, 38, y + 7, 16, c); // Tăng size chữ trong list lên 16 cho dễ đọc
        
        y += rH; cur = cur->next;
    }
    if (!*head) DrawText("List is empty!", (SW - MeasureText("List is empty!", 18)) / 2, 350, 18, C_WHITE);
}

/* ==========================================
   HÀM MAIN (CHƯƠNG TRÌNH CHÍNH)
   ========================================== */
int main() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // Tên cửa sổ mới theo chủ đề Mint
    InitWindow(SW, SH, "Personal Finance Tracker  v4.0 (Mint Edition)");
    SetTargetFPS(60); 

    Node* head = NULL;
    loadFromFile(&head);

    Screen scr = SCR_MENU;
    AddForm  addF = {0}; addF.focus = 0;
    SearchSt srch = {0};
    DelSt    del = {0};
    int      listSY = 0; 

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            if (scr == SCR_MENU) break; 
            else scr = SCR_MENU;        
        }

        BeginDrawing();
        ClearBackground(C_BG); 

        // Lưới mờ rêu-bạc hà
        for (int i = 0; i < SW; i += 100) DrawLine(i, 0, i, SH, {164, 195, 162, 3});
        for (int i = 0; i < SH; i += 100) DrawLine(0, i, SW, i, {164, 195, 162, 3});

        switch (scr) {
            case SCR_MENU:   DrawMenu(&scr);              break;
            case SCR_LIST:   DrawList(&scr, head, &listSY); break;
            case SCR_ADD:    DrawAdd(&scr, &head, &addF);   break;
            case SCR_SEARCH: DrawSearch(&scr, head, &srch); break;
            case SCR_STATS:  DrawStats(&scr, head);         break;
            case SCR_DELETE: DrawDelete(&scr, &head, &del); break;
        }

        EndDrawing(); 
    }

    saveToFile(head);
    freeAll(&head);
    CloseWindow();
    return 0;
}