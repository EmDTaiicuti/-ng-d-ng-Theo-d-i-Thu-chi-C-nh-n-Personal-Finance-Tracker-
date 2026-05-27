# 💰 Personal Finance Tracker
> Ứng dụng theo dõi thu chi cá nhân chạy trên console, viết bằng ngôn ngữ C.  
> Đồ án môn **Kỹ Thuật Lập Trình** — Phân hiệu Trường ĐH Giao thông Vận tải TP.HCM (UTC2)

---

## 📖 Mô tả

Ứng dụng giúp người dùng ghi lại, phân loại và theo dõi các khoản thu nhập và chi tiêu hàng ngày một cách đơn giản, nhanh gọn — không cần internet, không cần cài đặt phức tạp.

Lấy cảm hứng từ tính năng quản lý chi tiêu của ứng dụng MoMo, nhưng được xây dựng hoàn toàn trên console bằng C thuần, tập trung vào sự đơn giản và tốc độ thao tác.

**Được xây dựng dựa trên kết quả khảo sát 220 người dùng** về nhu cầu quản lý tài chính cá nhân thực tế.

---

## ✨ Tính năng

| # | Chức năng | Mô tả |
|---|-----------|-------|
| 1 | ➕ Thêm giao dịch | Chọn loại Thu/Chi → chọn danh mục nhanh (menu) hoặc tự nhập → chọn ngày hôm nay hoặc nhập thủ công (có validate năm nhuận) → nhập số tiền |
| 2 | 📋 Hiển thị danh sách | Bảng viền kép màu sắc — Thu xanh lá `+`, Chi đỏ `-` |
| 3 | 🔍 Tìm kiếm | Tìm gần đúng theo từ khóa danh mục (`strstr`), hiển thị kết quả dạng bảng |
| 4 | 🗑️ Xóa giao dịch | Xóa theo mã ID, xử lý đủ 3 trường hợp: đầu / giữa / cuối danh sách |
| 5 | 📊 Thống kê tổng hợp | Tổng Thu / Tổng Chi / Số dư — tính bằng **hàm đệ quy**, cảnh báo khi chi vượt thu |
| 6 | 🤖 Phân tích tài chính | Lọc dữ liệu theo tháng / năm, xác định danh mục chi nhiều nhất, đưa ra lời khuyên tự động (cảnh báo / khuyến khích tiết kiệm) |
| 7 | 💾 Lưu & Thoát | Ghi ra `data.txt`, giải phóng toàn bộ RAM trước khi thoát |

### Danh mục có sẵn

| Chi tiêu | Thu nhập |
|----------|----------|
| Ăn uống, Giải trí, Tiền nhà, Mua sắm, Sức khỏe, Đầu tư, Khác | Lương, Thưởng, Refund, Lãi, Thu nhập khác |

---

## 🏗️ Kỹ thuật sử dụng

- **Danh sách liên kết đơn** — cấu trúc lưu trữ dữ liệu chính (`Node` → `Node` → ... → `NULL`)
- **Bộ nhớ động** — `malloc` / `free`, không memory leak, hàm `freeMemory` dọn sạch trước khi thoát
- **Tệp tin văn bản** — `fscanf` / `fprintf` tự động load/save qua `data.txt`
- **Hàm đệ quy** — `calcTotal()` tính tổng Thu/Chi bằng đệ quy
- **Validate đầu vào toàn diện** — kiểm tra `scanf`, dọn bộ đệm `getchar()`, kiểm tra NULL pointer
- **Validate ngày hợp lệ** — kiểm tra năm nhuận, giới hạn ngày theo từng tháng
- **Phân tích dữ liệu** — lọc theo tháng/năm, gom nhóm danh mục chi, tính tỉ lệ Chi/Thu
- **Macro `#define`** — màu ANSI (`RED`, `GREEN`, `YELLOW`, `CYAN`...) + macro vẽ bảng (`ROW_PLAIN`, `ROW_MONEY`...)
- **Giao diện viền kép** — ký tự box-drawing CP437 (`╔`, `╠`, `╗`...) cho bảng và khung menu
- **`windows.h`** — `SetConsoleOutputCP(437)` và `enableANSI()` để hỗ trợ màu ANSI trên Windows

---

## 📁 Cấu trúc dự án

```
Personal-Finance-Tracker-Project/
│
├── main.c        # Toàn bộ logic chương trình (7 chức năng + menu + file I/O)
├── structs.h     # Định nghĩa struct Transaction và struct Node (danh sách liên kết)
├── data.txt      # Dữ liệu giao dịch (tự tạo khi lưu lần đầu)
├── .gitignore
└── README.md
```

---

## ⚙️ Hướng dẫn biên dịch và chạy

### Yêu cầu

- **OS:** Windows (do dùng `windows.h` để bật ANSI và set codepage CP437)
- **Compiler:** GCC (MinGW-w64) hoặc tương đương
- **Editor gợi ý:** Visual Studio Code + extension C/C++

### Biên dịch

```bash
gcc main.c -o finance.exe
```

### Chạy

```bash
.\finance.exe
```

Chương trình tự động tải `data.txt` khi khởi động. Nếu file chưa tồn tại (lần đầu chạy), chương trình sẽ thông báo và tạo mới khi bạn chọn **Lưu & Thoát**.

> ⚠️ **Lưu ý:** Để hiển thị đúng viền kép và màu sắc, terminal phải hỗ trợ **ANSI escape codes** và **codepage CP437** (Windows Terminal hoặc cmd.exe đều dùng được).

---

## 🖥️ Giao diện

```
 ╔═════════════════════════════════════════════╗
 ║         QUAN LY THU CHI CA NHAN v4.1        ║
 ╠═════════════════════════════════════════════╣
 ║ [1] Them giao dich moi                      ║
 ║ [2] Hien thi danh sach                      ║
 ║ [3] Tim kiem giao dich                      ║
 ║ [4] Xoa giao dich                           ║
 ║ [5] Thong ke tong hop                       ║
 ║ [6] Phan tich du lieu theo thang/nam        ║
 ║ [7] Luu va Thoat                            ║
 ╚═════════════════════════════════════════════╝
  => Chon (1-7):
```

---

## 📊 Định dạng file `data.txt`

Mỗi dòng lưu một giao dịch theo cấu trúc:

```
<ID> <DD/MM/YYYY> <DanhMuc> <SoTien> <Loai>
```

Ví dụ:

```
1 01/04/2026 Luong_thang 5000000 1
2 02/04/2026 Tien_tro_thang4 1500000 0
3 05/04/2026 An_sang_pho 35000 0
```

Trong đó `1` = Thu nhập, `0` = Chi tiêu. Danh mục không có dấu cách (dùng `_` thay thế).

---

## 🔁 Luồng hoạt động

```
Khởi động → loadFromFile() → Vòng lặp Menu
    ├─ [1] addTransaction()      → thêm node vào cuối linked list
    ├─ [2] displayTransactions() → duyệt và in bảng
    ├─ [3] searchTransaction()   → duyệt + strstr() lọc kết quả
    ├─ [4] deleteTransaction()   → tìm theo ID → free(node)
    ├─ [5] showStatistics()      → calcTotal() đệ quy → in báo cáo
    ├─ [6] analyzeAI()           → lọc theo tháng/năm → lời khuyên
    └─ [7] saveToFile() → freeMemory() → exit
```

---

**Năm học:** 2025 – 2026

---

## 📄 License

Dự án được thực hiện cho mục đích học thuật — môn Kỹ Thuật Lập Trình, UTC2.
