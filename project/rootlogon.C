{
    gSystem->AddIncludePath("-I/home/dcristof/Desktop/TANS/project/headers");

    if (!gSystem->AccessPathName("/home/dcristof/Desktop/TANS/project/src/Event_cxx.so")) {
        gSystem->Load("/home/dcristof/Desktop/TANS/project/src/Event_cxx.so");
    }


    if (!gSystem->AccessPathName("/home/dcristof/Desktop/TANS/project/src/Trajectory_cxx.so")) {
        gSystem->Load("/home/dcristof/Desktop/TANS/project/src/Trajectory_cxx.so");
    }
    


}
