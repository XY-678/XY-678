#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define MAX_FILE_SIZE 1024
#define MAX_FILES 100
#define MAX_DISK_BLOCKS 1000
#define SAVE_FILE "filesystem_.dat"

// FAT table entry
typedef struct FATEntry
{
	int nextBlock;
} FATEntry;

// File structure
typedef struct File
{
	char name[MAX_NAME_LEN];
	int startBlock;
	int length;
	int isOpen;
	struct File *next;
} File;

// Directory structure
typedef struct Directory
{
	char name[MAX_NAME_LEN];
	struct Directory *parent;
	struct Directory *subdirs;
	File *files;
	struct Directory *next;
} Directory;

// FAT table
FATEntry FAT[MAX_DISK_BLOCKS];
char disk[MAX_DISK_BLOCKS][MAX_FILE_SIZE];
int freeBlockIndex = 0;

Directory *root;
Directory *currentDir;
char currentPath[MAX_NAME_LEN * MAX_FILES] = "~";

void initialize();
void my_format();
void my_mkdir(const char *dirname);
void my_rmdir(const char *dirname);
void my_ls();
void my_cd(const char *dirname);
void my_create(const char *filename);
void my_open(const char *filename);
void my_close(const char *filename);
void my_write(const char *filename, const char *content);
void my_read(const char *filename);
void my_rm(const char *filename);
void my_exitsys();
void save_filesystem();
void load_filesystem();
void update_path();
int is_block_free(int block);
void save(Directory *dir, FILE *file);
Directory *create(Directory *parent, FILE *file);

int allocate_block()
{
	for (int i = freeBlockIndex; i < MAX_DISK_BLOCKS; ++i)
		{
			if (FAT[i].nextBlock == -1 && is_block_free(i))
				{
					freeBlockIndex = i + 1;
					return i;
				}
		}
	for (int i = 0; i < freeBlockIndex; ++i)
		{
			if (FAT[i].nextBlock == -1 && is_block_free(i))
				{
					freeBlockIndex = i + 1;
					return i;
				}
		}
	return -1;
}

int is_block_free(int block)
{
	for (int i = 0; i < MAX_DISK_BLOCKS; ++i)
		{
			if (FAT[i].nextBlock == block)
				{
					return 0;
				}
		}
	return 1;
}

void initialize()
{
	for (int i = 0; i < MAX_DISK_BLOCKS; ++i)
		{
			FAT[i].nextBlock = -1;
		}

	root = (Directory *)malloc(sizeof(Directory));
	strcpy(root->name, "/");
	root->parent = NULL;
	root->subdirs = NULL;
	root->files = NULL;
	root->next = NULL;
	currentDir = root;
	strcpy(currentPath, "~");
}

void my_format()
{
	free(root);
	initialize();
	printf("Filesystem formatted.\n");
}

void my_mkdir(const char *dirname)
{
	Directory *newDir = (Directory *)malloc(sizeof(Directory));
	strcpy(newDir->name, dirname);
	newDir->parent = currentDir;
	newDir->subdirs = NULL;
	newDir->files = NULL;
	newDir->next = currentDir->subdirs;
	currentDir->subdirs = newDir;
}

void my_rmdir(const char *dirname)
{
	Directory *prev = NULL, *curr = currentDir->subdirs;
	while (curr != NULL && strcmp(curr->name, dirname) != 0)
		{
			prev = curr;
			curr = curr->next;
		}
	if (curr == NULL)
		{
			printf("Directory %s does not exist.\n", dirname);
			return;
		}
	if (prev == NULL)
		{
			currentDir->subdirs = curr->next;
		}
	else
		{
			prev->next = curr->next;
		}
	free(curr);
}

void my_ls()
{
	Directory *subdir = currentDir->subdirs;
	while (subdir != NULL)
		{
			printf("%s\n", subdir->name);
			subdir = subdir->next;
		}
	File *file = currentDir->files;
	while (file != NULL)
		{
			printf("%s\n", file->name);
			file = file->next;
		}
}

void my_cd(const char *dirname)
{
	if (strcmp(dirname, "..") == 0)
		{
			if (currentDir->parent != NULL)
				{
					currentDir = currentDir->parent;
					update_path();
				}
			return;
		}
	Directory *subdir = currentDir->subdirs;
	while (subdir != NULL)
		{
			if (strcmp(subdir->name, dirname) == 0)
				{
					currentDir = subdir;
					update_path();
					return;
				}
			subdir = subdir->next;
		}
	printf("Directory %s does not exist.\n", dirname);
}

void update_path()
{
	if (currentDir == root)
		{
			strcpy(currentPath, "~");
		}
	else
		{
			Directory *temp = currentDir;
			char tempPath[MAX_NAME_LEN * MAX_FILES] = "";
			while (temp != root)
				{
					char tempName[MAX_NAME_LEN];
					strcpy(tempName, temp->name);
					strcat(tempName, "/");
					strcat(tempName, tempPath);
					strcpy(tempPath, tempName);
					temp = temp->parent;
				}
			strcpy(currentPath, "~/");
			strcat(currentPath, tempPath);
			currentPath[strlen(currentPath) - 1] = '\0';
		}
}

