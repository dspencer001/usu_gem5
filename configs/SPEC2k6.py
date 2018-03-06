#!/usr/bin/python
#
# Author: Yiding Han

class SPEC2k6Process:
    input=None
    output=None
    errout=None

class SPEC2k6LiveProcess(SPEC2k6Process):
    executable=None
    cmd=None
    cwd=None
    simpoint=0

binary_dir='/opt/software/architecture/CPU2006/'
data_dir='/opt/software/architecture/CPU2006/'

#400.perlbench
perlbench = SPEC2k6LiveProcess()
perlbench.executable =  binary_dir+'400.perlbench/exe/perlbench_base.amd64-m64-gcc43-nn'
lib=data_dir+'400.perlbench/data/all/input/lib'
#test attrs.pl
data=data_dir+'400.perlbench/data/test/input/attrs.pl'
perlbench.cmd = [perlbench.executable] + ['-I'+lib,data]
#checkspam
#data=data_dir+'400.perlbench/data/ref/input/checkspam.pl'
#perlbench.cmd = [perlbench.executable] + ['-I'+lib,data,2500,5,25,11,150,1,1,1,1]
perlbench.output = 'perlbench.out'

#401.bzip2
bzip2 = SPEC2k6LiveProcess()
bzip2.executable =  binary_dir+'401.bzip2/exe/bzip2_base.amd64-m64-gcc43-nn'
data=data_dir+'401.bzip2/data/all/input/input.program'
bzip2.cmd = [bzip2.executable] + [data, '1']
bzip2.output = 'bzip2.out'

#403.gcc
gcc = SPEC2k6LiveProcess()
gcc.executable =  binary_dir+'403.gcc/exe/gcc_base.amd64-m64-gcc43-nn'
data=data_dir+'403.gcc/data/test/input/cccp.i'
output='./cccp.s'
gcc.cmd = [gcc.executable] + [data]+['-o',output]
gcc.output = 'gcc.out'

#410.bwaves
bwaves = SPEC2k6LiveProcess()
bwaves.executable =  binary_dir+'410.bwaves/exe/bwaves_base.amd64-m64-gcc43-nn'
bwaves.cmd = [bwaves.executable]

#416.gamess
gamess=SPEC2k6LiveProcess()
gamess.executable =  binary_dir+'416.gamess/exe/gamess_base.amd64-m64-gcc43-nn'
gamess.cmd = [gamess.executable]
gamess.input=data_dir+'416.gamess/data/test/input/exam29.config'
gamess.output='gamess.out'

#429.mcf
mcf = SPEC2k6LiveProcess()
mcf.executable =  binary_dir+'429.mcf/exe/mcf_base.amd64-m64-gcc43-nn'
data=data_dir+'429.mcf/data/test/input/inp.in'
mcf.cmd = [mcf.executable] + [data]
mcf.output = 'mcf.out'

#433.milc
milc=SPEC2k6LiveProcess()
milc.executable = binary_dir+'433.milc/exe/milc_base.amd64-m64-gcc43-nn'
stdin=data_dir+'433.milc/data/test/input/su3imp.in'
milc.cmd = [milc.executable]
milc.input=stdin
milc.output='milc.out'

#434.zeusmp
zeusmp=SPEC2k6LiveProcess()
zeusmp.executable =  binary_dir+'434.zeusmp/exe/zeusmp_base.amd64-m64-gcc43-nn'
zeusmp.cmd = [zeusmp.executable]
zeusmp.output = 'zeusmp.out'

#435.gromacs
gromacs = SPEC2k6LiveProcess()
gromacs.executable =  binary_dir+'435.gromacs/exe/gromacs_base.amd64-m64-gcc43-nn'
data='gromacs.tpr'
gromacs.cmd = [gromacs.executable] + ['-silent','-deffnm',data,'-nice','0']

