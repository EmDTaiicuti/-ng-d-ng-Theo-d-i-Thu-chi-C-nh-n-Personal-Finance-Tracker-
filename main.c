#include <stdio.h>
#include <stdlib.h>
#include <string.h> // thêm thư viện xử lý chuỗi
#include "structs.h"

#define RESET   "\033[0m"       // Trả về màu mặc định
#define RED     "\033[1;31m"    // Đỏ (Dùng cho báo lỗi và Tiền Chi)
#define GREEN   "\033[1;32m"    // Xanh lá (Dùng cho thành công và Tiền Thu)
#define YELLOW  "\033[1;33m"    // Vàng (Dùng cho Tiêu đề)
#define CYAN    "\033[1;36m"    // Xanh lơ (Dùng cho Khung viền bảng)
#define BOLD    "\033[1m"       // In đậm

// khai báo sẵn các chức năng của hàm
Node* createNode(Transaction t);
void      appendNode(Node** head, Node* newNode);
void      addTransaction(Node** head); // đã sửa lỗi thiếu tham số
void      displayTransactions(Node* head); // thêm tham số
void      searchTransaction(Node* head); // thêm tham số truyền vào
void      deleteTransaction(Node** head); // thêm tham số con trỏ bậc 2
long long calculateTotalRecursive(Node* head, int type);
void      showStatistics(Node* head);
void      loadFromFile(Node** head); // thêm tham số
void      saveToFile(Node* head); // thêm tham số
void      freeMemory(Node** head); // khai báo thêm hàm dọn rác
void      showMenu();

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

void appendNode(Node** head, Node* newNode) {
    if (newNode == NULL) return;
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node* temp = *head;
    while (temp->next != NULL) temp = temp->next;
    temp->next = newNode;
}

// hàm thêm giao dịch do người dùng nhập vào Danh sách (Thêm vào cuối - Insert at Tail)
void addTransaction(Node** head) {
    Transaction t;

    int maxId = 0;
    Node* temp = *head;
    while (temp != NULL) {
        if (temp->data.id > maxId) maxId = temp->data.id;
        temp = temp->next;
    }
    t.id = maxId + 1;

    printf(YELLOW "\n--- NHAP THONG TIN GIAO DICH MOI ---\n" RESET);
    printf("  Ma ID tu dong: [%d]\n", t.id);

    printf("  Nhap Ngay (DD/MM/YYYY): ");
    scanf("%14s", t.date);

    // giao diện chọn nhanh danh mục (Chống lười)
    printf(CYAN "\n  --- CHON DANH MUC ---\n" RESET);
    printf("    1. An uong\n");
    printf("    2. Tien nha / Sinh hoat\n");
    printf("    3. Mua sam / Giai tri\n");
    printf("    4. Luong / Thu nhap\n");
    printf("    5. Danh muc khac...\n");
    printf("  -> Chon so (1-5): ");

    int catChoice;
    if (scanf("%d", &catChoice) != 1) {
        printf(RED "[!] Lua chon khong hop le!\n" RESET);
        while (getchar() != '\n');
        return;
    }

    switch (catChoice) {
        case 1: strcpy(t.category, "An_uong");    break;
        case 2: strcpy(t.category, "Sinh_hoat");  break;
        case 3: strcpy(t.category, "Mua_sam");    break;
        case 4: strcpy(t.category, "Thu_nhap");   break;
        default:
            printf("  Nhap ten danh muc (khong dau cach): ");
            scanf("%49s", t.category); 
    }

    printf("  Nhap So tien (VND): ");
    if (scanf("%lld", &t.amount) != 1 || t.amount < 0) {
        printf(RED "[!] So tien khong hop le!\n" RESET);
        while (getchar() != '\n');
        return;
    }

    printf("  Loai giao dich (1=Thu / 0=Chi): ");
    if (scanf("%d", &t.type) != 1 || (t.type != 0 && t.type != 1)) {
        printf(RED "[!] Loai giao dich khong hop le! Chi nhap 0 hoac 1.\n" RESET);
        while (getchar() != '\n');
        return;
    }

    // gọi máy tạo node
    Node* newNode = createNode(t);
    appendNode(head, newNode);

    printf(GREEN "\n  => Da them giao dich ID=[%d] danh muc [%s] thanh cong!\n" RESET,
           t.id, t.category);
}

