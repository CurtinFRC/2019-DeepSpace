#pragma once

namespace curtinfrc {
  namespace physics {
    struct IntersectionUser {
      int ID;
      int priority;
    };

    class Intersection {
     public:
      Intersection() { _owner = nullptr; };

      int GetOwnerID() { return _owner == nullptr ? -1 : _owner->ID; };
      int GetOwnerPriority() { return _owner == nullptr ? 0 : _owner->priority; };

      bool Claim(IntersectionUser &user);

     private:
      IntersectionUser *_owner;
    };
  } // ns physics
} // ns curtinfrc
