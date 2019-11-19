apt-get -y install libmpc-dev
apt-get -y install m4
python3 -m pip install mpi4py gmpy2
mkdir deleteme
cd deleteme
    wget https://gmplib.org/download/gmp/gmp-6.1.2.tar.bz2
    tar -xaf gmp-6.1.2.tar.bz2
    cd gmp-6.1.2
        ./configure
        make -j4
        make install
    cd ..
cd ..
rm -rf deleteme
