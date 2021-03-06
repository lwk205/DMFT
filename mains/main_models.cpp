#include "../source/CHM.h"
#include "../source/GRID.h"
#include "../source/Result.h"
#include "../source/routines.h"
#include "../source/Input.h"

int main()
{
  double T = 0.119;
  //double U = 2.45;

  for (double U = 2.3; U<3.2; U+=0.05)
  {
  // initialize grid with params read from the input file "params"
  GRID grid("params");

  // initialize result object for discretizing relevant functions on the above grid
  Result result(&grid);

  // fill in the omega grid of the object
  grid.assign_omega(result.omega);

  // read the hopping amplitude that will be used in CHM from the input file, to use it in initializeing Delta and NIDOS
  Input input("params");
  double t = 0.5; // always set a default value in case parameter is not found in the input file!!!
  input.ReadParam(t,"CHM::t");
  

  // initialize hybridization function that will be used for the initial guess 
  InitDelta( DOStypes::SemiCircle,	// type of DOS 
             grid.get_N(), 		// total number of omega grid points 
             0.5, 0.0, 0.01, t, 	// hybridization-V, chemical potential-mu (defines the omega shift), broadening eta, and hopping amplitude
             result.omega, result.Delta);  // omega-grid array and the array in which Delta will be stored

  // initiali the non-interacting DOS (NIDOS) that will be used for calculating G 
  // (...unless bethe-specific self-consistency is used. in that case you don't need this function call)
  InitDOS( DOStypes::SemiCircle, 			// type of DOS 
           t, 					// hopping amplitude
           grid.get_N(), result.omega, result.NIDOS);	// total number of omega grid points, omega-grid array, the array in which NIDOS will be stored

  // read the occupation number and put it in result to be used as input for CHM
  double n = 0.5; // always set a default value in case parameter is not found in the input file!!!
  input.ReadParam(n,"main::n");
  result.n = n;
  
  // create a CHM object with params set in the input file "params". Those params not found in the input file will be set to their default values
  CHM chm("params");
  chm.SetParams(U,T,t);
  // perform the DMFT CHM calculation on the result object in which initial Delta, NIDOS and n are now found and will be used as input
  chm.Run(&result);
  
  // now print the result to a file named accordingly to the parameters of the calculation
  char FN[50];
  sprintf( FN, "CHM.U%.3f.T%.3f", U, T );
  result.PrintResult(FN);
  result.PrintModel(U, T );
  //printf( "Conductivity 500.0, 3.0: %f\n", result.Conductivity( chm.get_T() , 0.5*chm.get_U() , 400.0, 400.0, 10000.0 ) );

  PrintFunc("Sigma", grid.get_N(), result.Sigma, result.omega);
   
  }
  return 0;
}
