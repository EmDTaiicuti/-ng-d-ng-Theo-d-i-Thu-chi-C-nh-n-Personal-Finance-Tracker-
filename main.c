/* 4.2 sửa giao diện cho khung*/
/*4.3 Sửa lỗi hiển thị bảng  cho chức năng AI */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> // thêm thư viện xử lý chuỗi
#include <time.h>
#include <windows.h>
#include "structs.h"

// ==========================================
// MAU ANSI
// ==========================================
#define RESET   "\033[0m"       // trả về màu mặc định
#define RED     "\033[1;31m"    // đỏ (dùng cho báo lỗi và tiền chi)
#define GREEN   "\033[1;32m"    // xanh lá (dùng cho thành công và tiền thu)
#define YELLOW  "\033[1;33m"    // vàng (dùng cho tiêu đề)
#define CYAN    "\033[1;36m"    // xanh lơ (dùng cho khung viền bảng)
#define MAGENTA "\033[1;35m"
#define BOLD    "\033[1m"       // in đậm
#define WHITE   "\033[1;37m"

#define MW 43  // menu width tong

// ==========================================
// KHAI BAO HAM
// ==========================================
// khai báo sẵn các chức năng của hàm
void        addTransaction(Node** head); // đã sửa lỗi thiếu tham số
void        displayTransactions(Node* head); // thêm tham số
void        searchTransaction(Node* head); // thêm tham số truyền vào
void        deleteTransaction(Node** head); // thêm tham số con trỏ bậc 2
void        showStatistics(Node* head);
void        analyzeAI(Node* head);
void        loadFromFile(Node** head); // thêm tham số
void        saveToFile(Node* head); // thêm tham số
void        freeMemory(Node** head); // khai báo thêm hàm dọn rác
Node* createNode(Transaction t);
int         getNextId(Node* head);
void        getCurrentDate(char* buf);
void        showMenu();
void        printLine(char ch);

// ==========================================
// HAM HO TRO CHUNG
// ==========================================

void enableANSI() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
}

void getCurrentDate(char* buf) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buf, 15, "%d/%m/%Y", tm_info);
}

int getNextId(Node* head) {
    int maxId = 0;
    Node* cur = head;
    while (cur != NULL) {
        if (cur->data.id > maxId) maxId = cur->data.id;
        cur = cur->next;
    }
    return maxId + 1;
}

// hàm cấp phát bộ nhớ động tạo nọt mới
Node* createNode(Transaction t) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf(RED "[!] Loi: Khong the cap phat bo nho!\n" RESET);
        return NULL;
    }
    // đổ dữ liệu vào node và cho con trỏ trỏ vào null
    newNode->data = t;
    newNode->next = NULL;
    return newNode;
}

void printLine(char ch) {
    printf(CYAN);
    for (int i = 0; i < 79; i++) putchar(ch);
    printf(RESET "\n");
}

// ==========================================
// HAM HO TRO VE GIAO DIEN DONG BO
// ==========================================
void printTableTop() {
    printf(CYAN "\n \xc9");
    for(int i=0;i<6;i++) putchar('\xcd'); printf("\xcb");
    for(int i=0;i<12;i++) putchar('\xcd'); printf("\xcb");
    for(int i=0;i<20;i++) putchar('\xcd'); printf("\xcb");
    for(int i=0;i<20;i++) putchar('\xcd'); printf("\xcb");
    for(int i=0;i<9;i++) putchar('\xcd'); printf("\xbb\n" RESET);
}
void printTableMid() {
    printf(CYAN " \xcc");
    for(int i=0;i<6;i++) putchar('\xcd'); printf("\xce");
    for(int i=0;i<12;i++) putchar('\xcd'); printf("\xce");
    for(int i=0;i<20;i++) putchar('\xcd'); printf("\xce");
    for(int i=0;i<20;i++) putchar('\xcd'); printf("\xce");
    for(int i=0;i<9;i++) putchar('\xcd'); printf("\xb9\n" RESET);
}
void printTableBot() {
    printf(CYAN " \xc8");
    for(int i=0;i<6;i++) putchar('\xcd'); printf("\xca");
    for(int i=0;i<12;i++) putchar('\xcd'); printf("\xca");
    for(int i=0;i<20;i++) putchar('\xcd'); printf("\xca");
    for(int i=0;i<20;i++) putchar('\xcd'); printf("\xca");
    for(int i=0;i<9;i++) putchar('\xcd'); printf("\xbc\n" RESET);
}
void printBoxTop(int w) {
    printf(CYAN "\n \xc9"); for(int i=0;i<w;i++) putchar('\xcd'); printf("\xbb\n" RESET);
}
void printBoxMid(int w) {
    printf(CYAN " \xcc"); for(int i=0;i<w;i++) putchar('\xcd'); printf("\xb9\n" RESET);
}
void printBoxBot(int w) {
    printf(CYAN " \xc8"); for(int i=0;i<w;i++) putchar('\xcd'); printf("\xbc\n" RESET);
}

