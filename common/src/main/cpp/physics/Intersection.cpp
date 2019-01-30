#include "physics/Intersection.h"

bool curtinfrc::physics::Intersection::Claim(curtinfrc::physics::IntersectionUser &user) {
  // I'm not sure if equal priority should override or not
  if (user.priority > GetOwnerPriority()) _owner = &user;

  return user.ID == GetOwnerID();
}
