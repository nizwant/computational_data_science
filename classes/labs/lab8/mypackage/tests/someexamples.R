library("mypackage")

stopifnot(amean(c(1, 2, 3)) == 2)
stopifnot(is.na(amean(c(1, NA, 3))))
stopifnot(amean(c(1, NA, 3), TRUE) == 2)
stopifnot(is.nan(amean(numeric(0))))

library("tinytest")
expect_equal(amean(c(1, 2, 3)), 2)
expect_error(amean("fs"))