#436.cactusADM
cactusADM = SPEC2k6LiveProcess()
cactusADM.executable =  binary_dir+'436.cactusADM/exe/cactusADM_base.amd64-m64-gcc43-nn'
data=data_dir+'436.cactusADM/data/test/input/benchADM.par'
cactusADM.cmd = [cactusADM.executable] + [data]
cactusADM.output = 'cactusADM.out'

#437.leslie3d
leslie3d=SPEC2k6LiveProcess()
leslie3d.executable =  binary_dir+'437.leslie3d/exe/leslie3d_base.amd64-m64-gcc43-nn'
stdin=data_dir+'437.leslie3d/data/test/input/leslie3d.in'
leslie3d.cmd = [leslie3d.executable]
leslie3d.input=stdin
leslie3d.output='leslie3d.out'

#444.namd
namd = SPEC2k6LiveProcess()
namd.executable =  binary_dir+'444.namd/exe/namd_base.amd64-m64-gcc43-nn'
input=data_dir+'444.namd/data/all/input/namd.input'
namd.cmd = [namd.executable] + ['--input',input,'--iterations','1','--output','namd.out']
namd.output='namd.out'

#445.gobmk
gobmk=SPEC2k6LiveProcess()
gobmk.executable =  binary_dir+'445.gobmk/exe/gobmk_base.amd64-m64-gcc43-nn'
stdin=data_dir+'445.gobmk/data/test/input/capture.tst'
gobmk.cmd = [gobmk.executable]+['--quiet','--mode','gtp']
gobmk.input=stdin
gobmk.output='gobmk.out'

#447.dealII
dealII=SPEC2k6LiveProcess()
dealII.executable =  binary_dir+'447.dealII/exe/dealII_base.amd64-m64-gcc43-nn'
dealII.cmd = [dealII.executable]+['4']
dealII.output='dealII.out'


#450.soplex
soplex=SPEC2k6LiveProcess()
soplex.executable =  binary_dir+'450.soplex/exe/soplex_base.amd64-m64-gcc43-nn'
data='test.mps'
soplex.cmd = [soplex.executable]+['-m10000',data]
soplex.output = 'soplex.out'

#453.povray
povray=SPEC2k6LiveProcess()
povray.executable =  binary_dir+'453.povray/exe/povray_base.amd64-m64-gcc43-nn'
data=data_dir+'453.povray/data/test/input/SPEC-benchmark-test.ini'
#povray.cmd = [povray.executable]+['SPEC-benchmark-test.ini']
povray.cmd = [povray.executable]+[data]
povray.output = 'povray.out'

#454.calculix
#Comment: Segfault
calculix=SPEC2k6LiveProcess()
calculix.executable =  binary_dir+'454.calculix/exe/calculix_base.amd64-m64-gcc43-nn'
data='beampic'
calculix.cmd = [calculix.executable]+['-i',data]
calculix.output = 'calculix.out'

#456.hmmer
hmmer=SPEC2k6LiveProcess()
hmmer.executable =  binary_dir+'456.hmmer/exe/hmmer_base.amd64-m64-gcc43-nn'
data='bombesin.hmm'
hmmer.cmd = [hmmer.executable]+['--fixed', '0', '--mean', '325', '--num', '5000', '--sd', '200', '--seed', '0', data]
hmmer.output = 'hmmer.out'

#458.sjeng
sjeng=SPEC2k6LiveProcess()
sjeng.executable =  binary_dir+'458.sjeng/exe/sjeng_base.amd64-m64-gcc43-nn'
data=data_dir+'458.sjeng/data/test/input/test.txt'
sjeng.cmd = [sjeng.executable]+[data]
sjeng.output = 'sjeng.out'

#459.GemsFDTD
GemsFDTD=SPEC2k6LiveProcess()
GemsFDTD.executable =  binary_dir+'459.GemsFDTD/exe/GemsFDTD_base.amd64-m64-gcc43-nn'
GemsFDTD.cmd = [GemsFDTD.executable]
GemsFDTD.output = 'GemsGDTD.out'

