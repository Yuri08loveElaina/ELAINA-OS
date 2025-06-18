void main() {
    char *video = (char*)0xb8000;
    const char *msg = "ElainaOS v0.1 Ready\n";
    for (int i = 0; msg[i]; i++) {
        video[i * 2] = msg[i];
        video[i * 2 + 1] = 0x07;
    }
    while (1) asm volatile("hlt");
}
