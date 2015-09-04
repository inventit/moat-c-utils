-include config.mk

BUILDTYPE ?= Release
ARCH ?= i386
OUTDIR ?= out/$(ARCH)
PYTHON ?= python
DESTDIR ?=
SIGN ?=
PREFIX ?= /usr/local

ifeq ($(BUILDTYPE),Release)
all: $(OUTDIR)/Makefile moatapp
else
all: $(OUTDIR)/Makefile moatapp_g
endif

.PHONY: moatapp moatapp_g test package clean distclean

moatapp: config.gypi $(OUTDIR)/Makefile
	$(MAKE) -C $(OUTDIR) BUILDTYPE=Release V=$(V)

moatapp_g: config.gypi $(OUTDIR)/Makefile
	$(MAKE) -C $(OUTDIR) BUILDTYPE=Debug V=$(V)

$(OUTDIR)/Makefile: common.gypi moatapp.gyp config.gypi
	$(PYTHON) tools/gyp_moatapp -f make

config.gypi: configure
	$(PYTHON) ./configure

package: all
	$(PYTHON) tools/package.py

clean:
	-rm -rf $(OUTDIR)/$(BUILDTYPE)/*
	-find $(OUTDIR)/ -name '*.o' -o -name '*.a' | xargs rm -rf

distclean:
	-rm -rf $(OUTDIR)
	-rm -f config.gypi
	-rm -f config.mk

preparetest: all
	$(PYTHON) tools/test.py --prepare

