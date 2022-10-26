#pragma once

namespace BDXKEngine
{
    class SwitchableObject;
    
    class AwakeHandler
    {
        friend SwitchableObject;
    public:
        virtual ~AwakeHandler() = default;
    protected:
        virtual void OnAwake() = 0;
    };

    class DestroyHandler
    {
        friend SwitchableObject;
    public:
        virtual ~DestroyHandler() = default;
    protected:
        virtual void OnDestroy() = 0;
    };
	
    class EnableHandler
    {
        friend SwitchableObject;
    public:
        virtual ~EnableHandler() = default;
    protected:
        virtual void OnEnable() = 0;
    };
    class DisableHandler
    {
        friend SwitchableObject;
    public:
        virtual ~DisableHandler() = default;
    protected:
        virtual void OnDisable() = 0;
    };
}
