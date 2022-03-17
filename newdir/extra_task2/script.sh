#!/bin/bash
echo `sed -n '8p' $1 | ./programA` >outputA
