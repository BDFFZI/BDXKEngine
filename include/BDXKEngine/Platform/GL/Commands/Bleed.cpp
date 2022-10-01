#include "Blend.h"
using namespace BDXKEngine;

Blend Blend::Opaque = { };
Blend Blend::Multiply = { true,Factor::SrcAlpha,Factor::OneMinusSrcAlpha,Operation::Add };
Blend Blend::Additive = { true,Factor::One,Factor::One,Operation::Add };

BDXKEngine::Blend::Blend()
{
	state = false;
	sourceFactor = Factor::One;
	destinationFactor = Factor::One;
	operation = Operation::Add;
}

BDXKEngine::Blend::Blend(bool state, Factor sourceFactor, Factor destinationFactor, Operation operation)
{
	this->state = state;
	this->sourceFactor = sourceFactor;
	this->destinationFactor = destinationFactor;
	this->operation = operation;
}
