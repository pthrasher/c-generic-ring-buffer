#!/bin/bash

make test >/dev/null &&
./test &&
rm test;

