#
#Mybench.py
#
# We use this script to create LiveProcess objects for 
# m5 simulation of the SPEC2k6 benchmarks
#
#author: Yiding Han

from m5.objects import LiveProcess
import SPEC2k6

binary_dir='/opt/software/architecture/CPU2006/'
data_dir='/opt/software/architecture/CPU2006/'

#400.perlbench
perlbench = LiveProcess()
perlbench.executable = SPEC2k6.perlbench.executable
perlbench.cmd = SPEC2k6.perlbench.cmd
perlbench.output = SPEC2k6.perlbench.output

#401.bzip2
bzip2 = LiveProcess()
bzip2.executable = SPEC2k6.bzip2.executable
bzip2.cmd =SPEC2k6.bzip2.cmd
bzip2.output =SPEC2k6.bzip2.output

#403.gcc
gcc = LiveProcess()
gcc.executable  = SPEC2k6.gcc.executable 
gcc.cmd  = SPEC2k6.gcc.cmd 
gcc.output  = SPEC2k6.gcc.output 

#410.bwaves
bwaves = LiveProcess()
bwaves.executable  = SPEC2k6.bwaves.executable 
bwaves.cmd  = SPEC2k6.bwaves.cmd 

#416.gamess
gamess=LiveProcess()
gamess.executable  = SPEC2k6.gamess.executable 
gamess.cmd  = SPEC2k6.gamess.cmd 
gamess.input = SPEC2k6.gamess.input
gamess.output = SPEC2k6.gamess.output

#429.mcf
mcf = LiveProcess()
mcf.executable  = SPEC2k6.mcf.executable 
mcf.cmd  = SPEC2k6.mcf.cmd 
mcf.output  = SPEC2k6.mcf.output 

#433.milc
milc=LiveProcess()
milc.executable  = SPEC2k6.milc.executable 
milc.cmd  = SPEC2k6.milc.cmd 
milc.input = SPEC2k6.milc.input
milc.output = SPEC2k6.milc.output

#434.zeusmp
zeusmp=LiveProcess()
zeusmp.executable  = SPEC2k6.zeusmp.executable 
zeusmp.cmd  = SPEC2k6.zeusmp.cmd 
zeusmp.output  = SPEC2k6.zeusmp.output 

#435.gromacs
gromacs = LiveProcess()
gromacs.executable = SPEC2k6.gromacs.executable 
gromacs.cmd = SPEC2k6.gromacs.cmd 

#436.cactusADM
cactusADM = LiveProcess()
cactusADM.executable = SPEC2k6.cactusADM.executable 
cactusADM.cmd = SPEC2k6.cactusADM.cmd 
cactusADM.output = SPEC2k6.cactusADM.output 

#437.leslie3d
leslie3d=LiveProcess()
leslie3d.executable = SPEC2k6.leslie3d.executable 
leslie3d.cmd = SPEC2k6.leslie3d.cmd 
leslie3d.input= SPEC2k6.leslie3d.input
leslie3d.output= SPEC2k6.leslie3d.output

#444.namd
namd = LiveProcess()
namd.executable = SPEC2k6.namd.executable 
namd.cmd = SPEC2k6.namd.cmd 
namd.output= SPEC2k6.namd.output

#445.gobmk
gobmk=LiveProcess()
gobmk.executable = SPEC2k6.gobmk.executable 
gobmk.cmd = SPEC2k6.gobmk.cmd 
gobmk.input= SPEC2k6.gobmk.input
gobmk.output= SPEC2k6.gobmk.output

#447.dealII
dealII=LiveProcess()
dealII.executable = SPEC2k6.dealII.executable 
dealII.cmd = SPEC2k6.dealII.cmd 
dealII.output= SPEC2k6.dealII.output


#450.soplex
soplex=LiveProcess()
soplex.executable = SPEC2k6.soplex.executable 
soplex.cmd = SPEC2k6.soplex.cmd 
soplex.output = SPEC2k6.soplex.output 

