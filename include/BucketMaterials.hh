#pragma once

#include <G4Material.hh>
#include <G4MaterialPropertiesTable.hh>

class BucketMaterials {
    public:
        BucketMaterials();
        ~BucketMaterials();

    public:
        void DefineMaterials() const;
};
