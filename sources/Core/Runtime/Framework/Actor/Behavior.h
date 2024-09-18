#pragma once
#include "Component.h"

namespace BDXKEngine
{
    class Behavior;
    //状态事件
    class AwakeEvent
    {
        friend Behavior;

    public:
        virtual ~AwakeEvent() = default;

    protected:
        virtual void OnAwake() = 0;
    };
    class DestroyEvent
    {
        friend Behavior;

    public:
        virtual ~DestroyEvent() = default;

    protected:
        virtual void OnDestroy() = 0;
    };
    class EnableEvent
    {
        friend Behavior;

    public:
        virtual ~EnableEvent() = default;

    protected:
        virtual void OnEnable() = 0;
    };
    class DisableEvent
    {
        friend Behavior;

    public:
        virtual ~DisableEvent() = default;

    protected:
        virtual void OnDisable() = 0;
    };
    //更新事件
    class StartEvent
    {
    public:
        virtual ~StartEvent() = default;

    private:
        friend Behavior;

    protected:
        virtual void OnStart() = 0;
    };
    class UpdateEvent
    {
    public:
        virtual ~UpdateEvent() = default;

    private:
        friend Behavior;

    protected:
        virtual void OnUpdate() = 0;
    };
    class LateUpdateEvent
    {
    public:
        virtual ~LateUpdateEvent() = default;

    private:
        friend Behavior;

    protected:
        virtual void OnLateUpdate() = 0;
    };

    class Behavior : public Component
    {
    public:
        static void DebugHandlersCount();
        static void RunStart(); //正常情况不用调用，因为RunUpdate和RunLateUpdate中会自动处理
        static void RunUpdate();
        static void RunLateUpdate();

    protected:
        void Enable() override;
        void Disable() override;
        void Awake() override;

    private:
        //保存所有Behavior的事件注册信息
        //遍历容器时删除物体会导致出现野指针，为保证遍历继续正常运行，使用bool标记目标指针是否可用
        static std::unordered_map<StartEvent*, bool> allStartHandlers;
        static std::unordered_map<UpdateEvent*, bool> allUpdateHandlers;
        static std::unordered_map<LateUpdateEvent*, bool> allLateUpdateHandlers;

        StartEvent* startHandler = nullptr;
        UpdateEvent* updateHandler = nullptr;
        LateUpdateEvent* lateUpdateHandler = nullptr;
        bool isFirstEnable = false;
    };
    CustomReflection(Behavior)
}
