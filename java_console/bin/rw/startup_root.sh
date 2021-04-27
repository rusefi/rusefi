#!/bin/sh

cd /rw
date >> startup.log
whoami >> startup.log

sudo su - pi /rw/startup_user.sh
