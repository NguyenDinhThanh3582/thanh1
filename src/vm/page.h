#ifndef VM_PAGE_H
#define VM_PAGE_H

#include <hash.h>
#include "filesys/file.h"
#include "threads/thread.h"

struct supplemental_page_table {
    struct hash page_table;
};

struct page {
    void *addr;                   // Địa chỉ ảo của trang
    struct frame *frame;          // Con trỏ đến frame chứa trang này
    struct hash_elem hash_elem;   // Phần tử hash cho bảng băm
    bool dirty;                   // Cờ cho biết trang có bị sửa đổi hay không
    struct file *file;            // File nếu trang được ánh xạ từ file
    off_t file_offset;            // Offset trong file
    size_t file_read_bytes;       // Số byte đọc từ file
    size_t file_zero_bytes;       // Số byte còn lại được điền bằng 0
    block_sector_t swap_slot;     // Swap slot nếu trang được swap ra
    bool writable;                // Cờ cho biết trang có thể ghi hay không
};

void supplemental_page_table_init(struct supplemental_page_table *spt);
struct page *supplemental_page_table_find(struct supplemental_page_table *spt, void *addr);
bool supplemental_page_table_insert(struct supplemental_page_table *spt, struct page *page);
void supplemental_page_table_remove(struct supplemental_page_table *spt, void *addr);
bool handle_page_fault(void *fault_addr);
bool load_page(struct page *page);

#endif /* VM_PAGE_H */

