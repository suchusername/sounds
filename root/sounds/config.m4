dnl config.m4 for extension sounds

PHP_ARG_ENABLE(sounds, 
	[whether to enable sounds support],
	[ --enable-sounds   Enable "sounds" extension support])

if test "$PHP_SOUNDS" = "yes"; then
	CXX_FLAGS="-std=c++11"
	PHP_REQUIRE_CXX()
	PHP_SUBST(SOUNDS_SHARED_LIBADD)
	PHP_ADD_LIBRARY(stdc++, 1, SOUNDS_SHARED_LIBADD)
	PHP_NEW_EXTENSION(sounds, sounds.cc, $ext_shared)
fi