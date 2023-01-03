#include "BDXKEditor/BDXKEditor.h"
//#include "BDXKEngine/BDXKEngine.h"
#include <unordered_set>

#include "BDXKEditor/Function/Debug.h"
#include "BDXKEngine/Framework/Physics/PhysicsEvent.h"
#include "BDXKEngine/Function/Random/Random.h"
using namespace BDXKEngine;

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

    void OnCollisionEnter(ObjectPtr<Collider> collider) override
    {
        BDXKEditor::Debug::Log("OnCollisionEnter");
        GetGameObject()->GetComponent<Renderer>()->GetMaterial()->SetVector(0, Random::ColorHSV());
    }

    void OnCollisionExit(ObjectPtr<Collider> collider) override
    {
        BDXKEditor::Debug::Log("OnCollisionExit");
    }

    void OnCollisionStay(ObjectPtr<Collider> collider) override
    {
        BDXKEditor::Debug::Log("OnCollisionStay");
    }
};

CustomReflection(CollisionTest)

int main()
{
    BDXKEditor::Run("HelloWorld.scene");
}
