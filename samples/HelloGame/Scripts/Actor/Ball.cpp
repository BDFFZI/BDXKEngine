#include "Ball.h"
#include "BDXKEngine/Function/Random/Random.h"

namespace HelloGame
{
    const std::function<void()>& Ball::GetDestroyCallback() const
    {
        return destroyCallback;
    }
    void Ball::SetDestroyCallback(const std::function<void()>& destroyCallback)
    {
        this->destroyCallback = destroyCallback;
    }
    void Ball::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(speed);
    }
    void Ball::OnStart()
    {
        rightbody = GetGameObject()->GetComponent<Rigidbody>();
    }
    void Ball::OnFixedUpdate()
    {
        Vector3 velocity = rightbody->GetVelocity().GetNormalized() * speed;
        if (velocity == Vector3::zero)velocity = Random::SphereSurface() * speed;
        rightbody->SetVelocity(velocity);
    }
    void Ball::OnDestroy()
    {
        if (destroyCallback != nullptr)
            destroyCallback();
    }
}
