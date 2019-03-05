![Veles-Logo](https://i.imgur.com/OP0aW7y.jpg?3)

Veles Core integration/staging tree
======================================
![Licence](https://img.shields.io/github/license/velescore/veles.svg?style=for-the-badge)   ![Latest Release](https://img.shields.io/github/release/velescore/veles.svg?style=for-the-badge) ![Master Build Status](https://img.shields.io/travis/com/velescore/veles/master.svg?style=for-the-badge)

https://veles.network

About Veles
------------
Veles Core is innovative cryptocurrency supporting multiple PoW algorithms, with unique dynamic block reward system. Main goal of the project is to implement decentralised VPN solution to protect privacy and anonymity of Internet users around the globe.


Development Process
-------------------

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/velescore/veles/tags) are created
regularly to indicate new official, stable release versions of Veles Core.

The contribution workflow is described in [CONTRIBUTING.md](CONTRIBUTING.md)
and useful hints for developers can be found in [doc/developer-notes.md](doc/developer-notes.md).

The Travis CI system makes sure that every pull request is built for Windows, Linux, and macOS, and that unit/sanity tests are run automatically.


Build Instructions and Notes
-----------------------------
1.  Clone the repository and checkout to latest stable release [Tag](https://github.com/velescore/veles/tags) using following commands. (Alternatively you can download and extract the latest source tarball manually from [Releases](https://github.com/velescore/veles/releases) page on our GitHub.)

        git clone https://github.com/velescore/veles.git
        cd veles
        git checkout `git tag | sort -V | grep -v "\-rc" | tail -1`

2.  Build Veles Core:
    Configure and build the headless Veles Core binaries as well as the GUI (if Qt is found).
    You can disable the GUI build by passing `--without-gui` to configure.
        
        ./autogen.sh
        ./configure
        make

3.  It is recommended to build and run the unit tests:

        make check
        
### Linux (Ubuntu) Notes
1.  Update your package index

        sudo apt-get update

2.  Install required dependencies from default repository

        sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils libboost-all-dev

3.  Install latest Berkeley DB 4.8 from Bitcoin repository

        sudo apt-get install software-properties-common
        sudo add-apt-repository ppa:bitcoin/bitcoin
        sudo apt-get update
        sudo apt-get install libdb4.8-dev libdb4.8++-dev

#### GUI wallet dependencies
If you also want to build an official GUI wallet, you'll need to perform the following step as well and install additional dependencies:

4.  Install QT 5

        sudo apt-get install libminiupnpc-dev libzmq3-dev
        sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev



### macOS Notes
See (doc/build-osx.md) for instructions on building on Mac OS X.

### Windows (64/32 bit) Notes
See (doc/build-windows.md) for instructions on building on Windows 64/32 bit.


License
-------

Veles Core is released under the terms of the MIT license. See [COPYING](COPYING) for more
information or see https://opensource.org/licenses/MIT.


