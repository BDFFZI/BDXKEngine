#pragma once
#include<vector>
#include<map>
#include<unordered_set>
#include<unordered_map>
#include<string>

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
		static unsigned int instanceIDCount;//Ϊ0����Noneռλ��
		static std::map<unsigned int, Object*> allObjects;//��ǰ��������
		static std::vector<Object*> activateBuffer;
		static std::vector<Object*> destroyBuffer;

		virtual void Awake() = 0;
		virtual void Destroy() = 0;
		virtual void Import(std::wstring data) = 0;
		virtual std::wstring Export() = 0;

		//����Ƕ�����ݽṹ�����Դ˱�������壬�Ը�֪Object��Ҫ��������
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