// hàm hiển thị danh sách giao dịch dưới dạng bảng (Giao diện mới)
// hàm hiển thị danh sách giao dịch dưới dạng bảng (Giao diện mới)
void displayTransactions(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Danh sach giao dich hien dang trong!\n" RESET);
        return;
    }

    // Thanh tiêu đề đóng khung viền kép
    printf(CYAN "\n  ╔═══════╦══════════════╦══════════════════════╦═════════════════╦════════╗\n" RESET);
    printf(CYAN "  ║" YELLOW " %-5s " CYAN "║" YELLOW " %-12s " CYAN "║" YELLOW " %-20s " CYAN "║" YELLOW " %-15s " CYAN "║" YELLOW " %-6s " CYAN "║\n" RESET, 
           "ID", "Ngay", "Danh muc", "So tien (VND)", "Loai");
    printf(CYAN "  ╠═══════╬══════════════╬══════════════════════╬═════════════════╬════════╣\n" RESET);

    Node* current = head;
    while (current != NULL) {
        if (current->data.type == 1) {
            printf(CYAN "  ║" RESET " %-5d " CYAN "║" RESET " %-12s " CYAN "║" RESET " %-20s " CYAN "║" GREEN " +%-14lld " CYAN "║" GREEN " %-6s " CYAN "║\n" RESET, 
                   current->data.id, current->data.date, current->data.category, current->data.amount, "Thu");
        } else {
            printf(CYAN "  ║" RESET " %-5d " CYAN "║" RESET " %-12s " CYAN "║" RESET " %-20s " CYAN "║" RED " -%-14lld " CYAN "║" RED " %-6s " CYAN "║\n" RESET, 
                   current->data.id, current->data.date, current->data.category, current->data.amount, "Chi");
        }
        current = current->next;
    }
    printf(CYAN "  ╚═══════╩══════════════╩══════════════════════╩═════════════════╩════════╝\n" RESET);
}

// hàm tìm kiếm giao dịch theo danh mục (tìm gần đúng bằng strstr)
void searchTransaction(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Danh sach trong, khong co gi de tim!\n" RESET);
        return;
    }

    char keyword[50];
    printf(YELLOW "\n--- TIM KIEM GIAO DICH ---\n" RESET);
    printf("Nhap tu khoa danh muc (VD: An, Luong...): ");
    scanf("%49s", keyword); 

    printf(GREEN "\n>>> KET QUA TIM KIEM CHO '%s':\n" RESET, keyword);
    
    // Khung viền bảng kết quả tìm kiếm
    printf(CYAN "  ╔═══════╦══════════════╦══════════════════════╦═════════════════╦════════╗\n" RESET);
    printf(CYAN "  ║" YELLOW " %-5s " CYAN "║" YELLOW " %-12s " CYAN "║" YELLOW " %-20s " CYAN "║" YELLOW " %-15s " CYAN "║" YELLOW " %-6s " CYAN "║\n" RESET, 
           "ID", "Ngay", "Danh muc", "So tien (VND)", "Loai");
    printf(CYAN "  ╠═══════╬══════════════╬══════════════════════╬═════════════════╬════════╣\n" RESET);

    Node* current = head;
    int foundCount = 0; 

    while (current != NULL) {
        if (strstr(current->data.category, keyword) != NULL) {
            if (current->data.type == 1) {
                printf(CYAN "  ║" RESET " %-5d " CYAN "║" RESET " %-12s " CYAN "║" RESET " %-20s " CYAN "║" GREEN " +%-14lld " CYAN "║" GREEN " %-6s " CYAN "║\n" RESET, 
                       current->data.id, current->data.date, current->data.category, current->data.amount, "Thu");
            } else {
                printf(CYAN "  ║" RESET " %-5d " CYAN "║" RESET " %-12s " CYAN "║" RESET " %-20s " CYAN "║" RED " -%-14lld " CYAN "║" RED " %-6s " CYAN "║\n" RESET, 
                       current->data.id, current->data.date, current->data.category, current->data.amount, "Chi");
            }
            foundCount++;
        }
        current = current->next; 
    }
    printf(CYAN "  ╚═══════╩══════════════╩══════════════════════╩═════════════════╩════════╝\n" RESET);

    if (foundCount == 0)
        printf(RED "[!] Khong tim thay giao dich nao khop voi tu khoa '%s'.\n" RESET, keyword);
    else
        printf(GREEN "=> Tim thay %d giao dich.\n" RESET, foundCount);
}

