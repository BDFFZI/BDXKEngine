#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <functional>
#include "String.h"
#include "Serializable.h"
#include "BinaryImporter.h"
#include "BinaryExporter.h"
#include "Debug.h"

/// <summary>
/// 创建：
/// 通过InstantiateNoAwake加载未实例化的物体
/// 在Import中通过Importer提供的函数嵌套加载子物体
/// 然后再利用MarkActivating嵌套标记要唤醒的物体
/// 最后通过FlushActivateBuffer一并唤醒
/// 
/// 删除：
/// Destory只是标记为删除中状态
/// 最后系统调用FlushDestroyBuffer来使所有被标记的物体删除
/// DestroyImmediate是立即删除
/// </summary>

namespace BDXKEngine {
	class ObjectManager;
	class Object :public Serializable
	{
		friend ObjectManager;
	public:
		// 代替构造函数
		// 使用Create()创建的Object的InstanceID不可能等于0
		// 以此区分是否为合法的Object
		template<typename TObject>
		static TObject* Create()
		{
			TObject* result = new TObject();
			result->instanceID = ++instanceIDCount;
			result->name = String::to_wstring((typeid(TObject).name()));
			allObjects[result->instanceID] = result;
			Debug::LogWarning(std::wstring(L"Object::Create ") + std::to_wstring(result->instanceID) + L" " + result->GetName());
			return result;
		}
		// 初始物体，仅加载序列化层面功能，不进行实例化，为无运行时功能的静态物体
		template<typename TObject>
		static TObject* InstantiateNoAwake(Importer& importer)
		{
			Object* result = Create<TObject>();
			result->Import(importer);
			return static_cast<TObject*>(result);
		}
		template<typename TObject>
		static TObject* InstantiateNoAwake(std::function<void(Exporter& exporter)> serialize)
		{
			std::stringstream stream = {};
			BinaryExporter exporter = { stream };
			serialize(exporter);
			BinaryImporter importer = { stream };

			return InstantiateNoAwake<TObject>(importer);
		}
		template<typename TObject>
		static TObject* InstantiateNoAwake(TObject* serializer)
		{
			std::stringstream stream = {};
			BinaryExporter exporter = { stream };
			static_cast<Object*>(serializer)->Export(exporter);
			BinaryImporter importer = { stream };

			return InstantiateNoAwake<TObject>(importer);
		}

		// 实例化物体，以加载运行时相关功能
		template<typename TObject>
		static TObject* Instantiate(TObject* source)
		{
			if (source == nullptr)
				throw std::exception("用于实例化的参照物体为空");

			TObject* object = InstantiateNoAwake<TObject>(source);
			if (source->instanceID != 0)object->SetName(object->GetName() + L" (Clone)");
			Awake(object);
			FlushActivateBuffer();
			return object;
		}
		/// 激活物体，只在Awake()中调用有效
		static void Awake(Object* object) {
			if (object->isActivating)return;
			//activateBuffer.push_back(object);
			object->isActivating = true;
			object->Awake();
		}
		/// 销毁物体，只在Destroy()中调用有效
		static void Destroy(Object* object) {
			if (object == nullptr || object->isDestroying)return;
			destroyBuffer.push_back(object);
			object->isDestroying = true;
			object->Destroy();
		}
		static void DestroyImmediate(Object* object) {
			std::vector<Object*> destroyBufferLast = destroyBuffer;

			destroyBuffer.clear();
			Destroy(object);
			FlushDestroyBuffer();

			destroyBuffer = destroyBufferLast;
		};

		template<typename TObject>
		static std::vector<TObject*> FindObjectsOfType()
		{
			std::vector<TObject*> result{};
			for (auto& item : allObjectsEnabling) {
				TObject* object = dynamic_cast<TObject*>(item.second);
				if (object != nullptr)result.push_back(object);
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

		bool IsInstantiated();
		virtual std::wstring ToString();
	protected:
		virtual void Export(Exporter& transfer)override;
		virtual void Import(Importer& transfer)override;
		virtual void Awake();//唤醒回调
		virtual void Destroy();//销毁回调
		// 设为虚函数来保证delete能销毁子类
		virtual ~Object() {};
	private:
		static unsigned int instanceIDCount;//0为None占位符,一般用作纯数据容器
		/// 内存中的所有Object，包括未实例化的Object
		static std::map<unsigned int, Object*> allObjects;
		static std::map<unsigned int, Object*> allObjectsEnabling;
		/// 实例化队列
		static std::vector<Object*> activateBuffer;
		/// 销毁队列
		static std::vector<Object*> destroyBuffer;

		// 统一激活被标记的物体，Instantiate()调用，
		static void FlushActivateBuffer()
		{
			for (auto& object : activateBuffer)
			{
				object->Awake();
			}
			activateBuffer.clear();
		}
		// 统一销毁被标记的物体，DestroyImmediate()调用
		static void FlushDestroyBuffer()
		{
			for (auto& object : destroyBuffer)
			{
				//object->Destroy();
				allObjects.erase(object->instanceID);
				delete object;
			}
			destroyBuffer.clear();
		}

		unsigned int instanceID;
		std::wstring name;
		bool isActivating = false;
		bool isDestroying = false;
		bool isEnabling = false;
	};
}

