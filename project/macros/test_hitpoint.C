#include <Riostream.h>
#include "HitPoint.h"
#include "Trajectory.h"
#include "Event.h"
#include <TRandom3.h>
#include <TFile.h>
#include <math.h>
#include <TH1F.h>



void printDelta(Event& ev, Trajectory& traj, double Rcil){
    cout<<"---Rcil="<<Rcil<<"---"<<endl;
    double first = pow(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1),2);
    double second = (pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2))*(pow(ev.GetVertix(0),2)+pow(ev.GetVertix(1),2)-pow(Rcil,2));
    double delta = pow(ev.GetVertix(0)*traj.GetParC(0)+ev.GetVertix(1)*traj.GetParC(1),2)-(pow(traj.GetParC(0),2)+pow(traj.GetParC(1),2))*(pow(ev.GetVertix(0),2)+pow(ev.GetVertix(1),2)-pow(Rcil,2));

    cout<<"1st fact:"<<first<<endl;
    cout<<"2st fact:"<<second<<endl;
    cout<<"Function Delta:"<<first - second<<endl;
    cout<<"Class Delta:"<<delta<<endl;


}


void test(unsigned int seed=123, unsigned int events=10){

    Event e(seed);
    Trajectory t(seed);
    //HitPoint hIn, hOut;


    TH1F* thetaHist = t.LoadDistribution("heta2");


    e.SetVertix(3);

    for(unsigned int i=0; i< events; ++i){

        t.SetThetaNPhi(thetaHist);
        t.SetParC(3);

        e.PrintEvent();
        t.PrintTrajectory();

        //printDelta(e, t, 4);
        //printDelta(e, t, 8);
//        cout<<"<=========================Default initialized hit=========================>"<<endl;
        HitPoint h0In, h0Out, hIn(e, t, 4), hOut(e, t, 7);
        cout<<"----Inner hit-----"<<endl; 
        hIn.SetPoint(e, t);
        hIn.PrintHit();
        cout<<"----Outer hit-----"<<endl;
        hOut.SetPoint(e, t);
        hOut.PrintHit();

//       cout<<"<=========================Standard initialized hit=========================>"<<endl;
//       cout<<"----Inner hit-----"<<endl;        
//       h0In.SetPoint(e, t);
//       h0In.PrintHit();
//       cout<<"----Outer hit-----"<<endl;        
//       h0Out.SetPoint(e, t);
//       h0Out.PrintHit();



        cout<<endl;
        cout<<"<=========================================================================>"<<endl;
        cout<<"<=========================================================================>"<<endl;
        cout<<"<=========================================================================>"<<endl;
        cout<<endl;
    }


}