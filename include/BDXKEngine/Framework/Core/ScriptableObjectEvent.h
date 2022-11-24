#pragma once

namespace BDXKEngine
{
    class ScriptableObject;

    class AwakeHandler
    {
        friend ScriptableObject;
    public:
        virtual ~AwakeHandler() = default;
    protected:
        virtual void OnAwake() = 0;
    };

    class DestroyHandler
    {
        friend ScriptableObject;
    public:
        virtual ~DestroyHandler() = default;
    protected:
        virtual void OnDestroy() = 0;
    };

    class EnableHandler
    {
        friend ScriptableObject;
    public:
        virtual ~EnableHandler() = default;
    protected:
        virtual void OnEnable() = 0;
    };

    class DisableHandler
    {
        friend ScriptableObject;
    public:
        virtual ~DisableHandler() = default;
    protected:
        virtual void OnDisable() = 0;
    };
}
