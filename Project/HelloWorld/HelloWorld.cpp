#include "BDXKEditor/BDXKEditor.h"
#include "BDXKEngine/BDXKEngine.h"
#include "BDXKEngine/Base/Package/Map.h"
#include "BDXKEngine/Framework/Physics/PhysicsEvent.h"
#include "BDXKEngine/Function/Debug/Debug.h"
#include "BDXKEngine/Function/Random/Random.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Function/Window/Cursor.h"
#include "BDXKEngine/Function/Window/Input.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "imgui/imgui.h"
using namespace BDXKEngine;

class CameraController : public Behavior, public StartHandler, public UpdateHandler
{
    ObjectPtr<GameObject> transform = {};

    void OnStart() override
    {
        transform = GetGameObject();
    }
    void OnUpdate() override
    {
        if (Input::GetMouseButton(1))
        {
            Vector3 position = transform->GetLocalPosition();
            const float deltaTime = Time::GetDeltaTime();

            if (Input::GetKey(KeyCode::W))
            {
                position += transform->GetFront().GetNormalized() * deltaTime * 4;
            }
            if (Input::GetKey(KeyCode::S))
            {
                position += -transform->GetFront().GetNormalized() * deltaTime * 4;
            }
            if (Input::GetKey(KeyCode::A))
            {
                position += -transform->GetRight().GetNormalized() * deltaTime * 4;
            }
            if (Input::GetKey(KeyCode::D))
            {
                position += transform->GetRight().GetNormalized() * deltaTime * 4;
            }
            if (Input::GetKey(KeyCode::Q))
            {
                position += -transform->GetUp().GetNormalized() * deltaTime * 4;
            }
            if (Input::GetKey(KeyCode::E))
            {
                position += transform->GetUp().GetNormalized() * deltaTime * 4;
            }

            transform->SetLocalPosition(position);

            Vector3 localEulerAngles = transform->GetLocalEulerAngles();
            const Vector2 mouseMoveDelta = Input::GetMouseMoveDelta();

            localEulerAngles.y += mouseMoveDelta.x * deltaTime * 5;
            localEulerAngles.x += mouseMoveDelta.y * deltaTime * 5;

            if (Input::GetKey(KeyCode::Z))
            {
                localEulerAngles.z += deltaTime * 20;
            }
            if (Input::GetKey(KeyCode::X))
            {
                localEulerAngles.z -= deltaTime * 20;
            }

            transform->SetLocalEulerAngles(localEulerAngles);
        }
        if (Input::GetMouseButtonDown(1))
        {
            Cursor::SetLockState(true);
            Cursor::SetVisible(false);
        }
        if (Input::GetMouseButtonUp(1))
        {
            Cursor::SetLockState(false);
            Cursor::SetVisible(true);
        }
    }
};

CustomReflection(CameraController)

class CollisionTest : public Behavior, public CollisionEnterHandler, public CollisionExitHandler, public CollisionStayHandler
{
    bool isCollision = false;
    int collisionCount = 0;

    void Transfer(Transferer& transferer) override
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(isCollision);
        TransferFieldInfo(collisionCount);
    }

    void OnCollisionEnter(const Collision& collision) override
    {
        DestroyImmediate(collision.rigidbody->GetGameObject());
        Debug::Log("OnCollisionEnter");
    }

    void OnCollisionExit(const Collision& collision) override
    {
        GetGameObject()->GetComponent<Renderer>()->GetMaterial()->SetVector(0, Random::ColorHSV());
        Debug::Log("OnCollisionExit");
    }

    void OnCollisionStay(const Collision& collision) override
    {
        Debug::Log("OnCollisionStay");
    }
};

CustomReflection(CollisionTest)

class Fire : public Behavior, public UpdateHandler, public AwakeHandler, public DrawGUIHandler
{
    ObjectPtr<GameObject> ammoPrefab;

    void Transfer(Transferer& transferer) override
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(ammoPrefab);
    }

    ObjectPtr<GameObject> transform;

    void OnUpdate() override
    {
        if (Input::GetMouseButtonDown(0))
        {
            const ObjectPtr<GameObject> ammo = Serialization::Clone(ammoPrefab);
            ammo->SetPosition(transform->GetPosition());
            ammo->SetIsEnabling(true);
            ammo->GetComponent<Rigidbody>()->AddForce(transform->GetFront() * 1000);
        }
    }
    void OnAwake() override
    {
        transform = GetGameObject();
    }
    void OnDrawGUI() override
    {
        ImGui::Button("Start Game");
        ImGui::Button("Over Game");
    }
};

CustomReflection(Fire);

int main()
{
    Reflection::SetReflection<Map<std::string, bool>>();
    const std::string data = BDXKEngine::ReadFile("projectWindow.ini");
    const Serializer1<JsonImporter, JsonExporter> serializer = {};
    Map<std::string, bool>* isUnfoldingPackage = dynamic_cast<Map<std::string, bool>*>(serializer.Deserialize(data));
    
    BDXKEditor::Run("HelloWorld.scene");
}
