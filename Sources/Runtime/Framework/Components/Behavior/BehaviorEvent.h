#pragma once

namespace BDXKEngine {
	class Behavior;
	class AwakeHandler
	{
		friend Behavior;
	public:
		virtual ~AwakeHandler() = default;
	protected:
		virtual void OnAwake() = 0;
	};

	class DestroyHandler
	{
		friend Behavior;
	public:
		virtual ~DestroyHandler() = default;
	protected:
		virtual void OnDestroy() = 0;
	};
	
	class EnableHandler
	{
		friend Behavior;
	public:
		virtual ~EnableHandler() = default;
	protected:
		virtual void OnEnable() = 0;
	};
	class DisableHandler
	{
		friend Behavior;
	public:
		virtual ~DisableHandler() = default;
	protected:
		virtual void OnDisable() = 0;
	};


	class BehaviorManager;
	class StartHandler {
	public:
		virtual ~StartHandler() = default;
	private:
		friend BehaviorManager;
	protected:
		virtual void OnStart() = 0;
	};
	class UpdateHandler {
	public:
		virtual ~UpdateHandler() = default;
	private:
		friend BehaviorManager;
	protected:
		virtual void OnUpdate() = 0;
	};
	class LateUpdateHandler {
	public:
		virtual ~LateUpdateHandler() = default;
	private:
		friend BehaviorManager;
	protected:
		virtual void OnLateUpdate() = 0;
	};
}
