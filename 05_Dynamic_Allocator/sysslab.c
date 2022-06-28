#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "spinlock.h"

#define TESTALLOCSIZE 9
#define NTEST 1000

struct test{
	char data[TESTALLOCSIZE];
};

void slabtest(){

	int i;
	struct test *t[NTEST];

	slabdump(); // Check initial states

	cprintf("kmalloc test\n");
	for(i=0;i<NTEST;i++){
		t[i] = (struct test *)kmalloc(sizeof(struct test));
		//cprintf("addr[%d] : %x\n", i, t[i]);
	}

	slabdump(); // Check kmalloc function

	cprintf("kmfree test\n");
	for(i=NTEST-1;i>=0;i--){
		kmfree((char *)t[i], TESTALLOCSIZE);
	}

	slabdump(); // Check kmfree function
}

int sys_slabtest(){
	slabtest();
	return 0;
}
