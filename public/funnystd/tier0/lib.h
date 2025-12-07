//================= Copyright kotofyt, All rights reserved ==================//
// Purpose: A header which implements or redefines libc implementation.
// It is defined with V_ prefix to differentiate own implementation from
// given libc by the OS.
//===========================================================================//

#ifndef TIER0_STDLIB_H
#define TIER0_STDLIB_H

#include "tier0/minmax_on.h"

#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"

//-----------------------------------------------------------------------------
// string.h
//-----------------------------------------------------------------------------
#define V_memcpy memcpy
#define V_memmove memmove
#define V_memchr memchr
#define V_memcmp memcmp
#define V_memset memset

#define V_strcat strcat
#define V_strncat strncat
#define V_strchr strchr
#define V_strrchr strrchr
#define V_strcmp strcmp
#define V_strncmp strncmp
#define V_strcoll strcoll
#define V_strcpy strcpy
#define V_strncpy strncpy
#define V_strlen strlen
#define V_strnlen strnlen
#define V_strspn strspn
#define V_strcspn strcspn
#define V_strpbrk strpbrk
#define V_strstr strstr
#define V_strtok strtok
#define V_strxfrm strxfrm

#ifdef __WIN32__
#define V_stricmp stricmp
#else
#define V_stricmp strcasecmp
#endif

//-----------------------------------------------------------------------------
// stdio.h
//-----------------------------------------------------------------------------
#define V_fclose fclose
#define V_fopen fopen
#define V_freopen freopen
#define V_fdopen fdopen
#define V_remove remove
#define V_rename rename
#define V_rewind rewind
#define V_tmpfile tmpfile

#define V_feof feof
#define V_ferror ferror
#define V_fflush fflush
#define V_fgetpos fgetpos
#define V_fgetc getc
#define V_fgets gets
#define V_fputc putc
#define V_fputs puts
#define V_ftell ftell
#define V_fseek fseek
#define V_fsetpos fsetpos
#define V_fread fread
#define V_fwrite fwrite
#define V_getc getc
#define V_getchar getchar
#define V_gets gets
#define V_printf printf
#define V_vprintf vprintf
#define V_fprintf fprintf
#define V_vfprintf vfprintf
#define V_sprintf sprintf
#define V_snprintf snprintf
#define V_vsprintf vsprintf
#define V_vsnprintf vsnprintf
#define V_perror perror
#define V_putc putc
#define V_putchar putchar
#define V_fputchar fputchar
#define V_scanf scanf
#define V_sscanf sscanf
#define V_vscanf vscanf
#define V_fscanf fscanf
#define V_vfscanf vfscanf
#define V_vsscanf vsscanf
#define V_setbuf setbuf
#define V_setvbuf setvbuf
#define V_tmpnam tmpnam
#define V_ungetc ungetc
#define V_puts puts


//-----------------------------------------------------------------------------
// stdlib.h
//-----------------------------------------------------------------------------
#define V_atoi atoi
#define V_atof atof

//-----------------------------------------------------------------------------
// ctype.h
//-----------------------------------------------------------------------------

#define V_isalnum isalnum
#define V_isalpha isalpha
#define V_isblank isblank
#define V_iscntrl iscntrl
#define V_isdigit isdigit
#define V_isgraph isgraph
#define V_islower islower
#define V_isprint isprint
#define V_ispunct ispunct
#define V_isspace isspace
#define V_isupper isupper
#define V_isxdigit isxdigit
#define V_tolower tolower
#define V_toupper toupper


#endif