#462.libquantum
libquantum=SPEC2k6LiveProcess()
libquantum.executable =  binary_dir+'462.libquantum/exe/libquantum_base.amd64-m64-gcc43-nn'
libquantum.cmd = [libquantum.executable]+['33','5']
libquantum.output = 'libquantum.test.out'

#464.h264ref
h264ref=SPEC2k6LiveProcess()
h264ref.executable =  binary_dir+'464.h264ref/exe/h264ref_base.amd64-m64-gcc43-nn'
data=data_dir+'464.h264ref/data/test/input/foreman_test_encoder_baseline.cfg'
h264ref.cmd = [h264ref.executable]+['-d',data]
h264ref.output = 'h264ref.out'

#465.tonto
tonto=SPEC2k6LiveProcess()
tonto.executable = binary_dir+'465.tonto/exe/tonto_base.amd64-m64-gcc43-nn'
tonto.cmd = [tonto.executable]
tonto.output = 'tonto.out'

#470.lbm
lbm=SPEC2k6LiveProcess()
lbm.executable =  binary_dir+'470.lbm/exe/lbm_base.amd64-m64-gcc43-nn'
data=data_dir+'470.lbm/data/test/input/100_100_130_cf_a.of'
lbm.cmd = [lbm.executable]+['20', 'reference.dat', '0', '1' ,data]
lbm.output = 'lbm.out'

#471.omnetpp
omnetpp=SPEC2k6LiveProcess()
omnetpp.executable =  binary_dir+'471.omnetpp/exe/omnetpp_base.amd64-m64-gcc43-nn'
data='omnetpp.ini'
omnetpp.cmd = [omnetpp.executable]+[data]
omnetpp.output = 'omnetpp.log'

#473.astar
astar=SPEC2k6LiveProcess()
astar.executable =  binary_dir+'473.astar/exe/astar_base.amd64-m64-gcc43-nn'
data=data_dir+'473.astar/data/test/input/lake.cfg'
astar.cmd = [astar.executable]+[data]
astar.output = 'astar.log'

#481.wrf
wrf=SPEC2k6LiveProcess()
wrf.executable =  binary_dir+'481.wrf/exe/wrf_base.amd64-m64-gcc43-nn'
data=data_dir+'481.wrf/data/test/input/namelist.input'
wrf.cmd = [wrf.executable]+[data]
wrf.output = 'rsl.out.0000'

#482.sphinx
sphinx3=SPEC2k6LiveProcess()
sphinx3.executable =  binary_dir+'482.sphinx3/exe/sphinx_livepretend_base.amd64-m64-gcc43-nn'
sphinx3.cmd = [sphinx3.executable]+['ctlfile', '.', 'args.an4']
sphinx3.output = 'an4.out'

#483.xalancbmk
xalancbmk=SPEC2k6LiveProcess()
xalancbmk.executable =  binary_dir+'483.xalancbmk/exe/Xalan_base.amd64-m64-gcc43-nn'
data1=data_dir+'483.xalancbmk/data/test/input/test.xml'
data2=data_dir+'483.xalancbmk/data/test/input/xalanc.xsl'
xalancbmk.cmd = [xalancbmk.executable]+['-v',data1,data2]
xalancbmk.output = 'xalancbmk'

#998.specrand
specrandi=SPEC2k6LiveProcess()
specrandi.executable = binary_dir+'998.specrand/exe/specrand_base.amd64-m64-gcc43-nn'
specrandi.cmd = [specrandi.executable] + ['324342','24239']
specrandi.output = 'rand.24239.out'

#999.specrand
specrandf=SPEC2k6LiveProcess()
specrandf.executable = binary_dir+'999.specrand/exe/specrand_base.amd64-m64-gcc43-nn'
specrandf.cmd = [specrandf.executable] + ['324342','24239']
specrandf.output = 'rand.24239.out'
