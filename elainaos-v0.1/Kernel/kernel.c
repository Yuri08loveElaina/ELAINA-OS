void print(const char* str) {
    char* video = (char*)0xB8000;
    int i = 0;
    while (str[i]) {
        video[i * 2] = str[i];
        video[i * 2 + 1] = 0x07;
        i++;
    }
}

void kernel_main() {
    print("Welcome to ElainaOS v0.1");
    while (1);
}
