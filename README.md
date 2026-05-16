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
|---|---|---|
| 1 | ➕ Thêm giao dịch | Nhập ID, ngày, danh mục (menu nhanh hoặc tự nhập), số tiền, loại Thu/Chi |
| 2 | 📋 Hiển thị danh sách | Bảng màu sắc — Thu xanh lá `+`, Chi đỏ `-` |
| 3 | 🔍 Tìm kiếm | Tìm gần đúng theo từ khóa danh mục (`strstr`) |
| 4 | 🗑️ Xóa giao dịch | Xóa theo mã ID |
| 5 | 📊 Thống kê | Tổng Thu / Tổng Chi / Số dư — tính bằng hàm đệ quy, cảnh báo khi chi vượt thu |
| 6 | 💾 Lưu & Thoát | Ghi ra `data.txt`, giải phóng RAM trước khi thoát |

---

## 🏗️ Kỹ thuật sử dụng

- **Danh sách liên kết đơn** — cấu trúc lưu trữ dữ liệu chính
- **Bộ nhớ động** — `malloc` / `free`, không memory leak
- **Tệp tin văn bản** — `fscanf` / `fprintf` tự động load/save
- **Hàm đệ quy** — `calculateTotalRecursive` tính tổng Thu/Chi
- **Macro `#define`** — màu ANSI cho giao diện console
- **Xử lý lỗi đầu vào** — validate `scanf`, dọn bộ đệm, kiểm tra con trỏ NULL

---

## 📁 Cấu trúc dự án
Personal-Finance-Tracker-Project/
│
├── main.c        # Toàn bộ logic chương trình
├── structs.h     # Định nghĩa struct Transaction và struct Node
├── data.txt      # Dữ liệu giao dịch (tự tạo khi lưu lần đầu)
├── .gitignore
└── README.md

---

## ⚙️ Hướng dẫn biên dịch và chạy

### Yêu cầu
- GCC (MinGW-w64) trên Windows
- Visual Studio Code + extension C/C++

### Biên dịch
```bash
gcc main.c -o finance_tracker.exe
```

### Chạy
```bash
.\finance_tracker.exe
```

Chương trình tự động tải `data.txt` khi khởi động. Nếu file chưa tồn tại (lần đầu chạy), chương trình sẽ thông báo và tạo mới khi bạn lưu.

---

## 🖥️ Giao diện
╔══════════════════════════════════════╗
║      QUAN LY THU CHI CA NHAN 2.0    ║
╠══════════════════════════════════════╣
║  [1] Them giao dich moi             ║
║  [2] Hien thi danh sach giao dich   ║
║  [3] Tim kiem giao dich             ║
║  [4] Xoa giao dich                  ║
║  [5] Thong ke tong hop              ║
║  [6] Luu va Thoat                   ║
╚══════════════════════════════════════╝
=> Nhap lua chon cua ban (1-6):

---

**Năm học:** 2025 - 2026

---

## 📄 License

Dự án được thực hiện cho mục đích học thuật — môn Kỹ Thuật Lập Trình, UTC2.
