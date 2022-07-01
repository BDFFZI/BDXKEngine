#pragma once
#include<vector>
#include<string>

class ObjectEditor;
class Object
{
	friend ObjectEditor;
public:
	template<typename TObject>
	static std::vector<TObject> FindObjectsOfType()
	{
		std::vector<TObject> result{};
		for (Object* object : objects) {
			TObject item = dynamic_cast<TObject>(object);
			if (item != NULL)result.push_back(item);
		}

		return result;
	}

	Object() {
		id = 0;
		name = L"New Object";
		objects.push_back(this);
	}

	int GetID() {
		return id;
	}
	std::wstring GetName()
	{
		return name;
	}
	void SetName(std::wstring name)
	{
		this->name = name;
	}
	virtual std::wstring ToString()
	{
		return name;
	}
private:
	static std::vector<Object*> objects;//所有物体

	int id;
	std::wstring name;
};

class ObjectEditor {
protected:
	static void InitializeObject(Object* object, int id, const wchar_t* name)
	{
		object->id = id;
		object->name = name;
	}

	static void SetID(Object* object, int id)
	{
		object->id = id;
	}
};

