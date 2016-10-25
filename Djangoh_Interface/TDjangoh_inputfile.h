
#include <string>

using namespace std;

/*

  All variables quasi-static for the Djangoh input file

*/

string outfilename = "TDjangoh";
int iounits[3] = {1,1,1};
string pr_beam[2] = {"0D0","0D0"};
int gsw_param[11] = {2,1,3,1,0,0,2,1,1,1,1};
int kinem_cut_var = 3;
string kinem_cut[7] = {"0.0001D0","1.00D0","0.01D0","0.95D0","1.0D0","1D5","1.4D0"};
string egam_min = "0D0";
int int_opt_nc[9] = {1, 18, 18, 18, 0, 0, 0, 0, 0};
int int_opt_cc[4] = {0, 0, 0, 0};
int int_only = 0;
int int_point = 3000;
int sam_opt_nc[9] = {1, 1, 1, 1, 0, 0, 0, 0, 0};
int sam_opt_cc[4] = {0, 0, 0, 0};
string nucl_e = "0.0D0";
int A = 0;
int Z = 0;
int structfunc[3] = {0, 2, 10150};
float flong[3] = {111, 0.01, 0.03};
float alfas[4] = {1, 1, 0.20, 0.235};
int nflavors[2] = {0, 3};
int rndm_seeds[2] = {-1, -1};
int start = 1;
float sophia = 1.5;
int out_lep = 1;
int frag = 1;
int cascades = 12;
int max_virt = 5;