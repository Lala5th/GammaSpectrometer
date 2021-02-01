#ifndef ActionInitialization_hh
#define ActionInitialization_hh
#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization{
    public:
        ActionInitialization();
        void Build() const;
        void BuildForMaster() const;
};

#endif
