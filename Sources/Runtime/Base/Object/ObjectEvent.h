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
}
