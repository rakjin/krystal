krystal
=======

lex&amp;yacc practice

- http://www.thebinaryidiot.com/archives/2010/03/06/flex-bison-cpp-example
- http://www.thebinaryidiot.com/archives/2010/04/19/ini-parser-using-flex-and-bison-in-c
- http://panthema.net/2007/flex-bison-cpp-example

on windows, install them:

- install MinGW (http://www.mingw.org/download/installer)
    - install MinGW-installer and get gcc, g++, flex, bison and etc.
    - MinGW packages to install:
        - MinGW > MinGW Base System >
            - mingw32-base
            - mingw32-gcc
            - mingw32-gcc-g++
        - MSYS > MSYS Base System >
            - msys-base
            - msys-make
        - MSYS > MinGW Developer Toolkit >
            - msys-bison
            - msys-flex
    - then,
        - add to $Path:
            - C:\MinGW\bin;C:\MinGW\msys\1.0\bin
        - copy C:\MinGW\bin\libstdc++-6.dll file to C:\Windows\SysWOW64\ (x64)
            - http://stackoverflow.com/questions/14889407/the-procedure-entry-point-gxx-personality-sj0-could-not-be-located-in

on os x, update bison version to 2.4.1 or newer. in terminal,

```
brew --prefix
brew versions bison
git checkout 804bcf6 Library/Formula/bison.rb
brew unlink bison
brew install bison
bison --version
brew switch bison 2.7.1
bison --version
which bison
ls -lah /usr/local/Cellar/bison/2.7.1/bin/
yacc --version
/usr/local/Cellar/bison/2.7.1/bin/bison --version
/usr/local/Cellar/bison/2.7.1/bin/yacc --version
which yacc
mv /usr/bin/yacc /usr/bin/yacc-2.3
sudo mv /usr/bin/yacc /usr/bin/yacc-2.3
sudo mv /usr/bin/bison /usr/bin/bison-2.3
sudo ln -s /usr/local/Cellar/bison/2.7.1/bin/bison /usr/bin/bison
sudo ln -s /usr/local/Cellar/bison/2.7.1/bin/yacc /usr/bin/yacc
bison --version
```

http://stackoverflow.com/questions/3987683/homebrew-install-specific-version-of-formula

## hash calculation

depends on boost::hash and boost::hash_combine

### `Packet.getHash`

* hash = hash(packetName)
* foreach PacketMembers as member:
    * combineHash(hash, member.getHash)
* return hash

### `PacketMember.getHash`

* hash = PacketMemberType.getHash
* combineHash(hash, PacketMemberName.getHash)
* return hash

### `PacketMemberType.getHash`

* if PRIMITIVE_TYPE:
    * hash = hash(typeName)
* else if REFERENCE_TYPE:
    * PacketNode = lookupNodeByName(value)
    * hash = hash(PacketNode.getHash)
* else if MAP:
    * hash = hash((int)Parser::token::MAP)
    * combineHash(hash, generic1.getHash)
    * combineHash(hash, generic2.getHash)
* else if LIST:
    * hash = hash((int)Parser::token::LIST)
    * combineHash(hash, generic1.getHash)
* return hash

### `PacketMemberName.getHash`

* return hash(name)

### example

```
packet A {
    long A1;
};

packet B {
    short B1;
};

packet C {
    double C1;
    A C2;
    map<int, A> C3;
    list<B> C4;
};
```

assume that `A + B` means `hash_combine(hash("A"), hash("B"))`:

hash of packet C produced by:

```
(
    C
    +
    (
        double
        +
        C1
    )
    +
    (
        (
            A
            +
            (
                long
                +
                A1
            )
        )
        +
        C2
    )
    +
    (
        (
            Parser::token::MAP
            +
            int
            +
            (
                A
                +
                (
                    long
                    +
                    A1
                )
            )
        )
        +
        C3
    )
    +
    (
        (
            Parser::token::LIST
            +
            (
                B
                +
                (
                    short
                    +
                    B1
                )
            )
        )
        +
        C4
    )
)
```

### NOTE:
dependency to Parser::token::MAP and LIST while hash calculation
may unexpectedly change hash result
if bison/flex implementations vary.