void my_create(const char *filename)
{
	File *newFile = (File *)malloc(sizeof(File));
	strcpy(newFile->name, filename);
	newFile->startBlock = allocate_block();
	if (newFile->startBlock == -1)
		{
			printf("No free blocks available.\n");
			free(newFile);
			return;
		}
	newFile->length = 0;
	newFile->isOpen = 0;
	newFile->next = currentDir->files;
	currentDir->files = newFile;
	printf("File %s created at block %d.\n", filename, newFile->startBlock);
}

void my_open(const char *filename)
{
	File *file = currentDir->files;
	while (file != NULL)
		{
			if (strcmp(file->name, filename) == 0)
				{
					file->isOpen = 1;
					printf("File %s opened.\n", filename);
					return;
				}
			file = file->next;
		}
	printf("File %s does not exist.\n", filename);
}

void my_close(const char *filename)
{
	File *file = currentDir->files;
	while (file != NULL)
		{
			if (strcmp(file->name, filename) == 0)
				{
					file->isOpen = 0;
					printf("File %s closed.\n", filename);
					return;
				}
			file = file->next;
		}
	printf("File %s does not exist.\n", filename);
}

void my_write(const char *filename, const char *content)
{
	File *file = currentDir->files;
	while (file != NULL)
		{
			if (strcmp(file->name, filename) == 0)
				{
					if (!file->isOpen)
						{
							printf("File %s is not open.\n", filename);
							return;
						}

					int blockIndex = file->startBlock;
					int offset = 0;
					int contentLength = strlen(content);

					while (offset < contentLength)
						{
							int remainingBytes = MAX_FILE_SIZE - (file->length % MAX_FILE_SIZE);
							int writeLength = (contentLength - offset < remainingBytes) ? contentLength - offset : remainingBytes;
							memcpy(disk[blockIndex] + (file->length % MAX_FILE_SIZE), content + offset, writeLength);
							offset += writeLength;
							file->length += writeLength;

							if (offset < contentLength)
								{
									if (FAT[blockIndex].nextBlock == -1)
										{
											int newBlock = allocate_block();
											if (newBlock == -1)
												{
													printf("No free blocks available.\n");
													return;
												}
											FAT[blockIndex].nextBlock = newBlock;
											blockIndex = newBlock;
											FAT[blockIndex].nextBlock = -1;
										}
									else
										{
											blockIndex = FAT[blockIndex].nextBlock;
										}
								}
						}

					printf("Written to file %s.\n", filename);
					return;
				}
			file = file->next;
		}
	printf("File %s does not exist.\n", filename);
}

void my_read(const char *filename)
{
	File *file = currentDir->files;
	while (file != NULL)
		{
			if (strcmp(file->name, filename) == 0)
				{
					if (!file->isOpen)
						{
							printf("File %s is not open.\n", filename);
							return;
						}
					int blockIndex = file->startBlock;
					int offset = 0;
					while (blockIndex != -1 && offset < file->length)
						{
							int readLength = (file->length - offset < MAX_FILE_SIZE) ? file->length - offset : MAX_FILE_SIZE;
							fwrite(disk[blockIndex], 1, readLength, stdout);
							offset += readLength;
							blockIndex = FAT[blockIndex].nextBlock;
						}
					printf("\n");
					return;
				}
			file = file->next;
		}
	printf("File %s does not exist.\n", filename);
}

void my_rm(const char *filename)
{
	File *prev = NULL, *curr = currentDir->files;
	while (curr != NULL && strcmp(curr->name, filename) != 0)
		{
			prev = curr;
			curr = curr->next;
		}
	if (curr == NULL)
		{
			printf("File %s does not exist.\n", filename);
			return;
		}
	if (prev == NULL)
		{
			currentDir->files = curr->next;
		}
	else
		{
			prev->next = curr->next;
		}

	int blockIndex = curr->startBlock;
	while (blockIndex != -1)
		{
			int nextBlock = FAT[blockIndex].nextBlock;
			FAT[blockIndex].nextBlock = -1;
			blockIndex = nextBlock;
		}

	free(curr);
	printf("File %s removed.\n", filename);
}

void my_exitsys()
{
	save_filesystem();
	printf("文件系统已保存\n");
	exit(0);
}

void save_filesystem()
{
	FILE *file = fopen(SAVE_FILE, "wb");
	if (file == NULL)
		{
			printf("Error saving filesystem.\n");
			return;
		}

	fwrite(&freeBlockIndex, sizeof(int), 1, file);
	fwrite(FAT, sizeof(FATEntry), MAX_DISK_BLOCKS, file);
	fwrite(disk, sizeof(char), MAX_DISK_BLOCKS * MAX_FILE_SIZE, file);//保存全局信息

	save(root, file);

	fclose(file);
	printf("Filesystem saved.\n");
}

