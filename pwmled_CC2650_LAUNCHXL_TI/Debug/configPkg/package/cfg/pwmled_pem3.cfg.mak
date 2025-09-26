# invoke SourceDir generated makefile for pwmled.pem3
pwmled.pem3: .libraries,pwmled.pem3
.libraries,pwmled.pem3: package/cfg/pwmled_pem3.xdl
	$(MAKE) -f C:\Users\Yantra\workspace_ccstheia\pwmled_CC2650_LAUNCHXL_TI/src/makefile.libs

clean::
	$(MAKE) -f C:\Users\Yantra\workspace_ccstheia\pwmled_CC2650_LAUNCHXL_TI/src/makefile.libs clean

