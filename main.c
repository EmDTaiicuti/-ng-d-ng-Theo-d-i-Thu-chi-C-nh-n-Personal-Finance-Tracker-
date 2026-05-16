#include <stdio.h>
#include <stdlib.h>
#include <string.h> // thêm thư viện xử lý chuỗi
#include "structs.h" 

// ==========================================
// THIET KE GIAO DIEN (Màu ANSI)
// ==========================================
#define RESET   "\033[0m"       // Trả về màu mặc định
#define RED     "\033[1;31m"    // Đỏ (Dùng cho báo lỗi và Tiền Chi)
#define GREEN   "\033[1;32m"    // Xanh lá (Dùng cho thành công và Tiền Thu)
#define YELLOW  "\033[1;33m"    // Vàng (Dùng cho Tiêu đề)
#define CYAN    "\033[1;36m"    // Xanh lơ (Dùng cho Khung viền bảng)
#define BOLD    "\033[1m"       // In đậm

// khai báo sẵn các chức năng của hàm 
void addTransaction(Node** head); // đã sửa lỗi thiếu tham số
void displayTransactions(Node* head); // thêm tham số
void searchTransaction(Node* head); // thêm tham số truyền vào
void deleteTransaction(Node** head); // thêm tham số con trỏ bậc 2
void showStatistics(Node** head);// 
void loadFromFile(Node** head); // thêm tham số
void saveToFile(Node* head); // thêm tham số
void freeMemory(Node** head); // khai báo thêm hàm dọn rác
// ==========================================
// MODULE: DANH SACH LIEN KET & XU LY DU LIEU
// ==========================================

// hàm cấp phát bộ nhớ động tạo nọt mới
Node* createNode(Transaction t) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("[!] Loi: Khong the cap phat bo nho!\n");
        return NULL;
    }
    // đổ dữ liệu vào node và cho con trỏ trỏ vào null
    newNode->data = t;
    newNode->next = NULL;
    return newNode;
}

// hàm thêm giao dịch do người dùng nhập vào Danh sách (Thêm vào cuối - Insert at Tail)
Transaction t;
    printf(YELLOW "\n--- NHAP THONG TIN GIAO DICH MOI ---\n" RESET);
    
    printf("Nhap Ma ID (so nguyen): "); scanf("%d", &t.id);
    printf("Nhap Ngay (DD/MM/YYYY): "); scanf("%s", t.date);
    
    // Giao diện chọn nhanh Danh mục (Chống lười)
    printf(CYAN "\n   --- CHON DANH MUC ---\n" RESET);
    printf("   1. An uong\n");
    printf("   2. Tien nha / Sinh hoat\n");
    printf("   3. Mua sam / Giai tri\n");
    printf("   4. Luong / Thu nhap\n");
    printf("   5. Danh muc khac...\n");
    printf("-> Chon so (1-5): ");
    
    int catChoice;
    scanf("%d", &catChoice);
    switch(catChoice) {
        case 1: strcpy(t.category, "An_uong"); break;
        case 2: strcpy(t.category, "Sinh_hoat"); break;
        case 3: strcpy(t.category, "Mua_sam"); break;
        case 4: strcpy(t.category, "Thu_nhap"); break;
        default: 
            printf("Nhap ten danh muc tu do (khong dau cach): ");
            scanf("%s", t.category);
    }
    
    printf("Nhap So tien (VND): "); scanf("%lld", &t.amount);
    printf("Loai giao dich (1 la Thu, 0 la Chi): "); scanf("%d", &t.type);

    // ... (Phần logic gắn Node vào danh sách giữ nguyên như cũ) ...
    // gọi máy tạo node
    Node* newNode = createNode(t);
    if (*head == NULL) *head = newNode;
    else {
        Node* temp = *head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
    }
    
    printf(GREEN "\n=> Da them giao dich [%s] thanh cong!\n" RESET, t.category);
}
    
    printf("\n=> Tinh tong... Da them giao dich [%s] thanh cong!\n", t.category);
}

