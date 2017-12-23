Name: efind-text-tools
Version: 0.2.0
Release:        1%{?dist}
Summary: Filter text files by content.

License: GPLv3+
URL: https://github.com/20centaurifux/efind-text-tools
Source0: efind-text-tools-0.2.0.tar.xz

Requires: efind

%description
Filter text files by content.

%global debug_package %{nil}

%prep
%setup -q


%build
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
%make_install


%files
%{_libdir}/efind/extensions/*.so



%changelog
