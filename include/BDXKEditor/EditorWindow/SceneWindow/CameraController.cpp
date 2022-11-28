#include "CameraController.h"
#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Function/Window/Cursor.h"
#include "BDXKEngine/Function/Window/Input.h"


namespace BDXKEngine
{
    bool CameraController::IsControlling() const
    {
        return isControlling;
    }
    void CameraController::OnStart()
    {
        transform = GetGameObject();
    }
    void CameraController::OnUpdate()
    {
        isControlling = Input::GetMouseButton(1);
        if (isControlling)
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
}