// hàm xóa giao dịch theo mã ID
void deleteTransaction(Node** head) {
    // kiểm tra danh sách rỗng
    if (*head == NULL) {
        printf(RED "\n[!] Danh sach trong, khong co gi de xoa!\n" RESET);
        return;
    }

    int deleteId;
    printf(YELLOW "\n--- XOA GIAO DICH ---\n" RESET);
    printf("Nhap Ma ID cua giao dich can xoa: ");

    if (scanf("%d", &deleteId) != 1) {
        printf(RED "[!] Loi: ID phai la so nguyen!\n" RESET);
        while (getchar() != '\n'); // dọn dẹp bộ nhớ đệm nếu nhập sai
        return;
    }

    Node* current = *head;
    Node* prev    = NULL;

    // trường hợp 1: node cần xóa nằm ngay đầu danh sách (chính là head)
    if (current != NULL && current->data.id == deleteId) {
        *head = current->next; // dời head sang node thứ 2
        free(current); // trả ram của node cũ
        printf(GREEN "=> Da xoa thanh cong giao dich co ID: %d\n" RESET, deleteId);
        return;
    }

    // trường hợp 2: tìm node cần xóa ở đoạn giữa hoặc cuối danh sách
    while (current != NULL && current->data.id != deleteId) {
        prev    = current; // prev đi sau lưu dấu vết
        current = current->next; // current đi trước dò tìm
    }

    // nếu duyệt đến cuối (null) mà vẫn không tìm thấy ID
    if (current == NULL) {
        printf(RED "[!] Khong tim thay giao dich voi ID: %d\n" RESET, deleteId);
        return;
    }

    // thuật toán xóa: ngắt kết nối node hiện tại, nối node trước với node sau
    prev->next = current->next;
    free(current); // tiêu hủy node cần xóa
    printf(GREEN "=> Da xoa thanh cong giao dich co ID: %d\n" RESET, deleteId);
}

// hàm đệ quy tính tổng tiền theo loại (1: Thu, 0: Chi) - yêu cầu chương 2
long long calculateTotalRecursive(Node* head, int type) {
    // điều kiện dừng của đệ quy: nếu danh sách hết thì trả về 0
    if (head == NULL) return 0;
    long long current = (head->data.type == type) ? head->data.amount : 0;
    // gọi đệ quy: tổng = tiền hiện tại + tổng của phần còn lại
    return current + calculateTotalRecursive(head->next, type);
}

// hàm hiển thị báo cáo tài chính tổng hợp
void showStatistics(Node* head) {
    if (head == NULL) {
        printf(RED "\n[!] Danh sach trong, khong co du lieu de thong ke!\n" RESET);
        return;
    }

    // gọi hàm đệ quy để tính toán
    long long totalIncome  = calculateTotalRecursive(head, 1); // Tổng Thu
    long long totalExpense = calculateTotalRecursive(head, 0); // Tổng Chi
    long long balance      = totalIncome - totalExpense;       // Số dư

    printf(CYAN "\n========================================\n" RESET);
    printf(CYAN "       BAO CAO TAI CHINH TONG HOP\n" RESET);
    printf(CYAN "========================================\n" RESET);
    printf("  1. Tong thu nhap : " GREEN "%15lld VND\n" RESET, totalIncome);
    printf("  2. Tong chi tieu : " RED   "%15lld VND\n" RESET, totalExpense);
    printf(CYAN "----------------------------------------\n" RESET);

    if (balance < 0)
        printf("  => SO DU HIEN TAI: " RED "%15lld VND\n" RESET, balance);
    else
        printf("  => SO DU HIEN TAI: " GREEN "%15lld VND\n" RESET, balance);

    printf(CYAN "----------------------------------------\n" RESET);
    if (balance < 0)
        printf(RED "  [!] Canh bao: Ban chi vuot thu!\n" RESET);
    else
        printf(GREEN "  [*] Trang thai: Tai chinh on dinh.\n" RESET);
    printf(CYAN "========================================\n" RESET);
}
 
// hàm đọc dữ liệu từ file khi khởi động
void loadFromFile(Node** head) {
    // mở file ở chế độ đọc ("r" - read)
    FILE* file = fopen("data.txt", "r");
    if (file == NULL) {
        printf(YELLOW "\n[*] He thong: Khong tim thay 'data.txt'. Se tao file moi khi ban luu.\n" RESET);
        return; // thoát hàm nếu file không tồn tại
    }

    Transaction t;
    int count = 0; // biến đếm xem đã tải được bao nhiêu dòng

    // đọc liên tục cho đến khi hết file (EOF)
    while (fscanf(file, "%d %14s %49s %lld %d",
                  &t.id, t.date, t.category, &t.amount, &t.type) == 5) {
        // dùng lại máy tạo node
        Node* newNode = createNode(t);
        // thuật toán gắn node vào danh sách liên kết
        appendNode(head, newNode); 
        count++;
    }

    // mở file xong nhớ phải đóng lại
    fclose(file);
    if (count > 0)
        printf(GREEN "\n[*] He thong: Da tai thanh cong %d giao dich tu file!\n" RESET, count);
}

