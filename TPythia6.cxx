// @(#)root/pythia6:$Id$
// Author: Rene Brun   19/10/99

/** \class TPythia6
   \ingroup pythia6

TPythia is an interface class to F77 version of Pythia 6.2

To use this class you must install a version of pythia6.
See the installation instructions at
 http://root.cern.ch/root/Install.html

CERNLIB event generators, written by T.Sjostrand.
For the details about these generators look at Pythia/Jetset manual:

\verbatim
******************************************************************************
**                                                                          **
**                                                                          **
**  PPP  Y   Y TTTTT H   H III   A        JJJJ EEEE TTTTT  SSS  EEEE TTTTT  **
**  P  P  Y Y    T   H   H  I   A A          J E      T   S     E      T    **
**  PPP    Y     T   HHHHH  I  AAAAA         J EEE    T    SSS  EEE    T    **
*  P      Y     T   H   H  I  A   A      J  J E      T       S E      T    **
   24 **  P      Y     T   H   H III A   A       JJ  EEEE   T    SSS  EEEE   T    **
   25 **                                                                          **
   26 **                                                                          **
   27 **              *......*                  Welcome to the Lund Monte Carlo!  **
   28 **         *:::!!:::::::::::*                                               **
   29 **      *::::::!!::::::::::::::*            This is PYTHIA version 5.720    **
   30 **    *::::::::!!::::::::::::::::*        Last date of change: 29 Nov 1995  **
   31 **   *:::::::::!!:::::::::::::::::*                                         **
   32 **   *:::::::::!!:::::::::::::::::*         This is JETSET version 7.408    **
   33 **    *::::::::!!::::::::::::::::*!       Last date of change: 23 Aug 1995  **
   34 **      *::::::!!::::::::::::::* !!                                         **
   35 **      !! *:::!!:::::::::::*    !!                 Main author:            **
   36 **      !!     !* -><- *         !!              Torbjorn Sjostrand         **
   37 **      !!     !!                !!        Dept. of theoretical physics 2   **
   38 **      !!     !!                !!              University of Lund         **
   39 **      !!                       !!                Solvegatan 14A           **
   40 **      !!        ep             !!             S-223 62 Lund, Sweden       **
   41 **      !!                       !!          phone: +46 - 46 - 222 48 16    **
   42 **      !!                 pp    !!          E-mail: torbjorn@thep.lu.se    **
   43 **      !!   e+e-                !!                                         **
   44 **      !!                       !!         Copyright Torbjorn Sjostrand    **
   45 **      !!                                     and CERN, Geneva 1993        **
   46 **                                                                          **
   47 **                                                                          **
   48 ** The latest program versions and documentation is found on WWW address    **
   49 ** http://thep.lu.se/tf2/staff/torbjorn/Welcome.html                        **
   50 **                                                                          **
   51 ** When you cite these programs, priority should always be given to the     **
   52 ** latest published description. Currently this is                          **
   53 ** T. Sjostrand, Computer Physics Commun. 82 (1994) 74.                     **
   54 ** The most recent long description (unpublished) is                        **
   55 ** T. Sjostrand, LU TP 95-20 and CERN-TH.7112/93 (revised August 1995).     **
   56 ** Also remember that the programs, to a large extent, represent original   **
   57 ** physics research. Other publications of special relevance to your        **
   58 ** studies may therefore deserve separate mention.                          **
   59 **                                                                          **
   60 **                                                                          **
   61 ******************************************************************************
   62 \endverbatim
   63 */
   64
   65 #include "TPythia6.h"
   66
   67 #include "TClonesArray.h"
   68 #include "TMCParticle.h"
   69 #include "TParticle.h"
   70
   71 TPythia6*  TPythia6::fgInstance = 0;
   72
   73
   74 #ifndef WIN32
   75 # define pydiff pydiff_
   76 # define pyevnt pyevnt_
   77 # define pyinit pyinit_
   78 # define pychge pychge_
   79 # define pycomp pycomp_
   80 # define pyedit pyedit_
   81 # define pyexec pyexec_
   82 # define pyhepc pyhepc_
   83 # define pygive pygive_
   84 # define pylist pylist_
   85 # define pymass pymass_
   86 # define pyname pyname_
   87 # define pyr    pyr_
   88 # define pyrget pyrget_
   89 # define pyrset pyrset_
   90 # define pystat pystat_
   91 # define pytest pytest_
   92 # define pytune pytune_
   93 # define pyupda pyupda_
   94 # define py1ent py1ent_
   95 # ifdef PYTHIA6_DOUBLE_UNDERSCORE
   96 #  define tpythia6_open_fortran_file tpythia6_open_fortran_file__
   97 #  define tpythia6_close_fortran_file tpythia6_close_fortran_file__
   98 #  define pythia6_common_address pythia6_common_address__
   99 # elif PYTHIA6_SINGLE_UNDERSCORE
  100 #  define tpythia6_open_fortran_file tpythia6_open_fortran_file_
  101 #  define tpythia6_close_fortran_file tpythia6_close_fortran_file_
  102 #  define pythia6_common_address pythia6_common_address
  103 # else
  104 #  define pythia6_common_address pythia6_common_address
  105 #  define tpythia6_open_fortran_file tpythia6_open_fortran_file_
  106 #  define tpythia6_close_fortran_file tpythia6_close_fortran_file_
  107 # endif
  108 # define type_of_call
  109 #else
  110 # define pydiff PYDIFF
  111 # define pyevnt PYEVNT
  112 # define pyinit PYINIT
  113 # define pychge PYCHGE
  114 # define pycomp PYCOMP
  115 # define pyedit PYEDIT
  116 # define pyexec PYEXEC
  117 # define pygive PYGIVE
  118 # define pyhepc PYHEPC
  119 # define pylist PYLIST
  120 # define pymass PYMASS
  121 # define pyname PYNAME
  122 # define pyr    PYR
  123 # define pyrget PYRGET
  124 # define pyrset PYRSET
  125 # define pystat PYSTAT
  126 # define pytest PYTEST
  127 # define pytune PYTUNE
  128 # define pyupda PYUPDA
  129 # define py1ent PY1ENT
  130 # define tpythia6_open_fortran_file TPYTHIA6_OPEN_FORTRAN_FILE
  131 # define tpythia6_close_fortran_file TPYTHIA6_CLOSE_FORTRAN_FILE
  132 # define type_of_call _stdcall
  133 #endif
  134
  135
  136 extern "C" void type_of_call pyevnt();
  137 extern "C" void type_of_call pystat(int *key);
  138 extern "C" void type_of_call pylist(int *key);
  139 extern "C" void type_of_call pyedit(int *medit);
  140 extern "C" void type_of_call pydiff();
  141 extern "C" void type_of_call pyexec();
  142 extern "C" void type_of_call pygive(const char *param, Long_t lparam);
  143 extern "C" void type_of_call pyhepc(int *mconv);
  144 extern "C" void type_of_call pylist(int *flag);
  145 extern "C" int  type_of_call pychge(int *kf);
  146 extern "C" int  type_of_call pycomp(int *kf);
  147 extern "C" double  type_of_call pymass(int *kf);
  148 extern "C" void type_of_call pyname(int *kf, char *name, Long_t l_name);
  149 extern "C" int  type_of_call pyr(int *dummy);
  150 extern "C" int  type_of_call pyrget(int *lun, int *move);
  151 extern "C" int  type_of_call pyrset(int *lun, int *move);
  152 extern "C" int  type_of_call pytest(int *flag);
  153 extern "C" int  type_of_call pytune(int *itune);
  154 extern "C" int  type_of_call pyupda(int *mupda, int *lun);
  155 extern "C" void type_of_call py1ent(Int_t&, Int_t&, Double_t&, Double_t&, Double_t&);
  156
  157 #ifndef WIN32
  158 extern "C" void type_of_call pyinit(char *frame, char *beam, char *target,
  159                                      double *win, Long_t l_frame, Long_t l_beam,
  160                                      Long_t l_target);
  161 #else
  162 extern "C" void type_of_call pyinit(char *frame,  Long_t l_frame,
  163                                     char *beam,   Long_t l_beam,
  164                                     char *target, Long_t l_target,
  165                                     double *win
  166                                     );
  167 #endif
  168
  169 extern "C" {
  170    void*  pythia6_common_address(const char*);
  171    void   type_of_call tpythia6_open_fortran_file(int* lun, char* name, int);
  172    void   type_of_call tpythia6_close_fortran_file(int* lun);
  173 }
  174
  175 ClassImp(TPythia6)
  176
  177 /** \class TPythia6::TPythia6Cleaner
  178     \ingroup pythia6
  179
  180 Utility class to manage the TPythia6 instance
  181 */
  182
  183 TPythia6::TPythia6Cleaner::TPythia6Cleaner() {
  184 }
  185
  186 ////////////////////////////////////////////////////////////////////////////////
  187 ///delete the TPythia6 insntance
  188
  189 TPythia6::TPythia6Cleaner::~TPythia6Cleaner() {
  190    if (TPythia6::fgInstance) {
  191       delete TPythia6::fgInstance;
  192       TPythia6::fgInstance = 0;
  193    }
  194 }
  195
  196 //------------------------------------------------------------------------------
  197 //  constructor is not supposed to be called from the outside - only
  198 //  Initialize() method
  199 ////////////////////////////////////////////////////////////////////////////////
  200 /// TPythia6 constructor: creates a TClonesArray in which it will store all
  201 /// particles. Note that there may be only one functional TPythia6 object
  202 /// at a time, so it's not use to create more than one instance of it.
  203
  204 TPythia6::TPythia6() : TGenerator("TPythia6","TPythia6") {
  205    // Protect against multiple objects.   All access should be via the
  206    // Instance member function.
  207    if (fgInstance)
  208       Fatal("TPythia6", "There's already an instance of TPythia6");
  209
  210    delete fParticles; // was allocated as TObjArray in TGenerator
  211
  212    fParticles = new TClonesArray("TMCParticle",50);
  213
  214    // initialize common-blocks
  215    // the functions/subroutines referenced by TPythia6 can be found
  216    // at ftp://root.cern.ch/root/pythia6.tar.gz
  217
  218    fPyjets = (Pyjets_t*) pythia6_common_address("PYJETS");
  219    fPydat1 = (Pydat1_t*) pythia6_common_address("PYDAT1");
  220    fPydat2 = (Pydat2_t*) pythia6_common_address("PYDAT2");
  221    fPydat3 = (Pydat3_t*) pythia6_common_address("PYDAT3");
  222    fPydat4 = (Pydat4_t*) pythia6_common_address("PYDAT4");
  223    fPydatr = (Pydatr_t*) pythia6_common_address("PYDATR");
  224    fPysubs = (Pysubs_t*) pythia6_common_address("PYSUBS");
  225    fPypars = (Pypars_t*) pythia6_common_address("PYPARS");
  226    fPyint1 = (Pyint1_t*) pythia6_common_address("PYINT1");
  227    fPyint2 = (Pyint2_t*) pythia6_common_address("PYINT2");
  228    fPyint3 = (Pyint3_t*) pythia6_common_address("PYINT3");
  229    fPyint4 = (Pyint4_t*) pythia6_common_address("PYINT4");
  230    fPyint5 = (Pyint5_t*) pythia6_common_address("PYINT5");
  231    fPyint6 = (Pyint6_t*) pythia6_common_address("PYINT6");
  232    fPyint7 = (Pyint7_t*) pythia6_common_address("PYINT7");
  233    fPyint8 = (Pyint8_t*) pythia6_common_address("PYINT8");
  234    fPyint9 = (Pyint9_t*) pythia6_common_address("PYINT9");
  235    fPymssm = (Pymssm_t*) pythia6_common_address("PYMSSM");
  236    fPyssmt = (Pyssmt_t*) pythia6_common_address("PYSSMT");
  237    fPyints = (Pyints_t*) pythia6_common_address("PYINTS");
  238    fPybins = (Pybins_t*) pythia6_common_address("PYBINS");
  239 }
  240
  241 ////////////////////////////////////////////////////////////////////////////////
  242
  243 TPythia6::TPythia6(const TPythia6& p6) :
  244   TGenerator(p6),
  245   fPyjets(p6.fPyjets),
  246   fPydat1(p6.fPydat1),
  247   fPydat2(p6.fPydat2),
  248   fPydat3(p6.fPydat3),
  249   fPydat4(p6.fPydat4),
  250   fPydatr(p6.fPydatr),
  251   fPysubs(p6.fPysubs),
  252   fPypars(p6.fPypars),
  253   fPyint1(p6.fPyint1),
  254   fPyint2(p6.fPyint2),
  255   fPyint3(p6.fPyint3),
  256   fPyint4(p6.fPyint4),
  257   fPyint5(p6.fPyint5),
  258   fPyint6(p6.fPyint6),
  259   fPyint7(p6.fPyint7),
  260   fPyint8(p6.fPyint8),
  261   fPyint9(p6.fPyint9),
  262   fPymssm(p6.fPymssm),
  263   fPyssmt(p6.fPyssmt),
  264   fPyints(p6.fPyints),
  265   fPybins(p6.fPybins)
  266 { }
  267
  268 ////////////////////////////////////////////////////////////////////////////////
  269 /// Destroys the object, deletes and disposes all TMCParticles currently on list.
  270
  271 TPythia6::~TPythia6()
  272 {
  273    if (fParticles) {
  274       fParticles->Delete();
  275       delete fParticles;
  276       fParticles = 0;
  277    }
  278 }
  279
  280 ////////////////////////////////////////////////////////////////////////////////
  281 /// model of automatic memory cleanup suggested by Jim Kowalkovski:
  282 /// destructor for local static variable `cleaner' is  always called in the end
  283 /// of the job thus deleting the only TPythia6 instance
  284
  285 TPythia6* TPythia6::Instance() {
  286    static TPythia6::TPythia6Cleaner cleaner;
  287    return fgInstance ? fgInstance : (fgInstance=new TPythia6()) ;
  288 }
  289
  290
  291
  292
  293
  294 ////////////////////////////////////////////////////////////////////////////////
  295 ///  generate event and copy the information from /HEPEVT/ to fPrimaries
  296
  297 void TPythia6::GenerateEvent() {
  298    pyevnt();
  299    ImportParticles();
  300 }
  301
  302 ////////////////////////////////////////////////////////////////////////////////
  303 ///interface with fortran i/o
  304
  305 void TPythia6::OpenFortranFile(int lun, char* name) {
  306    tpythia6_open_fortran_file(&lun, name, strlen(name));
  307 }
  308
  309 ////////////////////////////////////////////////////////////////////////////////
  310 ///interface with fortran i/o
  311
  312 void TPythia6::CloseFortranFile(int lun) {
  313    tpythia6_close_fortran_file(&lun);
  314 }
  315
  316
  317 ////////////////////////////////////////////////////////////////////////////////
  318 /// Fills TObjArray fParticles list with particles from common LUJETS
  319 /// Old contents of a list are cleared. This function should be called after
  320 /// any change in common LUJETS, however GetParticles() method  calls it
  321 /// automatically - user don't need to care about it. In case you make a call
  322 /// to LuExec() you must call this method yourself to transfer new data from
  323 /// common LUJETS to the fParticles list.
  324
  325 TObjArray *TPythia6::ImportParticles(Option_t *)
  326 {
  327    fParticles->Clear();
  328    Int_t numpart   = fPyjets->N;
  329    TClonesArray &a = *((TClonesArray*)fParticles);
  330    for (Int_t i = 0; i<numpart; i++) {
  331       new(a[i]) TMCParticle(fPyjets->K[0][i] ,
  332                             fPyjets->K[1][i] ,
  333                             fPyjets->K[2][i] ,
  334                             fPyjets->K[3][i] ,
  335                             fPyjets->K[4][i] ,
  336                             fPyjets->P[0][i] ,
  337                             fPyjets->P[1][i] ,
  338                             fPyjets->P[2][i] ,
  339                             fPyjets->P[3][i] ,
  340                             fPyjets->P[4][i] ,
  341                             fPyjets->V[0][i] ,
  342                             fPyjets->V[1][i] ,
  343                             fPyjets->V[2][i] ,
  344                             fPyjets->V[3][i] ,
  345                             fPyjets->V[4][i]);
  346    }
  347    return fParticles;
  348 }
  349
  350 ////////////////////////////////////////////////////////////////////////////////
  351 ///  Default primary creation method. It reads the /HEPEVT/ common block which
  352 ///  has been filled by the GenerateEvent method. If the event generator does
  353 ///  not use the HEPEVT common block, This routine has to be overloaded by
  354 ///  the subclasses.
  355 ///  The function loops on the generated particles and store them in
  356 ///  the TClonesArray pointed by the argument particles.
  357 ///  The default action is to store only the stable particles (ISTHEP = 1)
  358 ///  This can be demanded explicitly by setting the option = "Final"
  359 ///  If the option = "All", all the particles are stored.
  360 ///
  361
  362 Int_t TPythia6::ImportParticles(TClonesArray *particles, Option_t *option)
  363 {
  364    if (particles == 0) return 0;
  365    TClonesArray &clonesParticles = *particles;
  366    clonesParticles.Clear();
  367    Int_t numpart = fPyjets->N;
  368    Int_t nparts=0;
  369    if (!strcmp(option,"") || !strcmp(option,"Final")) {
  370       for (Int_t i = 0; i<numpart; i++) {
  371
  372       if (fPyjets->K[0][i] == 1) {
  373          //
  374          //  Use the common block values for the TParticle constructor
  375          //
  376          new(clonesParticles[nparts]) TParticle(
  377                             fPyjets->K[1][i] ,
  378                             fPyjets->K[0][i] ,
  379                             fPyjets->K[2][i] ,
  380                             -1,
  381                             fPyjets->K[3][i] ,
  382                             fPyjets->K[4][i] ,
  383                             fPyjets->P[0][i] ,
  384                             fPyjets->P[1][i] ,
  385                             fPyjets->P[2][i] ,
  386                             fPyjets->P[3][i] ,
  387                             fPyjets->V[0][i] ,
  388                             fPyjets->V[1][i] ,
  389                             fPyjets->V[2][i] ,
  390                             fPyjets->V[3][i]);
  391
  392          //     if(gDebug) printf("%d %d %d! ",i,fPyjets->K[1][i],numpart);
  393          nparts++;
  394          }
  395       }
  396    } else if (!strcmp(option,"All")) {
  397       for (Int_t i = 0; i<numpart; i++) {
  398          new(clonesParticles[i]) TParticle(
  399                             fPyjets->K[1][i] ,
  400                             fPyjets->K[0][i] ,
  401                             fPyjets->K[2][i] ,
  402                             -1,
  403                             fPyjets->K[3][i] ,
  404                             fPyjets->K[4][i] ,
  405                             fPyjets->P[0][i] ,
  406                             fPyjets->P[1][i] ,
  407                             fPyjets->P[2][i] ,
  408                             fPyjets->P[3][i] ,
  409                             fPyjets->V[0][i] ,
  410                             fPyjets->V[1][i] ,
  411                             fPyjets->V[2][i] ,
  412                             fPyjets->V[3][i]);
  413       }
  414       nparts=numpart;
  415    }
  416
  417    return nparts;
  418 }
  419
  420 ////////////////////////////////////////////////////////////////////////////////
  421 /// Calls PyInit with the same parameters after performing some checking,
  422 /// sets correct title. This method should preferably be called instead of PyInit.
  423 /// PURPOSE: to initialize the generation procedure.
  424 /// ARGUMENTS: See documentation for details.
  425 /// -  frame:  - specifies the frame of the experiment:
  426 ///                "CMS","FIXT","USER","FOUR","FIVE","NONE"
  427 /// -  beam,
  428 /// -  target: - beam and target particles (with additionaly charges, tildes or "bar":
  429 ///              e,nu_e,mu,nu_mu,tau,nu_tau,gamma,pi,n,p,Lambda,Sigma,Xi,Omega,
  430 ///              pomeron,reggeon
  431 /// -  win:    - related to energy system:
  432 ///              - for frame=="CMS" - total energy of system
  433 ///              - for frame=="FIXT" - momentum of beam particle
  434 ///              - for frame=="USER" - dummy - see documentation.
  435
  436 void TPythia6::Initialize(const char *frame, const char *beam, const char *target, float win)
  437 {
  438    char  cframe[4];
  439    strlcpy(cframe,frame,4);
  440    char  cbeam[10];
  441    strlcpy(cbeam,beam,10);
  442    char  ctarget[10];
  443    strlcpy(ctarget,target,10);
  444
  445    // For frames "3MOM", "4MOM" and "5MOM" see p. 181-182 of the version 6 manual,
  446    // http://home.thep.lu.se/~torbjorn/pythia/lutp0613man2.pdf
  447    // their usage may depend on the version of Pythia6 used
  448    if ( (!strncmp(frame, "CMS"  ,3)) &&
  449         (!strncmp(frame, "FIXT" ,4)) &&
  450         (!strncmp(frame, "USER" ,4)) &&
  451         (!strncmp(frame, "FOUR" ,4)) &&
  452         (!strncmp(frame, "FIVE" ,4)) &&
  453         (!strncmp(frame, "3MOM" ,4)) &&
  454         (!strncmp(frame, "4MOM" ,4)) &&
  455         (!strncmp(frame, "5MOM" ,4)) &&
  456         (!strncmp(frame, "NONE" ,4)) ) {
  457       printf("WARNING! In TPythia6:Initialize():\n");
  458       printf(" specified frame=%s is neither of CMS,FIXT,USER,FOUR,FIVE,NONE,3MOM,4MOM,5MOM\n",frame);
  459       printf(" resetting to \"CMS\" .");
  460       snprintf(cframe,4,"CMS");
  461    }
  462
  463    if ( (!strncmp(beam, "e"       ,1)) &&
  464         (!strncmp(beam, "nu_e"    ,4)) &&
  465         (!strncmp(beam, "mu"      ,2)) &&
  466         (!strncmp(beam, "nu_mu"   ,5)) &&
  467         (!strncmp(beam, "tau"     ,3)) &&
  468         (!strncmp(beam, "nu_tau"  ,6)) &&
  469         (!strncmp(beam, "gamma"   ,5)) &&
  470         (!strncmp(beam, "pi"      ,2)) &&
  471         (!strncmp(beam, "n"       ,1)) &&
  472         (!strncmp(beam, "p"       ,1)) &&
  473         (!strncmp(beam, "Lambda"  ,6)) &&
  474         (!strncmp(beam, "Sigma"   ,5)) &&
  475         (!strncmp(beam, "Xi"      ,2)) &&
  476         (!strncmp(beam, "Omega"   ,5)) &&
  477         (!strncmp(beam, "pomeron" ,7)) &&
  478         (!strncmp(beam, "reggeon" ,7)) ) {
  479       printf("WARNING! In TPythia6:Initialize():\n");
  480       printf(" specified beam=%s is unrecognized .\n",beam);
  481       printf(" resetting to \"p+\" .");
  482       snprintf(cbeam,8,"p+");
  483    }
  484
  485    if ( (!strncmp(target, "e"       ,1)) &&
  486         (!strncmp(target, "nu_e"    ,4)) &&
  487         (!strncmp(target, "mu"      ,2)) &&
  488         (!strncmp(target, "nu_mu"   ,5)) &&
  489         (!strncmp(target, "tau"     ,3)) &&
  490         (!strncmp(target, "nu_tau"  ,6)) &&
  491         (!strncmp(target, "gamma"   ,5)) &&
  492         (!strncmp(target, "pi"      ,2)) &&
  493         (!strncmp(target, "n"       ,1)) &&
  494         (!strncmp(target, "p"       ,1)) &&
  495         (!strncmp(target, "Lambda"  ,6)) &&
  496         (!strncmp(target, "Sigma"   ,5)) &&
  497         (!strncmp(target, "Xi"      ,2)) &&
  498         (!strncmp(target, "Omega"   ,5)) &&
  499         (!strncmp(target, "pomeron" ,7)) &&
  500         (!strncmp(target, "reggeon" ,7)) ){
  501       printf("WARNING! In TPythia6:Initialize():\n");
  502       printf(" specified target=%s is unrecognized.\n",target);
  503       printf(" resetting to \"p+\" .");
  504       snprintf(ctarget,8,"p+");
  505    }
  506
  507    Pyinit(cframe, cbeam ,ctarget, win);
  508
  509    char atitle[32];
  510    snprintf(atitle,32," %s-%s at %g GeV",cbeam,ctarget,win);
  511    SetTitle(atitle);
  512 }
  513
  514
  515 void TPythia6::Pyinit(char* frame, char* beam, char* target, double win)
  516 {
  517    // Calls Pyinit with the same parameters after performing some checking,
  518    // sets correct title. This method should preferably be called instead of PyInit.
  519    // PURPOSE: to initialize the generation procedure.
  520    // ARGUMENTS: See documentation for details.
  521    //    frame:  - specifies the frame of the experiment:
  522    //                "CMS","FIXT","USER","FOUR","FIVE","NONE"
  523    //    beam,
  524    //    target: - beam and target particles (with additionaly charges,
  525    //              tildes or "bar":
  526    //              e,nu_e,mu,nu_mu,tau,nu_tau,gamma,pi,n,p,Lambda,Sigma,Xi,Omega,
  527    //              pomeron,reggeon
  528    //    win:    - related to energy system:
  529    //              for frame=="CMS" - total energy of system
  530    //              for frame=="FIXT" - momentum of beam particle
  531    //              for frame=="USER" - dummy - see documentation.
  532
  533    Double_t lwin = win;
  534    Long_t  s1    = strlen(frame);
  535    Long_t  s2    = strlen(beam);
  536    Long_t  s3    = strlen(target);
  537 #ifndef WIN32
  538    pyinit(frame,beam,target,&lwin,s1,s2,s3);
  539 #else
  540    pyinit(frame, s1, beam , s2, target, s3, &lwin);
  541 #endif
  542 }
  543
  544
  545 int TPythia6::Pycomp(int kf) {
  546    //interface with fortran routine pycomp
  547    return pycomp(&kf);
  548 }
  549
  550 void TPythia6::Pyedit(int medit) {
  551    //interface with fortran routine pyedit
  552    pyedit(&medit);
  553    ImportParticles();
  554 }
  555
  556 void TPythia6::Pydiff() {
  557    //interface with fortran routine pydiff
  558    pydiff();
  559 }
  560
  561 void TPythia6::Pyevnt() {
  562    //interface with fortran routine pyevnt
  563    pyevnt();
  564 }
  565
  566 void TPythia6::Pyexec() {
  567    //interface with fortran routine pyexec
  568    pyexec();
  569 }
  570
  571 void TPythia6::Pygive(const char *param) {
  572    //interface with fortran routine pygive
  573    Long_t lparam = strlen(param);
  574    pygive(param,lparam);
  575 }
  576
  577 void TPythia6::Pyhepc(int mconv) {
  578    //interface with fortran routine pyhepc
  579    pyhepc(&mconv);
  580 }
  581
  582 void TPythia6::Pylist(int flag) {
  583    //interface with fortran routine pylist
  584    pylist(&flag);
  585 }
  586
  587 void TPythia6::Pyname(int kf, char* name) {
  588    //Note that the array name must be dimensioned in the calling program
  589    //to at least name[16]
  590
  591    pyname(&kf,name,15);
  592    // cut trailing blanks to get C string
  593    name[15] = 0;
  594    //for (int i=15; (i>=0) && (name[i] == ' '); i--) {
  595    //  name[i] = 0;
  596    // }
  597 }
  598
  599 double TPythia6::Pyr(int idummy) {
  600    //interface with fortran routine pyr
  601    return pyr(&idummy);
  602 }
  603
  604 void TPythia6::Pyrget(int lun, int move) {
  605    //interface with fortran routine pyrget
  606    pyrget(&lun,&move);
  607 }
  608
  609 void TPythia6::Pyrset(int lun, int move) {
  610    //interface with fortran routine pyrset
  611    pyrset(&lun,&move);
  612 }
  613
  614 void TPythia6::Pystat(int flag) {
  615    //interface with fortran routine pystat
  616    pystat(&flag);
  617 }
  618
  619 void TPythia6::Pytest(int flag) {
  620    //interface with fortran routine pytest
  621    pytest(&flag);
  622 }
  623
  624 void TPythia6::Pytune(int itune) {
  625    //interface with fortran routine pytune
  626    pytune(&itune);
  627 }
  628
  629 void TPythia6::Pyupda(int mupda, int lun) {
  630    //interface with fortran routine pyupda
  631    pyupda(&mupda,&lun);
  632 }
  633
  634 double TPythia6::Pymass(int kf) {
  635    //interface with fortran routine pymass
  636    return pymass(&kf);
  637 }
  638
  639 int TPythia6::Pychge(int kf) {
  640    //interface with fortran routine pychge
  641    return pychge(&kf);
  642 }
  643
  644 ////////////////////////////////////////////////////////////////////////////////
  645 /// Add one entry to the event record, i.e. either a parton or a
  646 /// particle.
  647 ///
  648 /// - IP:   normally line number for the parton/particle. There are two
  649 ///         exceptions:
  650 ///         - If IP = 0: line number 1 is used and PYEXEC is called.
  651 ///         - If IP < 0: line -IP is used, with status code K(-IP,2)=2
  652 ///                   rather than 1; thus a parton system may be built
  653 ///                   up by filling all but the last parton of the
  654 ///                   system with IP < 0.
  655 ///  - KF:   parton/particle flavour code (PDG code)
  656 ///  - PE:   parton/particle energy. If PE is smaller than the mass,
  657 ///          the parton/particle is taken to be at rest.
  658 ///  - THETA:
  659 ///  - PHI:  polar and azimuthal angle for the momentum vector of the
  660 ///          parton/particle.
  661
  662 void TPythia6::Py1ent(Int_t ip, Int_t kf, Double_t pe, Double_t theta, Double_t phi)
  663 {
  664    py1ent(ip, kf, pe, theta, phi);
  665 }
  666
  667
  668 ////////////////////////////////////////////////////////////////////////////////
  669 /// Exemplary setup of Pythia parameters:
  670 /// Switches on processes 102,123,124 (Higgs generation) and switches off
  671 /// interactions, fragmentation, ISR, FSR...
  672
  673 void TPythia6::SetupTest()
  674 {
  675    SetMSEL(0);            // full user controll;
  676
  677    SetMSUB(102,1);        // g + g -> H0
  678    SetMSUB(123,1);        // f + f' -> f + f' + H0
  679    SetMSUB(124,1);        // f + f' -> f" + f"' + H0
  680
  681
  682    SetPMAS(6,1,175.0);   // mass of TOP
  683    SetPMAS(25,1,300);    // mass of Higgs
  684
  685
  686    SetCKIN(1,290.0);     // range of allowed mass
  687    SetCKIN(2,310.0);
  688
  689    SetMSTP(61,  0);      // switch off ISR
  690    SetMSTP(71,  0);      // switch off FSR
  691    SetMSTP(81,  0);      // switch off multiple interactions
  692    SetMSTP(111, 0);      // switch off fragmentation/decay
  693 }
