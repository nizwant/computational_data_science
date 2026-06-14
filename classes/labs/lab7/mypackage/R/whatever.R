#' @useDynLib mypackage
"_PACKAGE"


#' @title My Function
#' @description
#' Something...
#'
#' @return
#' Computed mean
#'
#' @param x a numeric vector
#' @param na.rm a logical value
#'
#' @export
amean <- function(x, na.rm=TRUE)
    .Call("amean", x, na.rm, PACKAGE="mypackage")

hidden_fun <- function()
    cat(":D\n")

# R CMD build <folder>  # -> mypackage_0.0.1.tar.gz
# R CMD INSTALL mypackage_0.0.1.tar.gz
# R CMD check mypackage_0.0.1.tar.gz
