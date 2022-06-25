#pragma once
class TickEventEditor;

class AwakeEvent {
	friend TickEventEditor;
protected:
	virtual void OnAwake() = 0;
};

class StartEvent {
	friend TickEventEditor;
protected:
	virtual void OnStart() = 0;
};

class UpdateEvent {
	friend TickEventEditor;
protected:
	virtual void OnUpdate() = 0;
};

class LateUpdateEvent {
	friend TickEventEditor;
protected:
	virtual void OnLateUpdate() = 0;
};

class RenderObjectEvent {
	friend TickEventEditor;
protected:
	virtual void OnRenderObject() = 0;
};

class DrawGizmosEvent {
	friend TickEventEditor;
protected:
	virtual void OnDrawGizmos() = 0;
};

class TickEventEditor {
protected:
	static void Awake(AwakeEvent* eventP)
	{
		eventP->OnAwake();
	}
	static void Start(StartEvent* eventP)
	{
		eventP->OnStart();
	}
	static void Update(UpdateEvent* eventP)
	{
		eventP->OnUpdate();
	}
	static void LateUpdate(LateUpdateEvent* eventP)
	{
		eventP->OnLateUpdate();
	}
	static void RenderObject(RenderObjectEvent* eventP)
	{
		eventP->OnRenderObject();
	}
	static void DrawGizmos(DrawGizmosEvent* eventP)
	{
		eventP->OnDrawGizmos();
	}
};
