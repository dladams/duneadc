#!/bin/sh

ISRUNNING=goIsRunning
STOPFILE=stop

if [ -r $ISRUNNING ]; then
  echo Found $ISRUNNING.
  exit 1
fi
touch $ISRUNNING

echo
echo Checking Root:
if ! root.exe -q; then
  echo Root must be setup to process data
  rm $ISRUNNING
  exit 1
fi
echo Root is OK
echo

DSNAME=DUNE17-cold
if [ -r dsname.dat ]; then
  DSNAME=`cat dsname.dat`
fi
RMSMAX=2.0

DSLIST=$DSNAME
if [ -n "$1" -a "$1" != . ]; then
  DSLIST=$1
fi

if [ -r rmsmax.dat ]; then
  RMSMAX=`cat rmsmax.dat`
else
  echo Unable to find rmsmax.dat
  exit 1
fi

echo Dataset name: $DSNAME
echo Dataset list: $DSLIST.txt
echo      RMS max: $RMSMAX

LOGDIR=logs_$DSNAME
if [ "$1" = clean -o "$2" = clean ]; then
  echo Cleaning dataset $DSNAME...
  rm -rf $LOGDIR
  rm -f calib_$DSNAME.root
  rm -f perf_$DSNAME.root
  rm -f *.png
  exit 0
fi

if [ ! -r $DSLIST.txt ]; then
  echo Dataset list file not found: $DSLIST.txt
  exit 1
fi

if [ ! -r $LOGDIR ]; then mkdir $LOGDIR; fi

echo Processing dataset $DSNAME from list $DSLIST.txt
for NAME in `cat $DSLIST.txt`; do
  mkdir -p "jobs/running"
  mkdir -p "jobs/done"
  mkdir -p "jobs/failed"
  RUNDIR="jobs/running/$NAME"
  if [ -r $RUNDIR -o -r "jobs/done/$NAME" -o -r "jobs/failed/$NAME" ]; then
    echo "Skipping $NAME."
  else
    echo "Processing $NAME on "`date`
    echo $NAME >$ISRUNNING
    SAVEDIR=`pwd`
    export DUNEADCDIR=$SAVEDIR
    CHKDIR=$SAVEDIR/jobs/check
    mkdir -p $CHKDIR
    mkdir $RUNDIR
    if cd $RUNDIR; then
      cp $DUNEADCDIR/clientlogon.C rootlogon.C
      cp $DUNEADCDIR/processDataset.C .
      root.exe -b -q 'processDataset.C("'$NAME'",'$RMSMAX')' >run.log 2>&1
      STAT=$?
      FAILED=0
      if ! test $STAT -eq 0; then FAILED=1
      elif ! mv calib_$DSNAME.root calib_$NAME.root; then FAILED=2
      elif ! mv perf_$DSNAME.root perf_$NAME.root; then FAILED=3
      elif ! cp rawv_$NAME.png $CHKDIR; then FAILED=4
      elif ! test -r diff_$NAME.png; then FAILED=5
      elif ! test -r perf_$NAME.png; then FAILED=6
      fi
      cd $SAVEDIR
    else 
      echo "Unable to change dir to $RUNDIR"
      break
    fi
    if [ $FAILED -eq 0 ]; then
      echo "  Job successful."
      mv $RUNDIR "jobs/done"
    else
      echo "  Job failed with error $FAILED"
      mv $RUNDIR "jobs/failed"
    fi
    if test -r $STOPFILE; then
      echo Found $STOPFILE file. Exiting.
      rm $ISRUNNING
      rm $STOPFILE
      exit 0
    fi
  fi
done
echo Done on `date`
rm $ISRUNNING
