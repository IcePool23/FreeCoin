#!/bin/sh

TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

RAVEND=${RAVEND:-$SRCDIR/freed}
RAVENCLI=${RAVENCLI:-$SRCDIR/free-cli}
RAVENTX=${RAVENTX:-$SRCDIR/free-tx}
RAVENQT=${RAVENQT:-$SRCDIR/qt/free-qt}

[ ! -x $RAVEND ] && echo "$RAVEND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
RVNVER=($($RAVENCLI --version | head -n1 | awk -F'[ -]' '{ print $6, $7 }'))

# Create a footer file with copyright content.
# This gets autodetected fine for freed if --version-string is not set,
# but has different outcomes for free-qt and free-cli.
echo "[COPYRIGHT]" > footer.h2m
$RAVEND --version | sed -n '1!p' >> footer.h2m

for cmd in $RAVEND $RAVENCLI $RAVENTX $RAVENQT; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=${RVNVER[0]} --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-${RVNVER[1]}//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
