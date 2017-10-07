#define NUMINODES 80
#define NUMBLOCKS 56

char * fileSystem;

typedef struct __inode__ {

	int8_t inode_number;
	int8_t allocated_blocks;
	int8_t blocks[15];

} inode;

typedef struct __superdata__{

	int8_t filenumber;
} superdata;

void initSuperdata(FILE * fp)
{
	superdata temp;
	temp.filenumber = 0;

	fseek(fp, 0, SEEK_SET);
	fwrite(&temp, sizeof(temp), 1, fp);
}


void print_inode_data(inode * temp)
{
	printf("inode id : %d, blocks allocated : %d\n", temp->inode_number, temp->allocated_blocks);

	int i;

	for (i = 0; i < temp->allocated_blocks; i++)
		printf("blocks : %d ", temp->blocks[i]);
	printf("\n\n");
}

void initInodes(FILE * fp)
{
	int i, j;

	for (i = 0; i < NUMINODES; i++)
	{
		inode temp;
		temp.inode_number = i + 1;
		temp.allocated_blocks = 0;

		for (j = 0; j < 15; j++)
			temp.blocks[j] = 0;

		fseek(fp, 3 * 4 * 1024 + i * 256, SEEK_SET);
		fwrite(&temp, sizeof(inode), 1, fp);

	}

	int8_t val = 1;

	fseek(fp, 1 * 4 * 1024, SEEK_SET);
	fwrite(&val, sizeof(int8_t), 1, fp);	// first block has file names
}

void printInodes()
{

	FILE * fp = fopen(fileSystem, "r");

	int i;

	for (i = 0; i < NUMINODES; i++)
	{
		inode temp;

		fseek(fp, 3 * 4 * 1024 + i * 256, SEEK_SET);
		fread(&temp, sizeof(inode), 1, fp);
		print_inode_data(&temp);
	}

	fclose(fp);

}

int createSFS(char * filename, int nbytes)
{
	fileSystem = (char *) malloc((strlen(filename) + 1) * sizeof(char));
	strcpy(fileSystem, filename);

	FILE *fp = fopen(fileSystem, "wb+");
	int i = 0;
	int8_t byte = 0x0;

	for (i = 0; i < nbytes; i++)
		fwrite(&byte, sizeof(byte), 1, fp);

	int fd = fileno(fp);

	initInodes(fp);
	initSuperdata(fp);

	fclose(fp);
	return fd;
}


int writeData(int disk, int blockNum, void * block)
{
	// blocknum 0 -> blocknum 8 in file system
	// block should be 4 * 1024 bytes

	FILE * fp = fopen(fileSystem, "r+");

	fseek(fp, (blockNum + 8) * 4 * 1024, SEEK_SET);
	fwrite(block, 4 * 1024 * sizeof(char), 1, fp);
	fclose(fp);

	return 0;
}


int readData(int disk, int blockNum, void * block)
{
	// blocknum 0 -> blocknum 8 in file system
	// block should be 4 * 1024 bytes

	FILE * fp = fopen(fileSystem, "r");
	
	fseek(fp, (blockNum + 8) * 4 * 1024, SEEK_SET);
	fread(block, 4 * 1024 * sizeof(char), 1, fp);
	fclose(fp);

	return 0;
}

void print_inode_bitmap()
{
	int i;
	int8_t val;

	FILE *fp = fopen(fileSystem, "r");
	fseek(fp, 2 * 4 * 1024, SEEK_SET);

	for (i = 0; i < NUMINODES; i++)
	{
		fread(&val, sizeof(int8_t), 1, fp);
		printf("inode number : %d value : %d\n", i + 1, val);
	}

	fclose(fp);
}

void print_block_bitmap()
{
	int i;
	int8_t val;

	FILE *fp = fopen(fileSystem, "r");
	fseek(fp, 1 * 4 * 1024, SEEK_SET);

	for (i = 0; i < NUMBLOCKS; i++)
	{
		fread(&val, sizeof(int8_t), 1, fp);
		printf("block number : %d value : %d\n", i + 1, val);
	}

	fclose(fp);
}

