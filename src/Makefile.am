AUTOMAKE_OPTIONS = subdir-objects

# Name of libraries
lib_LIBRARIES = \
    libcontroller.a

# Compile flag for path to include header
AM_CFLAGS = \
    -I$(top_srcdir)/include/impl/eight_button \
    -I$(top_srcdir)/include/impl \
    -I$(top_srcdir)/include

# Source codes included in library
libcontroller_a_SOURCES = \
    @srcdir@/impl/eight_button/eight_button_controller.c