// hàm ghi dữ liệu từ danh sách liên kết ra file (ghi đè dữ liệu cũ)
void saveToFile(Node* head) {
    // mở file ở chế độ "w" (write): xóa sạch dữ liệu cũ và ghi mới lại từ đầu
    FILE* file = fopen("data.txt", "w");
    if (file == NULL) {
        printf(RED "\n[!] Loi: Khong the mo file de luu du lieu!\n" RESET);
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

    // ghi xong thì phải đóng cửa lại
    fclose(file);
    printf(GREEN "\n[*] He thong: Da luu an toan %d giao dich vao 'data.txt'.\n" RESET, count);
}

// hàm giải phóng bộ nhớ động để tránh rò rỉ bộ nhớ (memory leak) - yêu cầu chương 2
void freeMemory(Node** head) {
    Node* current = *head;
    Node* nextNode;

    while (current != NULL) {
        // giữ lại địa chỉ node tiếp theo trước khi xóa
        nextNode = current->next;
        // tiêu hủy (trả ram) node hiện tại
        free(current);
        // tiến lên node tiếp theo
        current = nextNode;
    }

    // reset danh sách về trống cho an toàn
    *head = NULL;
    printf(YELLOW "[*] He thong: Da don dep RAM thanh cong (Free Memory).\n" RESET);
}

void showMenu() {
    printf("\n");
    printf(CYAN "  ╔════════════════════════════════════════╗\n" RESET);
    printf(CYAN "  ║" YELLOW BOLD "       QUAN LY THU CHI CA NHAN 2.0      " CYAN "║\n" RESET);
    printf(CYAN "  ╠════════════════════════════════════════╣\n" RESET);
    printf(CYAN "  ║  " GREEN "[1]" RESET " Them giao dich moi                " CYAN "║\n" RESET);
    printf(CYAN "  ║  " GREEN "[2]" RESET " Hien thi danh sach giao dich      " CYAN "║\n" RESET);
    printf(CYAN "  ║  " GREEN "[3]" RESET " Tim kiem giao dich                " CYAN "║\n" RESET);
    printf(CYAN "  ║  " GREEN "[4]" RESET " Xoa giao dich                     " CYAN "║\n" RESET);
    printf(CYAN "  ║  " GREEN "[5]" RESET " Thong ke tong hop                 " CYAN "║\n" RESET);
    printf(CYAN "  ║  " RED   "[6]" RESET " Luu va Thoat                      " CYAN "║\n" RESET);
    printf(CYAN "  ╚════════════════════════════════════════╝\n" RESET);
    printf(YELLOW "  => Nhap lua chon cua ban (1-6): " RESET);
}

int main() {
    // ép Terminal Windows hiển thị đúng font UTF-8
    system("chcp 65001 > nul");

    int   choice;
    // khai báo con trỏ head quản lý Danh sách liên kết (Khởi tạo là NULL)
    Node* head = NULL;

    // gọi hàm tải dữ liệu khi vừa mở app
    loadFromFile(&head);

    do {
        showMenu();

        if (scanf("%d", &choice) != 1) {
            printf(RED "\n[!] Loi: Vui long nhap so nguyen!\n" RESET);
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1: addTransaction(&head);    // gọi hàm Thêm và truyền địa chỉ của head(10/4)
                    break;
            //các case khác giữ nguyên như cũ nha mấy bé(10/4)
            case 2: displayTransactions(head); // gọi hàm hiển thị
                    break;
            case 3: searchTransaction(head);   // gọi hàm tìm kiếm
                    break;
            case 4: deleteTransaction(&head);  // gọi hàm xóa (truyền địa chỉ)
                    break;
            case 5: showStatistics(head);      // gọi hàm thống kê
                    break;
            case 6:
                printf(YELLOW "\n>>> Dang luu du lieu va don dep he thong... Tam biet!\n" RESET);
                saveToFile(head);              // gọi hàm lưu file
                freeMemory(&head);             // gọi hàm dọn rác ram
                break;
            default:
                printf(RED "\n[!] Lua chon khong hop le! Vui long chon tu 1 den 6.\n" RESET);
        }
    } while (choice != 6);

    return 0;
}
