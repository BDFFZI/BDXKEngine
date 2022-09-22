#pragma once

namespace BDXKEngine {
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
