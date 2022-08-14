#pragma once
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "String"
#include "BinaryReader.h"
#include "BinaryWriter.h"

/// <summary>
/// ������
/// ͨ��PreInstantiate����δʵ����������
/// ��ImportǶ����ͨ��PreInstantiate����������
/// ���������Awake��������ʹ��ʵ������ͬ��Ƕ�׵���
/// 
/// ɾ����
/// Destoryֻ�Ǳ��Ϊɾ����״̬
/// ���ϵͳ����clear��ʹ���б���ǵ�����ɾ��
/// DestroyImmediate������ɾ��
/// </summary>

namespace BDXKEngine {
	class ObjectManager;
	class Object
	{
		friend ObjectManager;
	public:
		template<typename TObject>
		static TObject* InstantiateNoAwake(TransferBase& reader)
		{
			TObject* result = new TObject();
			result->TransferBase(reader);
			return result;
		}
		template<typename TObject>
		static TObject* Instantiate(TObject* source)
		{
			std::stringstream stream;
			//��ȡ����������
			Object* sourceObject = static_cast<Object*>(source);
			BinaryWriter writer = { stream };
			sourceObject->TransferBase(writer);
			//д������������
			TObject* destination = new TObject();
			Object* destinationObject = static_cast<Object*>(destination);
			BinaryReader reader = { stream };
			destinationObject->TransferBase(reader);

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
		static unsigned int instanceIDCount;//Ϊ0����Noneռλ��
		static std::map<unsigned int, Object*> allObjects;//��ǰ��������
		static std::vector<Object*> activateBuffer;
		static std::vector<Object*> destroyBuffer;

		virtual void Awake();
		virtual void Destroy();
		virtual void Transfer(TransferBase& transfer) = 0;

		//����Ƕ�����ݽṹ�����Դ˱�������壬�Ը�֪Object��Ҫ��������
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

	class ObjectManager {
	protected:
		void FlushDestroyBuffer()
		{
			Object::FlushDestroyBuffer();
		}
	};
}