//save() 函数递归地保存目录和文件信息，包括目录名、子目录数、文件名，文件数及文件的起始块和长度。
void save(Directory *dir, FILE *file)
{
	int subdirCount = 0, fileCount = 0;
	Directory *subdir = dir->subdirs;
	while (subdir)
		{
			subdirCount++;
			subdir = subdir->next;
		}

	File *currFile = dir->files;
	while (currFile)
		{
			fileCount++;
			currFile = currFile->next;
		}

	fwrite(dir->name, sizeof(char), MAX_NAME_LEN, file);
	fwrite(&subdirCount, sizeof(int), 1, file);
	fwrite(&fileCount, sizeof(int), 1, file);

	currFile = dir->files;
	while (currFile)
		{
			fwrite(currFile->name, sizeof(char), MAX_NAME_LEN, file);
			fwrite(&currFile->startBlock, sizeof(int), 1, file);
			fwrite(&currFile->length, sizeof(int), 1, file);
			currFile = currFile->next;
		}

	subdir = dir->subdirs;
	while (subdir)
		{
			save(subdir, file);
			subdir = subdir->next;
		}
}

void load_filesystem()
{
	FILE *file = fopen(SAVE_FILE, "rb");
	if (file == NULL)
		{
			initialize();
			printf("没有现有系统，初始化一个新系统\n");
			return;
		}

	fread(&freeBlockIndex, sizeof(int), 1, file);
	fread(FAT, sizeof(FATEntry), MAX_DISK_BLOCKS, file);
	fread(disk, sizeof(char), MAX_DISK_BLOCKS * MAX_FILE_SIZE, file);

	root = create(NULL, file);
	currentDir = root;
	strcpy(currentPath, "~");

	fclose(file);
	printf("Filesystem loaded.\n");
}

//create() 函数递归地从文件中加载目录和文件信息。
Directory *create(Directory *parent, FILE *file)
{
	Directory *dir = (Directory *)malloc(sizeof(Directory));
	fread(dir->name, sizeof(char), MAX_NAME_LEN, file);
	dir->parent = parent;

	int subdirCount, fileCount;
	fread(&subdirCount, sizeof(int), 1, file);
	fread(&fileCount, sizeof(int), 1, file);

	dir->subdirs = NULL;
	dir->files = NULL;

	for (int i = 0; i < fileCount; ++i)
		{
			File *newFile = (File *)malloc(sizeof(File));
			fread(newFile->name, sizeof(char), MAX_NAME_LEN, file);
			fread(&newFile->startBlock, sizeof(int), 1, file);
			fread(&newFile->length, sizeof(int), 1, file);
			newFile->isOpen = 0;
			newFile->next = dir->files;
			dir->files = newFile;
		}

	for (int i = 0; i < subdirCount; ++i)
		{
			Directory *subdir = create(dir, file);
			subdir->next = dir->subdirs;
			dir->subdirs = subdir;
		}

	return dir;
}

int main()
{
	char command[256];
	char name[256];
	char content[MAX_FILE_SIZE];

	load_filesystem();

	while (1)
		{
			printf("%s>", currentPath);
			fgets(command, sizeof(command), stdin);
			command[strcspn(command, "\n")] = 0;

			if (strcmp(command, "exit") == 0)

				{
					my_exitsys();
					break;
				}
			else if (strcmp(command, "format") == 0)

				{
					my_format();
				}
			else if (strncmp(command, "mkdir ", 6) == 0)
				{
					sscanf(command + 6, "%s", name);
					my_mkdir(name);
				}
			else if (strncmp(command, "rmdir ", 6) == 0)
				{
					sscanf(command + 6, "%s", name);
					my_rmdir(name);
				}
			else if (strcmp(command, "ls") == 0)
				{
					my_ls();
				}
			else if (strncmp(command, "cd ", 3) == 0)
				{
					sscanf(command + 3, "%s", name);
					my_cd(name);
				}
			else if (strncmp(command, "create ", 7) == 0)
				{
					sscanf(command + 7, "%s", name);
					my_create(name);
				}
			else if (strncmp(command, "open ", 5) == 0)
				{
					sscanf(command + 5, "%s", name);
					my_open(name);
				}
			else if (strncmp(command, "close ", 6) == 0)
				{
					sscanf(command + 6, "%s", name);
					my_close(name);
				}
			else if (strncmp(command, "write ", 6) == 0)
				{
					sscanf(command + 6, "%s", name);
					printf("Enter content: ");
					fgets(content, sizeof(content), stdin);
					content[strcspn(content, "\n")] = 0;
					my_write(name, content);
				}
			else if (strncmp(command, "read ", 5) == 0)
				{
					sscanf(command + 5, "%s", name);
					my_read(name);
				}
			else if (strncmp(command, "rm ", 3) == 0)
				{
					sscanf(command + 3, "%s", name);
					my_rm(name);
				}
			else
				{
					printf("Unknown command.\n");
				}
		}

	return 0;
}