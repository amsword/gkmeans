

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Fri Jan 11 21:30:48 2013
 */
/* Compiler settings for win64\mwcomutil.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=IA64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IMWUtil,0xC47EA90E,0x56D1,0x11d5,0xB1,0x59,0x00,0xD0,0xB7,0xBA,0x75,0x44);


MIDL_DEFINE_GUID(IID, LIBID_MWComUtil,0xFE430597,0x0370,0x4AD7,0x98,0xD1,0xBF,0x77,0x72,0xD1,0x27,0xC3);


MIDL_DEFINE_GUID(CLSID, CLSID_MWField,0xBA815CED,0x8D35,0x4E89,0x91,0x0F,0xE1,0xEB,0x03,0xAA,0x6E,0xFE);


MIDL_DEFINE_GUID(CLSID, CLSID_MWStruct,0x5B623855,0x547C,0x4BEB,0xAA,0x0F,0x74,0x05,0x15,0xE6,0xD4,0xF0);


MIDL_DEFINE_GUID(CLSID, CLSID_MWComplex,0x8CAC0E2D,0xEBAB,0x4D28,0xAB,0x16,0x37,0x29,0x58,0xE8,0x9B,0x5B);


MIDL_DEFINE_GUID(CLSID, CLSID_MWSparse,0x1C7D076B,0x55EB,0x418D,0x86,0x5D,0x40,0x88,0xF5,0x2A,0xC5,0x7C);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArg,0xF32B54A7,0xB4E0,0x4B44,0xA0,0x2F,0xF1,0xF0,0x98,0xF5,0xDE,0xF2);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArrayFormatFlags,0xF3C1416B,0x8AA2,0x4663,0xAA,0x99,0x40,0x21,0xC4,0xBA,0x37,0x0D);


MIDL_DEFINE_GUID(CLSID, CLSID_MWDataConversionFlags,0xC58F38A3,0xB5E8,0x44FF,0xB7,0x49,0xC3,0x8E,0x31,0xBE,0xB9,0x52);


MIDL_DEFINE_GUID(CLSID, CLSID_MWUtil,0x0FD82AF1,0xCC30,0x4DBD,0x97,0x31,0x61,0x24,0xED,0x60,0x0A,0xE2);


MIDL_DEFINE_GUID(CLSID, CLSID_MWFlags,0x292FF5B0,0x8D7E,0x482F,0xB2,0x94,0x92,0x95,0xE6,0x36,0x27,0xBB);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