#453.povray
povray=LiveProcess()
povray.executable = SPEC2k6.povray.executable 
povray.cmd = SPEC2k6.povray.cmd 
povray.output = SPEC2k6.povray.output 

#454.calculix
calculix=LiveProcess()
calculix.executable = SPEC2k6.calculix.executable 
calculix.cmd = SPEC2k6.calculix.cmd 
calculix.output = SPEC2k6.calculix.output 

#456.hmmer
hmmer=LiveProcess()
hmmer.executable = SPEC2k6.hmmer.executable 
hmmer.cmd = SPEC2k6.hmmer.cmd 
hmmer.output = SPEC2k6.hmmer.output 

#458.sjeng
sjeng=LiveProcess()
sjeng.executable = SPEC2k6.sjeng.executable 
sjeng.cmd = SPEC2k6.sjeng.cmd 
sjeng.output = SPEC2k6.sjeng.output 

#459.GemsFDTD
GemsFDTD=LiveProcess()
GemsFDTD.executable = SPEC2k6.GemsFDTD.executable 
GemsFDTD.cmd = SPEC2k6.GemsFDTD.cmd 
GemsFDTD.output = SPEC2k6.GemsFDTD.output 

#462.libquantum
libquantum=LiveProcess()
libquantum.executable = SPEC2k6.libquantum.executable 
libquantum.cmd = SPEC2k6.libquantum.cmd 
libquantum.output = SPEC2k6.libquantum.output 

#464.h264ref
h264ref=LiveProcess()
h264ref.executable = SPEC2k6.h264ref.executable 
h264ref.cmd = SPEC2k6.h264ref.cmd 
h264ref.output = SPEC2k6.h264ref.output 

#465.tonto
tonto=LiveProcess()
tonto.executable = SPEC2k6.tonto.executable 
tonto.cmd = SPEC2k6.tonto.cmd 
tonto.output = SPEC2k6.tonto.output 

#470.lbm
lbm=LiveProcess()
lbm.executable = SPEC2k6.lbm.executable 
lbm.cmd = SPEC2k6.lbm.cmd 
lbm.output = SPEC2k6.lbm.output 

#471.omnetpp
omnetpp=LiveProcess()
omnetpp.executable = SPEC2k6.omnetpp.executable 
omnetpp.cmd = SPEC2k6.omnetpp.cmd 
omnetpp.output = SPEC2k6.omnetpp.output 

#473.astar
astar=LiveProcess()
astar.executable = SPEC2k6.astar.executable 
astar.cmd = SPEC2k6.astar.cmd 
astar.output = SPEC2k6.astar.output 

#481.wrf
wrf=LiveProcess()
wrf.executable = SPEC2k6.wrf.executable 
wrf.cmd = SPEC2k6.wrf.cmd 
wrf.output = SPEC2k6.wrf.output 

#482.sphinx
sphinx3=LiveProcess()
sphinx3.executable = SPEC2k6.sphinx3.executable 
sphinx3.cmd = SPEC2k6.sphinx3.cmd 
sphinx3.output = SPEC2k6.sphinx3.output 

#483.xalancbmk
xalancbmk=LiveProcess()
xalancbmk.executable = SPEC2k6.xalancbmk.executable 
xalancbmk.cmd = SPEC2k6.xalancbmk.cmd 
xalancbmk.output = SPEC2k6.xalancbmk.output 

#998.specrand
specrandi=LiveProcess()
specrandi.executable = SPEC2k6.specrandi.executable 
specrandi.cmd = SPEC2k6.specrandi.cmd 
specrandi.output = SPEC2k6.specrandi.output 

#999.specrand
specrandf=LiveProcess()
specrandf.executable = SPEC2k6.specrandf.executable 
specrandf.cmd = SPEC2k6.specrandf.cmd 
specrandf.output = SPEC2k6.specrandf.output 
