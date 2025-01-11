#include <Riostream.h>
#include <TStopwatch.h>
#include <TFile.h>
#include <TTree.h>
#include <TSystem.h>
#include <TClonesArray.h>
#include "../headers/Event.h"         // TODO: remove relative paths
#include "../headers/Trajectory.h"
#include "../headers/HitPoint.h"
#include "../headers/Point.h"




// Main function for testing
int main() {
    // Create a Trajectory object with known angles
    Trajectory traj(1.0, 0.5); // phi = 1 rad, theta = 0.5 rad

    // Create another trajectory for computation
    Trajectory result(0, 0);

    // Perform the scattering rotation
    result.MSRotateParC(traj);

    // Output the computed vector
    std::cout << "Resulting vector (TParC):\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "TParC[" << i << "] = " << result.TParC[i] << "\n";
    }

    return 0;
}
