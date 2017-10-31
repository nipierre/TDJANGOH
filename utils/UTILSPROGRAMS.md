# Test programs

You can find several different programs :
- One is `pdist` and gives a large panel of information about radiative photon production as the energy, angle.. The usage is the following :
  - `./pdist [inputfile]`
  - The input file is one of the `TDjangoh_evt.dat` file produced with the `test` program.
- An other is `rc_calc` and is used for plotting the radiative correction factors. The usage is the following :
  - `./rc_calc -f [RC_file] [Born_file]` : computes incl. corr. from two files.
  - or `./rc_calc -l [filename_template]` : computes incl. corr. from list of files.
  - or `./rc_calc -sigf [RC_file] [Born_file]` : computes incl. corr. from cross-section files produced by `xsgen` (Best method !)
  - or `./rc_calc -bornratio [Born_file]` : compares born cross-section with TERAD's.
  - or `./rc_calc -sirc [input_file]` : plots semi-incl. corr. issued by `sirc`.
  - Other flags can be :
    - `-qel [q]` 0/1, exclude/include elastic/quasi-elastic contribution from TERAD corrections.
- An other is `sirc` and is computing semi-inclusive corrections.
  - `./sirc -f [RC_file] [Born_file]` : computes semi-incl. corr. from two files.
  - `./sirc -l [RC_filelist] [Born_filelist]` : computes semi-incl. corr. from lists of files.
- A last is `lowQ2` and is comparing structure functions for different parametrizations.
