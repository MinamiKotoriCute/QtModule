#ifndef WEB_GLOBAL_H
#define WEB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(WEB_LIBRARY)
#  define WEBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define WEBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // WEB_GLOBAL_H
