$ !***************************************************************************
$ !*
$ !*      PPP-name: MCE_QXQX.COM
$ !*
$ !***************************************************************************
$ !*
$ !*      This procedure gathers the MCERMI-program source
$ !*      modules, assembles and compiles them and binds
$ !*      object modules together and  builds load module.
$ !*
$ !***************************************************************************
$ !***************************************************************************
$ !
$ create mce_options.opt
        -a -b
        -w                      ! warming support
        -t const.ins            ! instruct TNSDL to export switches
!        -fcov_b=all             ! TNCOV support
!        -ltimers=timers.txt     ! text file about used timer numbers
        -d MT=F                 ! compile image for module test
$ !
$ ! TNSDL-translator version
$ TNSDL_TRANSLATOR :== "tnsdlz "
$ !
$ ! Instructions how to export compilation switches to C
$ create const.ins
  CREATE 'flags.h':MT;
$ !
$ !goto yli
$ !
$ !use /database=dxsw2 dtolib
$ !
$ svnfetch export dtolib/trunk/dtocstgx.spd@10
$ svnfetch export dtolib/trunk/dtopidgx.spd@10
$ svnfetch export dtolib/trunk/dtopidgx.h@10
$ svnfetch export dtolib/trunk/dtobitgx.spd@10
$ svnfetch export dtolib/trunk/dtobitgx.h@10
$ svnfetch export dtolib/trunk/dtohangx.h@10
$ !
$ !use/database=dxsw2    dxnumb
$ !
$ !fetch/version=1.6-0   dxnhexgx.h      /sub_system=dxnumb
$ svnfetch export pac-obh_qx-e1/qx/trunk/src/dxnhexgx.h@147
$ !use /database=dxsw2 dxn
$ !
$ !fetch/version=1.6-0   dxnhexgx.h
$ !
$ !
$ !**********************************************************
$ !basho:
$ ! build RRALIB library
$ !
$ !sfetch -version 2.2-0 rralibqx.com
$ !
$ !TNSDL_PARAMETERS :== " -a -b -fchk_m "
$ !
$ !@rralibqx.com 'TNSDL_TRANSLATOR "''TNSDL_PARAMETERS'"
$ !
$ !**********************************************************
$ ! basho:
$ ! build CE1LIB library
$ !
$ sfetch -version 1.2-8 CE1LIBQX.COM
$ !
$ TNSDL_PARAMETERS :== " -a -b -fchk_m "
$ !
$ @CE1LIBQX.COM 'TNSDL_TRANSLATOR "''MT'" "''TNSDL_PARAMETERS'"
$ !
$ yli:
$ !
$ ! Create runtime enviroment
$ !
$ idcomba  -inlist mcermiqx.in1 mcermiqx.in2 mcermiqx.in3 -out mcermiqx.scc
$ !
$ !**********************************************************
$ !
$ create mce_files.opt
-e error.txt
     -c mcermi.mod
     -g dtopidgx.h
     -g dtobitgx.h
     -g dtohangx.h
     -g mac00757.h
     -g mac043A0.h
     -g mac043A1.h
        mcermiqx.sst
        mcefi1qx.sdl
        mcefi2qx.sdl
        mcefi3qx.sdl
        mcemodqx.sdl
        mceha1qx.sdl
        mceha2qx.sdl
        mceha3qx.sdl
        mcesteqx.sdl
        mcemasqx.sdl
        
$ !
$ ! TNSDL - translation
$ !
$ 'TNSDL_TRANSLATOR @mce_options.opt @mce_files.opt
$ !
$ !**********************************************************
$ !
$ ! Copy and compile TNSDL-auxiliary files
$ !
$ copy tnsdl_lib:tnsdldef.h   *                 ! COMMON DEFINITIONS
$ copy tnsdl_lib:tnsdldef.c   *                 ! COMMON ROUTINES
$ copy tnsdl_lib:tnsdlwar.c   *                 ! WARMING ROUTINES
$ !
$ ! For TNCOV
$ !copy tnsdl_lib:tncovlib.h   *         ! TNCOV ADD-ON
$ !copy tnsdl_lib:tncovlib.cin *         ! TNCOV ADD-ON
$ !copy tnsdl_lib:tncovlib.c   *         ! TNCOV ADD-ON
$ !
$ ccu38g tnsdldef.c                     ! COMMON ROUTINES
$ ccu38g tnsdlwar.c                     ! WARMING ROUTINES
$ !
$ ! For TNCOV
$ !ccu38g tncovlib.c /ypp=tncovlib.cin   ! TNCOV ADD-ON
$ !
$ !**********************************************************
$ create mcermi.cbf
  -cc ccu38g
  -bnd lnku38c
  -if "l"
  -bf "mcemas: -pl *"
  -is 20000
  -il 1000
  -lazy
$  
$ ! Compile and bind translator-made modules for master and hand
$ !
$ ! For normal usage
$ combindj mcermi.mod   -cf "#ALL: /LISTING /DEBUG" -bf "#FINAL: -db -pl __set_computer_and_delivery"  -bf "#ALL: -db" -debug -f mcermi.cbf
$ !
$ ! Make control files to lnku38a

$ copy sys$input usr_lnk.cfu
    tnsdldef.obj
    tnsdlwar.obj
    mcermi.lnk
    ce1libqx.lib

$ !    rralibqx.lib
$ !    tncovlib.obj
$ copy sys$input env_lnk.cfv
    p_exp:dtolibgx.lib
    p_exp:dtolib1x.lib
    p_exp:dxsprl03.exp
    p_exp:dmxpl0gx.exp
    p_exp:dxnhexgx.lib
    p_exp:railibqx.exp
    p_exp:riolibgx.exp
    p_exp:seklibgx.exp
    p_exp:stklibgx.exp
    p_exp:efilibgx.exp
    p_exp:syfiligx.exp
    p_exp:invokegx.exp
    tnsdl_lib:tnsdllib.lib
    ccu38g_sys:dx06r.lib

$ !**********************************************************
$ ! Bind master and hand together
$ !
$ lnku38c -su all -li -db -
      -cf usr_lnk.cfu -
     -cf env_lnk.cfv -
      -pl mcemas -
      -oj mcermiqx.lnk -
      -pr mcermiqx.mp1 -
      -debug
$ !
$ !**********************************************************
$ ! Build mcermiqx
$ !
$ !In Hudson DEV build loops trunk or trunk_promoted is fetched 
$ !as trunk to get its revision to image
$ if f$search("./../trunk.dir") .eqs. "" 
$ then 
$    !if trunk does not exist, no changes

$ !fill/pid mcermi.inf "''MIDSTR"
$ svnfill -inf mcermi.inf -pid "''MIDSTR"

$ else 
$    !revision is taken from earlier fetched trunk/trunk_promoted folder
$   svnfill -inf mcermi.inf -pid "''MIDSTR" -svndir ./../trunk/
$ endif

$ tnblde mcermiqx.lnk /inf=mcermi.inf
$ tnmapc -t mcermiqx.386
$ map386a /PRINTCONTROLS=("DEBUG","LINES","PUBLICS", "SRCLINES", "SYMBOLS", "TABLES","TASKS") mcermiqx.386
$ !
$ !**********************************************************
$ !
$ rename MCERMIQX.IMG [-]MCERMIQX.IMG   ! LOADABLE IMG
$ exit
