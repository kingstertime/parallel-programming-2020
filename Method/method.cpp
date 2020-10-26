//
// Created by Кирилл Королёв on 19.10.2020.
//

#include <iostream>
#include <omp.h>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <math.h>
using namespace std;

int main()
{
    double xMin = 0.0;
    double xMax = 1.0;
    const int N = 10;
    double h = (xMax - xMin) / N;
    double x[N + 1];
    double y[N + 1];
    double D[N + 1];
    double F[N + 1];
    for (int i = 0; i <= N; i++)
    {
        x[i] = h * i;
        D[i] = 1;
        F[i] = -sin(x[i]);
        y[i] = 0;
    }
    double A[N + 1];
    double B[N + 1];
    double C[N + 1];
    double FV[N + 1];
    for (int i = 0; i <= N; i++)
    {
        A[i] = 1;
        B[i] = 1;
        C[i] = 2;
        FV[i] = -((F[i] * h * h) / D[i]);
    }
    const double m1 = 0;
    const double w1 = 0;
    const double w2 = 0;
    const double m2 = sin(1);
    double alf[N + 1];
    double bet[N + 1];
    alf[N] = w2;
    bet[N] = m2;
    alf[0] = 0;
    bet[0] = 0;

    for (int i = N - 1; i >= 1; i--)
    {
        alf[i] = A[i] / (C[i] - alf[i+1] * B[i]);
        bet[i] = (B[i] * bet[i + 1] + FV[i]) / (C[i] - alf[i + 1] * B[i]);
    }
    y[0] = (m1+w1*bet[1])/(1-alf[1]*w1);
    
    for (int i = 0; i <= N - 1; i++)
    {
        y[i + 1] = alf[i + 1] * y[i] + bet[i+1];
    }
    for (int i = 0; i <= N ; i++)
    {
        std::cout << "x[" << i << "]=" << x[i] << " ";
        std::cout << "y[" << i << "]=" << y[i] << "\n";
    }
}