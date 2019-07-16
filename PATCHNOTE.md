# TDJANGOH version 1.4 - Patchnote

## Generation applets

- Test directory modified to gen directory.
- Added an applet for event generation where event are stored in a ROOT tree in output.


# TDJANGOH version 1.3 - Patchnote

## TDjangoh.cc

- Addition of `ElasFS()` and `RadElasFS()` taking care of final state creation for (quasi)elastic events.
- Addition of `GetNucleusID()`.
- Addition of `SaveUnfragState()` to save unfragmented events in separate file for debugging purpose along with new CodeWord `UNFRAG-SAVE` to activate it.
- Addition of new CodeWord `HIGH-PT` for special pT parametrization.
- Addition of CodeWord `FORCE-GEN` to force generation of new event if fragmentation of event fails.
- Removal of ImportParticles method, causing trouble (segfaults) in newest configurations.

## djangoh_h.f

- HERACLES v. 4.8.0.I, I for interfaced.
- Addition of new CodeWord `HIGH-PT` for special pT parametrization.
- Addition of CodeWord `FORCE-GEN` to force generation of new event if fragmentation of event fails.
- Fixed the numbering for incoming particles from `201` to `21` @ subroutine `HSACPT(ICONT)`.

## djangoh_l.f

- Fixed the definition of incoming particle ID @ subroutine `DJGINIT(LEPIN,PLZ,PPZ,INTER)`.
- Fixed `ULMASS` that was not working properly by hardcoding e/mu masses @ subroutine `DJGINIT(LEPIN,PLZ,PPZ,INTER)`.

## sophia.f

- Addition of a retry loop if fragmentation of event fails @ subroutine `DJGSPH(L0,IMODE)`.
- Addition of the subroutine `SPHLEV` for restoration of event record from SOPHIA when hadronization has failed.
- Fixed `ULMASS` that was not working properly by hardcoding e/mu masses @ subroutine `DJGSPH(L0,IMODE)`.
- Fixed the numbering for incoming particles from `201` to `21` @ subroutine `DJGSPH(L0,IMODE)`.
- Fixed a confusion between two variables from different common blocks : `P` from `/S_PLIST/` and `/LUJETS/`

## Plot tools in utils

- Addition of disambiguation between electron and muon cases.
