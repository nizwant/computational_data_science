system2("R", c("CMD", "SHLIB", "lab10a.c"))
dyn.load("lab10a.so")

mvp <- function(A, x)
    .Call("matvecprod", A, x,
        PACKAGE="lab10a")

set.seed(123)
n <- 12345
m <- 9879
A <- matrix(rnorm(n*m), ncol=m)
x <- rnorm(m)


print(microbenchmark::microbenchmark(
    mvp={y1 <- mvp(A, x)},
    r={y2 <- as.vector(A %*% x)},
    times=3,
    unit="ms"
))
stopifnot(all.equal(y1, y2))
