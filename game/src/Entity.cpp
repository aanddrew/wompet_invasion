#include "../include/Entity.h"

const glm::vec3& Entity::getPos(){return pos;}
void Entity::setPos(glm::vec3 newPos) {pos = newPos;}

const glm::vec3& Entity::getVelocity() {return velocity;}
void Entity::setVelocity(glm::vec3 newVelocity) {velocity = newVelocity;}

const float& Entity::getRadius() {return radius;}
void Entity::setRadius(float newRadius) {radius = newRadius;}

