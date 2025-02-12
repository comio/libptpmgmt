#!/bin/bash -e
# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2023 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2023 Erez Geva
#
# CI (Continous integration) script for containers
###############################################################################
ecmd()
{
 set +e
 out="`$@`"
 last_ret=$?
 set -e
}
eacmd()
{
 set +e
 out="`$@ 2>&1`"
 last_ret=$?
 set -e
}
mquit()
{
 if [[ -n "$out" ]]; then
   echo "$out"
 fi
 echo "$color_red$color_red$@$color_norm"
 exit 1
}
equit()
{
 if [[ $last_ret -ne 0 ]]; then
   mquit "$@"
 fi
}
check_clean()
{
 local left=''
 local name=$1
 shift
 readarray <<< `git clean -fxdn`
 for n in "${MAPFILE[@]}"; do
   if [[ -z "${n%$'\n'}" ]] ||\
      [[ $n =~ \/\.[^\/]+\.swp ]] ||\
      [[ $n =~ ^$would\.[^\/]+\.swp ]]; then
     continue
   fi
   for m in "$@"; do
     if [[ $n =~ ^$would$m ]]; then
       continue 2
     fi
   done
   if [[ -n "${n%$'\n'}" ]]; then
     left+="$n"
   fi
 done
 if [[ -n "$left" ]]; then
   printf "$color_red$left"
   mquit "Make $name left unused files"
 fi
}
test_clean()
{
 if $have_git; then
   out=''
   make clean $mk_noc
   check_clean clean $clean_list $distclean_list $dist_clean_more
   make distclean $mk_noc
   check_clean distclean $distclean_list
 fi
}
distribution()
{
 if [[ -f /etc/debian_version ]]; then
   dist=debian
 else
   for n in /etc/*-release; do
     m="${n%-release}"
     dist="${m#/etc/}"
     break
   done
 fi
 if [[ "$dist" != "gentoo" ]]; then
   not_gentoo=true
 else
   not_gentoo=false
 fi
}
main()
{
 cd "$(dirname "$(realpath "$0")")/.."
 local -i jobs=1 # Number of Make parallel jobs
 local nocolor
 if [[ "$GITHUB_ACTIONS" = "true" ]] && [[ `id -u` -ne 0 ]]; then
   nocolor=yes
 fi
 while getopts 'oj:' opt; do
   case $opt in
     o)
       nocolor=yes
       ;;
     j)
       jobs="0 + $OPTARG"
       ;;
   esac
 done
 if [[ $jobs -le 0 ]]; then
   jobs=1
 fi
 local n m out distclean_list dist not_gentoo dist_clean_more
 local -i last_ret
 distribution
 # Make sure we output to STDOUT directly, no pipes
 # check our teminal support coulors
 if [[ -n "$nocolor" ]]; then
   local -r mk_noc='NO_COL=1'
 elif [[ -t 1 ]] && tput setaf 1; then
   local -r esc=`printf '\e['`
   local -r color_red=${esc}31m
   local -r color_blue=${esc}34m
   local -r color_norm=${esc}00m
   printf "$color_norm"
 fi
 local clean_list="autom4te.cache/ config.log config.status"
 clean_list+=" configure defs.mk"
 for n in archlinux debian gentoo rpm; do
   distclean_list+=" $n/.upgrade_cockie"
 done
 eacmd git rev-parse --is-inside-work-tree
 if [[ $last_ret -eq 0 ]]; then
   local -r have_git=true
   local -r would='Would[[:space:]]remove[[:space:]]'
   test_clean
 else
   local -r have_git=false
   # Make sure we do not have leftovers
   ecmd make clean $mk_noc
 fi
 if $have_git && [[ -n "$(which reuse 2> /dev/null)" ]]; then
   echo " * Check files licenses with 'reuse'"
   ecmd reuse lint
   equit "'reuse' detect missing SPDX tags"
 fi
 echo " * Configure"
 autoconf
 if $not_gentoo; then
   ecmd make config $mk_noc
 else
   # Were is Gentoo defualt configure setting?
   # This is after the build flags, we use 64 bits container.
   ecmd ./configure --prefix=/usr --sysconfdir=/etc --localstatedir=/var/lib\
     --libdir=/usr/lib64
 fi
 tools/config_report.sh
 equit "Configuratation fails"
 if $not_gentoo; then
   # Gentoo miss the dot tool
   # Run syntax checking
   make checkall -j$jobs $mk_noc
 fi
 echo " * Run unit test"
 eacmd make utest -j$jobs $mk_noc
 equit "Unit test fails"
 echo " * Build"
 ecmd make -j$jobs $mk_noc
 equit "Build fails"
 test_clean
 case $dist in
   debian)
     if [[ -n "$(which dpkg-buildpackage 2> /dev/null)" ]]; then
       echo " * Build Debian packages"
       eacmd make deb -j$jobs $mk_noc
       equit "Build Debian packages fails"
       test_clean
     fi
     ;;
   fedora)
     if [[ -n "$(which rpmbuild 2> /dev/null)" ]]; then
       echo " * Build Fedora RPM packages"
       eacmd make rpm -j$jobs $mk_noc
       equit "Build Fedora RPM packages fails"
       test_clean
     fi
     ;;
   arch)
     if [[ -n "$(which makepkg 2> /dev/null)" ]]; then
       echo " * Build Arch Linux packages"
       eacmd make pkg -j$jobs MAKEFLAGS="-j$jobs" $mk_noc
       equit "Build Arch Linux packages fails"
       for n in archlinux/libptpmgmt-*.txz; do
         dist_clean_more+=" $n"
       done
       test_clean
     fi
     ;;
   gentoo)
     if [[ -n "$(which ebuild 2> /dev/null)" ]]; then
       echo " * Build Gentoo packages"
       eacmd make gentoo -j$jobs $mk_noc
       equit "Build Gentoo fails"
       test_clean
     fi
     ;;
 esac
 echo "$color_blue * CI test done$color_norm"
}
main "$@"
