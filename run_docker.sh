#!/bin/bash
docker run --rm --user $(id -u):$(id -g) --name os-container -v $(pwd):/data -it pwilke/oscorrector $@