// ==========================================
// 1. THEM GIAO DICH
// ==========================================
// hàm thêm giao dịch do người dùng nhập vào Danh sách (Thêm vào cuối - Insert at Tail)
void addTransaction(Node** head) {
    Transaction t;
    t.id = getNextId(*head);

    printf(YELLOW "\n--- THEM GIAO DICH MOI (ID tu dong: %d) ---\n" RESET, t.id);

    // ---- BUOC 1: Chon loai Thu/Chi TRUOC ----
    printf(CYAN "\n --- LOAI GIAO DICH ---\n" RESET);
    printf(" [" RED "0" RESET "] Chi tieu\n");
    printf(" [" GREEN "1" RESET "] Thu nhap\n");
    printf(YELLOW " -> Chon (0 hoac 1): " RESET);

    while (1) {
        if (scanf("%d", &t.type) == 1 && (t.type == 0 || t.type == 1)) {
            while (getchar() != '\n');
            break;
        }
        while (getchar() != '\n');
        printf(RED " [!] Chi nhap 0 (Chi) hoac 1 (Thu): " RESET);
    }

    // ---- BUOC 2: Chon danh muc theo loai ----
    // giao diện chọn nhanh danh mục (Chống lười)
    int catChoice;
    if (t.type == 0) {
        printf(CYAN "\n --- DANH MUC CHI ---\n" RESET);
        printf(" [1] An uong\n");
        printf(" [2] Giai tri\n");
        printf(" [3] Tien nha / Sinh hoat\n");
        printf(" [4] Mua sam\n");
        printf(" [5] Suc khoe\n");
        printf(" [6] Dau tu\n");
        printf(" [7] Khac (tu nhap)...\n");
        printf(YELLOW " -> Chon so (1-7): " RESET);

        while (1) {
            if (scanf("%d", &catChoice) == 1 && catChoice >= 1 && catChoice <= 7) {
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');
            printf(RED " [!] Chon tu 1 den 7: " RESET);
        }

        switch (catChoice) {
            case 1: strcpy(t.category, "An_uong");    break;
            case 2: strcpy(t.category, "Giai_tri");   break;
            case 3: strcpy(t.category, "Tien_nha");   break;
            case 4: strcpy(t.category, "Mua_sam");    break;
            case 5: strcpy(t.category, "Suc_khoe");   break;
            case 6: strcpy(t.category, "Dau_tu");     break;
            default:
                printf(" Nhap ten danh muc (khong dau cach): ");
                scanf("%49s", t.category);
                while (getchar() != '\n');
        }
    } else {
        printf(CYAN "\n --- DANH MUC THU ---\n" RESET);
        printf(" [1] Luong\n");
        printf(" [2] Thuong\n");
        printf(" [3] Refund (hoan tien)\n");
        printf(" [4] Lai (tiet kiem / dau tu)\n");
        printf(" [5] Thu nhap khac...\n");
        printf(YELLOW " -> Chon so (1-5): " RESET);

        while (1) {
            if (scanf("%d", &catChoice) == 1 && catChoice >= 1 && catChoice <= 5) {
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');
            printf(RED " [!] Chon tu 1 den 5: " RESET);
        }

        switch (catChoice) {
            case 1: strcpy(t.category, "Luong");      break;
            case 2: strcpy(t.category, "Thuong");     break;
            case 3: strcpy(t.category, "Refund");     break;
            case 4: strcpy(t.category, "Lai");        break;
            default:
                printf(" Nhap ten danh muc (khong dau cach): ");
                scanf("%49s", t.category);
                while (getchar() != '\n');
        }
    }

    // ---- BUOC 3: Nhap ngay ----
    printf(CYAN "\n --- NGAY GIAO DICH ---\n" RESET);
    printf(" [1] Dung ngay hom nay\n");
    printf(" [2] Nhap thu cong\n");
    printf(YELLOW " -> Chon (1 hoac 2): " RESET);

    int dateChoice;
    while (1) {
        if (scanf("%d", &dateChoice) == 1 && (dateChoice == 1 || dateChoice == 2)) {
            while (getchar() != '\n');
            break;
        }
        while (getchar() != '\n');
        printf(RED " [!] Chi nhap 1 hoac 2: " RESET);
    }

    if (dateChoice == 1) {
        getCurrentDate(t.date);
        printf(GREEN " => Ngay tu dong: %s\n" RESET, t.date);
    } else {
        int maxDays[13] = {0, 31,28,31,30,31,30,31,31,30,31,30,31};

        int inputYear;
        printf(YELLOW " Nhap Nam (1900 - 2026): " RESET);
        while (1) {
            if (scanf("%d", &inputYear) == 1 && inputYear >= 1900 && inputYear <= 2026) {
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');
            printf(RED " [!] Nam phai tu 1900 den 2026: " RESET);
        }

        int inputMonth;
        printf(YELLOW " Nhap Thang (1 - 12): " RESET);
        while (1) {
            if (scanf("%d", &inputMonth) == 1 && inputMonth >= 1 && inputMonth <= 12) {
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');
            printf(RED " [!] Thang phai tu 1 den 12: " RESET);
        }

        int isLeap = (inputYear % 4 == 0 && inputYear % 100 != 0) || (inputYear % 400 == 0);
        if (inputMonth == 2 && isLeap) maxDays[2] = 29;

        int inputDay;
        printf(YELLOW " Nhap Ngay (1 - %d): " RESET, maxDays[inputMonth]);
        while (1) {
            if (scanf("%d", &inputDay) == 1 && inputDay >= 1 && inputDay <= maxDays[inputMonth]) {
                while (getchar() != '\n');
                break;
            }
            while (getchar() != '\n');
            printf(RED " [!] Ngay phai tu 1 den %d: " RESET, maxDays[inputMonth]);
        }

        snprintf(t.date, sizeof(t.date), "%02d/%02d/%04d", inputDay, inputMonth, inputYear);
        printf(GREEN " => Ngay da chon: %s\n" RESET, t.date);
    }

    // ---- BUOC 4: Nhap so tien ----
    printf(YELLOW " Nhap So tien (VND, > 0): " RESET);
    while (1) {
        int scanResult = scanf("%lld", &t.amount);
        while (getchar() != '\n');

        if (scanResult == 1 && t.amount > 0) {
            break;
        }
        printf(RED " [!] So tien khong hop le! Vui long nhap lai (so nguyen duong): " RESET);
    }

    // ---- THEM VAO DANH SACH ----
    Node* newNode = createNode(t);
    if (newNode == NULL) return;

    if (*head == NULL) {
        *head = newNode;
    } else {
        Node* temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }

    printf(GREEN "\n => Da them giao dich [%s - %lld VND] thanh cong!\n" RESET,
           t.category, t.amount);
}

// ==========================================
// 2. HIEN THI DANH SACH
// ==========================================
// hàm hiển thị danh sách giao dịch dưới dạng bảng (Giao diện mới)
void displayTransactions(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Danh sach giao dich hien dang trong!\n" RESET);
        return;
    }

    // Thanh tiêu đề đóng khung viền kép
    printTableTop();
    printf(CYAN " \xba" YELLOW " %-4s " CYAN "\xba" YELLOW " %-10s " CYAN "\xba" YELLOW " %-18s " CYAN "\xba" YELLOW " %-18s " CYAN "\xba" YELLOW " %-7s " CYAN "\xba\n" RESET, 
           "ID", "Ngay", "Danh muc", "So tien (VND)", "Loai");
    printTableMid();

    Node* cur = head;
    char bufTien[50];
    while (cur != NULL) {
        if (cur->data.type == 1) {
            snprintf(bufTien, sizeof(bufTien), "+%lld", cur->data.amount);
            printf(CYAN " \xba" RESET " %-4d " CYAN "\xba" RESET " %-10s " CYAN "\xba" RESET " %-18s " CYAN "\xba" GREEN " %-18s " CYAN "\xba" GREEN " %-7s " CYAN "\xba\n" RESET, 
                   cur->data.id, cur->data.date, cur->data.category, bufTien, "Thu");
        } else {
            snprintf(bufTien, sizeof(bufTien), "-%lld", cur->data.amount);
            printf(CYAN " \xba" RESET " %-4d " CYAN "\xba" RESET " %-10s " CYAN "\xba" RESET " %-18s " CYAN "\xba" RED " %-18s " CYAN "\xba" RED " %-7s " CYAN "\xba\n" RESET, 
                   cur->data.id, cur->data.date, cur->data.category, bufTien, "Chi");
        }
        cur = cur->next;
    }
    printTableBot();
}

// ==========================================
// 3. TIM KIEM
// ==========================================
// hàm tìm kiếm giao dịch theo danh mục (tìm gần đúng bằng strstr)
void searchTransaction(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Danh sach trong!\n" RESET);
        return;
    }

    char keyword[50];
    printf(YELLOW "\n--- TIM KIEM ---\n" RESET);
    printf(" Nhap tu khoa danh muc: ");
    scanf("%49s", keyword);
    while (getchar() != '\n');

    printf(YELLOW "\n>>> Ket qua cho '%s':\n" RESET, keyword);
    printTableTop();
    printf(CYAN " \xba" YELLOW " %-4s " CYAN "\xba" YELLOW " %-10s " CYAN "\xba" YELLOW " %-18s " CYAN "\xba" YELLOW " %-18s " CYAN "\xba" YELLOW " %-7s " CYAN "\xba\n" RESET, 
           "ID", "Ngay", "Danh muc", "So tien (VND)", "Loai");
    printTableMid();

    Node* cur = head;
    int found = 0;
    char bufTien[50];
    while (cur != NULL) {
        if (strstr(cur->data.category, keyword) != NULL) {
            if (cur->data.type == 1) {
                snprintf(bufTien, sizeof(bufTien), "+%lld", cur->data.amount);
                printf(CYAN " \xba" RESET " %-4d " CYAN "\xba" RESET " %-10s " CYAN "\xba" RESET " %-18s " CYAN "\xba" GREEN " %-18s " CYAN "\xba" GREEN " %-7s " CYAN "\xba\n" RESET, 
                       cur->data.id, cur->data.date, cur->data.category, bufTien, "Thu");
            } else {
                snprintf(bufTien, sizeof(bufTien), "-%lld", cur->data.amount);
                printf(CYAN " \xba" RESET " %-4d " CYAN "\xba" RESET " %-10s " CYAN "\xba" RESET " %-18s " CYAN "\xba" RED " %-18s " CYAN "\xba" RED " %-7s " CYAN "\xba\n" RESET, 
                       cur->data.id, cur->data.date, cur->data.category, bufTien, "Chi");
            }
            found++;
        }
        cur = cur->next;
    }
    printTableBot();

    if (found == 0) printf(RED " [!] Khong tim thay giao dich nao khop.\n" RESET);
    else printf(GREEN " => Tim thay %d giao dich.\n" RESET, found);
}

// ==========================================
// 4. XOA GIAO DICH
// ==========================================
// hàm xóa giao dịch theo mã ID
void deleteTransaction(Node** head) {
    if (*head == NULL) {
        printf(RED "\n[!] Danh sach trong!\n" RESET);
        return;
    }

    displayTransactions(*head);

    int deleteId;
    printf(YELLOW "\n--- XOA GIAO DICH ---\n" RESET);
    printf(" Nhap Ma ID can xoa: ");
    if (scanf("%d", &deleteId) != 1) {
        printf(RED "[!] ID phai la so nguyen!\n" RESET);
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    Node* cur = *head;
    Node* prev = NULL;

    // trường hợp 1: node cần xóa nằm ngay đầu danh sách (chính là head)
    if (cur != NULL && cur->data.id == deleteId) {
        *head = cur->next;
        free(cur);
        printf(GREEN " => Da xoa giao dich ID: %d\n" RESET, deleteId);
        return;
    }

    // trường hợp 2: tìm node cần xóa ở đoạn giữa hoặc cuối danh sách
    while (cur != NULL && cur->data.id != deleteId) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == NULL) {
        printf(RED " [!] Khong tim thay ID: %d\n" RESET, deleteId);
        return;
    }

    // thuật toán xóa: ngắt kết nối node hiện tại, nối node trước với node sau
    prev->next = cur->next;
    free(cur);
    printf(GREEN " => Da xoa giao dich ID: %d\n" RESET, deleteId);
}

// ==========================================
// 5. THONG KE TONG HOP
// ==========================================
// hàm đệ quy tính tổng tiền theo loại (1: Thu, 0: Chi) - yêu cầu chương 2
long long calcTotal(Node* head, int type) {
    if (head == NULL) return 0;
    long long cur = (head->data.type == type) ? head->data.amount : 0;
    return cur + calcTotal(head->next, type);
}

// hàm hiển thị báo cáo tài chính tổng hợp
void showStatistics(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Khong co du lieu de thong ke!\n" RESET);
        return;
    }

    long long thu  = calcTotal(head, 1);
    long long chi  = calcTotal(head, 0);
    long long sodu = thu - chi;

    char bufThu[50], bufChi[50], bufDu[50];
    snprintf(bufThu, sizeof(bufThu), "+%lld VND", thu);
    snprintf(bufChi, sizeof(bufChi), "-%lld VND", chi);
    snprintf(bufDu, sizeof(bufDu), "%s%lld VND", (sodu >= 0) ? "+" : "", sodu);

    printBoxTop(67);
    printf(CYAN " \xba" YELLOW "%20s%s%21s" CYAN "\xba\n" RESET, "", "BAO CAO TAI CHINH TONG HOP", "");
    printBoxMid(67);
    
    int padThu = 67 - 13 - strlen(bufThu); if(padThu < 0) padThu = 0;
    int padChi = 67 - 13 - strlen(bufChi); if(padChi < 0) padChi = 0;
    
    printf(CYAN " \xba" RESET " Tong Thu :  " GREEN "%s" RESET "%*s" CYAN "\xba\n" RESET, bufThu, padThu, "");
    printf(CYAN " \xba" RESET " Tong Chi :  " RED "%s" RESET "%*s" CYAN "\xba\n" RESET, bufChi, padChi, "");
    
    printBoxMid(67);
    
    char* msg = (sodu >= 0) ? "[*] Tai chinh on dinh." : "[!] Vuot muc thu nhap!";
    int padDu = 67 - 13 - strlen(bufDu) - 2 - strlen(msg); 
    if(padDu < 0) padDu = 0;
    
    if (sodu >= 0)
        printf(CYAN " \xba" RESET " So Du    :  " GREEN "%s" RESET "  " GREEN "%s" RESET "%*s" CYAN "\xba\n" RESET, bufDu, msg, padDu, "");
    else
        printf(CYAN " \xba" RESET " So Du    :  " RED "%s" RESET "  " RED "%s" RESET "%*s" CYAN "\xba\n" RESET, bufDu, msg, padDu, "");
    
    printBoxBot(67);
}

// ==========================================
// 6. PHAN TICH TAI CHINH (LOGIC C THUAN - KHONG CAN AI)
// ==========================================
// hàm lọc dữ liệu theo mốc thời gian và tự động đưa ra các mức cảnh báo chi tiêu
void analyzeAI(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Khong co du lieu!\n" RESET);
        return;
    }

    // --- Tim cac nam co trong du lieu de goi y ---
    int years[100];
    int numYears = 0;
    Node* temp = head;
    while (temp != NULL) {
        int d = 0, m = 0, y = 0;
        if (sscanf(temp->data.date, "%d/%d/%d", &d, &m, &y) == 3) {
            int found = 0;
            for (int i = 0; i < numYears; i++) {
                if (years[i] == y) { found = 1; break; }
            }
            if (!found && numYears < 100) years[numYears++] = y;
        }
        temp = temp->next;
    }

    printf(YELLOW "\n--- PHAN TICH TAI CHINH THEO THANG / NAM ---\n" RESET);
    printf(CYAN " Cac nam co du lieu: ");
    for (int i = 0; i < numYears; i++) printf("%d ", years[i]);
    printf("\n" RESET);

    // --- Chon Nam & Thang ---
    int nam;
    printf(" Nhap nam can phan tich (0 = tat ca cac nam): ");
    while (1) {
        if (scanf("%d", &nam) == 1 && nam >= 0) { while (getchar() != '\n'); break; }
        while (getchar() != '\n'); printf(RED " [!] Nhap nam hop le: " RESET);
    }
    
    int thang = 0;
    if (nam != 0) {
        printf(" Nhap thang can phan tich (1-12, 0 = tat ca cac thang nam %d): ", nam);
        while (1) {
            if (scanf("%d", &thang) == 1 && thang >= 0 && thang <= 12) { while (getchar() != '\n'); break; }
            while (getchar() != '\n'); printf(RED " [!] Nhap tu 0 den 12: " RESET);
        }
    }

    // --- Tong hop du lieu Bang Logic ---
    int count = 0;
    long long thu = 0, chi = 0;
    
    // Struct de tim danh muc chi nhieu nhat
    typedef struct {
        char name[50];
        long long total;
    } CatSum;
    CatSum catSums[50];
    int catCount = 0;

    Node* cur = head;
    while (cur != NULL) {
        int d = 0, m = 0, y = 0;
        sscanf(cur->data.date, "%d/%d/%d", &d, &m, &y);

        // Loc
        if (nam != 0 && y != nam) { cur = cur->next; continue; }
        if (thang != 0 && m != thang) { cur = cur->next; continue; }

        if (cur->data.type == 1) {
            thu += cur->data.amount;
        } else {
            chi += cur->data.amount;
            // Cong don vao danh muc chi
            int found = 0;
            for (int i = 0; i < catCount; i++) {
                if (strcmp(catSums[i].name, cur->data.category) == 0) {
                    catSums[i].total += cur->data.amount;
                    found = 1; break;
                }
            }
            if (!found && catCount < 50) {
                strcpy(catSums[catCount].name, cur->data.category);
                catSums[catCount].total = cur->data.amount;
                catCount++;
            }
        }
        count++;
        cur = cur->next;
    }

    if (count == 0) {
        if (nam != 0 && thang != 0) printf(RED " [!] Khong co giao dich nao trong thang %d/%d.\n" RESET, thang, nam);
        else if (nam != 0) printf(RED " [!] Khong co giao dich nao trong nam %d.\n" RESET, nam);
        else printf(RED " [!] Khong co giao dich nao.\n" RESET);
        return;
    }

    // Tim danh muc Top 1
    long long maxChi = 0;
    char maxCatName[50] = "Khong co";
    for (int i = 0; i < catCount; i++) {
        if (catSums[i].total > maxChi) {
            maxChi = catSums[i].total;
            strcpy(maxCatName, catSums[i].name);
        }
    }

    // --- Tao chuoi hien thi ---
    char timeStr[100] = {0};
    if (nam == 0) strcpy(timeStr, "Toan thoi gian");
    else if (thang == 0) snprintf(timeStr, sizeof(timeStr), "Nam %d", nam);
    else snprintf(timeStr, sizeof(timeStr), "Thang %d Nam %d", thang, nam);

    // --- IN GIAO DIEN BOX ---
    // BW = so ky tu noi dung (plain text, khong tinh ANSI escape)
    // Cau truc moi dong: " \xba " (3) + content (BW) + "\xba" (1) = BW+4
#define BW 63

    // In dong plain text
#define ROW_PLAIN(s) do { \
    int _l = (int)strlen(s); \
    printf(CYAN " \xba " RESET "%s", (s)); \
    for(int _i=_l; _i<BW; _i++) putchar(' '); \
    printf(CYAN "\xba\n" RESET); \
} while(0)

    // In dong header mau vang
