# invoke SourceDir generated makefile for empty.pem3
empty.pem3: .libraries,empty.pem3
.libraries,empty.pem3: package/cfg/empty_pem3.xdl
	$(MAKE) -f C:\Users\Yantra\workspace_ccstheia\empty_CC2650_LAUNCHXL_TI/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Yantra\workspace_ccstheia\empty_CC2650_LAUNCHXL_TI/src/makefile.libs clean

