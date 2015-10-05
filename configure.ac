# -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.59)
AC_INIT(cedar_handler, 2.3.3, opendap-tech@opendap.org)
AC_CONFIG_AUX_DIR([conf])
AC_CONFIG_MACRO_DIR([conf])

AM_INIT_AUTOMAKE(1.10)
AC_CONFIG_TESTDIR(bes-testsuite, [.])

AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_HEADER([config_cedar.h])

AC_CANONICAL_HOST
AC_SUBST(host)

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_CHECK_FUNCS([strchr])

dnl Checks for specific libraries
AC_CHECK_LIBDAP([3.11.1],
 [
  LIBS="$LIBS $DAP_SERVER_LIBS"
  CPPFLAGS="$CPPFLAGS $DAP_CFLAGS"
 ],
 [ AC_MSG_ERROR([Cannot find libdap])
])

AC_CHECK_BES([3.9.1],
[
],
[ AC_MSG_ERROR([Cannot find bes])
])

AC_CHECK_CEDAR([2.2.2],
   [
       LIBS="$LIBS $CEDAR_LIBS"
       CPPFLAGS="$CPPFLAGS $CEDAR_CFLAGS"
       AC_SUBST([LIBS])
       AC_SUBST([CPPFLAGS])
   ],
   [
       echo "*** Use --with-cedar for the root cedar directory."
       echo "*** Otherwise use --with-cedar-include switch for includes directory"
       echo "*** and --with-cedar-libdir switch for libraries directory."
       AC_MSG_ERROR([cedar library and cedar headers are required.])
   ]
)

dnl Check for the mysql library.
AC_DEFUN([AC_CHECK_MYSQL],
[
AC_ARG_WITH([mysql],
            [AS_HELP_STRING([--with-mysql=ARG],[mysql directory])],
            [MYSQL_PATH=${withval}], 
            [MYSQL_PATH=""])

  if test "$MYSQL_PATH" = "" ; then
    AC_PATH_PROG([MYSQL_CONFIG], [mysql_config], [no])
  else
    AC_MSG_CHECKING([for mysql in $MYSQL_PATH])
    MYSQL_CONFIG="$MYSQL_PATH/bin/mysql_config"
  fi
  mysql_min_version=m4_if([$1], [], [4.0.18], [$1])
  mysql_no=""
  if test -x "$MYSQL_CONFIG" ; then
    mysql_config_major_version=`$MYSQL_CONFIG --version | sed 's/\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$/\1/'`
    mysql_config_minor_version=`$MYSQL_CONFIG --version | sed 's/\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$/\2/'`
    mysql_config_micro_version=`$MYSQL_CONFIG --version | sed 's/\([[0-9]]\)*\.\([[0-9]]*\)\.\([[0-9]]*\)$/\3/'`
    mysql_min_major_version=`echo $mysql_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\1/'`
    mysql_min_minor_version=`echo $mysql_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\2/'`
    mysql_min_micro_version=`echo $mysql_min_version | sed 's/\([[0-9]]*\).\([[0-9]]*\).\([[0-9]]*\)/\3/'`
    mysql_config_is_lt=""
    if test $mysql_config_major_version -lt $mysql_min_major_version ; then
      mysql_config_is_lt=yes
    else
      if test $mysql_config_major_version -eq $mysql_min_major_version ; then
        if test $mysql_config_minor_version -lt $mysql_min_minor_version ; then
          mysql_config_is_lt=yes
        else
          if test $mysql_config_minor_version -eq $mysql_min_minor_version ; then
            if test $mysql_config_micro_version -lt $mysql_min_micro_version ; then
              mysql_config_is_lt=yes
            fi
          fi
        fi
      fi
    fi
    if test x$mysql_config_is_lt = xyes ; then
      mysql_no=yes
    else
      MYSQL_LIBS="`$MYSQL_CONFIG --libs`"
      MYSQL_CPPFLAGS="`$MYSQL_CONFIG --cflags`"
    fi
  else
     mysql_no=yes
  fi
  if test x$mysql_no = x ; then
    AC_MSG_RESULT([yes])
    m4_if([$2], [], [:], [$2])
  else
    AC_MSG_RESULT([no])
    if test -x "$MYSQL_CONFIG" ; then
      if test x$mysql_config_is_lt = xyes ; then
        AC_MSG_NOTICE([the installed mysql library is too old.])
      else
        AC_MSG_NOTICE([Problem running mysql_config.])
      fi
    else
      AC_MSG_NOTICE([The mysql_config script could not be found.])
    fi
    MYSQL_LIBS=""
    MYSQL_CPPFLAGS=""
    m4_if([$3], [], [:], [$3])
  fi
  AC_SUBST([MYSQL_CPPFALGS])
  AC_SUBST([MYSQL_LIBS])
])

AC_CHECK_MYSQL(
   [4.0.18],
   [
       LIBS="$LIBS $MYSQL_LIBS"
       CPPFLAGS="$CPPFLAGS $MYSQL_CPPFLAGS"
       AC_SUBST([LIBS])
       AC_SUBST([CPPFLAGS])
   ],
   [
       echo "*** Use --with-mysql for the root mysql directory."
       AC_MSG_ERROR([mysql library and headers are required.])
   ]
)

AC_ARG_WITH([https],
            [AS_HELP_STRING([--with-https=PORT],[port https is sitting on - default 443])],
            [HTTPS_PORT_VAL=${withval}], 
            [HTTPS_PORT_VAL=443])

AC_DEFINE_UNQUOTED([HTTPS_PORT], [$HTTPS_PORT_VAL], [Define port cedar login is done through https])

OPENDAP_DEBUG_OPTION

AM_PATH_CPPUNIT(1.12.0,
	[AM_CONDITIONAL([CPPUNIT], [true])],
	[AM_CONDITIONAL([CPPUNIT], [false])])

AC_CONFIG_FILES([Makefile
		 unit-tests/Makefile
		 bes-testsuite/atlocal
		 bes-testsuite/Makefile])
AC_OUTPUT

