#pragma once
#include "BDXKEngine/Framework/Core/Component.h"

namespace BDXKEngine
{
    class Behavior;

    class StartHandler
    {
    public:
        virtual ~StartHandler() = default;
    private:
        friend Behavior;
    protected:
        virtual void OnStart() = 0;
    };

    class UpdateHandler
    {
    public:
        virtual ~UpdateHandler() = default;
    private:
        friend Behavior;
    protected:
        virtual void OnUpdate() = 0;
    };

    class LateUpdateHandler
    {
    public:
        virtual ~LateUpdateHandler() = default;
    private:
        friend Behavior;
    protected:
        virtual void OnLateUpdate() = 0;
    };

    class Behavior : public Component
    {
    public:
        static void DebugHandlersCount();
        static void Start();
        static void Update();
        static void LateUpdate();

    protected:
        void Enable() override;
        void Disable() override;
        void Awake() override;
    private:
        //保存所有Behavior的事件注册信息
        //遍历容器时删除物体会导致出现野指针，为保证遍历继续正常运行，使用bool标记目标指针是否可用
        static std::unordered_map<StartHandler*, bool> allStartHandlers;
        static std::unordered_map<UpdateHandler*, bool> allUpdateHandlers;
        static std::unordered_map<LateUpdateHandler*, bool> allLateUpdateHandlers;

        StartHandler* startHandler = nullptr;
        UpdateHandler* updateHandler = nullptr;
        LateUpdateHandler* lateUpdateHandler = nullptr;
    };
}
