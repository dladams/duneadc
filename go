#!/bin/sh

DSNAME=DUNE17-cold
RMSMAX=2.0

if [ -n "$1" -a "$1" != . ]; then
  DSNAME=$1
fi

if [ -r rmsmax.dat ]; then
  RMSMAX=`cat rmsmax.dat`
else
  echo Unable to find rmsmax.dat
  exit 1
fi

echo RMS max: $RMSMAX

LOGDIR=logs_$DSNAME
if [ "$2" = clean ]; then
  echo Cleaning dataset $DSNAME...
  rm -rf $LOGDIR
  rm -f calib_$DSNAME.root
  rm -f perf_$DSNAME.root
  rm -f *.png
  exit 0
fi

if [ ! -r $DSNAME.txt ]; then
  echo Datset name file not found: $DSNAME.txt
  exit 1
fi

if [ ! -r $LOGDIR ]; then mkdir $LOGDIR; fi

echo Processing dataset $DSNAME
for NAME in `cat $DSNAME.txt`; do
  LOG=$LOGDIR/$NAME.log
  if [ -r $LOG ]; then
    echo "Skipping $NAME."
  else
    echo "Processing $NAME on "`date`
    root.exe -b -q 'processDataset.C("'$NAME'",'$RMSMAX')' >$LOG 2>&1
    if test -r stop; then
      echo Found stop file. Exiting.
      exit 0
    fi
  fi
done
echo Done on `date`