#define ROW_HDR(s) do { \
    int _l = (int)strlen(s); \
    printf(CYAN " \xba " YELLOW "%s" RESET, (s)); \
    for(int _i=_l; _i<BW; _i++) putchar(' '); \
    printf(CYAN "\xba\n" RESET); \
} while(0)

    // In dong trong
#define ROW_EMPTY() do { \
    printf(CYAN " \xba " RESET); \
    for(int _i=0; _i<BW; _i++) putchar(' '); \
    printf(CYAN "\xba\n" RESET); \
} while(0)

    // In dong co mau - col la bien const char* runtime
    // plain_len = strlen(prefix + numstr + suffix) khong tinh mau
#define ROW_MONEY(prefix, col, numstr, suffix, plain_len) do { \
    printf(CYAN " \xba " RESET "%s%s%s" RESET "%s", (prefix),(col),(numstr),(suffix)); \
    int _pad = BW - (plain_len); \
    for(int _i=0; _i<_pad; _i++) putchar(' '); \
    printf(CYAN "\xba\n" RESET); \
} while(0)

    // Tieu de canh giua
    char title[100];
    snprintf(title, sizeof(title), "PHAN TICH TAI CHINH (%s)", timeStr);
    int tlen = (int)strlen(title);
    int padL = (BW + 1 - tlen) / 2;
    int padR = (BW + 1) - tlen - padL;
    if(padL < 1) padL = 1;
    if(padR < 1) padR = 1;

    printBoxTop(BW + 1);
    printf(CYAN " \xba" YELLOW "%*s%s%*s" CYAN "\xba\n" RESET, padL, "", title, padR, "");
    printBoxMid(BW + 1);

    char buf[128], numStr[64];

    // [1] TONG QUAN
    ROW_HDR("[1] TONG QUAN:");

    snprintf(buf, sizeof(buf), "  - Tong so giao dich: %d", count);
    ROW_PLAIN(buf);

    snprintf(numStr, sizeof(numStr), "%lld", thu);
    snprintf(buf,    sizeof(buf),    "  - Tong Thu: %s VND", numStr);
    ROW_MONEY("  - Tong Thu: ", GREEN, numStr, " VND", (int)strlen(buf));

    snprintf(numStr, sizeof(numStr), "%lld", chi);
    snprintf(buf,    sizeof(buf),    "  - Tong Chi: %s VND", numStr);
    ROW_MONEY("  - Tong Chi: ", RED, numStr, " VND", (int)strlen(buf));

    {
        long long sodu_local = thu - chi;
        const char* scol = sodu_local >= 0 ? GREEN : RED;
        long long absVal = sodu_local < 0 ? -sodu_local : sodu_local;
        snprintf(numStr, sizeof(numStr), "%s%lld", sodu_local >= 0 ? "+" : "-", absVal);
        snprintf(buf,    sizeof(buf),    "  - So du:    %s VND", numStr);
        ROW_MONEY("  - So du:    ", scol, numStr, " VND", (int)strlen(buf));
    }

    ROW_EMPTY();

    // [2] DANH MUC NOI BAT
    ROW_HDR("[2] DANH MUC NOI BAT:");
    if (chi > 0) {
        snprintf(buf, sizeof(buf), "  - Chi nhieu nhat: %s", maxCatName);
        ROW_PLAIN(buf);

        snprintf(numStr, sizeof(numStr), "%lld", maxChi);
        snprintf(buf,    sizeof(buf),    "  - So tien:  %s VND", numStr);
        ROW_MONEY("  - So tien:  ", RED, numStr, " VND", (int)strlen(buf));

        if (thu > 0) {
            int pct = (int)(chi * 100 / thu);
            const char* pcol = pct > 80 ? RED : pct > 50 ? YELLOW : GREEN;
            snprintf(numStr, sizeof(numStr), "%d%%", pct);
            snprintf(buf,    sizeof(buf),    "  - Chi / Thu: %s", numStr);
            ROW_MONEY("  - Chi / Thu: ", pcol, numStr, "", (int)strlen(buf));
        }
    } else {
        ROW_PLAIN("  - Thoi gian nay ban khong co khoan chi nao.");
    }

    ROW_EMPTY();

    // [3] LOI KHUYEN
    ROW_HDR("[3] LOI KHUYEN:");

    if (chi > thu && thu > 0) {
        snprintf(buf, sizeof(buf), "  => CANH BAO: Chi tieu vuot muc thu nhap!");
        ROW_MONEY("  => CANH BAO:", RED, " Chi tieu vuot muc thu nhap!", "", (int)strlen(buf));
        ROW_PLAIN("  => Can cat giam ngay de tranh tham hut nang.");
    } else if (thu == 0 && chi > 0) {
        snprintf(buf, sizeof(buf), "  => CANH BAO: Chua co thu nhap nhung van dang tieu tien.");
        ROW_MONEY("  => CANH BAO:", RED, " Chua co thu nhap nhung van dang tieu tien.", "", (int)strlen(buf));
    } else if (chi > (long long)(thu * 0.8)) {
        ROW_PLAIN("  => Da xai hon 80% thu nhap. Nen de danh them khoan du phong.");
    } else if (thu > 0 && chi <= (long long)(thu * 0.5)) {
        snprintf(buf, sizeof(buf), "  => TUYET VOI: Tiet kiem rat tot. Tai chinh hien rat on dinh!");
        ROW_MONEY("  => TUYET VOI:", GREEN, " Tiet kiem rat tot. Tai chinh hien rat on dinh!", "", (int)strlen(buf));
    } else {
        ROW_PLAIN("  => Tinh hinh tai chinh can bang. Ban dang kiem soat tot!");
    }

    if (strcmp(maxCatName, "Giai_tri") == 0 || strcmp(maxCatName, "Mua_sam") == 0) {
        ROW_PLAIN("  => Danh muc chi nhieu la Giai tri / Mua sam.");
        ROW_PLAIN("  => Nen kiem soat lai cac so thich ca nhan nay.");
    }

    ROW_EMPTY();
    printBoxBot(BW + 1);

