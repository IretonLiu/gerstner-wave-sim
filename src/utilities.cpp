#include "utilities.hpp"

float viewportTransform(float oldX, float newStart, float newEnd, float oldStart, float oldEnd) {
    return newStart + (newEnd - newStart) * ((oldX - oldStart) / (oldEnd - oldStart));
}

float angleBetween(glm::vec3 v1, glm::vec3 v2) {
    return glm::acos(glm::dot(v1, v2));
}
