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
        if (status == FresnelReflection) {
            m_pAnalysisManager->IncrementNumFresnelReflection();
        } else if (status == TotalInternalReflection) {
            m_pAnalysisManager->IncrementNumTotalInternalReflection();
        } else if (status == LambertianReflection) {
            m_pAnalysisManager->IncrementNumLambertianReflection();
        } else if (status == LobeReflection) {
            m_pAnalysisManager->IncrementNumLobeReflection();
        } else if (status == SpikeReflection) {
            m_pAnalysisManager->IncrementNumSpikeReflection();
        } else if (status == BackScattering) {
            m_pAnalysisManager->IncrementNumBackscattering();
        }
    }

    // time that PMT detects a photon
    if (hPreVolume == "PMTWindow" && hPostVolume == "PMT") {
        const G4double dTime = pStep->GetPostStepPoint()->GetGlobalTime();
        m_pAnalysisManager->DetectPhoton(dTime);
    }

}
