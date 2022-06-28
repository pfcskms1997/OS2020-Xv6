#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"
#include "slab.h"

struct {
	struct spinlock lock;
	struct slab slab[NSLAB];
} stable;

void slabinit(){
	struct slab *s;
	int sz = 8;

	acquire(&stable.lock);
	for(s = stable.slab; s < &stable.slab[NSLAB]; s++){
		s->size = sz;
		s->num_pages = 1;
		s->num_free_objects = 4096 / s->size;
		s->num_used_objects = 0;
		s->num_objects_per_page = 4096 / s->size;
		s->bitmap = (char*)kalloc(); 	// allocate 1-page for bitmap
		s->page[0] = (char*)kalloc();	// allocate a slab page

		sz <<= 1;
	}
	release(&stable.lock);
}

char *kmalloc(int size){
	struct slab *s;
	char checkBit = 0;
	int slabNum = 0;
	int slabAddr = 0;
	int temp = 0;

	if(0 >= size || size > 2048){
		cprintf("Allocation is impossible!\n");
		exit();
	}

	// Calculating Index
	// Find a smallest power of 2 greater than or equal to size
	temp = (size - 1) >> 3;

	while(temp != 0){
		temp >>= 1;
		slabNum += 1;
	}

	acquire(&stable.lock);
	s = &stable.slab[slabNum];

	// bitmap setting
	for(int i = 0; i < 4096; i++){
		checkBit = s->bitmap[i];

		for(int j = 0; j < 8; j++){
			if(checkBit == 0){
				s->bitmap[i] = s->bitmap[i] | (1 << j); // bmp | (1 << i)
				goto allocate;
			}
			checkBit >>= 1;
		}
	}

// allocate slab
allocate:
	if(s->num_free_objects == 0){
		s->page[s->num_pages] = (char*)kalloc();
		s->num_pages += 1;
		s->num_free_objects += s->num_objects_per_page;
	}

	s->num_free_objects -= 1;
	s->num_used_objects += 1;

	slabAddr = (s->num_used_objects - 1) % s->num_objects_per_page;

	release(&stable.lock);
	return s->page[s->num_pages - 1] + ((s->size) * slabAddr);
}

void kmfree(char *addr, int size){
	struct slab *s;
	int count = 0;
	int temp = 0;
	int slabCacheNum = 0;
	int bitPageNum = 0;
	int bitNum = 0;

	if(0 >= size || size > 2048){
		cprintf("Deallocation is impossible!\n");
		exit();
	}

	// Calculating Index
	// Find a smallest power of 2 greater than or equal to size
	temp = (size - 1) >> 3;

	while(temp != 0){
		temp >>= 1;
		slabCacheNum += 1;
	}

	acquire(&stable.lock);
	s = &stable.slab[slabCacheNum];
	
	// bitmap을 해제 할 주소공간이 몇 번째 몇 번째 slab object에 있는지를 count
	for(int i = 0; i < MAX_PAGES_PER_SLAB; i++){
		for(int j = 0; j < s->num_objects_per_page; j++){
			if(addr == s->page[i] + (s->size * j)){
				//cprintf("%d %x %x\n", i, addr, s->page[i] + (s->size) * j);
				goto free;
			}
			count++;
		}
	}

free:
	bitPageNum = count / 8; // 해제할 bit가 4KB bitmap page 내에서 몇 번째 char 크기의 공간에 있는지를 저장
	bitNum = count % 8; // char 크기의 bitmap 공간에서 해제할 bit는 몇 번째에 있는지를 저장 (범위 : 0 ~ 7)
	
	s->bitmap[bitPageNum] = s->bitmap[bitPageNum] & ~(1 << bitNum); // bmp & ~(1 << i)

	if((s->num_free_objects == s->num_objects_per_page)){
		s->num_pages -= 1;
		s->num_free_objects -= s->num_objects_per_page;
	}

	s->num_free_objects += 1;
	s->num_used_objects -= 1;

	release(&stable.lock);
}

void slabdump(){
	cprintf("__slabdump__\n");

	struct slab *s;

	cprintf("size\tnum_pages\tused_objects\tfree_objects\n");

	for(s = stable.slab; s < &stable.slab[NSLAB]; s++){
		cprintf("%d\t%d\t\t%d\t\t%d\n", 
			s->size, s->num_pages, s->num_used_objects, s->num_free_objects);
	}
}
