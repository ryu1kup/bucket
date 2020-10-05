#include "BucketDetectorConstruction.hh"
#include "BucketPhysicsList.hh"
#include "BucketActionInitialization.hh"
#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <iostream>
#include <map>
#include <string>

std::map<std::string, std::string> parse_arguments(int argc, char **argv) {
    std::map<std::string, std::string> optionlist {
        {"-p", "../macros/preinit.mac"},
            {"-r", "../macros/run.mac"},
            {"-v", ""},
    };

    for (int i = 0; i < argc; ++i) {
        if (optionlist.count(argv[i])) {
            optionlist.at(argv[i]) = argv[i + 1];
        }
    }
    return optionlist;
}

void execute_macro(G4UImanager *manager, const std::string& macro) {
    if (macro != "") {
        std::ifstream ifs(macro);
        if (ifs.is_open()) {
            manager->ApplyCommand("/control/execute " + macro);
        } else {
            std::cerr << "WARNING! file does not exists: " << macro << std::endl;
        }
    }
}

int main(int argc, char **argv){
    auto&& args =  parse_arguments(argc, argv);
    const G4String preinit = args["-p"];
    const G4String visualization = args["-v"];
    const G4String run = args["-r"];

    // construct the defaulr run manager
    auto *pRunManager = new G4RunManager();
    pRunManager->SetUserInitialization(new BucketDetectorConstruction());
    pRunManager->SetUserInitialization(new BucketPhysicsList());
    pRunManager->SetUserInitialization(new BucketActionInitialization());

    // construct UI manager and define preinit states
    auto *pUIManager = G4UImanager::GetUIpointer();
    execute_macro(pUIManager, preinit);

    // initialize G4 kernel
    pRunManager->Initialize();

    // construct the visualization manager
    G4VisManager* pVisManager = new G4VisExecutive();
    pVisManager->SetVerboseLevel(0);
    pVisManager->Initialize();

    // start session
    execute_macro(pUIManager, visualization);
    execute_macro(pUIManager, run);

    delete pVisManager;
    delete pRunManager;

    return 0;
}
