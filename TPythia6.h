// @(#)root/pythia6:$Id$
    2 // Author: Rene Brun   19/10/99
    3
    4 /*************************************************************************
    5  * Copyright (C) 1995-2000, Rene Brun and Fons Rademakers.               *
    6  * All rights reserved.                                                  *
    7  *                                                                       *
    8  * For the licensing terms see $ROOTSYS/LICENSE.                         *
    9  * For the list of contributors see $ROOTSYS/README/CREDITS.             *
   10  *************************************************************************/
   11
   12 #ifndef PYTHIA_TPythia6
   13 #define PYTHIA_TPythia6
   14
   15 ////////////////////////////////////////////////////////////////////////////////
   16 //                                                                            //
   17 // TPythia6                                                                   //
   18 //                                                                            //
   19 // TPythia is an interface class to F77 version of Pythia 6.2                 //
   20 // CERNLIB event generators, written by T.Sjostrand.                          //
   21 // For the details about these generators look at Pythia/Jetset manual:       //
   22 //                                                                            //
   23 // ******************************************************************************
   24 // ******************************************************************************
   25 // **                                                                          **
   26 // **                                                                          **
   27 // **              *......*                  Welcome to the Lund Monte Carlo!  **
   28 // **         *:::!!:::::::::::*                                               **
   29 // **      *::::::!!::::::::::::::*          PPP  Y   Y TTTTT H   H III   A    **
   30 // **    *::::::::!!::::::::::::::::*        P  P  Y Y    T   H   H  I   A A   **
   31 // **   *:::::::::!!:::::::::::::::::*       PPP    Y     T   HHHHH  I  AAAAA  **
   32 // **   *:::::::::!!:::::::::::::::::*       P      Y     T   H   H  I  A   A  **
   33 // **    *::::::::!!::::::::::::::::*!       P      Y     T   H   H III A   A  **
   34 // **      *::::::!!::::::::::::::* !!                                         **
   35 // **      !! *:::!!:::::::::::*    !!       This is PYTHIA version 6.205      **
   36 // **      !!     !* -><- *         !!       Last date of change:  1 Mar 2002  **
   37 // **      !!     !!                !!                                         **
   38 // **      !!     !!                !!       Now is  0 Jan 2000 at  0:00:00    **
   39 // **      !!                       !!                                         **
   40 // **      !!        lh             !!       Disclaimer: this program comes    **
   41 // **      !!                       !!       without any guarantees. Beware    **
   42 // **      !!                 hh    !!       of errors and use common sense    **
   43 // **      !!    ll                 !!       when interpreting results.        **
   44 // **      !!                       !!                                         **
   45 // **      !!                                Copyright T. Sjostrand (2001)     **
   46 // **                                                                          **
   47 // ** An archive of program versions and documentation is found on the web:    **
   48 // ** http://www.thep.lu.se/~torbjorn/Pythia.html                              **
   49 // **                                                                          **
   50 // ** When you cite this program, currently the official reference is          **
   51 // ** T. Sjostrand, P. Eden, C. Friberg, L. Lonnblad, G. Miu, S. Mrenna and    **
   52 // ** E. Norrbin, Computer Physics Commun. 135 (2001) 238.                     **
   53 // ** The large manual is                                                      **
   54 // ** T. Sjostrand, L. Lonnblad and S. Mrenna, LU TP 01-21 [hep-ph/0108264].   **
   55 // ** Also remember that the program, to a large extent, represents original   **
   56 // ** physics research. Other publications of special relevance to your        **
   57 // ** studies may therefore deserve separate mention.                          **
   58 // **                                                                          **
   59 // ** Main author: Torbjorn Sjostrand; Department of Theoretical Physics 2,    **
   60 // **   Lund University, Solvegatan 14A, S-223 62 Lund, Sweden;                **
   61 // **   phone: + 46 - 46 - 222 48 16; e-mail: torbjorn@thep.lu.se              **
   62 // ** Author: Leif Lonnblad; Department of Theoretical Physics 2,              **
   63 // **   Lund University, Solvegatan 14A, S-223 62 Lund, Sweden;                **
   64 // **   phone: + 46 - 46 - 222 77 80; e-mail: leif@thep.lu.se                  **
   65 // ** Author: Stephen Mrenna; Computing Division, Simulations Group,           **
   66 // **   Fermi National Accelerator Laboratory, MS 234, Batavia, IL 60510, USA; **
   67 // **   phone: + 1 - 630 - 840 - 2556; e-mail: mrenna@fnal.gov                 **
   68 // ** Author: Peter Skands; Department of Theoretical Physics 2,               **
   69 // **   Lund University, Solvegatan 14A, S-223 62 Lund, Sweden;                **
   70 // **   phone: + 46 - 46 - 222 31 92; e-mail: zeiler@thep.lu.se                **
   71 // **                                                                          **
   72 // **                                                                          **
   73 // ******************************************************************************
   74 //#ifdef __GNUG__
   75 //#pragma interface
   76 //#endif
   77
   78 #ifndef ROOT_TPythia6Calls
   79 #include "TPythia6Calls.h"
   80 #endif
   81
   82 #ifndef ROOT_TGenerator
   83 #include "TGenerator.h"
   84 #endif
   85
   86 #ifndef ROOT_TObjArray
   87 #include "TObjArray.h"
   88 #endif
   89
   90 class TPythia6 : public TGenerator {
   91
   92 protected:
   93    static  TPythia6* fgInstance;
   94    // PYTHIA6 common-blocks
   95    Pyjets_t*  fPyjets;
   96    Pydat1_t*  fPydat1;
   97    Pydat2_t*  fPydat2;
   98    Pydat3_t*  fPydat3;
   99    Pydat4_t*  fPydat4;
  100    Pydatr_t*  fPydatr;
  101    Pysubs_t*  fPysubs;
  102    Pypars_t*  fPypars;
  103    Pyint1_t*  fPyint1;
  104    Pyint2_t*  fPyint2;
  105    Pyint3_t*  fPyint3;
  106    Pyint4_t*  fPyint4;
  107    Pyint5_t*  fPyint5;
  108    Pyint6_t*  fPyint6;
  109    Pyint7_t*  fPyint7;
  110    Pyint8_t*  fPyint8;
  111    Pyint9_t*  fPyint9;
  112    Pymssm_t*  fPymssm;
  113    Pyssmt_t*  fPyssmt;
  114    Pyints_t*  fPyints;
  115    Pybins_t*  fPybins;
  116    // ****** cleanup stuff (thanks Jim K.)
  117    class  TPythia6Cleaner {
  118    public:
  119       TPythia6Cleaner();
  120       ~TPythia6Cleaner();
  121    };
  122    friend class TPythia6Cleaner;
  123
  124    TPythia6(const TPythia6&);            // Cannot be copied
  125    TPythia6& operator=(const TPythia6&); // Cannot be copied
  126
  127 public:
  128    // ****** constructors and destructor
  129    TPythia6();
  130    virtual ~TPythia6();
  131
  132    static TPythia6 *Instance();
  133
  134    // ****** accessors
  135    // FORTRAN indexing in accessing the arrays,
  136    // indices start from 1 !!!!!
  137
  138    // ****** access to PYTHIA6 common-blocks
  139
  140    // ****** /PYJETS/
  141
  142    Pyjets_t*   GetPyjets        ()           { return fPyjets; }
  143    int         GetN             ()           { return fPyjets->N; }
  144    int         GetNPAD          ()           { return fPyjets->NPAD; }
  145    int         GetK(int ip, int i)           { return fPyjets->K[i-1][ip-1]; }
  146    double      GetP(int ip, int i)           { return fPyjets->P[i-1][ip-1]; }
  147    double      GetV(int ip, int i)           { return fPyjets->V[i-1][ip-1]; }
  148
  149    void        SetN     (int n)              { fPyjets->N = n;    }
  150    void        SetNPAD  (int n)              { fPyjets->NPAD = n;    }
  151    void        SetK(int ip, int i, int k)    { fPyjets->K[i-1][ip-1] = k; }
  152    void        SetP(int ip, int i, double p) { fPyjets->P[i-1][ip-1] = p;    }
  153    void        SetV(int ip, int i, double v) { fPyjets->V[i-1][ip-1] = v;    }
  154
  155    // ****** /PYDAT1/
  156
  157    Pydat1_t*   GetPydat1   () { return fPydat1; }
  158    int         GetMSTU(int i) { return fPydat1->MSTU[i-1]; }
  159    double      GetPARU(int i) { return fPydat1->PARU[i-1]; }
  160    int         GetMSTJ(int i) { return fPydat1->MSTJ[i-1]; }
  161    double      GetPARJ(int i) { return fPydat1->PARJ[i-1]; }
  162
  163    void        SetMSTU(int i, int m   ) { fPydat1->MSTU[i-1] = m; }
  164    void        SetPARU(int i, double p) { fPydat1->PARU[i-1] = p; }
  165    void        SetMSTJ(int i, int m   ) { fPydat1->MSTJ[i-1] = m; }
  166    void        SetPARJ(int i, double p) { fPydat1->PARJ[i-1] = p; }
  167
  168    // ****** /PYDAT2/
  169
  170    Pydat2_t*   GetPydat2           () { return fPydat2; }
  171    int         GetKCHG(int ip, int i) { return fPydat2->KCHG[i-1][ip-1]; }
  172    double      GetPMAS(int ip, int i) { return fPydat2->PMAS[i-1][ip-1]; }
  173    double      GetPARF        (int i) { return fPydat2->PARF[i-1]; }
  174    double      GetVCKM(int i,  int j) { return fPydat2->VCKM[j-1][i-1]; }
  175
  176    void        SetKCHG(int ip, int i, int k   ) { fPydat2->KCHG[i-1][ip-1] = k; }
  177    void        SetPMAS(int ip, int i, double m) { fPydat2->PMAS[i-1][ip-1] = m; }
  178    void        SetPARF        (int i, double p) { fPydat2->PARF[i-1]       = p; }
  179    void        SetVCKM (int i, int j, double v) { fPydat2->VCKM[j-1][i-1]  = v; }
  180
  181    // ****** /PYDAT3/
  182
  183    Pydat3_t*   GetPydat3() { return fPydat3; }
  184    int         GetMDCY(int i, int j) { return fPydat3->MDCY[j-1][i-1]; }
  185    int         GetMDME(int i, int j) { return fPydat3->MDME[j-1][i-1]; }
  186    double      GetBRAT       (int i) { return fPydat3->BRAT[i-1]; }
  187    int         GetKFDP(int i, int j) { return fPydat3->KFDP[j-1][i-1]; }
  188
  189    void        SetMDCY(int i, int j, int m) { fPydat3->MDCY[j-1][i-1] = m; }
  190    void        SetMDME(int i, int j, int m) { fPydat3->MDME[j-1][i-1] = m; }
  191    void        SetBRAT(int i, double b)     { fPydat3->BRAT[i-1]      = b; }
  192    void        SetKFDP(int i, int j, int k) { fPydat3->KFDP[j-1][i-1] = k; }
  193
  194    // ****** /PYDAT4/
  195
  196    Pydat4_t*   GetPydat4() { return fPydat4; }
  197
  198    // ****** /PYDATR/ - random number generator info
  199
  200    Pydatr_t*   GetPydatr   () { return fPydatr; }
  201    int         GetMRPY(int i) { return fPydatr->MRPY[i-1]; }
  202    double      GetRRPY(int i) { return fPydatr->RRPY[i-1]; }
  203
  204    void        SetMRPY(int i, int m)    { fPydatr->MRPY[i-1] = m; }
  205    void        SetRRPY(int i, double r) { fPydatr->RRPY[i-1] = r; }
  206
  207    // ****** /PYSUBS/
  208
  209    Pysubs_t*   GetPysubs     () { return fPysubs; }
  210    int         GetMSEL       () { return fPysubs->MSEL; }
  211    int         GetMSELPD     () { return fPysubs->MSELPD; }
  212    int         GetMSUB  (int i) { return fPysubs->MSUB[i-1]; }
  213    double      GetCKIN  (int i) { return fPysubs->CKIN[i-1]; }
  214    Int_t       GetKFIN(int i, int j)  {return fPysubs->KFIN[j+40][i-1]; }
  215
  216    void        SetMSEL   (int m)           { fPysubs->MSEL      = m; }
  217    void        SetMSELPD (int m)           { fPysubs->MSELPD    = m; }
  218    void        SetMSUB   (int i, int m)    { fPysubs->MSUB[i-1] = m; }
  219    void        SetCKIN   (int i, double c) { fPysubs->CKIN[i-1] = c; }
  220    void        SetKFIN(int i, int j, Int_t kfin=1) { fPysubs->KFIN[j+40][i-1] = kfin; }
  221
  222    // ****** /PYPARS/
  223
  224    Pypars_t*   GetPypars() { return fPypars; }
  225    int         GetMSTP(int i) { return fPypars->MSTP[i-1]; }
  226    double      GetPARP(int i) { return fPypars->PARP[i-1]; }
  227    int         GetMSTI(int i) { return fPypars->MSTI[i-1]; }
  228    double      GetPARI(int i) { return fPypars->PARI[i-1]; }
  229
  230    void        SetMSTP   (int i, int    m) { fPypars->MSTP[i-1] = m; }
  231    void        SetPARP   (int i, double p) { fPypars->PARP[i-1] = p; }
  232    void        SetMSTI   (int i, int    m) { fPypars->MSTI[i-1] = m; }
  233    void        SetPARI   (int i, double p) { fPypars->PARI[i-1] = p; }
  234
  235    // ****** /PYINT1/
  236
  237    Pyint1_t*   GetPyint1() { return fPyint1; }
  238    int         GetMINT(int i) { return fPyint1->MINT[i-1]; }
  239    double      GetVINT(int i) { return fPyint1->VINT[i-1]; }
  240
  241    void        SetMINT(int i, int m   ) { fPyint1->MINT[i-1] = m; }
  242    void        SetVINT(int i, double v) { fPyint1->VINT[i-1] = v; }
  243
  244    // ****** /PYINT2/ and /PYINT3/
  245
  246    Pyint2_t*   GetPyint2() { return fPyint2; }
  247    Pyint3_t*   GetPyint3() { return fPyint3; }
  248
  249    // ****** /PYINT4/
  250
  251    Pyint4_t*   GetPyint4() { return fPyint4; }
  252    int         GetMWID      (int i) { return fPyint4->MWID[i-1]; }
  253    double      GetWIDS(int i,int j) { return fPyint4->WIDS[j-1][i-1]; }
  254
  255    void        SetMWID(int i, int m)           { fPyint4->MWID[i-1]      = m; }
  256    void        SetWIDS(int i, int j, double w) { fPyint4->WIDS[j-1][i-1] = w; }
  257
  258    // ****** / PYINT5/
  259
  260    Pyint5_t*   GetPyint5() { return fPyint5; }
  261    int         GetNGENPD() { return fPyint5->NGENPD; }
  262    void        SetNGENPD(int n) { fPyint5->NGENPD = n; }
  263
  264    // ****** /PYINT6/
  265
  266    Pyint6_t*   GetPyint6   () { return fPyint6; }
  267    char*       GetPROC(int i) { fPyint6->PROC[i][27]=0; return fPyint6->PROC[i]; }
  268
  269    Pyint7_t*   GetPyint7() { return fPyint7; }
  270    Pyint8_t*   GetPyint8() { return fPyint8; }
  271    Pyint9_t*   GetPyint9() { return fPyint9; }
  272
  273    // ****** /PYMSSM/ - indexing in FORTRAN starts
  274    // from 0!
  275
  276    Pymssm_t*   GetPymssm()    { return fPymssm; }
  277    int         GetIMSS(int i) { return fPymssm->IMSS[i]; }
  278    double      GetRMSS(int i) { return fPymssm->RMSS[i]; }
  279
  280    void        SetIMSS(int i, int    m) { fPymssm->IMSS[i] = m; }
  281    void        SetRMSS(int i, double r) { fPymssm->RMSS[i] = r; }
  282
  283    // ****** /PYSSMT/
  284
  285    Pyssmt_t*   GetPyssmt()           { return fPyssmt; }
  286    double      GetZMIX(int i, int j) { return fPyssmt->ZMIX[j-1][i-1]; }
  287    double      GetUMIX(int i, int j) { return fPyssmt->UMIX[j-1][i-1]; }
  288    double      GetVMIX(int i, int j) { return fPyssmt->VMIX[j-1][i-1]; }
  289    double      GetSMZ        (int i) { return fPyssmt->SMZ[i-1]; }
  290    double      GetSMW        (int i) { return fPyssmt->SMW[i-1]; }
  291
  292    void        SetZMIX(int i, int j, double z) { fPyssmt->ZMIX[j-1][i-1] = z; }
  293    void        SetUMIX(int i, int j, double u) { fPyssmt->UMIX[j-1][i-1] = u; }
  294    void        SetSMZ (int i, double s)        { fPyssmt->SMZ[i-1]       = s; }
  295    void        SetSMW (int i, double s)        { fPyssmt->SMW[i-1]       = s; }
  296
  297    Pyints_t*   GetPyints() { return fPyints; }
  298    Pybins_t*   GetPybins() { return fPybins; }
  299
  300    // ****** TPYTHIA routines
  301
  302    void             GenerateEvent();
  303
  304    void             Initialize(const char *frame, const char *beam, const char *target, float win);
  305
  306    Int_t            ImportParticles(TClonesArray *particles, Option_t *option="");
  307    TObjArray       *ImportParticles(Option_t *option="");
  308
  309    void             OpenFortranFile(int lun, char* name);
  310    void             CloseFortranFile(int lun);
  311    int              Pychge(int kf);
  312    int              Pycomp(int kf);
  313    void             Pydiff();
  314    void             Pyedit(int medit);
  315    void             Pyevnt();
  316    void             Py1ent(Int_t line, Int_t kf, Double_t pe, Double_t theta, Double_t phi);
  317    void             Pyexec();
  318    void             Pyhepc(int mconv);
  319    void             Pygive(const char *param);
  320    void             Pyinit(char* frame, char* beam, char* target, double wint);
  321    void             Pylist(int flag);
  322    double           Pymass(int kf);
  323    void             Pyname(int kf, char* name);
  324    double           Pyr(int idummy);
  325    void             Pyrget(int lun, int move);
  326    void             Pyrset(int lun, int move);
  327    void             Pystat(int flag);
  328    void             Pytest(int flag);
  329    void             Pytune(int itune);
  330    void             Pyupda(int mupda, int lun);
  331    void             SetupTest();
  332
  333    ClassDef(TPythia6,0)  //Interface to Pythia6.1 Event Generator
  334 };
  335
  336 #endif
