#!/bin/env Rscript

dyn.load("lab07r.so")

amean <- function(x, na.rm=FALSE) {
    .Call("amean", x, na.rm, PACKAGE="lab07r")
}


print(mean(numeric(0)))
print(amean(numeric(0)))

mymean <- amean
stopifnot(mymean(c(1, 2, 3)) == 2.0)
stopifnot(mymean(1:3) == 2.0)
stopifnot(is.na(mymean(c(1, NA, 3))))
stopifnot(is.na(mymean(c(1, NA, 3), na.rm=FALSE)))
stopifnot(mymean(c(1, NA, 3), na.rm=TRUE) == 2.0)

# Rscript lab07r.R
# chmod u+x lab07r.R
# R CMD SHLIB lab07r.c && ./lab07r.R

'%+%' <- function(x, y) {
    .Call("asum", x, y, PACKAGE="lab07r")
}

stopifnot(
    identical(
        c(1, 2, 3, 4) %+% c(1, 2, 3, 4),
        c(2, 4, 6, 8)
    ),
    identical(
        c(1, 2, 3, NA) %+% c(NA, 2, 3, 4),
        c(NA, 4, 6, NA)
    ),
    identical(
        c(1, 2) %+% c(1, 2, 3, 4),
        c(2, 4, 4, 6)
    ),
    identical(
        numeric(0) %+% c(1, 2),
        numeric(0)
    )
)
