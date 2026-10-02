TEX   = report.tex
PDF   = $(TEX:.tex=.pdf)
IMAGE = texlive/texlive:latest-small

# Use a local pdflatex if there is one, otherwise run it in Docker.
ifeq ($(shell command -v pdflatex),)
PDFLATEX = docker run --rm -u $(shell id -u):$(shell id -g) -e HOME=/tmp \
           -v "$(CURDIR)":/work -w /work $(IMAGE) pdflatex
else
PDFLATEX = pdflatex
endif

.PHONY: render clean

# Run twice so figure references and labels resolve.
render:
	$(PDFLATEX) -interaction=nonstopmode -halt-on-error $(TEX)
	$(PDFLATEX) -interaction=nonstopmode -halt-on-error $(TEX)
	@echo "Wrote $(PDF)"

clean:
	rm -f $(TEX:.tex=.aux) $(TEX:.tex=.log) $(TEX:.tex=.out) $(PDF)
