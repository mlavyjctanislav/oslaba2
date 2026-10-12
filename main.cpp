#include <windows.h>
#include <iostream>

int* g_arr = nullptr;
int   g_n = 0;
int   g_min = 0;
int   g_max = 0;
double g_avg = 0.0;

DWORD WINAPI MinMaxThread(LPVOID lpParam)
{
    g_min = g_arr[0];
    g_max = g_arr[0];

    for (int i = 1; i < g_n; ++i)
    {
        if (g_arr[i] < g_min)
            g_min = g_arr[i];
        Sleep(7);

        if (g_arr[i] > g_max)
            g_max = g_arr[i];
        Sleep(7);
    }

    std::cout << "Min = " << g_min
        << ", Max = " << g_max << std::endl;

    return 0;
}

DWORD WINAPI AverageThread(LPVOID lpParam)
{
    long long sum = 0;

    for (int i = 0; i < g_n; ++i)
    {
        sum += g_arr[i];
        Sleep(12);
    }

    g_avg = (double)sum / g_n;

    std::cout << " Average = " << g_avg << std::endl;

    return 0;
}

int main()
{
    std::cout << "Enter the size of the array: ";
    std::cin >> g_n;

    if (g_n <= 0) {
        std::cerr << "Error: size must be greater than 0." << std::endl;
        return 1;
    }

    g_arr = new int[g_n];
    std::cout << "Enter " << g_n << " integer values: ";
    for (int i = 0; i < g_n; ++i) {
        std::cin >> g_arr[i];
    }

    HANDLE hMinMax = CreateThread(NULL, 0, MinMaxThread, NULL, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, AverageThread, NULL, 0, NULL);

    if (hMinMax == NULL || hAverage == NULL) {
        std::cerr << "Error: failed to create a thread. Code: "
            << GetLastError() << std::endl;
        delete[] g_arr;
        return 1;
    }

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    int minIdx = 0, maxIdx = 0;
    for (int i = 1; i < g_n; ++i) {
        if (g_arr[i] < g_arr[minIdx])
            minIdx = i;
        if (g_arr[i] > g_arr[maxIdx])
            maxIdx = i;
    }

    int avgInt = (int)g_avg;
    g_arr[minIdx] = avgInt;
    g_arr[maxIdx] = avgInt;

    std::cout << "\nArray after replacing min and max with the average ("
        << g_avg << "):\n";
    for (int i = 0; i < g_n; ++i)
        std::cout << g_arr[i] << ' ';
    std::cout << std::endl;

    CloseHandle(hMinMax);
    CloseHandle(hAverage);
    delete[] g_arr;
    return 0;
}