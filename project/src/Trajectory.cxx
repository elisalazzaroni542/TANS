#include "Trajectory.h"




ClassImp (Trajectory)

  Trajectory::Trajectory(): Event(),
  Ttheta(0),
  Tphi(0),
  TparC({-999, -999, -999})
  {
  }

  Trajectory::Trajectory(unsigned int seed):Event(seed),
  Ttheta(0),
  Tphi(0),
  TparC({-999, -999, -999})
  {
  }
  
  Trajectory::Trajectory (const Trajectory &source):
    Event(source),
    Ttheta(source.Ttheta),
    Tphi(source.Tphi),
    TparC(source.TparC)
    {
    }

  Trajectory::~Trajectory(){
  }


  double Trajectory::GetParC(unsigned const int i) const{
    if(i<TparC.size()){
      return TparC[i];
    }
    else{cout<<"Invalid trajectory input"<<endl;
        return 0;
        }
  }


  void Trajectory::PrintTrajectory()const{
    cout<<"Theta: "<<Ttheta<<endl;
    cout<<"Phi: "<<Tphi<<endl;
    cout<<"Parameters (c1,c2,c3): "<<endl;
    if (TparC.size()>0){
      for(unsigned int i=0;i<TparC.size();++i){
        cout<<TparC[i]<<" ,";
      }
      cout<<endl;
    }
    else{
      cout<<"No parameter found"<<endl;
    }
  }


//---------------MULTIPLE SCATTERING-------------------------------------------------------------------


void Trajectory::SetThetaNPhi(TH1F* customHist) {
    double heta = RndmCustom(customHist);
    Ttheta = 2 * atan(exp(-heta));
    Tphi = RndmUni(0, 2*M_PI);

}

void Trajectory::SetParC() {

  double  sinTheta = sin(Ttheta);

  TparC[0] = sinTheta * cos(Tphi);
  TparC[1] = sinTheta * sin(Tphi);
  TparC[2] = cos(Ttheta);
}


/*
  void Trajectory::MSRotateParC(Trajectory &traj){
    double thetap=RndmGaus(0,0.001);//1 mrad sigma
    double phip=RndmUni(0,2*M_PI);
    double mat[3][3];
    mat[0][0]=-sin(traj.GetPhi());
    mat[1][0]=cos(traj.GetPhi());
    mat[2][0]=0.;
    mat[0][1]=-cos(traj.GetPhi())*cos(traj.GetTheta());
    mat[1][1]=-cos(traj.GetTheta())*sin(traj.GetPhi());
    mat[2][1]=sin(traj.GetTheta());
    mat[0][2]=sin(traj.GetTheta())*cos(traj.GetPhi());
    mat[1][2]=sin(traj.GetTheta())*sin(traj.GetPhi());
    mat[2][2]=cos(traj.GetTheta());

    double newang[3];
    newang[0]=sin(thetap)*cos(phip);
    newang[1]=sin(thetap)*sin(phip);
    newang[2]=cos(thetap);


    for(int i=0; i<3; ++i){
      TparC[i]=0.;
      for(int j=0; j<3; ++j){
        TparC[i]+=mat[i][j]*newang[j];
      }
    }
  }    */

void Trajectory::MSRotateParC(Trajectory &traj) {

    const double thetap = RndmGaus(0, 0.001);
    const double phip = RndmUni(0, 2*M_PI);
    const double sinThetap = sin(thetap);
    const double cosThetap = cos(thetap);
    const double sinPhip = sin(phip);
    const double cosPhip = cos(phip);
    
    const double sinTheta = sin(traj.GetTheta());
    const double cosTheta = cos(traj.GetTheta());
    const double sinPhi = sin(traj.GetPhi());
    const double cosPhi = cos(traj.GetPhi());

    const double m01 = -sinPhi;
    const double m02 = -cosPhi * cosTheta;
    const double m03 = sinTheta * cosPhi;
    const double m11 = cosPhi;
    const double m12 = -cosTheta * sinPhi;
    const double m13 = sinTheta * sinPhi;
    const double m23 = cosTheta;

    const double newang1 = sinThetap * cosPhip;
    const double newang2 = sinThetap * sinPhip;
    const double newang3 = cosThetap;

    TparC[0] = m01 * newang1 + m02 * newang2 + m03 * newang3;
    TparC[1] = m11 * newang1 + m12 * newang2 + m13 * newang3;
    TparC[2] = m23 * newang3;
}

