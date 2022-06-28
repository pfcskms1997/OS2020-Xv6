#define NSLAB 9 // {8, 16, 32, 64, 128, 256, 512, 1024, 2048}
#define MAX_PAGES_PER_SLAB 100
#define BYTES_PER_PAGE 4096
#define BITS_PER_CHAR 8
#define MAX_OBJECTS_PER_PAGE 4096
struct slab {
	int size; //얼마의 크기단위로 할당을 하는지
	int num_pages; // 몇개의 페이지를 할당해서 들고 있는지
	int num_free_objects; // 해당 할당이 되어있는 오브젝트와 반대의 경우
	int num_used_objects;
	int num_objects_per_page;
	char *bitmap; //각각의 오브젝트들 어떤것을 할당했는지
	char *page[MAX_PAGES_PER_SLAB]; //고정된 크기를 오브젝트단위로 할당하는 방식이기에 페이지들을 갖고있는 배열
};
