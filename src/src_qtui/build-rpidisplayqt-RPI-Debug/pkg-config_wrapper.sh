#!/bin/sh
PKG_CONFIG_SYSROOT_DIR=/home/marcel/Desktop/atc_buildroot/src/src_buildroot/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot
export PKG_CONFIG_SYSROOT_DIR
PKG_CONFIG_LIBDIR=/home/marcel/Desktop/atc_buildroot/src/src_buildroot/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/pkgconfig:/home/marcel/Desktop/atc_buildroot/src/src_buildroot/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/share/pkgconfig:/home/marcel/Desktop/atc_buildroot/src/src_buildroot/buildroot/output/host/arm-buildroot-linux-gnueabihf/sysroot/usr/lib/arm-buildroot-linux-gnueabihf/pkgconfig
export PKG_CONFIG_LIBDIR
exec pkg-config "$@"
