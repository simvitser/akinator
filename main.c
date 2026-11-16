#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "tree.h"
#include "log.h"
#include "common.h"
#include "stack.h"

typedef enum {
    ERROR,
    GUESS,
    DESCRIPTION,
    SAVE,
    LOAD,
    DIFF,
    EXIT
} userChoice_t;

userChoice_t getUserChoice();

bool findAndStack(node_t *node, const char *name, stack_t *stack);

void akinatorGuess(node_t *root);
void akinatorDescription(node_t *root);
void akinatorSave(node_t *root);
void akinatorLoad(node_t **root);
void akinatorDiff(node_t *root);

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
            case DIFF:
                akinatorDiff(root);
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
    printf("1) Guess      2) Descritption\n");
    printf("3) Save       4) Load\n");
    printf("5) Difference 6) Exit\n");
    printf(">>> ");

    SAY("Привет!");

    int ans = 0;
    while (scanf("%d", &ans) == 1 && !(1 <= ans && ans <= 6)) {
        printf("Stupid leather bag! Enter better!\n");
        printf(">>> ");
    }

    clearInputBuffer();

    switch (ans) {
        case 1:  return GUESS;
        case 2:  return DESCRIPTION;
        case 3:  return SAVE;
        case 4:  return LOAD;
        case 5:  return DIFF;
        case 6:  return EXIT;
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
        say("%s?\n", node->data);
        
        int ans = getchar();
        if (ans != '\n') clearInputBuffer();

        if (ans == 'n') {
            if (node->right) {
                node = node->right;
            } else {
                printf(":( I don't know... What was that?\n>>> ");
                SAY("Я не знаю что это такое");
                
                char correct_answer[MAX_NODE_LEN + 1] = {};
                while (scanf("%" STR(MAX_NODE_LEN) "[^\n]", correct_answer) != 1) {
                    clearInputBuffer();
                    printf("smth went wrong, repeat please\n>>> ");
                }
                clearInputBuffer();
                
                printf("What is the difference? %s...\n>>> ", correct_answer);
                SAY("А чем они отличаются?");

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
        } else if (ans == 'Y' || ans == 'y' || ans == '\n') {
            if (node->left) {
                node = node->left;
            } else {
                printf("This was: %s. I'm very clever!\n", node->data);
                say("А я угадал! это %s", node->data);
                return;
            }
        }
    }
}

bool findAndStack(node_t *node, const char *name, stack_t *stack) {
    assert(name);

    if (!node) return false;

    char buffer[MAX_NODE_LEN + 1] = {};

    if (!strcmp(node->data, name)) {
        return true;
    }


    if (findAndStack(node->left, name, stack)) {
        sprintf(buffer, "%s", node->data);

        stackPush(stack, strdup(buffer));

        return true;
    }

    if (findAndStack(node->right, name, stack)) {
        sprintf(buffer, "не %s", node->data);

        stackPush(stack, strdup(buffer));

        return true;
    }

    return false;
}

void akinatorDescription(node_t *root) {
    printf("Enter a name, if I know it, I will find and say what it is\n>>> ");
    char name[MAX_NODE_LEN + 1] = {};
    while (scanf("%" STR(MAX_NODE_LEN) "s", name) != 1) {
        clearInputBuffer();
        printf("smth went wrong, repeat please\n>>> ");
    }
    clearInputBuffer();

    stack_t stack = {};
    stackCtor(&stack);

    if (!findAndStack(root, name, &stack)) {
        printf("I don't know who is %s\n", name);
    } else {
        printf("%s is: ", name);
        char *s = NULL;
        while (getStackSize(&stack) != 1) {
            s = stackPop(&stack);
            printf("%s, ", s);
            free(s);
        }
        printf("%s.\n", (s = stackPop(&stack)));
        free(s);
    }

    stackDtor(&stack);
}

void akinatorSave(node_t *root) {
    assert(root);

    printf("Enter a filename to save\n>>> ");
    char filename[MAX_NODE_LEN + 1] = {};
    while (scanf("%" STR(MAX_NODE_LEN) "s", filename) != 1) {
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
        char filename[MAX_NODE_LEN + 1] = {};
        while (scanf("%" STR(MAX_NODE_LEN) "s", filename) != 1) {
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

void akinatorDiff(node_t *root) {
    assert(root);

    printf("Enter a first name, if I know it, I will find and say what it is\n>>> ");
    char name1[MAX_NODE_LEN + 1] = {};
    while (scanf("%" STR(MAX_NODE_LEN) "s", name1) != 1) {
        clearInputBuffer();
        printf("smth went wrong, repeat please\n>>> ");
    }
    clearInputBuffer();

    stack_t stack1 = {};
    stack_t stack2 = {};
    stackCtor(&stack1);

    if (!findAndStack(root, name1, &stack1)) {
        printf("I don't know who is %s\n", name1);

        stackDtor(&stack1);

        return;
    } else {
        printf("Enter a second name, if I know it, I will find and say what it is\n>>> ");
        char name2[MAX_NODE_LEN + 1] = {};
        while (scanf("%" STR(MAX_NODE_LEN) "s", name2) != 1) {
            clearInputBuffer();
            printf("smth went wrong, repeat please\n>>> ");
        }
        clearInputBuffer();

        stackCtor(&stack2);

        if (!findAndStack(root, name2, &stack2)) {
            printf("I don't know who is %s\n", name2);
        } else {
            printf("%s and %s are same in: ", name1, name2);

            float k = 0;
            float max_size = (float)MAX(getStackSize(&stack1), getStackSize(&stack2));

            while (getStackSize(&stack1) && getStackSize(&stack2)) {
                char *s1 = stackPop(&stack1);
                char *s2 = stackPop(&stack2);

                if (!strcmp(s1, s2)) {
                    k++;
                    printf("%s, ", s1);
                    free(s1);
                    free(s2);
                } else {
                    printf("но %s is %s", name1, s1);
                    free(s1);
                    while (getStackSize(&stack1)) {
                        s1 = stackPop(&stack1);
                        printf(", %s", s1);
                        free(s1);
                    }
                    printf(", а %s is %s", name2, s2);
                    free(s2);
                    while (getStackSize(&stack2)) {
                        s2 = stackPop(&stack2);
                        printf(", %s", s2);
                        free(s2);
                    }
                    break;
                }
            }

            printf("\n%s на %f%% %s\n", name1, k * 100 / max_size, name2);
        }
    }

    stackDtor(&stack1);
    stackDtor(&stack2);
}
