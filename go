#!/bin/sh

if [ -r goIsRunning ]; then
  echo Found goIsRunning.
  exit 1
fi
touch goIsRunning

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
echo Log dir: $LOGDIR

if [ ! -r $DSLIST.txt ]; then
  echo Datset list file not found: $DSLIST.txt
  exit 1
fi

if [ ! -r $LOGDIR ]; then mkdir $LOGDIR; fi

echo Processing dataset $DSNAME from list $DSLIST.txt
for NAME in `cat $DSLIST.txt`; do
  LOG=$LOGDIR/$NAME.log
  if [ -r $LOG ]; then
    echo "Skipping $NAME."
  else
    echo "Processing $NAME on "`date`
    echo $NAME >goIsRunning
    root.exe -b -q 'processDataset.C("'$NAME'",'$RMSMAX')' >$LOG 2>&1
    if test -r stop; then
      echo Found stop file. Exiting.
      rm goIsRunning
      rm stop
      exit 0
    fi
  fi
done
echo Done on `date`
rm goIsRunning
