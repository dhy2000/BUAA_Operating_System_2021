

int _my_getchar();

int _my_putchar(char ch);

int _my_exit();

int rd() {
    int ret = 0;
    char c = _my_getchar();
    while (c != '\n' && !(c >= '0' && c <= '9'))
        c = _my_getchar();

    if (c == '\n')
        return ret;
    while (c >= '0' && c <= '9')
        ret = (ret << 3) + (ret << 1) + (c - '0'),
            c = _my_getchar();
    return ret;
}

void wt(int n) {
    char buf[32];
    int bufsz = 0;
    while (n > 0) {
        buf[bufsz] = (n % 10) + '0';
        n /= 10;
        bufsz++;
    }
    buf[bufsz] = 0;
    if (bufsz == 0) buf[0] = '0', bufsz = 1;
    while (bufsz > 0) {
        _my_putchar(buf[bufsz - 1]);
        bufsz--;
    }
}

void my_cal() {
    int a = rd();
    int b = rd();
    int c = a + b;
    wt(c);
    // _my_putchar('c');
    // _my_putchar('\n');
}

