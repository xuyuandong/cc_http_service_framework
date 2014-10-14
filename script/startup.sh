#!/bin/bash

cd $(dirname `ls -ls $0 | awk '{print $NF;}'`)/..
WK_DIR=`pwd`

EXE_NAME=service

function _start() 
{
  source /usr/home/x/.bashrc
  cd $WK_DIR/bin
  nohup ./$EXE_NAME --flagfile=../conf/$EXE_NAME.conf & echo $! >../$EXE_NAME.pid &
  echo "$EXE_NAME start on pid of `cat ../$EXE_NAME.pid`"
}

function _stop()
{
  if [ -f $WK_DIR/$EXE_NAME.pid ]; then
    echo "pid file exist!"
    for PID in `cat ${WK_DIR}/${EXE_NAME}.pid`; do
      echo "pid: $PID"
      RUN=`ps -eo pid,cmd | grep $PID | grep $EXE_NAME`
      if [ "$RUN" != "" ]; then
        kill $PID
        echo "$PID has been killed!"
      fi
    done
    rm -rf $WK_DIR/$EXE_NAME.pid
  else
    echo "pid file not exist!"
  fi
}

function _restart() 
{
  _stop
  _start
}

function _show_useage()
{
  echo "useage: $0 {start|stop|restart}"
}

##
# main

case $1 in 
  start|stop|restart)
  _${1}
  ;;
  *)
  _show_useage
  ;;
esac

