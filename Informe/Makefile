LATEXMK = latexmk
TARGET = main.pdf

all: $(TARGET)

$(TARGET): main.tex
	$(LATEXMK) -pdf -interaction=nonstopmode -halt-on-error main.tex

clean:
	$(LATEXMK) -C

.PHONY: all clean
