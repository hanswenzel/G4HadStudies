#!/bin/bash
rm *.log; rm *.out ;rm *.root; rm *.png; for i in  *.in ; do  ./g4had $i >&  ${i%.*}.log; done
