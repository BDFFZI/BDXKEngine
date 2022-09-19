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
/// ������
/// ͨ��InstantiateNoAwake����δʵ����������
/// ��Import��ͨ��Importer�ṩ�ĺ���Ƕ�׼���������
/// Ȼ��������MarkActivatingǶ�ױ��Ҫ���ѵ�����
/// ���ͨ��FlushActivateBufferһ������
/// 
/// ɾ����
/// Destoryֻ�Ǳ��Ϊɾ����״̬
/// ���ϵͳ����FlushDestroyBuffer��ʹ���б���ǵ�����ɾ��
/// DestroyImmediate������ɾ��
/// </summary>

namespace BDXKEngine {
	class ObjectManager;
	class Object :public Serializable
	{
		friend ObjectManager;
	public:
		// ���湹�캯��
		// ʹ��Create()������Object��InstanceID�����ܵ���0
		// �Դ������Ƿ�Ϊ�Ϸ���Object
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
		// ��ʼ���壬���������л����湦�ܣ�������ʵ������Ϊ������ʱ���ܵľ�̬����
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

		// ʵ�������壬�Լ�������ʱ��ع���
		template<typename TObject>
		static TObject* Instantiate(TObject* source)
		{
			if (source == nullptr)
				throw std::exception("����ʵ�����Ĳ�������Ϊ��");

			TObject* object = InstantiateNoAwake<TObject>(source);
			if (source->instanceID != 0)object->SetName(object->GetName() + L" (Clone)");
			Awake(object);
			return object;
		}
		/// �������壬ֻ��Awake()�е�����Ч
		static void Awake(Object* object);
		/// �������壬ֻ��Destroy()�е�����Ч
		static void Destroy(Object* object);
		static void DestroyImmediate(Object* object);;

		template<typename TObject>
		static std::vector<TObject*> FindObjectsOfType()
		{
			std::vector<TObject*> result{};
			for (auto& item : allObjectsRuntime) {
				TObject* object = dynamic_cast<TObject*>(item.second);
				if (object != nullptr)result.push_back(object);
			}

			return result;
		}
		static Object* FindObjectOfInstanceID(unsigned int instanceID);

		unsigned int GetInstanceID();
		std::wstring GetName();
		void SetName(std::wstring name);

		bool IsActivating();
		virtual std::wstring ToString();
	protected:
		virtual void Export(Exporter& transfer)override;
		virtual void Import(Importer& transfer)override;
		virtual void Awake();//���ѻص�
		virtual void Destroy();//���ٻص�
		virtual ~Object() {};// ��Ϊ�麯������֤delete����������
	private:
		static unsigned int instanceIDCount;//0ΪNoneռλ��,һ����������������
		/// �ڴ��е�����Object������δʵ������Object
		static std::map<unsigned int, Object*> allObjects;
		static std::map<unsigned int, Object*> allObjectsRuntime;
		/// ���ٶ���
		static std::vector<Object*> destroyBuffer;

		// ͳһ���ٱ���ǵ����壬DestroyImmediate()���ã�Ԥ��Unity�ӳ�ɾ�����ܣ�Ӧ����Ϊ���ڴ���յ�����
		static void FlushDestroyBuffer();

		unsigned int instanceID;
		std::wstring name;
		bool isActivating = false;
		bool isDestroying = false;
	};
}

