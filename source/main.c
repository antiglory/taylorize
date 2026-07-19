#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int fatorial(int n)
{
    int y = 1;

    for (int i = 2; i <= n; i++)
        y *= i;

    return y;
}

int powi(int n, int e)
{
    int y = 1;

    for (int i = 0; i < e; i++)
        y *= n;

    return y;
}

struct Polynom
{
    int degree;
    int coef[32];
};

int f(int x, struct Polynom* p)
{
    int eval = 0;

    for (int i = 0; i <= p->degree; i++)
        eval += p->coef[i] * powi(x, i);

    return eval;
}

struct Polynom* dx(const struct Polynom* p)
{
    struct Polynom* d = (struct Polynom*)malloc(sizeof(struct Polynom));
    d->degree = p->degree - 1;

    for (int i = 1; i <= p->degree; i++)
        d->coef[i - 1] = p->coef[i] * i;

    return d;
}

// x is what (numeric value), a is where, k is how much
int taylorize(int x, int a, int k, const struct Polynom* p)
{
    int eval = 0;

    struct Polynom* d = malloc(sizeof *d);
    if (!d) return -1;

    *d = *p;

    for (int i = 0; i <= k && d->degree >= 0; i++)
    {
        eval += f(a, d) * powi(x - a, i) / fatorial(i);

        if (d->degree > 0)
        {
            struct Polynom* next = dx(d);
            free(d);

            d = next;
        }
        else
            break;
    }

    free(d);
    return eval;
}

struct Polynom p =
{
    .degree = 3,
    .coef = {5, 2, 7, 4}
};

int main(void)
{
    for (int i = 0; i < 1000000; i++)
    {
        system("cls");

        int y1 = f(i, &p);
        int y2 = taylorize(i, 1 /* it doesnt matter in polynomials */, p.degree, &p);

        printf("polynom: ");
        for (int i = p.degree; i > 0; i--)
            printf("%dx^%d + ", p.coef[i], i);

        printf("%d^0\n", p.coef[0]);

        printf("with x = %d:\n", i);
        printf("  og function: %d\n", y1);
        printf("  with taylor: %d\n", y2);
        printf("  accuracy: %d\%\n", y1 / y2 * 100);

        Sleep(100);
    }
}

// it overflows at 32 bits because i'm using int