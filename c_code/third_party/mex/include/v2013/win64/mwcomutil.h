

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


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


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __mwcomutil_h__
#define __mwcomutil_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IMWUtil_FWD_DEFINED__
#define __IMWUtil_FWD_DEFINED__
typedef interface IMWUtil IMWUtil;
#endif 	/* __IMWUtil_FWD_DEFINED__ */


#ifndef __MWField_FWD_DEFINED__
#define __MWField_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWField MWField;
#else
typedef struct MWField MWField;
#endif /* __cplusplus */

#endif 	/* __MWField_FWD_DEFINED__ */


#ifndef __MWStruct_FWD_DEFINED__
#define __MWStruct_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWStruct MWStruct;
#else
typedef struct MWStruct MWStruct;
#endif /* __cplusplus */

#endif 	/* __MWStruct_FWD_DEFINED__ */


#ifndef __MWComplex_FWD_DEFINED__
#define __MWComplex_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWComplex MWComplex;
#else
typedef struct MWComplex MWComplex;
#endif /* __cplusplus */

#endif 	/* __MWComplex_FWD_DEFINED__ */


#ifndef __MWSparse_FWD_DEFINED__
#define __MWSparse_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWSparse MWSparse;
#else
typedef struct MWSparse MWSparse;
#endif /* __cplusplus */

#endif 	/* __MWSparse_FWD_DEFINED__ */


#ifndef __MWArg_FWD_DEFINED__
#define __MWArg_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWArg MWArg;
#else
typedef struct MWArg MWArg;
#endif /* __cplusplus */

#endif 	/* __MWArg_FWD_DEFINED__ */


#ifndef __MWArrayFormatFlags_FWD_DEFINED__
#define __MWArrayFormatFlags_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWArrayFormatFlags MWArrayFormatFlags;
#else
typedef struct MWArrayFormatFlags MWArrayFormatFlags;
#endif /* __cplusplus */

#endif 	/* __MWArrayFormatFlags_FWD_DEFINED__ */


#ifndef __MWDataConversionFlags_FWD_DEFINED__
#define __MWDataConversionFlags_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWDataConversionFlags MWDataConversionFlags;
#else
typedef struct MWDataConversionFlags MWDataConversionFlags;
#endif /* __cplusplus */

#endif 	/* __MWDataConversionFlags_FWD_DEFINED__ */


#ifndef __MWUtil_FWD_DEFINED__
#define __MWUtil_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWUtil MWUtil;
#else
typedef struct MWUtil MWUtil;
#endif /* __cplusplus */

#endif 	/* __MWUtil_FWD_DEFINED__ */


#ifndef __MWFlags_FWD_DEFINED__
#define __MWFlags_FWD_DEFINED__

#ifdef __cplusplus
typedef class MWFlags MWFlags;
#else
typedef struct MWFlags MWFlags;
#endif /* __cplusplus */

#endif 	/* __MWFlags_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "mwcomtypes.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IMWUtil_INTERFACE_DEFINED__
#define __IMWUtil_INTERFACE_DEFINED__

