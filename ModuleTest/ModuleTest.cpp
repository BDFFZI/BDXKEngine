#include <cstdio>
#include <iostream>
#include <filesystem>
#include "BDXKEngine/Base/Object/ObjectSerializer.h"
#include "BDXKEngine/Base/Object/Core/Object.h"
#include "BDXKEngine/Platform/Window/Window.h"

using namespace BDXKEngine;

int main()
{
    std::setlocale(LC_ALL, "zh-CN.UTF-8");

    {
        Window window = {L"鸡汤来喽"};
        window.Show();
    }

    auto b = Object::GetAllObjects();
    ObjectPtrBase::PrintRefCountMap();

    getchar();
}
