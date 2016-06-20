Name:       libttssmt
Summary:    Text To Speech smt plugin shared library
Version:    0.1.2
Release:    1
Group:      Graphics & UI Framework/Voice Framework
License:    Flora-1.1
Source0:    %{name}-%{version}.tar.gz
Source1001: 	libttssmt.manifest
BuildRequires:  cmake
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(libtzplatform-config)
BuildRequires:  pkgconfig(tts)
BuildRequires:	pkgconfig(tts-engine)
BuildRequires:  pkgconfig(libxml-2.0)

BuildRequires:  pkgconfig(appcore-efl)
BuildRequires:  pkgconfig(elementary)
BuildRequires:  pkgconfig(ecore)
BuildRequires:  pkgconfig(aul)
BuildRequires:  pkgconfig(capi-appfw-application)
BuildRequires:  pkgconfig(efl-extension)
BuildRequires:  pkgconfig(capi-web-url-download)

provides : libsmt.so

%description
Description: Text To Speech smt plugin shared library


%prep
%setup -q
cp %{SOURCE1001} .

%define APP_PREFIX	%{TZ_SYS_RO_APP}/org.tizen.ttssmt-setting
%define APP_MANIFESTDIR %{TZ_SYS_RO_PACKAGES}

cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DLIBDIR=%{_libdir} -DINCLUDEDIR=%{_includedir} \
        -DTZ_SYS_RO_SHARE=%TZ_SYS_RO_SHARE -DTZ_SYS_BIN=%TZ_SYS_BIN \
        -DAPP_INSTALL_PREFIX=%{APP_PREFIX} -DAPP_MANIFESTDIR=%{APP_MANIFESTDIR}

%build
export CFLAGS="${CFLAGS} -fPIC -fvisibility=hidden"
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}%{TZ_SYS_RO_SHARE}/license
cp %{_builddir}/%{name}-%{version}/LICENSE.Flora %{buildroot}%{TZ_SYS_RO_SHARE}/license/%{name}

%files
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libsmt.so*
%{TZ_SYS_RO_SHARE}/voice/tts/1.0/engine/*
%{TZ_SYS_RO_SHARE}/voice/tts/smt_vdata/*
%{TZ_SYS_RO_SHARE}/voice/tts/1.0/engine-info/ttssmt-info.xml
%{TZ_SYS_RO_SHARE}/license/%{name}
%{TZ_SYS_RO_APP}/org.tizen.ttssmt-setting/bin/ttssmt-setting
%{TZ_SYS_RO_PACKAGES}/org.tizen.ttssmt-setting.xml