
# Tests

AUTOMAKE_OPTIONS = foreign

# Arrange to build with the backward compatibility mode enabled.
AM_CPPFLAGS = -I$(top_srcdir) $(BES_CPPFLAGS)
AM_LDADD = $(BES_DAP_LIBS)
if CPPUNIT
AM_CPPFLAGS += $(CPPUNIT_CFLAGS)
AM_LDADD += $(CPPUNIT_LIBS)
endif

# These are not used by automake but are often useful for certain types of
# debugging. 
CXXFLAGS_DEBUG = -g3 -O0 -fno-defer-pop -Wall -W -Wcast-align -Werror
TEST_COV_FLAGS = -ftest-coverage -fprofile-arcs

check_PROGRAMS = $(TESTS)

noinst_HEADERS = test_config.h

# This determines what gets run by 'make check.'
if CPPUNIT
TESTS = dbT authT kinstT parcodsT reporterT
else
TESTS = 

check-local:
	@echo ""
	@echo "**********************************************************"
	@echo "You must have cppunit 1.12.x or greater installed to run *"
	@echo "check target in cedar_handler unit-tests directory       *"
	@echo "**********************************************************"
	@echo ""
endif

noinst_DATA = bes.conf

EXTRA_DIST = bes.conf.in test_config.h.in

DISTCLEANFILES = test_config.h bes.conf

CLEANFILES = *.log *.sum real*

test_config.h: test_config.h.in Makefile
	sed -e "s%[@]srcdir[@]%${srcdir}%" $< > test_config.h

bes.conf: bes.conf.in ../config.status
	sed -e "s%[@]pkgdatadir[@]%${pkgdatadir}%" \
		-e "s%[@]sysconfdir[@]%${sysconfdir}%" \
		-e "s%[@]libdir[@]%${libdir}%" \
		-e "s%[@]abs_top_srcdir[@]%${abs_top_srcdir}%" \
		-e "s%[@]abs_top_builddir[@]%${abs_top_builddir}%" \
		-e "s%[@]datadir[@]%${datadir}%" \
		-e "s%[@]bindir[@]%${bindir}%" $< > bes.conf

############################################################################

CEDAR_DB_SRCS:=../CedarDB.cc ../CedarMySQLDB.cc ../CedarMySQLConnect.cc	\
	../CedarMySQLResult.cc ../CedarEncode.cc

CEDAR_DB_HDRS:=../CedarDB.h ../CedarMySQLDB.h ../CedarMySQLConnect.h	\
	../CedarDBResult.h ../CedarMySQLResult.h ../CedarDBFields.h	\
	../CedarEncode.h

dbT_SOURCES = dbT.cc $(CEDAR_DB_SRCS) $(CEDAR_DB_HDRS)
dbT_LDADD =  $(AM_LDADD)

authT_SOURCES = authT.cc $(CEDAR_DB_SRCS) ../CedarAuthenticate.cc ../CedarAuthenticateException.cc $(CEDAR_DB_HDRS) ../CedarAuthenticate.h ../CedarAuthenticateException.h
authT_LDADD =  $(AM_LDADD)

kinstT_SOURCES = kinstT.cc $(CEDAR_DB_SRCS) ../CedarReadKinst.cc $(CEDAR_DB_HDRS) ../CedarReadKinst.h
kinstT_LDADD =  $(AM_LDADD)

parcodsT_SOURCES = parcodsT.cc $(CEDAR_DB_SRCS) ../CedarReadParcods.cc $(CEDAR_DB_HDRS) ../CedarReadParcods.h
parcodsT_LDADD =  $(AM_LDADD)

reporterT_SOURCES = reporterT.cc $(CEDAR_DB_SRCS) ../CedarReporter.cc ../ContainerStorageCedar.cc ../CedarFSDir.cc ../CedarFSFile.cc $(CEDAR_DB_HDRS) ../CedarReporter.h ../ContainerStorageCedar.h ../CedarFSDir.h ../CedarFSFile.h
reporterT_LDADD =  $(AM_LDADD)

