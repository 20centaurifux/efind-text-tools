# Maintainer: Sebastian Fedrau <sebastian.fedrau@gmail.com>
pkgname=efind-text-tools
pkgver=0.1.0
pkgrel=1
epoch=
pkgdesc="Filter text files by content."
arch=('i686' 'x86_64')
url="https://github.com/20centaurifux/efind-text-tools"
license=('GPL3')
groups=()
depends=('efind>=0.1.0')
makedepends=()
checkdepends=()
optdepends=()
provides=()
conflicts=()
replaces=()
backup=()
options=()
install=
changelog=
source=("$pkgname-$pkgver.tar.xz")
noextract=()
md5sums=('')
validpgpkeys=()

build() {
	cd "$pkgname-$pkgver"
	make
}

package() {
	cd "$pkgname-$pkgver"
	make DESTDIR="$pkgdir/" install
}
