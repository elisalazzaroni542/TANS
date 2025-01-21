// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME Dict
#define R__NO_DEPRECATION

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "ROOT/RConfig.hxx"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Header files passed as explicit arguments
#include "/home/dcristof/Desktop/TANS/project/headers/Point.h"

// Header files passed via #pragma extra_include

// The generated code does not explicitly qualify STL entities
namespace std {} using namespace std;

namespace tansNamespace {
   namespace ROOTDict {
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance();
      static TClass *tansNamespace_Dictionary();

      // Function generating the singleton type initializer
      inline ::ROOT::TGenericClassInfo *GenerateInitInstance()
      {
         static ::ROOT::TGenericClassInfo 
            instance("tansNamespace", 0 /*version*/, "Point.h", 8,
                     ::ROOT::Internal::DefineBehavior((void*)nullptr,(void*)nullptr),
                     &tansNamespace_Dictionary, 0);
         return &instance;
      }
      // Insure that the inline function is _not_ optimized away by the compiler
      ::ROOT::TGenericClassInfo *(*_R__UNIQUE_DICT_(InitFunctionKeeper))() = &GenerateInitInstance;  
      // Static variable to force the class initialization
      static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstance(); R__UseDummy(_R__UNIQUE_DICT_(Init));

      // Dictionary for non-ClassDef classes
      static TClass *tansNamespace_Dictionary() {
         return GenerateInitInstance()->GetClass();
      }

   }
}

namespace ROOT {
   static void *new_tansNamespacecLcLPoint(void *p = nullptr);
   static void *newArray_tansNamespacecLcLPoint(Long_t size, void *p);
   static void delete_tansNamespacecLcLPoint(void *p);
   static void deleteArray_tansNamespacecLcLPoint(void *p);
   static void destruct_tansNamespacecLcLPoint(void *p);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::tansNamespace::Point*)
   {
      ::tansNamespace::Point *ptr = nullptr;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::tansNamespace::Point >(nullptr);
      static ::ROOT::TGenericClassInfo 
         instance("tansNamespace::Point", ::tansNamespace::Point::Class_Version(), "Point.h", 9,
                  typeid(::tansNamespace::Point), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::tansNamespace::Point::Dictionary, isa_proxy, 4,
                  sizeof(::tansNamespace::Point) );
      instance.SetNew(&new_tansNamespacecLcLPoint);
      instance.SetNewArray(&newArray_tansNamespacecLcLPoint);
      instance.SetDelete(&delete_tansNamespacecLcLPoint);
      instance.SetDeleteArray(&deleteArray_tansNamespacecLcLPoint);
      instance.SetDestructor(&destruct_tansNamespacecLcLPoint);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::tansNamespace::Point*)
   {
      return GenerateInitInstanceLocal(static_cast<::tansNamespace::Point*>(nullptr));
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_DICT_(Init) = GenerateInitInstanceLocal(static_cast<const ::tansNamespace::Point*>(nullptr)); R__UseDummy(_R__UNIQUE_DICT_(Init));
} // end of namespace ROOT

namespace tansNamespace {
//______________________________________________________________________________
atomic_TClass_ptr Point::fgIsA(nullptr);  // static to hold class pointer

//______________________________________________________________________________
const char *Point::Class_Name()
{
   return "tansNamespace::Point";
}

//______________________________________________________________________________
const char *Point::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tansNamespace::Point*)nullptr)->GetImplFileName();
}

//______________________________________________________________________________
int Point::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::tansNamespace::Point*)nullptr)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *Point::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tansNamespace::Point*)nullptr)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *Point::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::tansNamespace::Point*)nullptr)->GetClass(); }
   return fgIsA;
}

} // namespace tansNamespace
namespace tansNamespace {
//______________________________________________________________________________
void Point::Streamer(TBuffer &R__b)
{
   // Stream an object of class tansNamespace::Point.

   if (R__b.IsReading()) {
      R__b.ReadClassBuffer(tansNamespace::Point::Class(),this);
   } else {
      R__b.WriteClassBuffer(tansNamespace::Point::Class(),this);
   }
}

} // namespace tansNamespace
namespace ROOT {
   // Wrappers around operator new
   static void *new_tansNamespacecLcLPoint(void *p) {
      return  p ? new(p) ::tansNamespace::Point : new ::tansNamespace::Point;
   }
   static void *newArray_tansNamespacecLcLPoint(Long_t nElements, void *p) {
      return p ? new(p) ::tansNamespace::Point[nElements] : new ::tansNamespace::Point[nElements];
   }
   // Wrapper around operator delete
   static void delete_tansNamespacecLcLPoint(void *p) {
      delete (static_cast<::tansNamespace::Point*>(p));
   }
   static void deleteArray_tansNamespacecLcLPoint(void *p) {
      delete [] (static_cast<::tansNamespace::Point*>(p));
   }
   static void destruct_tansNamespacecLcLPoint(void *p) {
      typedef ::tansNamespace::Point current_t;
      (static_cast<current_t*>(p))->~current_t();
   }
} // end of namespace ROOT for class ::tansNamespace::Point

namespace {
  void TriggerDictionaryInitialization_Dict_Impl() {
    static const char* headers[] = {
"headers/Point.h",
nullptr
    };
    static const char* includePaths[] = {
"/home/dcristof/Desktop/TANS/project/headers",
"/snap/root-framework/943/usr/local/include/",
"/home/dcristof/Desktop/TANS/project/",
nullptr
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_AutoLoading_Map;
namespace tansNamespace{class __attribute__((annotate("$clingAutoload$headers/Point.h")))  Point;}
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "Dict dictionary payload"


#define _BACKWARD_BACKWARD_WARNING_H
// Inline headers
#include "headers/Point.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[] = {
"tansNamespace::Point", payloadCode, "@",
nullptr
};
    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_Dict_Impl, {}, classesHeaders, /*hasCxxModule*/false);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_Dict() {
  TriggerDictionaryInitialization_Dict_Impl();
}
