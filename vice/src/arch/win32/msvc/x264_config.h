#define X264_BIT_DEPTH     8
#define X264_GPL           1
#define X264_INTERLACED    1
#define X264_CHROMA_FORMAT 0
#define X264_VERSION ""
#define X264_POINTVER "0.142.x"

/* Disable some generic MSVC 7+ warnings for quicker compile */
#if (_MSC_VER >= 1300)
#pragma warning(disable: 4003) /* not enough actual parameters for macro ... */
#pragma warning(disable: 4018) /* signed/unsigned mismatch */
#pragma warning(disable: 4090) /* different 'const' qualifiers */
#pragma warning(disable: 4244) /* conversion from ... to ..., possible loss of data */
#pragma warning(disable: 4305) /* truncation from 'double' to 'float' */
#pragma warning(disable: 4700) /* local variable ... used without having been initialized */
#endif

/* Disable some MSVC 10+ warnings for quicker compile */
#if (_MSC_VER >= 1600)
#pragma warning(disable: 4996) /* '...': The POSIX name for this item is deprecated. Instead, use the ISO C++ conformant name: .... See online help for details. */
#endif