// hàm đọc dữ liệu từ file khi khởi động
void loadFromFile(Node** head) {
    // mở file ở chế độ đọc ("r" - read)
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        printf("\n[*] He thong: Khong tim thay file 'data.txt'. Se tao file moi khi ban luu.\n");
        return; // thoát hàm nếu file không tồn tại
    }

    Transaction t;
    int count = 0; // biến đếm xem đã tải được bao nhiêu dòng

    // đọc liên tục cho đến khi hết file (EOF)
    while (fscanf(file, "%d %s %s %lld %d", &t.id, t.date, t.category, &t.amount, &t.type) == 5) {
        // dùng lại máy tạo node
        Node* newNode = createNode(t);
        
        // thuật toán gắn node vào danh sách liên kết
        if (*head == NULL) {
            *head = newNode;
        } else {
            Node* temp = *head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
        count++;
    }

    fclose(file); // mở file xong nhớ phải đóng lại
    if (count > 0) {
        printf("\n[*] He thong: Da tai thanh cong %d giao dich tu file!\n", count);
    }
}
// hàm ghi dữ liệu từ danh sách liên kết ra file (ghi đè dữ liệu cũ)
void saveToFile(Node* head) {
    // mở file ở chế độ "w" (write): xóa sạch dữ liệu cũ và ghi mới lại từ đầu
    FILE* file = fopen("data.txt", "w"); 
    if (file == NULL) {
        printf("\n[!] Loi: Khong the mo file de luu du lieu!\n");
        return;
    }

    Node* current = head;
    int count = 0;
    
    // duyệt qua từng node và dùng fprintf để in vào file
    while (current != NULL) {
        fprintf(file, "%d %s %s %lld %d\n", 
                current->data.id, 
                current->data.date, 
                current->data.category, 
                current->data.amount, 
                current->data.type);
        current = current->next;
        count++;
    }

    fclose(file); // ghi xong thì phải đóng cửa lại
    printf("\n[*] He thong: Da luu an toan %d giao dich vao file 'data.txt'.\n", count);
}

