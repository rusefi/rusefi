#!/usr/bin/env bash

cd "${0%/*}"
sed -e '/###CSS###/{r style.css' -e 'd}' -e '/###JS###/{r script.js' -e 'd}' pinout.html | sed -e "/###DATA###/{r $1" -e 'd}'
