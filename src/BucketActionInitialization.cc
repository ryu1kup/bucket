#include "BucketActionInitialization.hh"
#include "BucketPrimaryGeneratorAction.hh"
#include "BucketEventAction.hh"
#include "BucketSteppingAction.hh"

BucketActionInitialization::BucketActionInitialization(){}

BucketActionInitialization::~BucketActionInitialization(){}

void BucketActionInitialization::Build() const {
    SetUserAction(new BucketPrimaryGeneratorAction());
    SetUserAction(new BucketEventAction());
    SetUserAction(new BucketSteppingAction());
}
