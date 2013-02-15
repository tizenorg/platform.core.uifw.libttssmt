#sbs-git:slp/pkgs/l/libttssmt libttssmt 0.0.15 9c82c715d55cf32bb3d69112526145654ec2fbcc
%define _optdir	/opt
%define _appdir	%{_optdir}/apps



Name:       libttssmt
Summary:    Text To Speech smt plugin shared library
Version:    0.0.28
Release:    1
Group:      TO_BE/FILLED_IN
License:    TO_BE/FILLED_IN
Source0:    %{name}-%{version}.tar.gz
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(tts)

provides : libsmt.so

%description
Description: Text To Speech smt plugin shared library


%prep
%setup -q

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}

%build
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest libttssmt.manifest
%defattr(-,root,root,-)
%{_libdir}/voice/tts/1.0/engine/*
/usr/share/voice/tts/smt_vdata/*
%{_libdir}/libsmt.so*
