#include "vm/supplemental_page_table.h"
#include "threads/malloc.h"
#include "userprog/pagedir.h"
#include "devices/block.h"
#include "threads/vaddr.h"

/* Khởi tạo Supplemental Page Table */
void supplemental_page_table_init(struct supplemental_page_table *spt) {
    hash_init(&spt->page_table, page_hash, page_less, NULL);
}

/* Hàm băm cho trang */
unsigned page_hash(const struct hash_elem *p_, void *aux UNUSED) {
    const struct page *p = hash_entry(p_, struct page, hash_elem);
    return hash_bytes(&p->addr, sizeof p->addr);
}

/* So sánh các trang */
bool page_less(const struct hash_elem *a_, const struct hash_elem *b_, void *aux UNUSED) {
    const struct page *a = hash_entry(a_, struct page, hash_elem);
    const struct page *b = hash_entry(b_, struct page, hash_elem);
    return a->addr < b->addr;
}

/* Tìm một trang trong SPT */
struct page *supplemental_page_table_find(struct supplemental_page_table *spt, void *addr) {
    struct page p;
    struct hash_elem *e;

    p.addr = addr;
    e = hash_find(&spt->page_table, &p.hash_elem);
    return e != NULL ? hash_entry(e, struct page, hash_elem) : NULL;
}

/* Thêm một trang vào SPT */
bool supplemental_page_table_insert(struct supplemental_page_table *spt, struct page *page) {
    struct hash_elem *prev = hash_insert(&spt->page_table, &page->hash_elem);
    return prev == NULL;
}

/* Xóa một trang khỏi SPT */
void supplemental_page_table_remove(struct supplemental_page_table *spt, void *addr) {
    struct page *page = supplemental_page_table_find(spt, addr);
    if (page != NULL) {
        hash_delete(&spt->page_table, &page->hash_elem);
        free(page);
    }
}

