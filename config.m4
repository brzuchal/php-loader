PHP_ARG_ENABLE(LOADER, whether to enable Loader support,
[ --enable-loader   Enable Loader support])

if test "$PHP_LOADER" = "yes"; then
  AC_DEFINE(HAVE_LOADER, 1, [loader])
  PHP_NEW_EXTENSION(loader, loader.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
