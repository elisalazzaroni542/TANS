{
    gSystem->AddIncludePath("-I/home/elisa/TANS/project/headers");

    if (!gSystem->AccessPathName("/home/elisa/TANS/project/src/Event_cxx.so")) {
        gSystem->Load("/home/elisa/TANS/project/src/Event_cxx.so");
    }


    if (!gSystem->AccessPathName("/home/elisa/TANS/project/src/Trajectory_cxx.so")) {
        gSystem->Load("/home/elisa/TANS/project/src/Trajectory_cxx.so");
    }
    
    if (!gSystem->AccessPathName("/home/elisa/TANS/project/src/HitPoint_cxx.so")) {
        gSystem->Load("/home/elisa/TANS/project/src/HitPoint_cxx.so");
    }

    if (!gSystem->AccessPathName("/home/elisa/TANS/project/src/Point_cxx.so")) {
        gSystem->Load("/home/elisa/TANS/project/src/Point_cxx.so");
    }

}
