#pragma once
#include<vector>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<string>

/// <summary>
/// 创建：
/// 通过PreInstantiate加载未实例化的物体
/// 在Import嵌套中通过PreInstantiate加载子物体
/// 最后再利用Awake唤醒物体使其实例化，同样嵌套调用
/// 
/// 删除：
/// Destory只是标记为删除中状态
/// 最后系统调用clear来使所有被标记的物体删除
/// DestroyImmediate是立即删除
/// </summary>

namespace BDXKEngine {
	class ObjectManager;
	class Object
	{
		friend ObjectManager;
	public:
		template<typename TObject>
		static TObject* PreInstantiate(std::wstring data)
		{
			TObject* result = new TObject();
			result->Import(data);
			return result;
		}
		template<typename TObject>
		static TObject* Instantiate(TObject* object)
		{
			TObject* result = new TObject();
			result->Import(object->Export());
			result->MarkActivating();
			FlushActivateBuffer();
			return result;
		}

		static void Destroy(Object* object) {
			object->MarkDestroying();
		}
		static void DestroyImmediate(Object* object) {
			std::vector<Object*> destroyBufferLast = destroyBuffer;

			destroyBuffer.clear();
			object->MarkDestroying();
			FlushDestroyBuffer();

			destroyBuffer = destroyBufferLast;
		};

		template<typename TObject>
		static std::vector<TObject*> FindObjectsOfType()
		{
			std::vector<TObject*> result{};
			for (Object* object : allObjects) {
				TObject* item = dynamic_cast<TObject*>(object);
				if (item != NULL)result.push_back(item);
			}

			return result;
		}
		static Object* FindObjectOfInstanceID(unsigned int instanceID)
		{
			auto pair = allObjects.find(instanceID);
			if (pair != allObjects.end())
				return pair->second;
			else
				return nullptr;
		}

		Object(std::wstring name = L"New Object");
		~Object();

		unsigned int GetInstanceID();
		std::wstring GetName();

		void SetName(std::wstring name);

		virtual std::wstring ToString();
	protected:
		static unsigned int instanceIDCount;//为0代表None占位符
		static std::map<unsigned int, Object*> allObjects;//当前所有物体
		static std::vector<Object*> activateBuffer;
		static std::vector<Object*> destroyBuffer;

		virtual void Awake() = 0;
		virtual void Destroy() = 0;
		virtual void Import(std::wstring data) = 0;
		virtual std::wstring Export() = 0;

		//用于嵌套数据结构。请以此标记子物体，以告知Object需要批量处理
		virtual void MarkActivating() {
			if (isActivating == false)activateBuffer.push_back(this);
			isActivating = true;
		};
		virtual void MarkDestroying() {
			if (isDestroying == false)destroyBuffer.push_back(this);
			isDestroying = true;
		};
	private:
		static void FlushActivateBuffer()
		{
			for (auto& object : activateBuffer)
			{
				object->Awake();
			}
			activateBuffer.clear();
		}
		static void FlushDestroyBuffer()
		{
			for (auto& object : destroyBuffer)
			{
				object->Destroy();
				allObjects.erase(object->instanceID);
				delete object;
			}
			destroyBuffer.clear();
		}

		unsigned int instanceID;
		std::wstring name;
		bool isDestroying = false;
		bool isActivating = false;
	};

	class ObjectManager {
	protected:
		void FlushDestroyBuffer()
		{
			Object::FlushDestroyBuffer();
		}
	};
}

