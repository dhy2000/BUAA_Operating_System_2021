#!/bin/bash

old=$2
new=$3

sed -i 's/'$old'/'$new'/g' $1

