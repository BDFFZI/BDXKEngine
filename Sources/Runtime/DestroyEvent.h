#pragma once

class DestoryEventEditor;
class DestoryEvent {
	friend DestoryEventEditor;
protected:
	virtual void OnDestory() = 0;
};

class DestoryEventEditor {
protected:
	static void Destory(DestoryEvent* eventP)
	{
		eventP->OnDestory();
	}
};