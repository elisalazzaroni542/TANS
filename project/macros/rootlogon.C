{
    gSystem->AddIncludePath("-I../headers");

    gSystem->AddDynamicPath("../src");
    
    gSystem->Load("Event_cxx.so");
    gSystem->Load("Trajectory_cxx.so");
    gSystem->Load("HitPoint_cxx.so");
    gSystem->Load("Point_cxx.so");



}
