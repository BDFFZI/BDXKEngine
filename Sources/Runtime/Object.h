#pragma once
#include<vector>
#include<string>

namespace BDXKEngine {
	class ObjectEditor;
	class Object
	{
		friend ObjectEditor;
	public:
		template<typename TObject>
		static std::vector<TObject*> FindObjectsOfType()
		{
			std::vector<TObject*> result{};
			for (Object* object : objects) {
				TObject* item = dynamic_cast<TObject*>(object);
				if (item != NULL)result.push_back(item);
			}

			return result;
		}

		Object() {
			name = L"New Object";
			id = objects.size();
			objects.push_back(this);
		}

		int GetID();
		std::wstring GetName();
		void SetName(std::wstring name);

		virtual std::wstring ToString();
	private:
		static std::vector<Object*> objects;//所有物体

		int id;
		std::wstring name;
	};

	class ObjectEditor {
	protected:
		//static void InitializeObject(Object* object, int id, std::wstring name);
		//static void SetID(Object* object, int id);
	};
}

