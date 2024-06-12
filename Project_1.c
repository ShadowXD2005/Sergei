#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct File {
    char *name;
    char *content;
    struct File *next;
} File;

typedef struct Directory {
    char *name;
    struct Directory *parent;
    struct Directory *subDirs;
    struct File *files;
    struct Directory *next;
} Directory;

Directory* initializeFileSystem() {
    Directory *root = (Directory*)malloc(sizeof(Directory));
    root->name = strdup("/");
    root->parent = NULL;
    root->subDirs = NULL;
    root->files = NULL;
    root->next = NULL;
    return root;
}

void createFile(Directory *currentDir, const char *name, const char *content) {
    File *newFile = (File*)malloc(sizeof(File));
    newFile->name = strdup(name);
    newFile->content = strdup(content);
    newFile->next = currentDir->files;
    currentDir->files = newFile;
}

void createDirectory(Directory *currentDir, const char *name) {
    Directory *newDir = (Directory*)malloc(sizeof(Directory));
    newDir->name = strdup(name);
    newDir->parent = currentDir;
    newDir->subDirs = NULL;
    newDir->files = NULL;
    newDir->next = currentDir->subDirs;
    currentDir->subDirs = newDir;
}

void listContents(Directory *currentDir) {
    Directory *dir = currentDir->subDirs;
    File *file = currentDir->files;

    printf("Directories:\n");
    while (dir) {
        printf("  %s\n", dir->name);
        dir = dir->next;
    }

    printf("Files:\n");
    while (file) {
        printf("  %s\n", file->name);
        file = file->next;
    }
}

Directory* changeDirectory(Directory *currentDir, const char *name) {
    if (strcmp(name, "..") == 0) {
        return currentDir->parent ? currentDir->parent : currentDir;
    }

    Directory *dir = currentDir->subDirs;
    while (dir) {
        if (strcmp(dir->name, name) == 0) {
            return dir;
        }
        dir = dir->next;
    }

    printf("Directory not found!\n");
    return currentDir;
}

void deleteFile(Directory *currentDir, const char *name) {
    File *prevFile = NULL;
    File *currFile = currentDir->files;

    while (currFile) {
        if (strcmp(currFile->name, name) == 0) {
            if (prevFile) {
                prevFile->next = currFile->next;
            } else {
                currentDir->files = currFile->next;
            }
            free(currFile->name);
            free(currFile->content);
            free(currFile);
            printf("File %s deleted.\n", name);
            return;
        }
        prevFile = currFile;
        currFile = currFile->next;
    }

    printf("File %s not found!\n", name);
}

void PrintWorkingDirectory(Directory *currentDir) {
    if (currentDir == NULL) {
        return;
    }

    int depth = 0;
    Directory *temp = currentDir;
    while (temp != NULL) {
        depth++;
        temp = temp->parent;
    }

    char **path = (char **)malloc(depth * sizeof(char *));
    int i = depth - 1;

    temp = currentDir;
    while (temp != NULL) {
        path[i] = temp->name;
        i--;
        temp = temp->parent;
    }

    printf("C:/");
    for (i = 0; i < depth; i++) {
        if (i == 0 && strcmp(path[i], "/") == 0) {
            continue;  
        }
        if(i==1){
            printf("%s", path[i]);
        }
        else{
        printf("/%s", path[i]);
        }
    }
    printf("> ");

    free(path);
}

void deleteDirectory(Directory *currentDir, const char *name) {
    Directory *prevDir = NULL;
    Directory *currDir = currentDir->subDirs;

    while (currDir) {
        if (strcmp(currDir->name, name) == 0) {
            if (currDir->subDirs != NULL || currDir->files != NULL) {
                printf("Directory %s is not empty!\n", name);
                return;
            }

            if (prevDir) {
                prevDir->next = currDir->next;
            } else {
                currentDir->subDirs = currDir->next;
            }
            free(currDir->name);
            free(currDir);
            printf("Directory %s deleted.\n", name);
            return;
        }
        prevDir = currDir;
        currDir = currDir->next;
    }

    printf("Directory %s not found!\n", name);
}

void printHelp() {
    printf("Available commands:\n");
    printf("  mkdir <name>         - Create a new directory\n");
    printf("  touch <name> <content> - Create a new file with content\n");
    printf("  ls                   - List files and directories\n");
    printf("  cd <name>            - Change directory\n");
    printf("  rm <name>            - Delete a file\n");
    printf("  rmdir <name>         - Delete a directory\n");
    printf("  pwd                  - Print working directory\n");
    printf("  help                 - Show this help menu\n");
    printf("  exit                 - Exit the program\n");
}

int main() {
    Directory *root = initializeFileSystem();
    Directory *currentDir = root;
    char command[100], name[100], content[1000];

    printHelp();  

    while (1) {
        PrintWorkingDirectory(currentDir);  
        scanf("%s", command);

        if (strcmp(command, "mkdir") == 0) {
            scanf("%s", name);
            createDirectory(currentDir, name);
        } else if (strcmp(command, "touch") == 0) {
            scanf("%s", name);
            scanf(" %[^\n]s", content); 
            createFile(currentDir, name, content);
        } else if (strcmp(command, "ls") == 0) {
            listContents(currentDir);
        } else if (strcmp(command, "cd") == 0) {
            scanf("%s", name);
            currentDir = changeDirectory(currentDir, name);
        } else if (strcmp(command, "rm") == 0) {
            scanf("%s", name);
            deleteFile(currentDir, name);
        } else if (strcmp(command, "rmdir") == 0) {
            scanf("%s", name);
            deleteDirectory(currentDir, name);
        } else if (strcmp(command, "pwd") == 0) {
            PrintWorkingDirectory(currentDir);
        } else if (strcmp(command, "help") == 0) {
            printHelp();
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Invalid command! Type 'help' to see the list of available commands.\n");
        }
    }

    return 0;
}
