#include <stdio.h>
#include <stdint.h>

#define STACK_SIZE 256
int stack[STACK_SIZE];
int sp = 0;

void push(int val) { if (sp < STACK_SIZE) stack[sp++] = val; }
int pop() { return (sp > 0) ? stack[--sp] : 0; }

void interpret(const char *src) {
    for (int i = 0; src[i]; i++) {
        switch (src[i]) {
            case '+': push(pop() + pop()); break;
            case '-': { int b = pop(); push(pop() - b); break; }
            case '*': push(pop() * pop()); break;
            case '/': { int b = pop(); push(pop() / b); break; }
            case '0'...'9': push(src[i] - '0'); break;
            case 'P': printf("= %d\n", pop()); break;
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) return 1;
    interpret(argv[1]); // Ví dụ: "./elaina_interp 23+P" -> 2+3 = 5
    return 0;
}
