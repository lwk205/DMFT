#include "CHM.h"
#include <complex>

using namespace std;

class GRID;
class Result;

class MHCHM : public CHM
{
  protected:
    void Defaults();
    void ReleaseMemory();
    
    complex<double>* rho;
    complex<double>* rhot;
    complex<double>* Sigmat;

    void get_G0();
    void get_rhot();
    void get_Sigmat();
    void get_Sigma();
    void get_G();
    
    virtual bool SolveSIAM();

    void FastFourier(complex<double>* X, complex<double>* Y, double delta, int isign);
    //void FastFourier(complex<double>* X, double delta, int isign);
    //void FastFourier(double data[], int N, int isign);

  public:
    MHCHM();
    MHCHM(const char* ParamsFN);
    ~MHCHM();

    void SetUseBethe(bool UseBethe);
};
