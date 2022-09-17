#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "String"
#include "Serializable.h"
#include "BinaryImporter.h"
#include "BinaryExporter.h"

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
	class Object :public Serializable
	{
		friend ObjectManager;
	public:
		template<typename TObject>
		static TObject* InstantiateNoAwake(Importer& importer)
		{
			Object* result = new TObject();
			result->Import(importer);
			return static_cast<TObject*>(result);
		}
		template<typename TObject>
		static TObject* InstantiateNoAwake(std::function<void(Exporter& exporter)> serialize)
		{
			std::stringstream stream = {};
			BinaryExporter exporter = { stream };
			serialize(exporter);

			Object* result = new TObject();
			BinaryImporter importer = { stream };
			result->Import(importer);
			return static_cast<TObject*>(result);
		}
		template<typename TObject>
		static TObject* Instantiate(TObject* source)
		{
			std::stringstream stream;
			//获取旧物体数据
			Object* sourceObject = static_cast<Object*>(source);
			BinaryExporter exporter = { stream };
			sourceObject->Export(exporter);
			//写入新物体数据
			TObject* destination = new TObject();
			Object* destinationObject = static_cast<Object*>(destination);
			BinaryImporter importer = { stream };
			destinationObject->Import(importer);

			destinationObject->MarkActivating();
			FlushActivateBuffer();
			return destination;
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

		unsigned int GetInstanceID();
		std::wstring GetName();

		void SetName(std::wstring name);

		void Import(Importer& transfer)override {};
		void Export(Exporter& transfer)override {};
		virtual std::wstring ToString();

		//bool operator==(std::nullptr_t null)
		//{
		//	return allObjects.contains(instanceID);
		//}
		//bool operator!=(std::nullptr_t null)
		//{
		//	return allObjects.contains(instanceID) == false;
		//}
	protected:
		static unsigned int instanceIDCount;//为0代表None占位符
		static std::map<unsigned int, Object*> allObjects;//当前所有物体
		static std::vector<Object*> activateBuffer;
		static std::vector<Object*> destroyBuffer;

		virtual void Awake();
		virtual void Destroy();

		//用于嵌套数据结构。请以此标记子物体，以告知Object需要批量处理
		virtual void MarkActivating() {
			if (isActivating == false)activateBuffer.push_back(this);
			isActivating = true;
		};
		virtual void MarkDestroying() {
			if (isDestroying == false)destroyBuffer.push_back(this);
			isDestroying = true;
		};

		Object();
		~Object();
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

}

