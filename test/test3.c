void print(int);

void foo(int *w, int n) {
    int wmax = 2500000;
    int wcur = 0;
    int hax = 1;
    int haxbuf;
    int i;
    if(hax <= n) {
        haxbuf = 1;
        if(wcur > wmax) {
            wmax = wcur;
            i = 0;
            while(i < n) {
                int t19 = w[i];
                int t20 = hax / haxbuf;
                int t21 = t19 * t20;
                wcur = t21 + wcur;
                int t23 = wcur % 2;
                if(t23 == 0) {
                    haxbuf = haxbuf + 1;
                }
                else {
                    haxbuf = haxbuf + 2;
                }
                i = i + 1;
            }
        }
        else {
            int u = w[hax];
        }
        haxbuf = haxbuf + 1;
    }
    print(wmax);
    return;
}