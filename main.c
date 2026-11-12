#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"
#include "log.h"
#include "common.h"

typedef enum {
    ERROR,
    GUESS,
    DESCRIPTION,
    SAVE,
    LOAD,
    EXIT
} userChoice_t;

userChoice_t getUserChoice();

int findAndPrint(node_t *node, const char *name);

void akinatorGuess(node_t *root);
void akinatorDescription(node_t *root);
void akinatorSave(node_t *root);
void akinatorLoad(node_t **root);

int main() {
    logInit("log.htm");
    log("<pre>");

    node_t *root = treeRead("bd.tree");

    treeDump(root);
    log("start akinator");

    bool runned = true;
    while (runned) {
        userChoice_t choice = getUserChoice();

        switch (choice) {
            case GUESS:        
                akinatorGuess(root);
                break;
            case DESCRIPTION:
                akinatorDescription(root);
                break;
            case SAVE:
                akinatorSave(root);
                break;
            case LOAD:
                akinatorLoad(&root);
                break;
            case EXIT:
                runned = false;
                break;
            case ERROR: 
                printf("smth went wrong...\n"); 
                break;
            default: 
                printf("WOW! new choice!\n");
                break;
        }

        treeDump(root);
    }

    printf("Bye bye!\n");

    treeDestroy(root);
    logDestroy();
}

userChoice_t getUserChoice() {
    printf("=====Akinator ver 0.1=====\n");
    printf("What do you want to do?\n");
    printf("1) Guess    2) Descritption\n");
    printf("3) Save     4) Load\n");
    printf("5) Exit\n");
    printf(">>> ");

    int ans = 0;
    while (scanf("%d", &ans) == 1 && !(1 <= ans && ans <= 5)) {
        printf("Stupid leather bag! Enter better!\n");
        printf(">>> ");
    }

    clearInputBuffer();

    switch (ans) {
        case 1:  return GUESS;
        case 2:  return DESCRIPTION;
        case 3:  return SAVE;
        case 4:  return LOAD;
        case 5:  return EXIT;
        default: return ERROR;
    }

    return ERROR;
}

void akinatorGuess(node_t *root) {
    assert(root);

    node_t *node = root;
    while (node) {
        printf("%s?\n", node->data);
        printf("[Y/n] ");
        
        int ans = getchar();
        if (ans != '\n') clearInputBuffer();

        if (ans == 'n') {
            if (node->right) {
                node = node->right;
            } else {
                printf(":( I don't know... What was that?\n>>> ");
                
                char correct_answer[MAX_NODE_LEN + 1] = {};
                while (scanf("%" STR(MAX_NODE_LEN) "[^\n]", correct_answer) != 1) {
                    clearInputBuffer();
                    printf("smth went wrong, repeat please\n>>> ");
                }
                clearInputBuffer();
                
                printf("What is the difference? %s...\n>>> ", correct_answer);

                char question[MAX_NODE_LEN + 1] = {};
                while (scanf("%" STR(MAX_NODE_LEN) "[^\n]", question) != 1) {
                    clearInputBuffer();
                    printf("smth went wrong, repeat please\n>>> ");
                }
                clearInputBuffer();

                node_t *left  = (node_t*)calloc(1, sizeof(node_t));
                node_t *right = (node_t*)calloc(1, sizeof(node_t));

                if (!(left && right)) {
                    logError("Can't calloc at main for left/right");
                    return;
                }
 
                *right = *node;

                left->data  = strdup(correct_answer);
                node->data  = strdup(question);

                node->left  = left;
                node->right = right;

                return;
            }
        }
        else {
            if (node->left) {
                node = node->left;
            } else {
                printf("This was: %s. I'm very clever!\n", node->data);
                return;
            }
        }
    }
}

int findAndPrint(node_t *node, const char *name) {
    assert(name);

    if (!node) return 0;

    if (!strcmp(node->data, name)) {
        printf("%s: ", node->data);
        return 1;
    }

    int level = 0;

    if ((level = findAndPrint(node->left, name))) {
        if (level == 1) printf("%s",   node->data);
        else            printf(", %s", node->data);
        return level + 1;
    }

    if ((level = findAndPrint(node->right, name))) {
        if (level == 1) printf("не %s",   node->data);
        else            printf(", не %s", node->data);
        return level + 1;
    }

    return 0;
}

void akinatorDescription(node_t *root) {
    printf("Enter a name, if I know it, I will find and say what it is\n>>> ");
    char name[101] = {};
    while (scanf("%s", name) != 1) {
        clearInputBuffer();
        printf("smth went wrong, repeat please\n>>> ");
    }
    clearInputBuffer();

    if (!findAndPrint(root, name)) {
        printf("I don't know\n");
    } else {
        printf(".\n");
    }
}

void akinatorSave(node_t *root) {
    assert(root);

    printf("Enter a filename to save\n>>> ");
    char filename[101] = {};
    while (scanf("%s", filename) != 1) {
        clearInputBuffer();
        printf("smth went wrong, repeat please\n>>> ");
    }
    clearInputBuffer();

    treeWrite(root, filename);
}

void akinatorLoad(node_t **root) {
    assert(root);
    assert(*root);

    node_t *new_root = NULL;
    while (!new_root) {
        printf("Enter a filename to load\n>>> ");
        char filename[101] = {};
        while (scanf("%s", filename) != 1) {
            clearInputBuffer();
            printf("smth went wrong, repeat please\n>>> ");
        }
        clearInputBuffer();
        
        new_root = treeRead(filename);

        if (!new_root) printf("unknown file\n");
    }

    treeDestroy(*root);
    *root = new_root;
}
