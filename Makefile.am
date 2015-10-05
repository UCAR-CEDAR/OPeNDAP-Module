
# Automake file for cedar_handler
#
# 5/17/2005 jhrg
#
# $Id: Makefile.am 12972 2006-01-05 15:41:07Z pwest $

AUTOMAKE_OPTIONS = foreign check-news

ACLOCAL_AMFLAGS = -I conf

AM_CPPFLAGS = $(BES_CPPFLAGS)

SUBDIRS = unit-tests bes-testsuite

bin_PROGRAMS = checkKinst checkParcod encode

lib_besdir=$(libdir)/bes
lib_bes_LTLIBRARIES = libcedar_module.la

CEDAR_DB_SRCS:=CedarDB.cc CedarMySQLDB.cc CedarMySQLConnect.cc		\
	CedarMySQLResult.cc CedarEncode.cc

CEDAR_DB_HDRS:=CedarDB.h CedarMySQLDB.h CedarMySQLConnect.h		\
	CedarDBResult.h CedarMySQLResult.h CedarDBFields.h CedarEncode.h

CEDAR_SRCS:=cedar_read_attributes.cc cedar_read_descriptors.cc		\
	cedar_read_tab.cc cedar_read_tab_support.cc cedar_read_info.cc	\
	cedar_read_flat.cc cedar_read_stream.cc CedarRequestHandler.cc	\
	CedarFilter.cc CedarTab.cc CedarFlat.cc CedarInfo.cc		\
	FlatResponseHandler.cc TabResponseHandler.cc			\
	StreamResponseHandler.cc ContainerStorageCedar.cc		\
	CedarReporter.cc InfoResponseHandler.cc				\
	CedarAuthenticate.cc CedarAuthenticateException.cc		\
	CedarReadKinst.cc CedarReadParcods.cc				\
	CedarFSDir.cc CedarFSFile.cc CedarTransmitter.cc		\
	$(CEDAR_DB_SRCS)


CEDAR_HDRS:=CedarFilter.h CedarFlat.h CedarRequestHandler.h		\
	CedarResponseNames.h CedarTab.h CedarInfo.h			\
	FlatResponseHandler.h StreamResponseHandler.h			\
	TabResponseHandler.h cedar_read_attributes.h			\
	cedar_read_descriptors.h cedar_read_flat.h cedar_read_info.h	\
	cedar_read_stream.h cedar_read_tab.h cedar_read_tab_support.h	\
	ContainerStorageCedar.h CedarReporter.h InfoResponseHandler.h	\
	CedarAuthenticate.h CedarAuthenticateException.h		\
	CedarReadKinst.h CedarReadParcods.h				\
	config_cedar.h CedarFSDir.h CedarFSFile.h CedarTransmitter.h	\
	$(CEDAR_DB_HDRS)

libcedar_module_la_SOURCES = $(CEDAR_SRCS) CedarModule.cc $(CEDAR_HDRS) CedarModule.h
libcedar_module_la_CPPFLAGS = $(AM_CPPFLAGS)
libcedar_module_la_LDFLAGS = -avoid-version -module 
libcedar_module_la_LIBADD = $(BES_DAP_LIBS)

checkKinst_SOURCES = checkKinst.cc CedarReadKinst.cc $(CEDAR_DB_SRCS)	\
	CedarReadKinst.h $(CEDAR_DB_HDRS)
checkKist_CPPFLAGS = $(AM_CPPFLAGS)
checkKinst_LDADD = $(BES_DAP_LIBS)

checkParcod_SOURCES = checkParcod.cc CedarReadParcods.cc $(CEDAR_DB_SRCS) \
	CedarReadParcods.h $(CEDAR_DB_HDRS)
checkParcod_CPPFLAGS = $(AM_CPPFLAGS)
checkParcod_LDADD =$(BES_DAP_LIBS)

encode_SOURCES = encode.cc CedarEncode.cc CedarEncode.h
encode_CPPFLAGS = $(AM_CPPFLAGS)
encode_LDADD = $(BES_DAP_LIBS)

pkgdata_DATA = cedar/cedar_help.html cedar/cedar_help.txt cedar/cedar_login.html

EXTRA_DIST = COPYRIGHT COPYING cedar.conf.in cedar/cedar_help.html cedar/cedar_help.txt cedar/cedar_login.html data

CLEANFILES = *~ cedar.conf

sample_datadir = $(datadir)/hyrax/data/cedar
sample_data_DATA = data/mfp920504a.cbf data/mlh090323g.001

install-data-local: cedar.conf
	@currdate=`date +"%y%m%d%H%M"`; \
	test -d $(DESTDIR)$(sysconfdir)/bes || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes; \
	test -d $(DESTDIR)$(sysconfdir)/bes/modules || $(MKDIR_P) $(DESTDIR)$(sysconfdir)/bes/modules; \
	test -f $(DESTDIR)$(sysconfdir)/bes/modules/cedar.conf && mv -f $(DESTDIR)$(sysconfdir)/bes/modules/cedar.conf $(DESTDIR)$(sysconfdir)/bes/modules/cedar.conf.$$currdate; \
	$(INSTALL_DATA) cedar.conf $(DESTDIR)$(sysconfdir)/bes/modules/cedar.conf

uninstall-local:
	rm -f $(DESTDIR)$(sysconfdir)/bes/modules/cedar.conf

cedar.conf: cedar.conf.in config.status
	sed -e "s%[@]bes_modules_dir[@]%${lib_besdir}%g" \
	    -e "s%[@]sysconfdir[@]%$(sysconfdir)%g" \
	    -e "s%[@]libdir[@]%$(libdir)%g" \
	    -e "s%[@]pkgdatadir[@]%$(pkgdatadir)%g" \
	    -e "s%[@]datadir[@]%$(datadir)%g" $< > cedar.conf

###########################################################################

# Build linux RPMs

srpm:
	rpmbuild -ts --clean @PACKAGE@-@PACKAGE_VERSION@.tar.gz

rpm:
	rpmbuild -tb --clean @PACKAGE@-@PACKAGE_VERSION@.tar.gz

info:
	@echo ""
	@echo ""
	@echo "Makefile Information"
	@echo "    CEDAR_CPPFLAGS = $(CEDAR_CPPFLAGS)"
	@echo "    CEDAR_LDFLAGS = $(CEDAR_LDFLAGS)"
	@echo "    CEDAR_LIBS = $(CEDAR_LIBS)"
