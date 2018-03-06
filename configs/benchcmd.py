#!/usr/bin/python
#
#Author: Yiding
#Print the cmd used to run the SPEC2k6 benchmarks

import sys, getopt
import SPEC2k6

def main(argv):
   benchmark = ''
   try:
      opts, args = getopt.getopt(argv,"hb:",["benchmark-name="])
   except getopt.GetoptError:
      print 'benchcmd.py -b <benchmark>'
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         print 'benchcmd.py -b <benchmark>'
         sys.exit()
      elif opt in ("-b", "--benchmark-name"):
         benchmark = arg
   #print 'Benchmark is '+benchmark

   if benchmark == 'perlbench':
       process = SPEC2k6.perlbench
   elif benchmark == 'bzip2':
      process = SPEC2k6.bzip2
   elif benchmark == 'gcc':
      process = SPEC2k6.gcc
   elif benchmark == 'bwaves':
      process = SPEC2k6.bwaves
   elif benchmark == 'gamess':
      process = SPEC2k6.gamess
   elif benchmark == 'mcf':
      process = SPEC2k6.mcf
   elif benchmark == 'milc':
      process = SPEC2k6.milc
   elif benchmark == 'zeusmp':
      process = SPEC2k6.zeusmp
   elif benchmark == 'gromacs':
      process = SPEC2k6.gromacs
   elif benchmark == 'cactusADM':
      process = SPEC2k6.cactusADM
   elif benchmark == 'leslie3d':
      process = SPEC2k6.leslie3d
   elif benchmark == 'namd':
      process = SPEC2k6.namd
   elif benchmark == 'gobmk':
      process = SPEC2k6.gobmk;
   elif benchmark == 'dealII':
      process = SPEC2k6.dealII
   elif benchmark == 'soplex':
      process = SPEC2k6.soplex
   elif benchmark == 'povray':
      process = SPEC2k6.povray
   elif benchmark == 'calculix':
      process = SPEC2k6.calculix
   elif benchmark == 'hmmer':
      process = SPEC2k6.hmmer
   elif benchmark == 'sjeng':
      process = SPEC2k6.sjeng
   elif benchmark == 'GemsFDTD':
      process = SPEC2k6.GemsFDTD
   elif benchmark == 'libquantum':
      process = SPEC2k6.libquantum
   elif benchmark == 'h264ref':
      process = SPEC2k6.h264ref
   elif benchmark == 'tonto':
      process = SPEC2k6.tonto
   elif benchmark == 'lbm':
      process = SPEC2k6.lbm
   elif benchmark == 'omnetpp':
      process = SPEC2k6.omnetpp
   elif benchmark == 'astar':
      process = SPEC2k6.astar
   elif benchmark == 'wrf':
      process = SPEC2k6.wrf
   elif benchmark == 'sphinx3':
      process = SPEC2k6.sphinx3
   elif benchmark == 'xalancbmk':
      process = SPEC2k6.xalancbmk
   elif benchmark == 'specrandi':
      process = SPEC2k6.specrandi
   elif benchmark == 'specrandf':
      process = SPEC2k6.specrandf
   
   #sys.stdout.write(' ')
   for i in range(len(process.cmd)):
      sys.stdout.write(process.cmd[i]+' ')
   if process.input != None: 
      sys.stdout.write('< '+process.input)
   print

if __name__ == "__main__":
   main(sys.argv[1:])
