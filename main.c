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

void akinator_guess(node_t *root);
void akinator_description(node_t *root);
void akinator_save(node_t *root);
void akinator_load(node_t **root);

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
                akinator_guess(root);
                break;
            case DESCRIPTION:
                akinator_description(root);
                break;
            case SAVE:
                akinator_save(root);
                break;
            case LOAD:
                akinator_load(&root);
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

void akinator_guess(node_t *root) {
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

void akinator_description(node_t *root) {}

void akinator_save(node_t *root) {
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

void akinator_load(node_t **root) {
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
