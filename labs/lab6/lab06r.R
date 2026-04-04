#!/bin/env Rscript

dyn.load("lab06r.so")
mysquare <- function(x)
    .Call("mysquare", x, PACKAGE="lab06r")
print(mysquare(3.14))

# Rscript lab06r.R

# chmod u+x lab06r.R
# ./lab06r.R
