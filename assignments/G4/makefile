# package macros
PACK_NAME=hansen_hansen_hovgaard
PACK_FILES=makefile {makefile,*.{tex,pdf}} src

all:
	pdflatex report.tex

pack:
	cp -r ../../src .
	zip $(PACK_NAME)_$(shell basename ${PWD}) -r $(PACK_FILES)
	rm -r src

clean:
	rm -rf *.{aux,log,toc,out} $(PACK_NAME)_$(shell basename ${PWD}).zip .*.*.{swp,swo} .*.{swp,swo} src

.PHONY: pack clean

