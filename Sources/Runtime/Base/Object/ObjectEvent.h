#pragma once

namespace BDXKEngine
{
    class Object;
    
    class AwakeHandler
    {
        friend Object;
    public:
        virtual ~AwakeHandler() = default;
    protected:
        virtual void OnAwake() = 0;
    };

    class DestroyHandler
    {
        friend Object;
    public:
        virtual ~DestroyHandler() = default;
    protected:
        virtual void OnDestroy() = 0;
    };

    class EnableHandler
    {
        friend Object;
    public:
        virtual ~EnableHandler() = default;
    protected:
        virtual void OnEnable() = 0;
    };

    class DisableHandler
    {
        friend Object;
    public:
        virtual ~DisableHandler() = default;
    protected:
        virtual void OnDisable() = 0;
    };
}
