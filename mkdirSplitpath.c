#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

//helper function
void reverse(char *str) {
        int left = 0;
        int right = strlen(str)-1;

        while (left < right) {
            char temp = str[left];
            str[left] = str[right];
            str[right] = temp;
            left++;
            right--;
        }
    }

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW

    if (strcmp(pathName, "/")==0 || strlen(pathName)==0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[128];
    char baseName[128];
    dirName[0] = '\0';
    baseName[0] = '\0';

    struct NODE *parent = splitPath(pathName, baseName, dirName);

    if (parent == NULL) return;

    struct NODE *child = parent->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName)==0) {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
            return;
        }
        child = child->siblingPtr;
    }

    struct NODE *newNode = malloc(sizeof(struct NODE));
    if (!newNode) {
        perror("malloc failed");
        return;
    }

    strncpy(newNode->name, baseName, 63);
    newNode->name[63] = '\0';
    newNode->fileType = 'D';
    newNode->siblingPtr = NULL;
    newNode->childPtr = NULL;
    newNode->parentPtr = parent;

    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE *sibling = parent->childPtr;
        while (sibling->siblingPtr != NULL) sibling = sibling->siblingPtr;
        sibling->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);

    return;
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE

    //edge case if only a slash
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        baseName[0] = '\0';
        return root;
    }

    char *lastSlash = strrchr(pathName, '/');
    if (lastSlash == NULL) {
        dirName[0] = '\0';
        strcpy(baseName, pathName);
        return root;
    }

    int dirLen = lastSlash - pathName;
    if (dirLen == 0) {
        strcpy(dirName, "/");
    } else {
        strncpy(dirName, pathName, dirLen);
        dirName[dirLen] = '\0';
    }

    strcpy(baseName, lastSlash+1);

    //naming part part done + edge cases

    //traverse path
    struct NODE *curr = root;
    if (strcmp(dirName, "/") == 0 || strlen(dirName) == 0) {
        return curr;
    }

    char dirCopy[128];
    strcpy(dirCopy, dirName);

    char *token2 = strtok(dirCopy, "/");
    while (token2 != NULL) {
        struct NODE *child = curr->childPtr;
        int found = 0;

        while (child != NULL) {
            if (strcmp(child->name, token2)==0 && child->fileType=='D') {
                curr = child;
                found = 1;
                break;
            }
            child=child->siblingPtr;

        }
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token2);
            return NULL;
        }
        token2 = strtok(NULL, "/");
    }

    return curr;
}
