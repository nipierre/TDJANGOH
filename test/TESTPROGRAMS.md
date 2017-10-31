# Test programs

You can find two different programs :
- One is `test` and is used to test the generator. The usage is the following :
  - `./test [inputfile] [nb_evts] + flags`
  - The input file is a specified .xml config file. You can find a template in `utils/`
  - Flags can be :
    - `-energy [e]` : nominal energy of the beam.
    - `-verbose [v]` : v=0/1/2, default is 1. Use 0 when in batch mode.
    - `-rand [r]` : r=0/1, gaussian randomization of input energy within ±40 GeV relative to nominal energy.
    - `-finalstate` : save the final states inside a file (eg. for semi-inclusive correction calculation)
- The other is `xsgen` and is used for inclusive cross-section generation. The usage is the following :
  - `./xsgen [inputfile] [RC(1)/Born(0)]`
