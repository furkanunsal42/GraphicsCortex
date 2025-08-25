#pragma once

#include <iostream>
#include <complex>

#define MAX 2048
constexpr int32_t N = 2048;
constexpr float M_PI = 3.14159265358979323846;

using namespace std;

int log2(int N)    //funzione per calcolare il logaritmo in base 2 di un intero
{
    int k = N, i = 0;
    while (k) {
        k >>= 1;
        i++;
    }
    return i - 1;
}

int check(int n)    //usato per controllare se il numero di componenti del vettore di input è una potenza di 2
{
    return n > 0 && (n & (n - 1)) == 0;
}

int reverse(int N, int n)    //calcola il reverse number di ogni intero n rispetto al numero massimo N
{
    int j, p = 0;
    for (j = 1; j <= log2(N); j++) {
        if (n & (1 << (log2(N) - j)))
            p |= 1 << (j - 1);
    }
    return p;
}

void ordina(complex<double>* f1, int N)     //dispone gli elementi del vettore ordinandoli per reverse order
{
    complex<double> f2[MAX];
    for (int i = 0; i < N; i++)
        f2[i] = f1[reverse(N, i)];
    for (int j = 0; j < N; j++)
        f1[j] = f2[j];
}

void transform(complex<double>* f)     //calcola il vettore trasformato
{
   

    ordina(f, N);    //dapprima lo ordina col reverse order
    complex<double> W[N / 2]; //vettore degli zeri dell'unità.
    //Prima N/2-1 ma genera errore con ciclo for successivo
   //in quanto prova a copiare in una zona non allocata "W[N/2-1]"
    W[1] = polar(1., -2. * M_PI / N);
    W[0] = 1;
    for (int i = 2; i < N / 2; i++)
        W[i] = pow(W[1], i);
    int n = 1;
    int a = N / 2;
    for (int j = 0; j < log2(N); j++) {
        for (int i = 0; i < N; i++) {
            if (!(i & n)) {
                /*ad ogni step di raddoppiamento di n, vengono utilizzati gli indici */
                /*'i' presi alternativamente a gruppetti di n, una volta si e una no.*/
                complex<double> temp = f[i];
                complex<double> Temp = W[(i * a) % (n * a)] * f[i + n];
                f[i] = temp + Temp;
                f[i + n] = temp - Temp;
            }
        }
        n *= 2;
        a = a / 2;
    }
}

void FFT(complex<double>* f, double d)
{
    transform(f);
    for (int i = 0; i < N; i++)
        f[i] *= d; //moltiplica il vettore per il passo in modo da avere il vettore trasformato effettivo
}

int main()
{
    int n;
    do {
        cout << "vektörün boyutunu belirtir, bu da 2'nin bir kuvvetidir" << endl;
        cin >> n;
    } while (!check(n));
    double d;
    cout << "örnekleme ad?m boyutunu girin" << endl;
    cin >> d;
    cout << "örnekleme ad?m? =" << d << endl;
    complex<double> vec[MAX];
    cout << "örnekleme vektörünü girin" << endl;
    for (int i = 0; i < n; i++) {
        cout << "dizin bile?enini ekle " << i << endl;
        cin >> vec[i];
        cout << "dizin " << i << "= " << vec[i] << endl;
    }
    FFT(vec, d);
    cout << "dönü?türülmü? vektör" << endl;
    for (int j = 0; j < n; j++)
        cout << vec[j] << endl;
    return 0;
}