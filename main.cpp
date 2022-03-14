#include <iostream>
#include <fstream>
#include <random>
#include <vector>
#include <ctime>
#include <chrono>
#include <algorithm>
using namespace std;
ifstream fin("sortari.in");
void generate_numbers(vector<long long> &a, long long N, long long Max)
{
    unsigned long long x;

    for(int i = 0; i < N; i++)
    {
        int num = rand()% 19;
        x = 0;

        do
            x = rand() % 10;
        while(x == 0);

        for(int j = 1; j < num; j++)
            x = (x * 10 + rand() % 10) % Max;

        a[i] = x;
    }
}
long long maxim(vector<long long> &a, long long N)
{
    int Max = a[0];

    for (int i = 1; i < N; i++)
        if (a[i] > Max)
            Max = a[i];

    return Max;
}

void countsort_radix(vector<long long> &a, int N, int exp, int base)
{
    int nr[base] = {0};
    vector <long long> output(N);

    for (int i = 0; i < N; i++)
        nr[(a[i] / exp) % base]++;

    for (int i = 1; i < base; i++)
        nr[i] += nr[i - 1];

    for (int i = N - 1; i >= 0; i--)
    {
        int x = (a[i] / exp) % base;
        output[nr[x] - 1] = a[i];
        nr[x]--;
    }
    for(int i =0; i < N; i++)
        a[i] = output[i];
}

void radixsort(vector<long long> &a, long long N, int base)
{
    int m = maxim(a, N);

    for (int exp = 1; m / exp > 0; exp *= base)
        countsort_radix(a, N, exp, base);
}

void merge_sort(vector<long long> &a, vector<long long> &b, int st, int dr)
{
    if (st < dr)
    {
        int m = (st + dr) / 2;
        merge_sort(a, b, st, m);
        merge_sort(a, b, m+1, dr);

        int i = st; int j = m + 1; int k = 0;

        while (i <= m && j <= dr)
            if (a[i] < a[j])
                b[++k] = a[i++];
            else
                b[++k] = a[j++];

        while(i <= m)
            b[++k] = a[i++];

        while(j <= dr)
            b[++k] = a[j++];

        for(i = st, j = 1; i <= dr; i ++, j ++)
            a[i] = b[j];
    }


}
void shellsort(vector <long long> &a, long long N)
{
    for (int g = N/2; g > 0; g /= 2)
    {
        for (int i = g; i < N; i++)
        {
            int t = a[i];

            int j;
            for (j = i; j >= g && a[j - g] > t; j -= g)
                a[j] = a[j - g];

            a[j] = t;
        }
    }
}

void heapify(vector <long long> &a, int n, int i)
{
    int Max = i;
    int st = 2 * i + 1;
    int dr = 2 * i + 2;

    if(st < n && a[Max] < a[st])
        Max = st;

    if(dr < n && a[Max] < a[dr])
        Max = dr;

    if(Max != i)
    {
        swap(a[i], a[Max]);
        heapify(a, n, Max);
    }
}

void heapsort(vector <long long> &a, int N)
{
    for(int i = N / 2 - 1; i >= 0; i--)
        heapify(a, N, i);

    for(int i = N - 1; i > 0; i--)
    {
        swap(a[0], a[i]);
        heapify(a, i, 0);
    }
}

void bubblesort(vector <long long> &a, int N)
{
    for(int i = 0; i < N-1; i++)
    {
        int s = 0;
        for(int j = 0; j < N-i-1; j++)
            if(a[j] > a[i])
            {
                swap(a[i], a[j]);
                s = 1;
            }
        if(s == 0)
            break;
    }
}

void print(vector <long long> &a, long long N)
{
    for (int i = 0; i < N; i++)
        cout << a[i] << " ";
    cout<<'\n';
}

bool test_sort(vector <long long> &a, long long N)
{
    for(int i = 0; i < N-1; i++)
        if(a[i] > a[i+1])
            return false;
    return true;
}

int main()
{
    long long nr_test, N, Max;
    fin>>nr_test;

    vector <long long> a;
    vector <long long> a1;
    vector <long long> b;
    for(int i = 0; i <= 10000000; i++)
    {
        a.push_back(0);
        b.push_back(0);
    }

    std::srand(std::time(nullptr));
    for(int l = 1; l <= nr_test; l++)
    {
        fin >> N >> Max;
        cout << "Test: " << l << ' ' << "Numarul de elemente: " << N << " Valoarea maxima: " << Max <<'\n';
        generate_numbers(a, N, Max);
        cout<<'\n';
        a1 = a;
        vector <long long> c(N);
        for(int j = 0; j < N; j++)
            c[j] = a[j];

        cout << "Functia sort -------- Corect -------- timp: ";
        auto begin = chrono::steady_clock::now();
        sort(c.begin(), c.end());
        auto end = chrono::steady_clock::now();
        auto eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        cout << eta << " microsecunde. \n";

        a = a1;
        cout << "MergeSort ----------- ";
        b.assign(N, 0);
        begin = chrono::steady_clock::now();
        merge_sort(a, b, 0, N-1);
        end = chrono::steady_clock::now();
        eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        if(test_sort(a, N) == false)
            cout << "Gresit";
        else
            cout << "Corect -------- timp: " << eta << " microsecunde. \n";

        a = a1;
        cout << "ShellSort ----------- ";
        begin = chrono::steady_clock::now();
        shellsort(a, N);
        end = chrono::steady_clock::now();
        eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        if(test_sort(a, N) == false)
            cout << "Gresit";
        else
            cout << "Corect -------- timp: " << eta << " microsecunde. \n";

        a = a1;
        cout << "RadixSort(baza 10) -- ";
        begin = chrono::steady_clock::now();
        radixsort(a, N, 10);
        end = chrono::steady_clock::now();
        eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        if(test_sort(a, N) == false)
            cout << "Gresit";
        else
            cout << "Corect -------- timp: " << eta << " microsecunde. \n";

        a = a1;
        cout << "RadixSort(baza 128) - ";
        begin = chrono::steady_clock::now();
        radixsort(a, N, 128);
        end = chrono::steady_clock::now();
        eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        if(test_sort(a, N) == false)
            cout << "Gresit";
        else
            cout << "Corect -------- timp: " << eta << " microsecunde. \n";

        a = a1;
        cout << "HeapSort ------------ ";
        begin = chrono::steady_clock::now();
        heapsort(a, N);
        end = chrono::steady_clock::now();
        eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        if(test_sort(a, N) == false)
            cout << "Gresit";
        else
            cout << "Corect -------- timp: " << eta << " microsecunde. \n";

        a = a1;
        cout << "BubbleSort ---------- ";
        begin = chrono::steady_clock::now();
        heapsort(a, N);
        end = chrono::steady_clock::now();
        eta = chrono::duration_cast<chrono::microseconds>(end - begin).count();
        if(test_sort(a, N) == false)
            cout << "Gresit";
        else
            cout << "Corect -------- timp: " << eta << " microsecunde. \n\n";
    }
    return 0;
}
