dyn.load("lab04d-squarer.so")

#getLoadedDLLs()
.Call("square", 5,
    PACKAGE="lab04d-squarer")

.Call("square", "wtf",
    PACKAGE="lab04d-squarer")

.Call("square", c(1, 2, 3),
    PACKAGE="lab04d-squarer")

mysquare <- function(x) .Call("square", x,
    PACKAGE="lab04d-squarer")
mysquare(3.4)
