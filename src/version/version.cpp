#include "version/version.h"

namespace UniversalModels
{
    QString GetVersion()
    {
#ifdef STATIC_PROJECT
        return QString("NOT DEFINED FOR STATIC BUILD");
#else
        return QString(GIT_VERSION);
#endif
    }
}