/* interface IMWUtil */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_IMWUtil;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("C47EA90E-56D1-11d5-B159-00D0B7BA7544")
    IMWUtil : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MWPack( 
            /* [out] */ VARIANT *pVarArg,
            /* [optional][in] */ VARIANT Var0,
            /* [optional][in] */ VARIANT Var1,
            /* [optional][in] */ VARIANT Var2,
            /* [optional][in] */ VARIANT Var3,
            /* [optional][in] */ VARIANT Var4,
            /* [optional][in] */ VARIANT Var5,
            /* [optional][in] */ VARIANT Var6,
            /* [optional][in] */ VARIANT Var7,
            /* [optional][in] */ VARIANT Var8,
            /* [optional][in] */ VARIANT Var9,
            /* [optional][in] */ VARIANT Var10,
            /* [optional][in] */ VARIANT Var11,
            /* [optional][in] */ VARIANT Var12,
            /* [optional][in] */ VARIANT Var13,
            /* [optional][in] */ VARIANT Var14,
            /* [optional][in] */ VARIANT Var15,
            /* [optional][in] */ VARIANT Var16,
            /* [optional][in] */ VARIANT Var17,
            /* [optional][in] */ VARIANT Var18,
            /* [optional][in] */ VARIANT Var19,
            /* [optional][in] */ VARIANT Var20,
            /* [optional][in] */ VARIANT Var21,
            /* [optional][in] */ VARIANT Var22,
            /* [optional][in] */ VARIANT Var23,
            /* [optional][in] */ VARIANT Var24,
            /* [optional][in] */ VARIANT Var25,
            /* [optional][in] */ VARIANT Var26,
            /* [optional][in] */ VARIANT Var27,
            /* [optional][in] */ VARIANT Var28,
            /* [optional][in] */ VARIANT Var29,
            /* [optional][in] */ VARIANT Var30,
            /* [optional][in] */ VARIANT Var31) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MWUnpack( 
            /* [in] */ VARIANT VarArg,
            /* [defaultvalue][in] */ long nStartAt,
            /* [defaultvalue][in] */ VARIANT_BOOL bAutoResize,
            /* [optional][out][in] */ VARIANT *pVar0,
            /* [optional][out][in] */ VARIANT *pVar1,
            /* [optional][out][in] */ VARIANT *pVar2,
            /* [optional][out][in] */ VARIANT *pVar3,
            /* [optional][out][in] */ VARIANT *pVar4,
            /* [optional][out][in] */ VARIANT *pVar5,
            /* [optional][out][in] */ VARIANT *pVar6,
            /* [optional][out][in] */ VARIANT *pVar7,
            /* [optional][out][in] */ VARIANT *pVar8,
            /* [optional][out][in] */ VARIANT *pVar9,
            /* [optional][out][in] */ VARIANT *pVar10,
            /* [optional][out][in] */ VARIANT *pVar11,
            /* [optional][out][in] */ VARIANT *pVar12,
            /* [optional][out][in] */ VARIANT *pVar13,
            /* [optional][out][in] */ VARIANT *pVar14,
            /* [optional][out][in] */ VARIANT *pVar15,
            /* [optional][out][in] */ VARIANT *pVar16,
            /* [optional][out][in] */ VARIANT *pVar17,
            /* [optional][out][in] */ VARIANT *pVar18,
            /* [optional][out][in] */ VARIANT *pVar19,
            /* [optional][out][in] */ VARIANT *pVar20,
            /* [optional][out][in] */ VARIANT *pVar21,
            /* [optional][out][in] */ VARIANT *pVar22,
            /* [optional][out][in] */ VARIANT *pVar23,
            /* [optional][out][in] */ VARIANT *pVar24,
            /* [optional][out][in] */ VARIANT *pVar25,
            /* [optional][out][in] */ VARIANT *pVar26,
            /* [optional][out][in] */ VARIANT *pVar27,
            /* [optional][out][in] */ VARIANT *pVar28,
            /* [optional][out][in] */ VARIANT *pVar29,
            /* [optional][out][in] */ VARIANT *pVar30,
            /* [optional][out][in] */ VARIANT *pVar31) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MWDate2VariantDate( 
            /* [out][in] */ VARIANT *pVar) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MWInitApplication( 
            /* [in] */ IDispatch *pApp) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MWInitApplicationWithMCROptions( 
            /* [in] */ IDispatch *pApp,
            /* [optional][in] */ VARIANT mcrOptionList) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE CreateRemoteObject( 
            /* [in] */ BSTR progId,
            /* [retval][out] */ IDispatch **pObject) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsMCRJVMEnabled( 
            /* [retval][out] */ VARIANT_BOOL *enabled) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IsMCRInitialized( 
            /* [retval][out] */ VARIANT_BOOL *initialized) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IMWUtilVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IMWUtil * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IMWUtil * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IMWUtil * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IMWUtil * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IMWUtil * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IMWUtil * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IMWUtil * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MWPack )( 
            IMWUtil * This,
            /* [out] */ VARIANT *pVarArg,
            /* [optional][in] */ VARIANT Var0,
            /* [optional][in] */ VARIANT Var1,
            /* [optional][in] */ VARIANT Var2,
            /* [optional][in] */ VARIANT Var3,
            /* [optional][in] */ VARIANT Var4,
            /* [optional][in] */ VARIANT Var5,
            /* [optional][in] */ VARIANT Var6,
            /* [optional][in] */ VARIANT Var7,
            /* [optional][in] */ VARIANT Var8,
            /* [optional][in] */ VARIANT Var9,
            /* [optional][in] */ VARIANT Var10,
            /* [optional][in] */ VARIANT Var11,
            /* [optional][in] */ VARIANT Var12,
            /* [optional][in] */ VARIANT Var13,
            /* [optional][in] */ VARIANT Var14,
            /* [optional][in] */ VARIANT Var15,
            /* [optional][in] */ VARIANT Var16,
            /* [optional][in] */ VARIANT Var17,
            /* [optional][in] */ VARIANT Var18,
            /* [optional][in] */ VARIANT Var19,
            /* [optional][in] */ VARIANT Var20,
            /* [optional][in] */ VARIANT Var21,
            /* [optional][in] */ VARIANT Var22,
            /* [optional][in] */ VARIANT Var23,
            /* [optional][in] */ VARIANT Var24,
            /* [optional][in] */ VARIANT Var25,
            /* [optional][in] */ VARIANT Var26,
            /* [optional][in] */ VARIANT Var27,
            /* [optional][in] */ VARIANT Var28,
            /* [optional][in] */ VARIANT Var29,
            /* [optional][in] */ VARIANT Var30,
            /* [optional][in] */ VARIANT Var31);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MWUnpack )( 
            IMWUtil * This,
            /* [in] */ VARIANT VarArg,
            /* [defaultvalue][in] */ long nStartAt,
            /* [defaultvalue][in] */ VARIANT_BOOL bAutoResize,
            /* [optional][out][in] */ VARIANT *pVar0,
            /* [optional][out][in] */ VARIANT *pVar1,
            /* [optional][out][in] */ VARIANT *pVar2,
            /* [optional][out][in] */ VARIANT *pVar3,
            /* [optional][out][in] */ VARIANT *pVar4,
            /* [optional][out][in] */ VARIANT *pVar5,
            /* [optional][out][in] */ VARIANT *pVar6,
            /* [optional][out][in] */ VARIANT *pVar7,
            /* [optional][out][in] */ VARIANT *pVar8,
            /* [optional][out][in] */ VARIANT *pVar9,
            /* [optional][out][in] */ VARIANT *pVar10,
            /* [optional][out][in] */ VARIANT *pVar11,
            /* [optional][out][in] */ VARIANT *pVar12,
            /* [optional][out][in] */ VARIANT *pVar13,
            /* [optional][out][in] */ VARIANT *pVar14,
            /* [optional][out][in] */ VARIANT *pVar15,
            /* [optional][out][in] */ VARIANT *pVar16,
            /* [optional][out][in] */ VARIANT *pVar17,
            /* [optional][out][in] */ VARIANT *pVar18,
            /* [optional][out][in] */ VARIANT *pVar19,
            /* [optional][out][in] */ VARIANT *pVar20,
            /* [optional][out][in] */ VARIANT *pVar21,
            /* [optional][out][in] */ VARIANT *pVar22,
            /* [optional][out][in] */ VARIANT *pVar23,
            /* [optional][out][in] */ VARIANT *pVar24,
            /* [optional][out][in] */ VARIANT *pVar25,
            /* [optional][out][in] */ VARIANT *pVar26,
            /* [optional][out][in] */ VARIANT *pVar27,
            /* [optional][out][in] */ VARIANT *pVar28,
            /* [optional][out][in] */ VARIANT *pVar29,
            /* [optional][out][in] */ VARIANT *pVar30,
            /* [optional][out][in] */ VARIANT *pVar31);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MWDate2VariantDate )( 
            IMWUtil * This,
            /* [out][in] */ VARIANT *pVar);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MWInitApplication )( 
            IMWUtil * This,
            /* [in] */ IDispatch *pApp);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MWInitApplicationWithMCROptions )( 
            IMWUtil * This,
            /* [in] */ IDispatch *pApp,
            /* [optional][in] */ VARIANT mcrOptionList);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *CreateRemoteObject )( 
            IMWUtil * This,
            /* [in] */ BSTR progId,
            /* [retval][out] */ IDispatch **pObject);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsMCRJVMEnabled )( 
            IMWUtil * This,
            /* [retval][out] */ VARIANT_BOOL *enabled);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *IsMCRInitialized )( 
            IMWUtil * This,
            /* [retval][out] */ VARIANT_BOOL *initialized);
        
        END_INTERFACE
    } IMWUtilVtbl;

    interface IMWUtil
    {
        CONST_VTBL struct IMWUtilVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IMWUtil_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IMWUtil_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IMWUtil_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IMWUtil_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IMWUtil_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IMWUtil_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IMWUtil_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#define IMWUtil_MWPack(This,pVarArg,Var0,Var1,Var2,Var3,Var4,Var5,Var6,Var7,Var8,Var9,Var10,Var11,Var12,Var13,Var14,Var15,Var16,Var17,Var18,Var19,Var20,Var21,Var22,Var23,Var24,Var25,Var26,Var27,Var28,Var29,Var30,Var31)	\
    ( (This)->lpVtbl -> MWPack(This,pVarArg,Var0,Var1,Var2,Var3,Var4,Var5,Var6,Var7,Var8,Var9,Var10,Var11,Var12,Var13,Var14,Var15,Var16,Var17,Var18,Var19,Var20,Var21,Var22,Var23,Var24,Var25,Var26,Var27,Var28,Var29,Var30,Var31) ) 

#define IMWUtil_MWUnpack(This,VarArg,nStartAt,bAutoResize,pVar0,pVar1,pVar2,pVar3,pVar4,pVar5,pVar6,pVar7,pVar8,pVar9,pVar10,pVar11,pVar12,pVar13,pVar14,pVar15,pVar16,pVar17,pVar18,pVar19,pVar20,pVar21,pVar22,pVar23,pVar24,pVar25,pVar26,pVar27,pVar28,pVar29,pVar30,pVar31)	\
    ( (This)->lpVtbl -> MWUnpack(This,VarArg,nStartAt,bAutoResize,pVar0,pVar1,pVar2,pVar3,pVar4,pVar5,pVar6,pVar7,pVar8,pVar9,pVar10,pVar11,pVar12,pVar13,pVar14,pVar15,pVar16,pVar17,pVar18,pVar19,pVar20,pVar21,pVar22,pVar23,pVar24,pVar25,pVar26,pVar27,pVar28,pVar29,pVar30,pVar31) ) 

#define IMWUtil_MWDate2VariantDate(This,pVar)	\
    ( (This)->lpVtbl -> MWDate2VariantDate(This,pVar) ) 

#define IMWUtil_MWInitApplication(This,pApp)	\
    ( (This)->lpVtbl -> MWInitApplication(This,pApp) ) 

#define IMWUtil_MWInitApplicationWithMCROptions(This,pApp,mcrOptionList)	\
    ( (This)->lpVtbl -> MWInitApplicationWithMCROptions(This,pApp,mcrOptionList) ) 

#define IMWUtil_CreateRemoteObject(This,progId,pObject)	\
    ( (This)->lpVtbl -> CreateRemoteObject(This,progId,pObject) ) 

#define IMWUtil_IsMCRJVMEnabled(This,enabled)	\
    ( (This)->lpVtbl -> IsMCRJVMEnabled(This,enabled) ) 

#define IMWUtil_IsMCRInitialized(This,initialized)	\
    ( (This)->lpVtbl -> IsMCRInitialized(This,initialized) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IMWUtil_INTERFACE_DEFINED__ */



#ifndef __MWComUtil_LIBRARY_DEFINED__
#define __MWComUtil_LIBRARY_DEFINED__

/* library MWComUtil */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_MWComUtil;

EXTERN_C const CLSID CLSID_MWField;

#ifdef __cplusplus

class DECLSPEC_UUID("BA815CED-8D35-4E89-910F-E1EB03AA6EFE")
MWField;
#endif

EXTERN_C const CLSID CLSID_MWStruct;

#ifdef __cplusplus

class DECLSPEC_UUID("5B623855-547C-4BEB-AA0F-740515E6D4F0")
MWStruct;
#endif

EXTERN_C const CLSID CLSID_MWComplex;

#ifdef __cplusplus

class DECLSPEC_UUID("8CAC0E2D-EBAB-4D28-AB16-372958E89B5B")
MWComplex;
#endif

EXTERN_C const CLSID CLSID_MWSparse;

#ifdef __cplusplus

class DECLSPEC_UUID("1C7D076B-55EB-418D-865D-4088F52AC57C")
MWSparse;
#endif

EXTERN_C const CLSID CLSID_MWArg;

#ifdef __cplusplus

class DECLSPEC_UUID("F32B54A7-B4E0-4B44-A02F-F1F098F5DEF2")
MWArg;
#endif

EXTERN_C const CLSID CLSID_MWArrayFormatFlags;

#ifdef __cplusplus

class DECLSPEC_UUID("F3C1416B-8AA2-4663-AA99-4021C4BA370D")
MWArrayFormatFlags;
#endif

EXTERN_C const CLSID CLSID_MWDataConversionFlags;

#ifdef __cplusplus

class DECLSPEC_UUID("C58F38A3-B5E8-44FF-B749-C38E31BEB952")
MWDataConversionFlags;
#endif

EXTERN_C const CLSID CLSID_MWUtil;

#ifdef __cplusplus

class DECLSPEC_UUID("0FD82AF1-CC30-4DBD-9731-6124ED600AE2")
MWUtil;
#endif

EXTERN_C const CLSID CLSID_MWFlags;

#ifdef __cplusplus

class DECLSPEC_UUID("292FF5B0-8D7E-482F-B294-9295E63627BB")
MWFlags;
#endif
#endif /* __MWComUtil_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long *, unsigned long            , BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserMarshal(  unsigned long *, unsigned char *, BSTR * ); 
unsigned char * __RPC_USER  BSTR_UserUnmarshal(unsigned long *, unsigned char *, BSTR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long *, BSTR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long *, unsigned long            , VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserMarshal(  unsigned long *, unsigned char *, VARIANT * ); 
unsigned char * __RPC_USER  VARIANT_UserUnmarshal(unsigned long *, unsigned char *, VARIANT * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long *, VARIANT * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