#undef BW
#undef ROW_PLAIN
#undef ROW_HDR
#undef ROW_EMPTY
#undef ROW_MONEY
}

// ==========================================
// FILE I/O
// ==========================================
// hàm quét file text để nạp dữ liệu cũ vào danh sách liên kết khi khởi động ứng dụng
void loadFromFile(Node** head) {
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        printf(YELLOW "\n[*] Khong tim thay 'data.txt'. Se tao moi khi luu.\n" RESET);
        return;
    }
    Transaction t;
    int count = 0;
    while (fscanf(file, "%d %14s %49s %lld %d",
                  &t.id, t.date, t.category, &t.amount, &t.type) == 5) {
        Node* newNode = createNode(t);
        if (newNode == NULL) continue;
        if (*head == NULL) {
            *head = newNode;
        } else {
            Node* temp = *head;
            while (temp->next != NULL) temp = temp->next;
            temp->next = newNode;
        }
        count++;
    }
    fclose(file);
    if (count > 0)
        printf(GREEN "\n[*] Da tai %d giao dich tu file.\n" RESET, count);
}

// hàm duyệt danh sách liên kết để ghi đè toàn bộ dữ liệu sạch vào ổ cứng khi thoát
void saveToFile(Node* head) {
    FILE* file = fopen("data.txt", "w");
    if (file == NULL) {
        printf(RED "\n[!] Khong the mo file de luu!\n" RESET);
        return;
    }
    Node* cur = head;
    int count = 0;
    while (cur != NULL) {
        fprintf(file, "%d %s %s %lld %d\n",
                cur->data.id, cur->data.date,
                cur->data.category, cur->data.amount, cur->data.type);
        cur = cur->next;
        count++;
    }
    fclose(file);
    printf(GREEN "\n[*] Da luu %d giao dich vao 'data.txt'.\n" RESET, count);
}

