TOP=../..

include $(TOP)/configure/CONFIG
#----------------------------------------
#  ADD MACRO DEFINITIONS BELOW HERE

LIBRARY_IOC += aiSecond
LIBRARY_IOC += eventWf

DBD += aiSecond.dbd
DBD += eventWf.dbd

aiSecond_SRCS += devAiSecond.c
aiSecond_LIBS += $(EPICS_BASE_IOC_LIBS)

eventWf_SRCS += devEventWf.c
eventWf_LIBS += $(EPICS_BASE_IOC_LIBS)

# myappSupport_LIBS += $(EPICS_BASE_IOC_LIBS)


# Build the IOC application
PROD_IOC = myapp

# myapp.dbd will be created and installed
DBD += myapp.dbd

# myapp.dbd will include these files:
myapp_DBD += base.dbd
myapp_DBD += aiSecond.dbd
myapp_DBD += eventWf.dbd

myapp_LIBS += aiSecond
myapp_LIBS += eventWf

# myapp_registerRecordDeviceDriver.cpp derives from myapp.dbd
myapp_SRCS += myapp_registerRecordDeviceDriver.cpp

# Build the main IOC entry point where needed
myapp_SRCS_DEFAULT += myappMain.cpp
myapp_SRCS_vxWorks += -nil-

# Link in the code from our support library
# myapp_LIBS += myappSupport

# To build SNL programs, SNCSEQ must be defined
# in the <top>/configure/RELEASE file

# Finally link IOC to the EPICS Base libraries
myapp_LIBS += $(EPICS_BASE_IOC_LIBS)

include $(TOP)/configure/RULES
#----------------------------------------
#  ADD EXTRA GNUMAKE RULES BELOW HERE

