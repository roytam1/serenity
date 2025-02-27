#!/usr/bin/env -S bash ../.port_include.sh
port=ncurses
version=6.2
useconfigure=true
configopts="--with-termlib --enable-pc-files --with-pkg-config=/usr/local/lib/pkgconfig --with-pkg-config-libdir=/usr/local/lib/pkgconfig --without-ada --enable-sigwinch --with-shared"
files="https://ftpmirror.gnu.org/gnu/ncurses/ncurses-${version}.tar.gz ncurses-${version}.tar.gz
https://ftpmirror.gnu.org/gnu/ncurses/ncurses-${version}.tar.gz.sig ncurses-${version}.tar.gz.sig
https://ftpmirror.gnu.org/gnu/gnu-keyring.gpg gnu-keyring.gpg"
auth_type="sig"
auth_opts="--keyring ./gnu-keyring.gpg ncurses-${version}.tar.gz.sig"

pre_configure() {
    export CPPFLAGS="-P"
}
