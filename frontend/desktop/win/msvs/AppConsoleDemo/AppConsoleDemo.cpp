// AppConsoleDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#include "AppStroke.h"

using namespace std::chrono;

int main()
{
    std::cout << "Demo: Approximating a square-shaped stroke." << std::endl << std::endl;

    // Define a 5-points long stroke in the shape of a square; last vertex coincides with the first one.
    float d = 300.f;  // side length

    float quadStroke[10] = 
    {
                   //    vertexes  
        0.f, 0.f,  // (left,  upper)
        d,   0.f,  // (right, upper)
        d,   d,    // (right, down)
        0.f, d,    // (left,  down)
        0.f, 0.f   // (left,  upper)
    };

    AppStroke AppStrk;

    int Dim   = 2;    // 2-dimensional stroke  
    int Sam   = 5;    // number of original sampling points (vertexes)
    int ReSam = 32;   // Number of re-sampling points for approximation
    int Ord   = 11;    // Approximation order 

    std::cout << "Approximation parameters: " << std::endl;
    std::cout << "ReSam = " << ReSam << std::endl;
    std::cout << "Ord = " << Ord << std::endl << std::endl;

    AppStrk.Init(Dim, Sam, Ord, ReSam, quadStroke);

    std::cout << "Initial Approximation Error: " << AppStrk.GetErr() << std::endl;

    int  nItr = 64;

    auto start = high_resolution_clock::now();

    AppStrk.ParamApp(nItr);
    
    auto stop  = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    float Err = AppStrk.GetErr();

    std::cout << "Approximation Error after " << nItr << " iterations: " << Err << std::endl;
    std::cout << "Approximation time: " << duration.count() << " microseconds" << std::endl;

    std::cout << std::endl << "Press <Enter> to exit..." << std::endl;
    std::cin.get();
}
