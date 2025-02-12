# SPDX-License-Identifier: GPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright © 2021 Erez Geva <ErezGeva2@gmail.com>
#
# @author Erez Geva <ErezGeva2@@gmail.com>
# @copyright © 2021 Erez Geva
#
# RPM specification file for libptpmgmt rpm packages
###############################################################################
Name:           libptpmgmt
Version:        1.1
Release:        1%{?dist}
URL:            https://%{name}.nwtime.org
BuildRequires:  swig m4
BuildRequires:  perl perl-devel perl-ExtUtils-Embed
BuildRequires:  which
BuildRequires:  python3 python3-devel
BuildRequires:  lua lua-devel
BuildRequires:  ruby ruby-devel
BuildRequires:  php php-devel
BuildRequires:  tcl tcl-devel
BuildRequires:  golang
BuildRequires:  libfastjson libfastjson-devel json-c-devel
BuildRequires:  doxygen graphviz
#Source0:        https://github.com/erezgeva/%%{name}/archive/refs/tags/%%{version}.tar.gz
Source0:        %{name}-%{version}.txz

%define bname   ptpmgmt

License:        LGPLv3+
Summary:        PTP management library, to communicate with ptp4l
%description
PTP management library, to communicate with ptp4l

%package        jsonc
Summary:        PTP management library JSON plugin using the json-c library
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       json-c
%description    jsonc
PTP management library JSON plugin using the json-c library

%package        fastjson
Summary:        PTP management library JSON plugin using the fastjson library
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       libfastjson
%description    fastjson
PTP management library JSON plugin using the fastjson library

%package        devel
Summary:        Development files for the PTP management library
License:        LGPLv3+
Provides:       %{name}-static = %{version}-%{release}
Requires:       %{name}%{?_isa} = %{version}-%{release}
%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use the PTP management library.

%package        doc
Summary:        Documentation files for the PTP management library
License:        GFDLv1.3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
%description    doc
PTP management library documentation, to communicate with ptp4l

%package        perl
Summary:        PTP management library Perl wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       perl
%description    perl
PTP management library Perl wrapper

%package -n     python3-%{bname}
Summary:        PTP management library python version 3 wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       python3
%description -n python3-%{bname}
PTP management library python version 3 wrapper

%package -n     lua-%{bname}
Summary:        PTP management library Lua wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       lua
%description -n lua-%{bname}
PTP management library Lua wrapper

%package -n     ruby-%{bname}
Summary:        PTP management library ruby wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       ruby
%description -n ruby-%{bname}
PTP management library ruby wrapper

%package -n     php-%{bname}
Summary:        PTP management library php wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       php
%description -n php-%{bname}
PTP management library php wrapper

%package -n     tcl-%{bname}
Summary:        PTP management library tcl wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       tcl
%description -n tcl-%{bname}
PTP management library tcl wrapper

%package -n     golang-%{bname}
Summary:        PTP management library golang development wrapper
License:        LGPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
Requires:       golang
%description -n golang-%{bname}
PTP management library golang development wrapper

%package -n     pmc-%{bname}
Summary:        pmc tool
License:        GPLv3+
Requires:       %{name}%{?_isa} = %{version}-%{release}
%description -n pmc-%{bname}
 new rewrite of linuxptp pmc tool using the PTP managemen library.
 This tool is faster than the original linuxptp tool.

%package -n     phc-ctl-%{bname}
Summary:        phc_ctl tool
License:        GPLv3+
Requires:       python3-%{bname} = %{version}-%{release}
%description -n phc-ctl-%{bname}
 new rewrite of linuxptp phc_ctl tool using the PTP managemen library.

%prep
%setup -q

%build
autoconf
%configure --with-pmc-flags='-fPIE'
%make_build PMC_USE_LIB=so --no-print-directory all doxygen

%install
%make_install DEV_PKG=%{name}-devel --no-print-directory

%files
%{_libdir}/%{name}.so.1{,.*}

%files jsonc
%{_libdir}/%{name}_jsonc.so.1{,.*}

%files fastjson
%{_libdir}/%{name}_fastjson.so.1{,.*}