typedef struct __inode_table_entry__ {

	char name[127];
	int8_t inode_num;

} inode_table_entry;


void print_FileList()
{
	int i;
	inode_table_entry temp;

	FILE *fp = fopen(fileSystem, "r");

	superdata superdatatemp;
	fread(&superdatatemp, sizeof(superdatatemp), 1, fp);

	fseek(fp, 8 * 4 * 1024, SEEK_SET);

	for (i = 0; i < superdatatemp.filenumber; i++)
	{
		fread(&temp, sizeof(temp), 1, fp);
		printf("name : %s inode number : %d\n", temp.name, temp.inode_num);
	}

	fclose(fp);
}


int writeFile(int disk, char * filename, void* block)
{
	// update dir list

	FILE *fp = fopen(fileSystem, "r+");

	superdata superdatatemp;
	fread(&superdatatemp, sizeof(superdatatemp), 1, fp);
	

	inode_table_entry newEntry;
	strcpy(newEntry.name, filename);
	
	// find inode

	fseek(fp, 2 * 4 * 1024, SEEK_SET);
	int8_t val = 1;
	int i = 0, j = 0;

	while (1 && (i < 80))
	{
		fread(&val, sizeof(val), 1, fp);

		if (val == 0)
			break;
		else
			i += 1;
	}

	newEntry.inode_num = i;
	fseek(fp, 2 * 4 * 1024 + i, SEEK_SET);
	val = 1;
	fwrite(&val, sizeof(val), 1, fp);

	fseek(fp, 8 * 4 * 1024 + superdatatemp.filenumber * 128, SEEK_SET);
	fwrite(&newEntry, sizeof(newEntry), 1, fp);

	inode new_inode;
	fseek(fp, 3 * 4 * 1024 + i * 256, SEEK_SET);
	fread(&new_inode, sizeof(new_inode), 1, fp);

	fseek(fp, 1 * 4 * 1024, SEEK_SET);
	
	while (1 && (j < 56))
	{
		fread(&val, sizeof(val), 1, fp);

		if (val == 0)
			break;
		else
			j += 1;
	}
	
	fseek(fp, 1 * 4 * 1024 + j, SEEK_SET);
	val = 1;
	fwrite(&val, sizeof(val), 1, fp);
	new_inode.blocks[new_inode.allocated_blocks] = j;
	new_inode.allocated_blocks += 1;

	writeData(disk, j, block);

	fseek(fp, 3 * 4 * 1024 + i * 256, SEEK_SET);
	fwrite(&new_inode, sizeof(new_inode), 1, fp);

	superdatatemp.filenumber += 1;
	fseek(fp, 0, SEEK_SET);
	fwrite(&superdatatemp, sizeof(superdatatemp), 1, fp);

	fclose(fp);

	return 0;
}


int readFile(int disk, char* filename, char ** block)
{

	FILE *fp = fopen(fileSystem, "r");

	superdata superdatatemp;
	inode fileinode;
	inode_table_entry inodetableentry;

	fread(&superdatatemp, sizeof(superdatatemp), 1, fp);
	fseek(fp, 8 * 4 * 1024, SEEK_SET);

	int i;

	for (i = 0; i < superdatatemp.filenumber; i++)
	{
		fread(&inodetableentry, sizeof(inodetableentry), 1, fp);
		if (strcmp(inodetableentry.name, filename) == 0)
			break;
	}

	fseek(fp, 3 * 4 * 1024 + inodetableentry.inode_num * 256, SEEK_SET);
	fread(&fileinode, sizeof(fileinode), 1, fp);

	(*block) = (char *) malloc(((fileinode.allocated_blocks) * 4 * 1024 + 1) * sizeof(char));
	char * temp = (char *) malloc((4 * 1024 + 1 ) * sizeof(char));
	(*block)[0] = '\0';

	for (i = 0; i < fileinode.allocated_blocks; i++)
	{
		readData(disk, fileinode.blocks[i], temp);
		strcat(*block, temp);
	}

	fclose(fp);

	return 0;

}







