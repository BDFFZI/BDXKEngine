#include "Bar.h"

#include <algorithm>

#include "BDXKEngine/Function/Time/Time.h"
#include "BDXKEngine/Function/Window/Input.h"
#include "BDXKEngine/Platform/Serialization/Serialization.h"
#include "BDXKEngine/Platform/Window/Window.h"

namespace HelloGame
{
    void Bar::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(sphereBar);
        TransferFieldInfo(ballPrefab);
        TransferFieldInfo(maxBallCount);
    }
    void Bar::OnUpdate()
    {
        const float deltaTime = Time::GetDeltaTime();
        //移动控制
        {
            const ObjectPtr<GameObject> transform = GetGameObject();
            Vector3 position = transform->GetLocalPosition();
            if (Input::GetKey(KeyCode::A))
            {
                position += -transform->GetRight().GetNormalized() * deltaTime * 6;
            }
            if (Input::GetKey(KeyCode::D))
            {
                position += transform->GetRight().GetNormalized() * deltaTime * 6;
            }
            position.x = std::clamp(position.x, -6.0f, 6.0f);
            transform->SetLocalPosition(position);
        }
        //球型挡板
        {
            Vector3 position = sphereBar->GetLocalPosition();
            if (Input::GetKey(KeyCode::W))
            {
                position.z += deltaTime * 1.5f;
            }
            if (Input::GetKey(KeyCode::S))
            {
                position.z -= deltaTime * 1.5f;
            }
            position.z = std::clamp(position.z, -0.25f, 0.25f);
            sphereBar->SetLocalPosition(position);
        }
        //准备弹球
        if (currentBall == nullptr)
        {
            if (currentBallCount < maxBallCount)
            {
                const ObjectPtr<GameObject> ballGameObject = Serialization::Clone(ballPrefab);
                ballGameObject->GetComponent<Rigidbody>()->SetIsEnabling(false);
                ballGameObject->SetParent(GetGameObject());
                ballGameObject->SetLocalPosition(Vector3::front);
                ballGameObject->SetIsEnabling(true);
                const ObjectPtr<Ball> ball = ballGameObject->GetComponent<Ball>();
                ball->SetDestroyCallback([&] { currentBallCount--; });

                currentBall = ballGameObject;
                currentBallCount++;
            }
        }
        else if (Input::GetMouseButtonDown(0))
        {
            const ObjectPtr<Rigidbody> ballRigidbody = currentBall->GetComponent<Rigidbody>();
            ballRigidbody->SetIsEnabling(true);
            ballRigidbody->SetVelocity(GetGameObject()->GetFront());
            currentBall = nullptr;
        }
    }
}
