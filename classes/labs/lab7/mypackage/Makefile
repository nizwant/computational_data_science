all:
	Rscript -e 'roxygen2::roxygenise(".")'
	cd .. && R CMD build mypackage && R CMD INSTALL mypackage_0.0.1.tar.gz

clean:
	rm -f src/*.o src/*.so
	rm -rf mypackage.Rcheck
