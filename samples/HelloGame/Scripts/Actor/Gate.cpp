#include "Gate.h"
#include "BDXKEngine/Framework/Physics/Rigidbody/Rigidbody.h"
#include "BDXKEngine/Framework/Renderer/MeshRenderer.h"

namespace HelloGame
{
    void Gate::Transfer(Transferer& transferer)
    {
        Behavior::Transfer(transferer);

        TransferFieldInfo(maxHealthPoint);
        TransferFieldInfo(healthPoint);
    }
    void Gate::OnCollisionEnter(const Collision& collision)
    {
        healthPoint -= collision.rigidbody->GetVelocity().GetMagnitude() * 4;
        DestroyImmediate(collision.rigidbody->GetGameObject());

        if (healthPoint <= 0)
        {
            DestroyImmediate(GetGameObject());
            return;
        }

        material->SetVector(0, originalColor * (healthPoint / maxHealthPoint));
    }
    void Gate::OnAwake()
    {
        material = GetGameObject()->GetComponent<MeshRenderer>()->GetMaterial();
        originalColor = material->GetVector(0);
        healthPoint = maxHealthPoint;
    }
}
