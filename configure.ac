#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([iodafe_sea_battle], [0.0a1], [r.e.p@yandex.ru])
AC_CONFIG_SRCDIR([src/client.c])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

#Если юзается какая-то функция, которая не всегда есть в этой либе
#(к примеру, зависит от версии), то нужно делать проверку и на неё тоже,
#так же, как ниже проверяется memset и прочие. Все проверки установят
#соответствующие макросы в config.h при ./configure
AC_CHECK_LIB([ncurses], [main])
AC_CHECK_LIB([pthread], [main])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h locale.h netdb.h netinet/in.h stdlib.h string.h sys/socket.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SSIZE_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([memset setlocale socket])

AC_CONFIG_FILES([Makefile src/Makefile])
AC_OUTPUT
