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
#' @examples
#' stopifnot(amean(c(1, 2, 3)) == 2)
#' stopifnot(is.na(amean(c(1, NA, 3))))
#' stopifnot(amean(c(1, NA, 3), TRUE) == 2)
#' stopifnot(is.nan(amean(numeric(0))))
#'
#' @export
amean <- function(x, na.rm=FALSE)
    .Call("amean", x, na.rm, PACKAGE="mypackage")

hidden_fun <- function()
    cat(":D\n")

# R CMD build <folder>  # -> mypackage_0.0.1.tar.gz
# R CMD INSTALL mypackage_0.0.1.tar.gz
# R CMD check mypackage_0.0.1.tar.gz
