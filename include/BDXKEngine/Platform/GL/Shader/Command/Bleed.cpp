#include "Blend.h"

namespace BDXKEngine
{
    Blend Blend::Opaque = {};
    Blend Blend::Multiply = {true, Factor::SrcAlpha, Factor::OneMinusSrcAlpha, Operation::Add};
    Blend Blend::Additive = {true, Factor::One, Factor::One, Operation::Add};

    Blend::Blend() = default;
    Blend::Blend(bool state, Factor sourceFactor, Factor destinationFactor, Operation operation)
    {
        this->state = state;
        this->sourceFactor = sourceFactor;
        this->destinationFactor = destinationFactor;
        this->operation = operation;
    }
    void Blend::Transfer(Transferer& transferer)
    {
        TransferFieldInfo(state);
        TransferFieldInfoOf(sourceFactor, int);
        TransferFieldInfoOf(destinationFactor, int);
        TransferFieldInfoOf(operation, int);
    }
}