// hàm duyệt vòng lặp giải phóng vùng nhớ heap của từng node tránh rò rỉ ram
void freeMemory(Node** head) {
    Node* cur = *head;
    while (cur != NULL) {
        Node* next = cur->next;
        free(cur);
        cur = next;
    }
    *head = NULL;
}

// ==========================================
// MENU CHINH
// ==========================================
// hàm hiển thị danh sách các lựa chọn chức năng chính của ứng dụng
void showMenu() {
    int w = 45;
    printBoxTop(w);
    printf(CYAN " \xba" YELLOW BOLD "%8s%s%9s" RESET CYAN "\xba\n" RESET, "", "QUAN LY THU CHI CA NHAN v4.1", "");
    printBoxMid(w);
    printf(CYAN " \xba" GREEN " [1] " RESET "%-40s" CYAN "\xba\n" RESET, "Them giao dich moi");
    printf(CYAN " \xba" GREEN " [2] " RESET "%-40s" CYAN "\xba\n" RESET, "Hien thi danh sach");
    printf(CYAN " \xba" GREEN " [3] " RESET "%-40s" CYAN "\xba\n" RESET, "Tim kiem giao dich");
    printf(CYAN " \xba" GREEN " [4] " RESET "%-40s" CYAN "\xba\n" RESET, "Xoa giao dich");
    printf(CYAN " \xba" GREEN " [5] " RESET "%-40s" CYAN "\xba\n" RESET, "Thong ke tong hop");
    printf(CYAN " \xba" MAGENTA " [6] " RESET "%-40s" CYAN "\xba\n" RESET, "Phan tich du lieu theo thang/nam ");
    printf(CYAN " \xba" RED " [7] " RESET "%-40s" CYAN "\xba\n" RESET, "Luu va Thoat");
    printBoxBot(w);
    
    printf(YELLOW "  => Chon (1-7): " RESET);
}

// ==========================================
// MAIN
// ==========================================
int main() {
    enableANSI();
    SetConsoleOutputCP(437);

    Node* head = NULL;
    loadFromFile(&head);

    int choice;
    do {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf(RED "\n[!] Vui long nhap so nguyen!\n" RESET);
            while (getchar() != '\n');
            choice = 0;
            continue;
        }
        while (getchar() != '\n');

        switch (choice) {
            case 1: addTransaction(&head);     break;
            case 2: displayTransactions(head); break;
            case 3: searchTransaction(head);   break;
            case 4: deleteTransaction(&head);  break;
            case 5: showStatistics(head);      break;
            case 6: analyzeAI(head);           break;
            case 7:
                printf(YELLOW "\n>>> Dang luu va don dep... Tam biet!\n" RESET);
                saveToFile(head);
                freeMemory(&head);
                break;
            default:
                printf(RED "\n[!] Lua chon khong hop le (1-7)!\n" RESET);
        }
    } while (choice != 7);

    return 0;
}
