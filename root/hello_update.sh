cd sounds
phpize
./configure --enable-sounds
make clean all
sudo apachectl restart