#include "BucketSteppingAction.hh"
#include <G4Step.hh>
#include <G4ProcessManager.hh>
#include <G4OpBoundaryProcess.hh>
#include <G4SystemOfUnits.hh>

BucketSteppingAction::BucketSteppingAction(BucketAnalysisManager *pAnalysisManager) : m_pAnalysisManager(pAnalysisManager) {}

BucketSteppingAction::~BucketSteppingAction(){}

void BucketSteppingAction::UserSteppingAction(const G4Step *pStep){
    const G4String hPreVolume = pStep->GetPreStepPoint()->GetPhysicalVolume()->GetName();
    const G4String hPostVolume = pStep->GetPostStepPoint()->GetPhysicalVolume()->GetName();

    //find the boundary process only once
    G4OpBoundaryProcessStatus status = Undefined;
    G4OpBoundaryProcess* boundary = nullptr;
    G4ProcessManager* pmanager = pStep->GetTrack()->GetDefinition()->GetProcessManager();
    G4int nprocesses = pmanager->GetProcessListLength();
    G4ProcessVector* pvector = pmanager->GetProcessList();
    for (G4int i = 0; i < nprocesses; ++i) {
        if ((*pvector)[i]->GetProcessName() == "OpBoundary") {
            boundary = dynamic_cast<G4OpBoundaryProcess *>((*pvector)[i]);
            break;
        }
    }

    constexpr bool bPrintBoundaryProcess = true;
    if (bPrintBoundaryProcess
            && pStep->GetPostStepPoint()->GetStepStatus() == fGeomBoundary
            && pStep->GetTrack()->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
        status = boundary->GetStatus();
        if (status == Undefined) {
            //G4cout << "Undefined" << G4endl;
        } else if (status == FresnelRefraction) {
            //G4cout << "FresnelRefraction" << G4endl;
        } else if (status == FresnelReflection) {
            m_pAnalysisManager->IncrementNumFresnelReflection();
            //G4cout << "FresnelReflection" << G4endl;
        } else if (status == TotalInternalReflection) {
            m_pAnalysisManager->IncrementNumTotalInternalReflection();
            //G4cout << "TotalInternalReflection" << G4endl;
        } else if (status == LambertianReflection) {
            //G4cout << "LambertianReflection" << G4endl;
            m_pAnalysisManager->IncrementNumLambertianReflection();
        } else if (status == LobeReflection) {
            //G4cout << "LobeReflection" << G4endl;
            m_pAnalysisManager->IncrementNumLobeReflection();
        } else if (status == SpikeReflection) {
            //G4cout << "SpikeReflection" << G4endl;
            m_pAnalysisManager->IncrementNumSpikeReflection();
        } else if (status == BackScattering) {
            //G4cout << "BackScattering" << G4endl;
            m_pAnalysisManager->IncrementNumBackscattering();
        } else if (status == Absorption) {
            //G4cout << "Absorption" << G4endl;
        } else if (status == Detection) {
            //G4cout << "Detection" << G4endl;
        } else if (status == NotAtBoundary) {
            //G4cout << "NotAtBoundary" << G4endl;
        } else {
            //G4cout << "Unexpected process: " << status << G4endl;
        }
    }

    // time that PMT detects a photon
    if (hPreVolume == "PMTWindow" && hPostVolume == "PMT") {
        const G4double dTime = pStep->GetPostStepPoint()->GetGlobalTime();
        m_pAnalysisManager->DetectPhoton(dTime);
    }

}
