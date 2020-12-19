struct Cp {
    double Re, Im;

    inline Cp operator+(Cp const &_) const {
        return {Re + _.Re, Im + _.Im};
    }

    inline Cp operator-(Cp const &_) const {
        return {Re - _.Re, Im - _.Im};
    }

    inline Cp operator*(Cp const &_) const {
        return {Re * _.Re - Im * _.Im, Re * _.Im + Im * _.Re};
    }
};
void DFT(Cp *a, int n) {
    static Cp w[M];
    for (int i = 0, j = 0; i < n; ++i) {
        if (i < j)
            swap(a[i], a[j]);
        for (int k = n >> 1; (j ^= k) < k; k >>= 1);
    }
    w[0] = {1, 0};
    for (int i = 1; i < n; i <<= 1) {
        Cp wn = (Cp) {cos(pi / i), sin(pi / i)};
        for (int j = i - 2; j >= 0; j -= 2) {
            w[j] = w[j >> 1];
            w[j + 1] = wn * w[j];
        }
        for (int j = 0; j < n; j += i << 1) {
            Cp *p = a + j, *q = a + j + i;
            for (int k = 0; k < i; ++k) {
                Cp x = q[k] * w[k];
                q[k] = p[k] - x;
                p[k] = p[k] + x;
            }
        }
    }
}

void multiply(int *A, int *B, int *C, int n, int m，int mod) {
    static Cp a[M], b[M], c[M], d[M], w[M];
    for (int i = 0; i < n; ++i)
        a[i] = {(double) (A[i] & 32767), (double) (A[i] >> 15)};
    for (int i = 0; i < m; ++i)
        b[i] = {(double) (B[i] & 32767), (double) (B[i] >> 15)};
    int l = get_length(m + n - 1);
    DFT(a, l);
    DFT(b, l);
    for (int i = 0; i < l; ++i) {
        int j = (l - 1) & (l - i);
        c[j] = (Cp) {0.5 * (a[i].Re + a[j].Re), 0.5 * (a[i].Im - a[j].Im)} * b[i];
        d[j] = (Cp) {0.5 * (a[j].Im + a[i].Im), 0.5 * (a[j].Re - a[i].Re)} * b[i];
    }
    DFT(c, l);
    DFT(d, l);
    double inv = 1.0 / l;
    for (int i = 0; i < n + m - 1; ++i) {
        long long u = c[i].Re * inv + 0.5, v = c[i].Im * inv + 0.5;
        long long x = d[i].Re * inv + 0.5, y = d[i].Im * inv + 0.5;
        a[i] = b[i] = c[i] = d[i] = (Cp) {0, 0};
        C[i] = (u + ((v + x) << 15) + (y % mod << 30)) % mod;
    }
}