// hàm giải phóng bộ nhớ động để tránh rò rỉ bộ nhớ (memory leak) - yêu cầu chương 2
void freeMemory(Node** head) {
    Node* current = *head;
    Node* nextNode;
    
    while (current != NULL) {
        nextNode = current->next; // giữ lại địa chỉ node tiếp theo trước khi xóa
        free(current);            // tiêu hủy (trả ram) node hiện tại
        current = nextNode;       // tiến lên node tiếp theo
    }
    
    *head = NULL; // reset danh sách về trống cho an toàn
    printf("[*] He thong: Da don dep RAM thanh cong (Free Memory).\n");
}
// hàm hiển thị danh sách giao dịch dưới dạng bảng (Giao diện mới)
void displayTransactions(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Danh sach giao dich hien dang trong!\n" RESET);
        return;
    }

    // In thanh tiêu đề của bảng với màu CYAN và YELLOW
    printf(CYAN "\n===============================================================================\n" RESET);
    printf(CYAN "|" YELLOW " %-5s " CYAN "|" YELLOW " %-12s " CYAN "|" YELLOW " %-20s " CYAN "|" YELLOW " %-15s " CYAN "|" YELLOW " %-6s " CYAN "|\n" RESET, 
           "ID", "Ngay", "Danh muc", "So tien (VND)", "Loai");
    printf(CYAN "===============================================================================\n" RESET);

    Node* current = head;
    while (current != NULL) {
        // Tô màu dòng dữ liệu dựa vào Thu hay Chi
        if (current->data.type == 1) {
            // Loại 1 (Thu): In chữ "Thu" và Số tiền màu XANH LÁ
            printf(CYAN "|" RESET " %-5d " CYAN "|" RESET " %-12s " CYAN "|" RESET " %-20s " CYAN "|" GREEN " +%-14lld" CYAN "|" GREEN " %-6s " CYAN "|\n" RESET, 
                   current->data.id, current->data.date, current->data.category, current->data.amount, "Thu");
        } else {
            // Loại 0 (Chi): In chữ "Chi" và Số tiền màu ĐỎ
            printf(CYAN "|" RESET " %-5d " CYAN "|" RESET " %-12s " CYAN "|" RESET " %-20s " CYAN "|" RED " -%-14lld" CYAN "|" RED " %-6s " CYAN "|\n" RESET, 
                   current->data.id, current->data.date, current->data.category, current->data.amount, "Chi");
        }
        current = current->next;
    }
    printf(CYAN "===============================================================================\n" RESET);
}
        // in thông tin của node hiện tại
        printf("| %-5d | %-12s | %-20s | %15lld | %-6s |\n", 
               current->data.id, 
               current->data.date, 
               current->data.category, 
               current->data.amount, 
               typeStr);
        
        // nhảy sang node tiếp theo
        current = current->next;
    }
    printf("===========================================================================\n");
}
// hàm tìm kiếm giao dịch theo danh mục (tìm gần đúng bằng strstr)
void searchTransaction(Node* head) {
    // kiểm tra danh sách rỗng
    if (head == NULL) {
        printf("\n[!] Danh sach trong, khong co gi de tim!\n");
        return;
    }

    char keyword[50];
    printf("\n--- TIM KIEM GIAO DICH ---\n");
    printf("Nhap tu khoa danh muc (VD: An, Luong...): ");
    scanf("%s", keyword);

    // in thanh tiêu đề bảng kết quả
    printf("\n>>> KET QUA TIM KIEM CHO '%s':\n", keyword);
    printf("===========================================================================\n");
    printf("| %-5s | %-12s | %-20s | %-15s | %-6s |\n", "ID", "Ngay", "Danh muc", "So tien (VND)", "Loai");
    printf("===========================================================================\n");

    Node* current = head;
    int foundCount = 0; // biến đếm số kết quả tìm được

    // duyệt từ đầu đến cuối danh sách
    while (current != NULL) {
        // hàm strstr sẽ kiểm tra xem 'keyword' có nằm trong 'category' không
        if (strstr(current->data.category, keyword) != NULL) {
            char typeStr[10];
            if (current->data.type == 1) strcpy(typeStr, "Thu");
            else strcpy(typeStr, "Chi");

            // in ra giao dịch khớp với từ khóa
            printf("| %-5d | %-12s | %-20s | %15lld | %-6s |\n", 
                   current->data.id, 
                   current->data.date, 
                   current->data.category, 
                   current->data.amount, 
                   typeStr);
            foundCount++;
        }
        current = current->next; // đi tới node tiếp theo
    }
    printf("===========================================================================\n");

    // thông báo nếu không tìm thấy gì
    if (foundCount == 0) {
        printf("[!] Khong tim thay giao dich nao khop voi tu khoa '%s'.\n", keyword);
    } else {
        printf("=> Tim thay %d giao dich.\n", foundCount);
    }
}
// hàm xóa giao dịch theo mã ID
void deleteTransaction(Node** head) {
    // kiểm tra danh sách rỗng
    if (*head == NULL) {
        printf("\n[!] Danh sach trong, khong co gi de xoa!\n");
        return;
    }

    int deleteId;
    printf("\n--- XOA GIAO DICH ---\n");
    printf("Nhap Ma ID cua giao dich can xoa: ");
    if (scanf("%d", &deleteId) != 1) {
        printf("[!] Loi: ID phai la so nguyen!\n");
        while(getchar() != '\n'); // dọn dẹp bộ nhớ đệm nếu nhập sai
        return;
    }

    Node* current = *head;
    Node* prev = NULL;

    // trường hợp 1: node cần xóa nằm ngay đầu danh sách (chính là head)
    if (current != NULL && current->data.id == deleteId) {
        *head = current->next; // dời head sang node thứ 2
        free(current);         // trả ram của node cũ
        printf("=> Da xoa thanh cong giao dich co ID: %d\n", deleteId);
        return;
    }

    // trường hợp 2: tìm node cần xóa ở đoạn giữa hoặc cuối danh sách
    while (current != NULL && current->data.id != deleteId) {
        prev = current;          // prev đi sau lưu dấu vết
        current = current->next; // current đi trước dò tìm
    }

    // nếu duyệt đến cuối (null) mà vẫn không tìm thấy ID
    if (current == NULL) {
        printf("[!] Khong tim thay giao dich voi ID: %d\n", deleteId);
        return;
    }

    // thuật toán xóa: ngắt kết nối node hiện tại, nối node trước với node sau
    prev->next = current->next;
    free(current); // tiêu hủy node cần xóa
    printf("=> Da xoa thanh cong giao dich co ID: %d\n", deleteId);
}
// hàm đệ quy tính tổng tiền theo loại (1: Thu, 0: Chi) - yêu cầu chương 2
long long calculateTotalRecursive(Node* head, int type) {
    // điều kiện dừng của đệ quy: nếu danh sách hết thì trả về 0
    if (head == NULL) return 0;

    long long currentAmount = 0;
    if (head->data.type == type) {
        currentAmount = head->data.amount;
    }

    // gọi đệ quy: tổng = tiền hiện tại + tổng của phần còn lại
    return currentAmount + calculateTotalRecursive(head->next, type);
}

