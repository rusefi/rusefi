#!/bin/bash

echo "Testing new evoTech targets..."

echo "Testing evoTech12 target..."
bash bin/compile.sh config/boards/proteus/meta-info-evoTech12.env config

echo "Testing evoTech12plus target..."
bash bin/compile.sh config/boards/proteus/meta-info-evoTech12plus.env config

echo "Test completed!" 