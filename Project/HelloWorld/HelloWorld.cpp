#include "BDXKEditor/BDXKEditor.h"
#include "BDXKEngine/Base/Package/Map.h"
#include "BDXKEngine/Framework/Behavior/AudioSource.h"
#include "BDXKEngine/Framework/Physics/PhysicsEvent.h"
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

class CollisionTest : public Behavior, public AwakeHandler, public UpdateHandler, public CollisionEnterHandler
{
    ObjectPtr<AudioClip> bounceOff;
    ObjectPtr<AudioClip> explode;
    void Transfer(Transferer& transferer) override
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(bounceOff);
        TransferFieldInfo(explode);
    }

    float startTime = 0;
    bool isExploded = false;
    ObjectPtr<AudioSource> audioSource;

    void OnAwake() override
    {
        startTime = Time::GetRealtimeSinceStartup();
        audioSource = GetGameObject()->GetComponent<AudioSource>();
    }
    void OnUpdate() override
    {
        if (isExploded)
        {
            if (audioSource->IsPlaying() == false)
                DestroyImmediate(GetGameObject());
        }
        else
        {
            if (Time::GetRealtimeSinceStartup() - startTime > 4)
                DestroyImmediate(GetGameObject());
        }
    }
    void OnCollisionEnter(const Collision& collision) override
    {
        if (isExploded)
            return;

        GetGameObject()->GetComponent<Renderer>()->GetMaterial()->SetVector(0, Random::ColorHSV());
        if (collision.rigidbody->GetIsKinematic() == false)
        {
            ObjectPtr<Renderer> renderer = collision.rigidbody->GetGameObject()->GetComponent<Renderer>();
            Color color = renderer->GetMaterial()->GetVector(0);
            color -= 0.1f;
            if (color.a < 0)
                DestroyImmediate(collision.rigidbody->GetGameObject());
            else
                renderer->GetMaterial()->SetVector(0, color);

            audioSource->SetClip(explode);
            audioSource->Play();
            isExploded = true;
        }
        else
        {
            GetGameObject()->GetComponent<Renderer>()->GetMaterial()->SetVector(0, Random::ColorHSV());
            audioSource->SetClip(bounceOff);
            audioSource->Play();
        }
    }
};

CustomReflection(CollisionTest)

class Fire : public Behavior, public UpdateHandler, public AwakeHandler, public DrawGUIHandler
{
    ObjectPtr<GameObject> ammoPrefab;
    ObjectPtr<AudioSource> audioSource;

    void Transfer(Transferer& transferer) override
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(ammoPrefab);
        TransferFieldInfo(audioSource);
    }

    ObjectPtr<GameObject> transform;

    void OnUpdate() override
    {
        if (Input::GetMouseButtonDown(0))
        {
            audioSource->Play();
            const ObjectPtr<GameObject> ammo = Serialization::Clone(ammoPrefab);
            ammo->SetPosition(transform->GetPosition());
            ammo->SetIsEnabling(true);
            ammo->GetComponent<Rigidbody>()->AddForce(transform->GetFront() * 1500);
        }
    }
    void OnAwake() override
    {
        transform = GetGameObject();
    }
    void OnDrawGUI() override
    {
        ImGui::Button("开始游戏");
        ImGui::Button("结束游戏");
    }
};

CustomReflection(Fire);

int main()
{
    BDXKEditor::Run("HelloWorld.scene");
}