%files devel
%{_includedir}/*
%{_libdir}/%{name}*.a
%{_libdir}/%{name}*.so
%{_datadir}/%{name}-devel/*.mk

%files doc
%{_datadir}/doc/%{name}-doc/*

%files perl
%{_libdir}/perl5/PtpMgmtLib.pm
%{_libdir}/perl5/auto/PtpMgmtLib/PtpMgmtLib.so

%files -n python3-%{bname}
%{_libdir}/python3*/*/_%{bname}.cpython-*.so
%{_libdir}/python3*/*/%{bname}.py
%{_libdir}/python3*/*/*/%{bname}.*.pyc

%files -n lua-%{bname}
%{_libdir}/liblua-%{bname}.so*

%files -n ruby-%{bname}
%{_libdir}/ruby/*/%{bname}.so

%files -n php-%{bname}
%{_libdir}/php/*/%{bname}.so
%{_datadir}/pear/%{bname}.php

%files -n tcl-%{bname}
%{_libdir}/tcl*/%{bname}/%{bname}.so
%{_libdir}/tcl*/%{bname}/pkgIndex.tcl

%files -n golang-%{bname}
/usr/lib/golang/src/%{bname}/*

%files -n pmc-%{bname}
%{_sbindir}/pmc-%{bname}
%{_mandir}/man8/pmc-%{bname}.8*

%files -n phc-ctl-%{bname}
%{_sbindir}/phc_ctl-%{bname}
%{_mandir}/man8/phc_ctl-%{bname}.8*

###############################################################################
# The changelog is updated with the 'update_changelog.pl' script.
# Anything add after here will be toss, add only above.
# The changelog is under GFDL-1.3-no-invariants-or-later license.
###############################################################################
%changelog
* Sat Jun 10 2023 ErezGeva2@gmail.com 1.1-1
- Support Linuxptp version 4.0
- GitHub workflow
  - Pages: Install doxygen and generate documents on Ubuntu.
  - Continuous Integration
  - Add docker label.
  - Upload Docker containers.
  - Unit tests with valgrind and Address Sanitizer.
  - Run testing with a Dummy clock for linuxptp daemon.
    With valgrind and Address Sanitizer.
- Add setPhase() that call clock_adjtime() with ADJ_OFFSET.
- Improve Linux kernel support for PTP clases
- Fix wrong alignment assignment in MsgProc::proc.
- Add external "C" to system function override in
  unit test and dummy clock simulation.
- Fix phc_tool testing.
- Multiple indirect includes headers cleaning.
- Add unit test for the pmc tool build and output of different TLVs.
- Move common part of make docker script to tools.
- Improve format script.
- Move tools from root folder to tools folder.
  Previous tool was moved to ptp-tools
- Update Swig files from Swig project.
  The files are provided for backward compatible of old Swig.
- Remove the '-j' flag from make files.
  The number of jobs should be set by user.
- CI script
   - Use the CI script for all distributions.
   - Use proper unit tests for each distribution.
   - Build packages per distribution.
- Restore PHP in RPM, as new Fedora swig support PHP 8.
- Fix RPM 'config' target.
- Use system include for swig files.
- Add `utest_cpp` target to run unit test for C++ only.
- Remove stretch, as it's Long Term Support is over.
  https://wiki.debian.org/LTS/Stretch
  Remove `buster` as it is old Debian version.
- Add Go language wrapper.
- Add enumeration for delay mechanism.
- Add checks in buildTlv(), so we do not call callback for false cases.
- Handle cppcheck warning.
- Add cookie to docker creation, for updating docker containers.
- Ignore PTP message control Field as it was used by PTP v1.
- Add remark on Arc Linux license for documents.
  Most call them GFDL, yet Arc Linux use only FDL.
- Add support for Linuxptp power profile TLV.
- Remove restriction on domain number.
- Add maintainer tag to the Gentoo docker file.
- Follow linuxptp.
  Alphabetise configuration options and bump date in the pmc man page.
- Add support to Gentoo.
- Add release scripts to Arc Linux and for RPM.
- Improve make docker scripts.
- Add copyright sign to SPDX copyright.
- pmc tool: add macros for flags.
- Unit test: Use expect near for range assertion.
- Unit test: Use float compare for float numbers.
- Fix the source filed in reuse copyright file.
- Improve comments in SWIG definitions files.
- improve comments in callDef header.
- Add float types for seconds, floating and frequency.
- Swig
   - Apply int on clockid_t type.
   - Timestamp_t operator uses the float_seconds type.
- Add shell scripts to check format.
- Add working directory to docker run.
- Fix TCL package version to match library version.
- Fix JsonProcFromJson::procMng() it should not return the string.
- Remove small git ignore files.

* Mon Nov 21 2022 ErezGeva2@gmail.com 1.0-1
- Project uses 5 script languages, 2 JSON libraries,
  support 3 Linux distribution and support cross compilation.
- Add autoconf configuration as most distribution provide autoconf
  probing of current system.
- Remove all options from make file that are probed or
   set by the configuration script.
- Add unit tests
  - ptpmgmt library
  - scripts languages Message dispatcher and builder classes
  - Json to messages
  - load Json to messages libraries
- Build
  - Split make file
  - Catch Doxygen warning and exit with error.
  - Add unit test main to the unit test make file.
  - Improve format script and exit with error on error.
  - Probe astyle change and exit with error.
    So we can use the make format goal in
    a continuous integration checking container.
  - Check for dot application to use with doxygen.
  - Fix clean.
  - Update source files list for archive.
  - Fix installed man pages file mode.
  - Prevent copy of man pages on seconds install.
  - Add all markdown documentation to documentation package.
- Add update_doxygen.pl script file header.
- Fix Doxygen configuration.
- Update Doxygen to version 1.9.1
- Improve Doxygen handling.
- testing
  - Fix Lua using local library.
  - Improve testing.
  - Run all testing from root folder.
  - Use system libraries only if specify with a flag.
  - Add rule to run default configuration of distribution system.
- Add Message Builder Base C++ class to Lua.
  The C++ class destructor free the send TLV in the message object.
- Debian Bookworm have a installation bug with GCC C++ cross compiler.
- Add CI script to run all steps needed in CI:
   - check licenses with reuse tool
   - build Debian packages
   - pass format and Doxygen
   - run unit tests
   - verify no left over after clean and distribution clean.
   - The system test and build packages on other distribution is done
     out side CI.
- Merge headers used during compilation only,
  into a single compilation only header.
  The compilation header is used during compilation only,
  and do not hold any public API.
- Add a new error class to store the last error happened in the library.
  The library do not print the error to standard error no longer.
  It is up to the application how to handle the error.
- Improve error in: socket, PTP and clock classes.
- Rebase binary class to support operator [] with reference.
- Add support for Debian bookworm.
- Add docker for Debian to use on github CI.
- Create the version header by the make file.
  So we have only one configuration file created by configure.
  All the reset is created by the make file.
- Fix tcl library package version in index file.
- Fix Perl library folder location.
- Move source code and objects to sub-folders.
- Replace GCC prepossess with m4 to generate headers files.

* Tue Jul 26 2022 ErezGeva2@gmail.com 0.9-1
- Add header to define the C++ namespace.
- Swig generated code
  - Clean compilation warnings of swig generated code.
  - Define swig vector classes for new PTP structure.
  - Add vector class for LinuxptpUnicastMaster_t.
  - Remove python2, it is discontinue for 2 years.
  - Use proper typemap to initialise variables in argcargv.
- Swig project files
  - Add readme for swig files.
  - Follow swig 4.1.0
  - Add tcl std_vector.i from version 4.1.0 with bug fix.
- Message Dispatcher and builder classes.
  - Add Message.isValidId() to verify an management TLV ID.
  - Add C++ classes and native classes in each script language.
  - Call Message.clearData() in destructor of MessageBuilder.
  - Lua use C++ MessageBuilder object to ensure
    calling Message.clearData(), as Lua native do not use destructors.
  - Beside Lua using C++ MessageBuilder, C++ classes are not wrapped by Swig!
  - Use abstract classes in PHP
  - MessageDispatcher::callHadler() call virtual noTlvCallBack()
    if the TLV call back is not inherited.
  - Use C++ dispatcher and builder classes in the pmc tool.
- Remove ids.h header from public headers, use generated headers instead.
- Move process functions of the message class to a private class.
- Testing
  - Use python and lua environment to load the wrapper library.
  - Add support to test with AddressSanitizer.
  - Add simple test for std::vector<> class in scripts.
  - Add testing with phc_ctl and valgrind with phc_ctl.
  - And valgrind test of testJson.pl.
- Improve Debian build.
- List source files with git when possible.
- JSON
  - Move From JSON part to a new source file.
    So user can call msg2json() using static link without linking
     the jsonFrom static library.
  - Move jsonFrom to separate static libraries.
    One library using JSON-c, the other one using the fast-JSON.
  - Users can select which library they wish to use during link.
    The shared library is loaded dynamically on run time.
    User can prefer shared library to use by calling selectLib().
  - Add function to get name of used fromJson library.
- PTP class
  - Add all PHC Linux kernel ioctls.
  - Add PTP and system clock reading and setting.
  - Use Timestamp_t for clock read and set.
  - Add PTP clock frequency and offset functions.
  - Add PHC pins functionality.
  - Add header with time conversion constants.
  - Clone the PHC control tool using the python wrapper library,
    and add new package for it.
- Add pure attribute for proper functions.
- Make file
  - Use Make file one shell option.
  - Improve make file syntax.
  - Use one rule for all swig creation.
  - Remove empty lines when using V=1.
  - Improve Doxygen filter of warnings.
- Follow IEEE 1558 alternative terms for "master" and "slave".
  Source and client are not confirming to IEEE 1558 amendment.
- Perform valgrind test on pmc.
  - valgrind found an issue in the ConfigFile
    Change cfgGlobal type from a reference to a pointer.
- Documentation.
  - Update documentation.
  - Add pmc.8 from linuxptp with updates.
  - Add phc_ctl.8 from linuxptp with updates.
  - Update vecDef.cc and
    add documentation on std::vector<> mapping.
  - Add Frequently Asked Questions.
  - Doxygen configuration: remark all default setting.
  - New doxygen on Arch Linux have:
      https://github.com/doxygen/doxygen/issues/9319
      The bug needs a fix in Arch Linux, it is out of scope of this project.

* Sat May 14 2022 ErezGeva2@gmail.com 0.8-1
- Fix cross compilation in make-file.
  install target should install and not build.
- Remove end.h err.h headers from Doxygen and development package.
- Properly read all dependencies files in make-file.
- Socket class
  - Default socket file for non-root users.
    Prefer system run folder per user over home directory.
  - Improve socket closing.
- From JSON to message support
  - Create separate libraries to process JSON into message.
    Each of them depends on a JSON library.
    Load one of them on run time when user try to process a JSON.
    This way we keep the main library loading
     with standard libraries only.
  - Add function to load a specific jsonFrom library.
  - Static library uses only one JSON library.
      User will need to link with this JSON library
      once the user application needs the functionality.
- Add support for Options class in scripts
  - Add SWIG support to call argc-argv using C main style from scripts.
  - Add Lua, Perl, Tcl and PHP SWIG argcargv.i
    that are missing from the SWIG projecy.
    These argcargv.i are plan for the SWIG project.
  - Use the Options class in testing scripts.
  - Change the string types in Pmc_option structure to C++.
  - Add the Option and Init class to SWIG build.
- Create a new header with types used by the message class.
- Reduce the number of C macros and use project style for the reset
  - As macros could conflict with other projects or headers.
    We use C++, so we can define them in enumerators as possible,
    use project style, to reduce conflicts, and move macros to module,
    and reduce the number of public macros.
  - Use project macro name style in SWIG definition file.
  - Remove macros from ids header.
  - Rename 'caseXXX' macros. But leave 'A' unchanged for now.
  - Move first and last IDs to the new 'types' header, in the enumerator.
    So, they are not macros anymore.
  - Move shortcut macro of allowed action for
    a management ID to the message module. No reason to have them public.
  - Replace vector macros with template functions.
  - Move macros from message header to process module.
    No reason to have them public.
  - We left:
    14 headers with protection macros follow the __PTPMGMT_XXX_H format.
    3 macros with library version on compilation, LIBPTPMGMT_VER...
    3 macros for 48 bits limit INT48_XXX, follow the standard.
    5 _ptpmCaseXX macros and 'A' macro used internally with the ids.h header.
    _ptpmParseFunc macro used internally in the message class.
- Improve documentation in headers.
- Add functions for using the allowSigTlvs map in MsgParams.
  So, scripts can not access the map.

* Mon May 02 2022 ErezGeva2@gmail.com 0.7-1
- Change socket receive functions to non-block be default.
- Make file
  - Use macros for system includes in dependencies files.
  - Add dependencies files to SWIG created source code.
- Use C++ namespace and Tcl namespace.
- Use bash internal test, save calling external test.
- Arch Linux
  - Use SHA256
  - Fix description of development and PMC packages.

* Mon Apr 18 2022 ErezGeva2@gmail.com 0.6-1
- Improve make file
  - Add missing files to source tar file for RPM and Arch Linux
  - Add Cppcheck code analysis on format rule.
- Compliant with the FSF REUSE Specification 3.0
- Python SWIG
  - Support multithread, make sure poll() and tpoll()
-     do not block other threads
  - Use flag for single thread mode
- Add fileno() interface as it follow POSIX function and
-   also supported by Python select module, and probably more.
- Convert C macros for NP_SUBSCRIBE_EVENTS with method
-   to support scripting.
- Sample code
  - Add sample/sync_watch.py by Martin Pecka form
-     Czech Technical University in Prague
  - Add sample code for probing PTP daemon for synchronization.
- Improve RPM specification.
- Add no return attribute to signal handlers.
- LinuxPTP new management TLVs:
  - PORT_HWCLOCK_NP TLV.
  - UNICAST_MASTER_TABLE_NP TLV.
- Improve copyright.
- Add header with version for compilation.
-   in addition to version in run time.
- Improve testing script:
  - Verify Linux PTP configuration file exit.
  - Check that script can use sudo unless we use the root user.
  - Probe Unix socket file location from configuration file.
  - Try to change Linux PTP Unix socket file permissions.
  - Patch Linux PTP daemon if Unix socket file does not exist.
  - Show Linux PTP daemon command line only if we find the folder.
- Message process of float 64
  - Remove wrong calling to move
  - Improve function syntax
  - Add macro for rare hardware which does not use IEEE 754.
- Add new linuxptp linuxptpPowerProfileVersion_e enumerator
- Fix wrong process of linuxptpTimeStamp_e enumerator in json module.
- Use short string form of clockAccuracy_e enumerator.
- cast characters in Binary::eui48ToEui64().
- Add constant modifier to configuration file reference
-   in the socket classes.

* Sun Oct 31 2021 ErezGeva2@gmail.com 0.5-1
- Add equal and less than operators to Binary, ClockIdentity_t,
  PortIdentity_t and PortAddress_t.
- Update the read me.
- Add sample code after Vladimir Oltean checksync application.
- Add constant modifier to methods that do not modify the object.
- Comply to format and improve Doxygen comments.
- Remove python 2 from mandatory list of testing.
- Improve includes in source code.
- Fix make file help errors.
- Add init class for the PMC application.
- Add class to process command line parameters for the pmc tool.
  So, users can use it for other applications.
- Fix cross compilation errors.
- Add tcl wrapper.
- Remove Debian depends on python2, as it will be removed in the future.
- Ensure port number is 16 bits,
  as Linux process ID can be larger than 16 bits.
- Ignore deprecated ruby functions created by swig.
- Use python 3 configuration application to set the library extension.
- Add rule for Debian cross target build.
- Fix proper capitalization in RPM specification.

* Mon Jun 14 2021 ErezGeva2@gmail.com 0.4-1
- Improve check after calling strtol.
- Use C++ short loop form.
- Add reference and constant for automatic when possible.
- Add constant when possible.
- Add message clear send data, to prevent use after caller delete the data.
- Binary module:
  - Encapsulate strtok_r in a class.
  - Add function to call strtol.
- Add header for the pmc tool.
- Testing scripts for script wrappers:
  - Set domainNumber from config file
  - Set domain number and transport Specific form configuration file.
  - Add new Master only TLV to testing.
  - Add function for next sequence with range check.
  - return -1 on error and 0 on success.
  - Fix indentation to 2 spaces.
  - Add global statement in python.
- Move Debian rules to Debian make
- Adding Arch Linux packages build.
- Adding RPM build with Fedora container.
- Make:
  - Support single python version.
  - improve verCheck to support 3 version numbers.
  - PHP 7 need Swig 3.0.12

* Tue Apr 20 2021 ErezGeva2@gmail.com 0.3-1
- Add licence to Javadoc comments for Doxygen process in addition to SPDX tag.
- Set document licence to GNU Free Documentation License version 1.3
- JSON module: for
  - Message to JSON and JSON to message
  - JSON to message require C JSON library or the fast C JSON library.
  - Parse signaling messages.
  - Handle TLVs with array.
  - Handle linuxptp Events and statistics TLVs.
  - Add testing for JSON module.
  - Add macros for JSON library function and types,
    In case we need to change then in future.
  - Add function to parse from JSON object,
    User can embedded the message in a JSON message.
  - Add convector of JSON types.
- Add error macros
- Add PHP wrapper.
- Replace use of std::move with unique_ptr reset() function.
- Parse MANAGEMENT_ERROR_STATUS in signaling message.
- PMC tool:
  - Set unique_ptr after socket creation and before internalizing.
    In case socket internalize fials, unique_ptr will release it.
  - Use unique_ptr reset() function.
  - Add macros in pmc tool for both errors and normal dumps.
  - Fix TimeInterval sign, update peerMeanPathDelay,
- Fix headers in development package.
- Make file
  - Move SONAME definition from Debian rules.
  - Debian rules only set a flag to link with soname.
- Improve socket for wrappers.
  - Use Buffer object in send and receive functions.
  - Move all virtual functions to protected,
    and add functions in the base class to call them.
  - Mark SockBase and SockBaseIf in SWIG file.
  - Add rcvFrom with from address split to additional function for scripting.
- Add parse and build in message that uses reference to Buffer object.
- Fix TimeInterval sign.
  - getIntervalInt() return sign integer.
- Add getBin() to Binary to fetch octet from Binary.
- Update the read-me and the Time-Interval documentation in message module.

* Mon Apr 05 2021 ErezGeva2@gmail.com 0.2-1
- Add Ruby to read-me.
- Add long options to the pmc tool.
- Add pmc tool help after linuxptp.
- Add support for padding get action management TLVs.
- Fix Debian cross compilation.
- Support Debian Stretch rename python2 to python.
- Designated initializers are not supported in old compilers.
- Old math.h header uses DOMAIN macro, as we do not use math macro,
-  just remove it.
- Add help for make file.
- Add macros in make file to prevent Swig targets.
- testing script support linuxptp location with spaces.
- Fix python3 by adding a new class for allocating the buffer.
- Remove convert to buffer. All scripts use the buffer class.
- pmc tool: add mode for PTP network layer in run mode.
- Add check in Ruby for capitalizing first letter.
- testing script: fix check for installed libraries on system.
- Add support for ruby
- Start classes with capital.
- Prevent format of rebuild all.
- Improve testing scripts.
- Add masking for flags in proc module.
- Add flag for build only and for TLVs with variable length.
- Add vector handling classes for scripts.
- Prepare for using a different implementation specific management TLVs.
- Add key flag to pmc build.
- Use optimization for fast execution When packaging.
- Add signaling messages support.
- Add IEEE 754 64-bit floating point.
- Add install goal in make file.
- Debian rules uses make file install goal.
- Fix overflow in configuration class.
- Fix compilation warnings.
- Spelling.
- Use Debian site man pages as they are more updated.

* Sun Mar 21 2021 ErezGeva2@gmail.com 0.1-1
- Alpha version.
