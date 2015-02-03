%define _optdir	/opt
%define _appdir	%{_optdir}/apps

Name:       libttssmt
Summary:    Text To Speech smt plugin shared library
Version:    0.0.32
Release:    1
Group:      Graphics & UI Framework/Voice Framework
License:    Flora-1.1
Source0:    %{name}-%{version}.tar.gz
Source1001: 	libttssmt.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(tts)

provides : libsmt.so

%description
Description: Text To Speech smt plugin shared library


%prep
%setup -q
cp %{SOURCE1001} .

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}

%build
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/voice/tts/1.0/engine/*
/usr/share/voice/tts/smt_vdata/*
%{_libdir}/libsmt.so*