// hàm hiển thị báo cáo tài chính tổng hợp
void showStatistics(Node* head) {
    if (head == NULL) {
        printf("\n[!] Danh sach trong, khong co du lieu de thong ke!\n");
        return;
    }

    // gọi hàm đệ quy để tính toán
    long long totalIncome = calculateTotalRecursive(head, 1); // Tổng Thu
    long long totalExpense = calculateTotalRecursive(head, 0); // Tổng Chi
    long long balance = totalIncome - totalExpense;           // Số dư

    printf("\n========================================\n");
    printf("        BAO CAO TAI CHINH TONG HOP\n");
    printf("========================================\n");
    printf(" 1. Tong thu nhap:  %15lld VND\n", totalIncome);
    printf(" 2. Tong chi tieu:  %15lld VND\n", totalExpense);
    printf("----------------------------------------\n");
    printf(" => SO DU HIEN TAI: %15lld VND\n", balance);
    
    if (balance < 0) {
        printf(" [!] Canh bao: Ban dang chi tieu vuot muc thu nhap!\n");
    } else {
        printf(" [*] Trang thai: Tai chinh on dinh.\n");
    }
    printf("========================================\n");
}
void showMenu() {
    printf("\n");
    printf(CYAN " ╔══════════════════════════════════════╗\n" RESET);
    printf(CYAN " ║ " YELLOW BOLD "     QUAN LY THU CHI CA NHAN 2.0    " CYAN " ║\n" RESET);
    printf(CYAN " ╠══════════════════════════════════════╣\n" RESET);
    printf(CYAN " ║ " RESET " " GREEN "[1]" RESET " Them giao dich moi             " CYAN " ║\n" RESET);
    printf(CYAN " ║ " RESET " " GREEN "[2]" RESET " Hien thi danh sach giao dich   " CYAN " ║\n" RESET);
    printf(CYAN " ║ " RESET " " GREEN "[3]" RESET " Tim kiem giao dich             " CYAN " ║\n" RESET);
    printf(CYAN " ║ " RESET " " GREEN "[4]" RESET " Xoa giao dich                  " CYAN " ║\n" RESET);
    printf(CYAN " ║ " RESET " " GREEN "[5]" RESET " Thong ke tong hop              " CYAN " ║\n" RESET);
    printf(CYAN " ║ " RESET " " RED   "[6]" RESET " Luu va Thoat                   " CYAN " ║\n" RESET);
    printf(CYAN " ╚══════════════════════════════════════╝\n" RESET);
    printf(YELLOW " => Nhap lua chon cua ban (1-6): " RESET);
}

int main() {
    int choice;
    
    // khai báo con trỏ head quản lý Danh sách liên kết (Khởi tạo là NULL)
    Node* head = NULL; 

    // gọi hàm tải dữ liệu khi vừa mở app
    loadFromFile(&head);

    do {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf("\n[!] Loi: Vui long nhap so nguyen!\n");
            while(getchar() != '\n'); 
            continue;
        }

        switch(choice) {
            case 1:
                addTransaction(&head); // gọi hàm Thêm và truyền địa chỉ của head(10/4)
                break;
            //các case khác giữ nguyên như cũ nha mấy bé(10/4)
            case 2:
                displayTransactions(head); // gọi hàm hiển thị
                break;
            case 3:
                searchTransaction(head); // gọi hàm tìm kiếm
                break;
         case 4:
                deleteTransaction(&head); // gọi hàm xóa (truyền địa chỉ)
                break;
        case 5:
                showStatistics(head); // gọi hàm thống kê
                break;
            case 6:
                printf("\n>>> Dang luu du lieu va don dep he thong... Tam biet!\n");
                saveToFile(head);      // gọi hàm lưu file
                freeMemory(&head);     // gọi hàm dọn rác ram
                break;
            default:
                printf("\n[!] Lua chon khong hop le! Vui long chon tu 1 den 6.\n");
        }
    } while(choice != 6);

    return 0;
}
