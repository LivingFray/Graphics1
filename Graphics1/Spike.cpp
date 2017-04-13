#include "Spike.h"



Spike::Spike() {
	id = "spike";
}


Spike::~Spike() {
}


// Called when a collision occurs
void Spike::onCollide(Collider* other) {
	other->onDamage(Damage::SPIKE);
}