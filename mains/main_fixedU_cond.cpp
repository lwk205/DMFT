#include "../source/CHM.h"
#include "../source/GRID.h"
#include "../source/Result.h"
#include "../source/routines.h"
#include "../source/Input.h"


int main(int argc, char *argv[])
{
  double U;
  int StartFromSC;
  if (argc == 3)
  {
     U = atof(argv[1]);
     StartFromSC = atoi(argv[2]);
  }
  else
  {  //read input - get U and T
     FILE * f = fopen("input","r");  
     fscanf(f,"%le", &U);
     fclose(f);
  }

  GRID grid("params");
  Result result(&grid);
  grid.assign_omega(result.omega);

  Input input("params");
  double t = 0.5;
  input.ReadParam(t,"CHM::t"); 

  InitDelta( DOStypes::SemiCircle,	
             grid.get_N(), 		
             0.5, 0.0, 0.01, t, 	
             result.omega, result.Delta);

  InitDOS( DOStypes::SemiCircle, 		
           t, 					
           grid.get_N(), result.omega, result.NIDOS);	

  double n = 0.5; 
  input.ReadParam(n,"main::n");
  result.n = n;

  char condFN[50];
  sprintf(condFN,"cond.U%.3f%s",U, (StartFromSC==1) ? ".FromSC" : "");

  for(double T = 0.220; T > 0.001; T -= 0.002)
  {
    CHM chm("params");
    chm.SetParams(U,T,t);

    chm.Run(&result);
  
    char FN[50];
    sprintf( FN, "CHM.U%.3f.T%.3f%s", U, T, (StartFromSC==1) ? ".FromSC" : "");
    result.PrintResult(FN);

    double cond = result.Conductivity( T , 0.5*U , 400.0, 400.0, 10000.0 );
    printf( "Conductivity: %f\n", cond );

    FILE* condFile = fopen(condFN,"a");
    fprintf(condFile,"%.15le %.15le %.15le\n", T, 1.0/cond, cond);
    fclose(condFile);

    if (StartFromSC == 1)
      InitDelta( DOStypes::SemiCircle,	
                 grid.get_N(), 		
                 0.5, 0.0, 0.01, t, 	
                 result.omega, result.Delta);    
  }
  return 0;
}
