#include <fstream>
#include <xaudio2.h>
#include <atlbase.h>
#include <iostream>

#include "BDXKEngine/BDXKEngine.h"


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "zh-CN.UTF-8");

    BDXKEngine::Run();
}
