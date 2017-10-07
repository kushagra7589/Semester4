#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "string.h"
#include "15010_15049_1.h"

// 1 -> S
// 2 -> Block bitmap
// 3 -> Inode bitmap
// 4 -- 8 -> Inodes (5 * 4 * 4 * 256) total 80
// 9 -- 64 ->Data

int main()
{
	int fd = createSFS("myFileSystem", 64 * 4 * 1024);

	char * buffer = (char *) malloc((4 * 1024 + 1) * sizeof(char));
	memset(buffer, 'a', 4 * 1024 * sizeof(char));
	buffer[4 * 1024] = '\0';

	// writeData(fd, 0, buffer);
	// printf("%s\n", buffer);
	// readData(fd, 0, buffer);
	// print_block_bitmap();
	// print_FileList();
	
	memset(buffer, 'b', 4 * 1024 * sizeof(char));
	writeFile(fd, "b file", buffer);
	
	memset(buffer, 'c', 4 * 1024 * sizeof(char));
	writeFile(fd, "c file", buffer);

	memset(buffer, 'a', 4 * 1024 * sizeof(char));
	writeFile(fd, "akarsha", buffer);
	
	printf("%s\n", "File list :");
	print_FileList();

	printf("Inodes : \n");
	printInodes();

	printf("Inode bitmap\n");
	print_inode_bitmap();

	readFile(fd, "b file", &buffer);
	printf("%s\n", buffer);

	readFile(fd, "c file", &buffer);
	printf("%s\n", buffer);

	readFile(fd, "akarsha", &buffer);
	printf("%s\n", buffer);
	return 0;
}