// AppConsoleDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <chrono>

#include "cplusplus\AppStroke.h"
#include "c\AppStroke.h"

using namespace std::chrono;

int main()
{
    std::cout << "Demo: Approximating a square-shaped stroke." << std::endl << std::endl;

    // Define a 5-points long stroke in the shape of a square; last vertex coincides with the first one.
    double d = 300.f;  // side length

    double quadStroke[10] = 
    {
                   //    vertexes  
        0.f, 0.f,  // (left,  upper)
        d,   0.f,  // (right, upper)
        d,   d,    // (right, down)
        0.f, d,    // (left,  down)
        0.f, 0.f   // (left,  upper)
    };

    AppStroke<2> AppStrk;

    int Dim   = 2;    // 2-dimensional stroke  
    int Sam   = 5;    // number of original sampling points (vertexes)
    int ReSam = 64;   // Number of re-sampling points for approximation
    int Ord   = 11;   // Approximation order 

    std::cout << "Approximation parameters: " << std::endl;
    std::cout << "ReSam = " << ReSam << std::endl;
    std::cout << "Ord = " << Ord << std::endl << std::endl;

    std::cout << std::endl << "========= Using C++ Library ===========" << std::endl;

    AppStrk.Init(Ord, ReSam, Sam, quadStroke);


    std::cout << "Initial Approximation Error: " << AppStrk.GetRMSErr() << std::endl;

    int  nItr = 32;

    auto start = high_resolution_clock::now();

    AppStrk.ParamApp(nItr);
    
    auto stop  = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    double Err = AppStrk.GetRMSErr();

    std::cout << "Approximation Error after " << nItr << " iterations: " << Err << std::endl;
    std::cout << "Approximation time: " << duration.count() << " microseconds" << std::endl;

    std::cout << std::endl << "========= Using C Library ===========" << std::endl;

    FLOAT dc = 300.0;
    FLOAT quadStrokeC[10] =
    {
        //    vertexes  
        0.0,  0.0,  // (left,  upper)
        dc,   0.0,  // (right, upper)
        dc,   dc,   // (right, down)
        0.0,  dc,   // (left,  down)
        0.0,  0.0   // (left,  upper)
    };

    ERR_CODE err_code = err_code_OK;

    hAPPSTROKE hAppStrk = AppStroke_Create();

    err_code = AppStroke_Init(hAppStrk, Dim, Ord, ReSam, Sam, quadStrokeC);
    if(err_code != err_code_OK)
    {
        std::cout << "AppStroke_ZeroInit Failed! Error code: " << err_code << std::endl;
        std::cout << std::endl << "Press <Enter> to exit..." << std::endl;
        std::cin.get();
        return (int)err_code;
    }

    std::cout << "Initial Approximation Error C: " << AppStroke_GetRSMError(hAppStrk) << std::endl;

    auto startC = high_resolution_clock::now();

    err_code = AppStroke_ParamApp(hAppStrk, nItr, -1.0, NULL);
    if (err_code != err_code_OK)
    {
        std::cout << "AppStroke_ParamApp Failed! Error code: " << err_code << std::endl;
        std::cout << std::endl << "Press <Enter> to exit..." << std::endl;
        std::cin.get();
        return (int)err_code;
    }

    auto stopC = high_resolution_clock::now();
    auto durationC = duration_cast<microseconds>(stopC - startC);

    std::cout << "Approximation Error after " << nItr << " iterations: " << AppStroke_GetRSMError(hAppStrk) << std::endl;
    std::cout << "Approximation time: " << durationC.count() << " microseconds" << std::endl;

    AppStroke_Delete(hAppStrk);

    std::cout << std::endl << "Press <Enter> to exit..." << std::endl;
    std::cin.get();

    return 0;
}
