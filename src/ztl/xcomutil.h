#ifndef XCOMDEF_H
#define XCOMDEF_H

/***
* comutil.h - Native C++ compiler COM support - BSTR, VARIANT wrappers header
*
* Copyright (c) Microsoft Corporation. All rights reserved.
*
****/
#include "comdef.h"
#include "zcomapi.h"
#include "zallocex.h"
#include <new>

//////////////////////////////////////////////////////////////////////////////
//
// Forward class declarations
//
//////////////////////////////////////////////////////////////////////////////

class _xbstr_t;
class _xvariant_t;

//////////////////////////////////////////////////////////////////////////////
//
// Routines for handling conversions between BSTR and char*
//
//////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////
//
// Wrapper class for BSTR
//
//////////////////////////////////////////////////////////////////////////////

class _xbstr_t {
public:
    // Constructors
    //
    _xbstr_t() throw();
    _xbstr_t(const _xbstr_t& s) throw();
    _xbstr_t(const char* s) ;
    _xbstr_t(const wchar_t* s) ;
    _xbstr_t(const _xvariant_t& var) ;
    _xbstr_t(BSTR bstr, bool fCopy) ;


    void _ctor_0(char const* s) {
        new (this) _xbstr_t(s);
    }

    void _ctor_1(wchar_t const* s) {
        new (this) _xbstr_t(s);
    }

    void _ctor_2(BSTR bstr, bool fCopy) {
        new (this) _xbstr_t(bstr, fCopy);
    }

    void _ctor_3(const _xvariant_t& var) {
        new (this) _xbstr_t(var);
    }

    const char* op_char_str() {
        return (const char *)this->operator const char *();
    }

    const wchar_t* op_ushort_str() {
        return (const wchar_t*)this->operator const wchar_t *();
    }

    // Destructor
    //
    ~_xbstr_t() noexcept;

    // Assignment operators
    //
    _xbstr_t& operator=(const _xbstr_t& s) throw();
    _xbstr_t& operator=(const char* s) ;
    _xbstr_t& operator=(const wchar_t* s) ;
    _xbstr_t& operator=(const _xvariant_t& var) ;

    // Operators
    //
    _xbstr_t& operator+=(const _xbstr_t& s) ;
    _xbstr_t operator+(const _xbstr_t& s) const ;

    // Friend operators
    //
    friend _xbstr_t operator+(const char* s1, const _xbstr_t& s2) ;
    friend _xbstr_t operator+(const wchar_t* s1, const _xbstr_t& s2) ;

    // Extractors
    //
    operator const wchar_t*() const throw();
    operator wchar_t*() const throw();
    operator const char*() const ;
    operator char*() const ;

    // Comparison operators
    //
    bool operator!() const throw();
    bool operator==(const _xbstr_t& str) const throw();
    bool operator!=(const _xbstr_t& str) const throw();
    bool operator<(const _xbstr_t& str) const throw();
    bool operator>(const _xbstr_t& str) const throw();
    bool operator<=(const _xbstr_t& str) const throw();
    bool operator>=(const _xbstr_t& str) const throw();

    // Low-level helper functions
    //
    BSTR copy(bool fCopy = true) const ;
    unsigned int length() const throw();

    // Binary string assign
    //
    void Assign(BSTR s) ;

    // Get the physical BSTR
    //
    BSTR& GetBSTR() ;
    BSTR* GetAddress() ;

    // Attach to the internal BSTR w/o copying
    //
    void Attach(BSTR s) ;

    // Detach the internal BSTR
    //
    BSTR Detach();

private:
    // Referenced counted wrapper
    //
    class Data_t {
    public:
        // Constructors
        //
        Data_t(const char* s) ;
        Data_t(const wchar_t* s) ;
        Data_t(BSTR bstr, bool fCopy) ;
        Data_t(const _xbstr_t& s1, const _xbstr_t& s2) ;

        // Reference counting routines
        //
        unsigned long AddRef() throw();
        unsigned long Release() throw();
        unsigned long RefCount() const throw();

        // Extractors
        //
        operator const wchar_t*() const throw();
        operator const char*() const ;

        // Low-level helper functions
        //
        const wchar_t* GetWString() const throw();
        wchar_t*& GetWString() throw();
        const char* GetString() const ;

        BSTR Copy() const ;
        void Assign(BSTR s) ;
        void Attach(BSTR s) throw();
        unsigned int Length() const throw();
        int Compare(const Data_t& str) const throw();

        // Exception agnostic wrapper for new
        //
        void* operator new(size_t sz);
        void operator delete(void* p);

    private:
        BSTR            m_wstr;
        mutable char*   m_str;
        unsigned long   m_RefCount;

        // Never allow default construction
        //
        Data_t() throw();

        // Never allow copy
        //
        Data_t(const Data_t& s) throw();

        // Prevent deletes from outside. Release() must be used.
        //
        ~Data_t() noexcept;

        void _Free() throw();
    };

private:
    // Reference counted representation
    //
    Data_t* m_Data;

private:
    // Low-level utilities
    //
    void _AddRef() throw();
    void _Free() throw();
    int _Compare(const _xbstr_t& str) const throw();
};

//////////////////////////////////////////////////////////////////////////////
//
// Constructors
//
//////////////////////////////////////////////////////////////////////////////

// Default constructor
//
inline _xbstr_t::_xbstr_t() throw()
        : m_Data(NULL)
{
}

// Copy constructor
//
inline _xbstr_t::_xbstr_t(const _xbstr_t& s) throw()
        : m_Data(s.m_Data)
{
    _AddRef();
}

// Construct a _xbstr_t from a const char*
//
inline _xbstr_t::_xbstr_t(const char* s)
        : m_Data(new Data_t(s))
{
    if (m_Data == NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

// Construct a _xbstr_t from a const whar_t*
//
inline _xbstr_t::_xbstr_t(const wchar_t* s)
        : m_Data(new Data_t(s))
{
    if (m_Data == NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

// Construct a _xbstr_t from a BSTR.  If fCopy is false, give control of
// data to the _xbstr_t without making a new copy.
//
inline _xbstr_t::_xbstr_t(BSTR bstr, bool fCopy)
        : m_Data(new Data_t(bstr, fCopy))
{
    if (m_Data == NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

// Destructor
//
inline _xbstr_t::~_xbstr_t() noexcept
{
    _Free();
}

//////////////////////////////////////////////////////////////////////////////
//
// Assignment operators
//
//////////////////////////////////////////////////////////////////////////////

// Default assign operator
//
inline _xbstr_t& _xbstr_t::operator=(const _xbstr_t& s) throw()
{
    if (this != &s) {
        _Free();

        m_Data = s.m_Data;
        _AddRef();
    }

    return *this;
}

// Assign a const char* to a _xbstr_t
//
inline _xbstr_t& _xbstr_t::operator=(const char* s)
{
    _COM_ASSERT(s == NULL || static_cast<const char*>(*this) != s);

    if (s == NULL || static_cast<const char*>(*this) != s)
    {
        _Free();

        m_Data = new Data_t(s);
        if (m_Data == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }

    return *this;
}

// Assign a const wchar_t* to a _xbstr_t
//
inline _xbstr_t& _xbstr_t::operator=(const wchar_t* s)
{
    _COM_ASSERT(s == NULL || static_cast<const wchar_t*>(*this) != s);

    if (s == NULL || static_cast<const wchar_t*>(*this) != s)
    {
        _Free();

        m_Data = new Data_t(s);
        if (m_Data == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }

    return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
// Operators
//
//////////////////////////////////////////////////////////////////////////////

// Concatenate a _xbstr_t onto this _xbstr_t
//
inline _xbstr_t& _xbstr_t::operator+=(const _xbstr_t& s)
{
    Data_t* newData = new Data_t(*this, s);
    if (newData == NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
    else {
        _Free();
        m_Data = newData;
    }

    return *this;
}

// Return the concatenation of this _xbstr_t with another _xbstr_t
//
inline _xbstr_t _xbstr_t::operator+(const _xbstr_t& s) const
{
    _xbstr_t b = *this;
    b += s;

    return b;
}

//////////////////////////////////////////////////////////////////////////////
//
// Friend Operators
//
//////////////////////////////////////////////////////////////////////////////

// Return the concatenation of a const char* with a _xbstr_t
//
inline _xbstr_t operator+(const char* s1, const _xbstr_t& s2)
{
    _xbstr_t b = s1;
    b += s2;

    return b;
}

// Return the concatenation of a const char* with a _xbstr_t
//
inline _xbstr_t operator+(const wchar_t* s1, const _xbstr_t& s2)
{
    _xbstr_t b = s1;
    b += s2;

    return b;
}

//////////////////////////////////////////////////////////////////////////////
//
// Extractors
//
//////////////////////////////////////////////////////////////////////////////

// Extract a const wchar_t*
//
inline _xbstr_t::operator const wchar_t*() const throw()
{
    return (m_Data != NULL) ? m_Data->GetWString() : NULL;
}

// Extract a wchar_t*
//
inline _xbstr_t::operator wchar_t*() const throw()
{
    return const_cast<wchar_t*>((m_Data != NULL) ? m_Data->GetWString() : NULL);
}

// Extract a const char_t*
//
inline _xbstr_t::operator const char*() const
{
    return (m_Data != NULL) ? m_Data->GetString() : NULL;
}

// Extract a char_t*
//
inline _xbstr_t::operator char*() const
{
    return const_cast<char*>((m_Data != NULL) ? m_Data->GetString() : NULL);
}

//////////////////////////////////////////////////////////////////////////////
//
// Comparison operators
//
//////////////////////////////////////////////////////////////////////////////

inline bool _xbstr_t::operator!() const throw()
{
    return (m_Data != NULL) ? !m_Data->GetWString() : true;
}

inline bool _xbstr_t::operator==(const _xbstr_t& str) const throw()
{
    return _Compare(str) == 0;
}

inline bool _xbstr_t::operator!=(const _xbstr_t& str) const throw()
{
    return _Compare(str) != 0;
}

inline bool _xbstr_t::operator<(const _xbstr_t& str) const throw()
{
    return _Compare(str) < 0;
}

inline bool _xbstr_t::operator>(const _xbstr_t& str) const throw()
{
    return _Compare(str) > 0;
}

inline bool _xbstr_t::operator<=(const _xbstr_t& str) const throw()
{
    return _Compare(str) <= 0;
}

inline bool _xbstr_t::operator>=(const _xbstr_t& str) const throw()
{
    return _Compare(str) >= 0;
}

//////////////////////////////////////////////////////////////////////////////
//
// Low-level help functions
//
//////////////////////////////////////////////////////////////////////////////

// Extract a copy of the wrapped BSTR
//
inline BSTR _xbstr_t::copy(bool fCopy) const
{
    return (m_Data != NULL) ? (fCopy ? m_Data->Copy() : m_Data->GetWString()) : NULL;
}

// Return the length of the wrapped BSTR
//
inline unsigned int _xbstr_t::length() const throw()
{
    return (m_Data != NULL) ? m_Data->Length() : 0;
}

// Binary string assign
//
inline void _xbstr_t::Assign(BSTR s)
{
    _COM_ASSERT(s == NULL || m_Data == NULL || m_Data->GetWString() != s);

    if (s == NULL || m_Data == NULL || m_Data->GetWString() != s)
    {
        _Free();

        m_Data = new Data_t(s, true);
        if (m_Data == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
}

// Get the physical BSTR
//
inline BSTR& _xbstr_t::GetBSTR()
{
    if (m_Data == NULL) {
        m_Data = new Data_t(0, false);
        if (m_Data == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    return m_Data->GetWString();
}

// Get the address of the physical BSTR to pass as an 'out'-parameter
//
inline BSTR* _xbstr_t::GetAddress()
{
    Attach(0);
    return &m_Data->GetWString();
}

// Attach to the internal BSTR w/o copying
//
inline void _xbstr_t::Attach(BSTR s)
{
    _Free();

    m_Data = new Data_t(s, false);
    if (m_Data == NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

// Detach the internal BSTR
//
inline BSTR _xbstr_t::Detach()
{
    _COM_ASSERT(m_Data != NULL && m_Data->RefCount() == 1);

    if (m_Data != NULL && m_Data->RefCount() == 1) {
        BSTR b = m_Data->GetWString();
        m_Data->GetWString() = NULL;
        _Free();
        return b;
    }
    else {
        _com_issue_error(E_POINTER);
    }
}

// AddRef the BSTR
//
inline void _xbstr_t::_AddRef() throw()
{
    if (m_Data != NULL) {
        m_Data->AddRef();
    }
}

// Free the BSTR
//
inline void _xbstr_t::_Free() throw()
{
    if (m_Data != NULL) {
        m_Data->Release();
        m_Data = NULL;
    }
}

// Compare two _xbstr_t objects
//
inline int _xbstr_t::_Compare(const _xbstr_t& str) const throw()
{
    if (m_Data == str.m_Data) {
        return 0;
    }

    if (m_Data == NULL) {
        if (str.length() == 0) {
            return 0;
        }
        else {
            return -1;
        }
    }

    if (str.m_Data == NULL){
        if (this->length() == 0) {
            return 0;
        }
        else {
            return 1;
        }
    }

    return m_Data->Compare(*str.m_Data);
}

//////////////////////////////////////////////////////////////////////////////
//
// Reference counted wrapper - Constructors
//
//////////////////////////////////////////////////////////////////////////////

// Construct a Data_t from a const char*
//
inline _xbstr_t::Data_t::Data_t(const char* s)
        : m_str(NULL), m_RefCount(1)
{
    m_wstr = ZComAPI::ZtlConvertStringToBSTR(s);
}

// Construct a Data_t from a const wchar_t*
//
inline _xbstr_t::Data_t::Data_t(const wchar_t* s)
        : m_str(NULL), m_RefCount(1)
{
    m_wstr = ZComAPI::ZComSysAllocString(s);

    if (m_wstr == NULL && s != NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

// Construct a Data_t from a BSTR.  If fCopy is false, give control of
// data to the Data_t without doing a SysAllocStringByteLen.
//
inline _xbstr_t::Data_t::Data_t(BSTR bstr, bool fCopy)
        : m_str(NULL), m_RefCount(1)
{
    if (fCopy && bstr != NULL) {
        m_wstr = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(bstr),
                                                    ZComAPI::ZComSysStringByteLen(bstr));

        if (m_wstr == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
    else {
        m_wstr = bstr;
    }
}

// Construct a Data_t from the concatenation of two _xbstr_t objects
//
inline _xbstr_t::Data_t::Data_t(const _xbstr_t& s1, const _xbstr_t& s2)
        : m_str(NULL), m_RefCount(1)
{
    const unsigned int l1 = s1.length();
    const unsigned int l2 = s2.length();
    unsigned int l3;

    if (FAILED(_com_util::UIntAdd(l1, l2, &l3)) ||
        FAILED(_com_util::UIntMult(l3, sizeof(wchar_t), &l3)))
    {
        _com_issue_error(E_OUTOFMEMORY);
        return;
    }

    m_wstr = ZComAPI::ZComSysAllocStringByteLen(NULL, (l1 + l2) * sizeof(wchar_t));
    if (m_wstr == NULL)
    {
        if (l1 + l2 == 0)
        {
            return;
        }
        _com_issue_error(E_OUTOFMEMORY);
        return;
    }

    const wchar_t* wstr1 = static_cast<const wchar_t*>(s1);

    if (wstr1 != NULL)
    {
        _COM_MEMCPY_S(m_wstr, (l1 + l2 + 1) * sizeof(wchar_t), wstr1, (l1 + 1) * sizeof(wchar_t));
    }

    const wchar_t* wstr2 = static_cast<const wchar_t*>(s2);

    if (wstr2 != NULL)
    {
        _COM_MEMCPY_S(m_wstr + l1, (l2 + 1) * sizeof(wchar_t), wstr2, (l2 + 1) * sizeof(wchar_t));
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Reference counted wrapper - reference counting routines
//
//////////////////////////////////////////////////////////////////////////////

inline unsigned long _xbstr_t::Data_t::AddRef() throw()
{
    InterlockedIncrement(reinterpret_cast<long*>(&m_RefCount));
    return m_RefCount;
}

inline unsigned long _xbstr_t::Data_t::Release() throw()
{
    unsigned long cRef = static_cast<unsigned long>(
            InterlockedDecrement(reinterpret_cast<long*>(&m_RefCount)));
    if (cRef == 0) {
        z_anon_free(this);
    }

    return cRef;
}

inline unsigned long _xbstr_t::Data_t::RefCount() const throw()
{
    return m_RefCount;
}

//////////////////////////////////////////////////////////////////////////////
//
// Reference counted wrapper - extractors
//
//////////////////////////////////////////////////////////////////////////////

// Extract a const wchar_t*
//
inline _xbstr_t::Data_t::operator const wchar_t*() const throw()
{
    return m_wstr;
}

// Extract a const char_t*
//
inline _xbstr_t::Data_t::operator const char*() const
{
    return GetString();
}

//////////////////////////////////////////////////////////////////////////////
//
// Reference counted wrapper - helper functions
//
//////////////////////////////////////////////////////////////////////////////

inline const wchar_t* _xbstr_t::Data_t::GetWString() const throw()
{
    return m_wstr;
}

inline wchar_t*& _xbstr_t::Data_t::GetWString() throw()
{
    return m_wstr;
}

inline const char* _xbstr_t::Data_t::GetString() const
{
    if (m_str == NULL) {
        m_str = ZComAPI::ZtlConvertBSTRToString(m_wstr);
    }

    return m_str;
}

// Return a copy of the wrapped BSTR
//
inline BSTR _xbstr_t::Data_t::Copy() const
{
    if (m_wstr != NULL) {
        BSTR bstr = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(m_wstr), ZComAPI::ZComSysStringByteLen(m_wstr));

        if (bstr == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }

        return bstr;
    }

    return NULL;
}

inline void _xbstr_t::Data_t::Assign(BSTR s)
{
    _Free();

    if (s != NULL) {
        m_wstr = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(s), ZComAPI::ZComSysStringByteLen(s));
        m_str = 0;
    }
}

inline void _xbstr_t::Data_t::Attach(BSTR s) throw()
{
    _Free();

    m_wstr = s;
    m_str = 0;
    m_RefCount = 1;
}

// Return the length of the wrapper BSTR
//
inline unsigned int _xbstr_t::Data_t::Length() const throw()
{
    return m_wstr ? ZComAPI::ZComSysStringLen(m_wstr) : 0;
}

// Compare two wrapped BSTRs
//
inline int _xbstr_t::Data_t::Compare(const _xbstr_t::Data_t& str) const throw()
{
    // Dont need to check for NULL here, because
    // SysStringLen will return 0 if you pass in NULL
    const unsigned int l1 = ZComAPI::ZComSysStringLen(m_wstr);
    const unsigned int l2 = ZComAPI::ZComSysStringLen(str.m_wstr);
    unsigned int len = l1;
    if (len > l2) {
        len = l2;
    }

    BSTR bstr1 = m_wstr;
    BSTR bstr2 = str.m_wstr;

    while (len-- > 0) {
        if (*bstr1++ != *bstr2++) {
            return bstr1[-1] - bstr2[-1];
        }
    }

    return (l1 < l2) ? -1 : (l1 == l2) ? 0 : 1;
}

// Exception agnostic wrapper for new
//
#ifdef _COM_OPERATOR_NEW_THROWS
inline void* _xbstr_t::Data_t::operator new(size_t sz)
{
    try {
        return z_anon_alloc(sz);
    }
    catch (...) {
        return NULL;
    }
}

inline void _xbstr_t::Data_t::operator delete(void *_p)
{
    //return ::operator delete(sz);
    z_anon_free(_p);
}
#else // _COM_OPERATOR_NEW_THROWS
inline void* _xbstr_t::Data_t::operator new(size_t sz)
{
    //return ::operator new(sz);
    return z_anon_alloc(sz);
}


inline void _xbstr_t::Data_t::operator delete(void *_p)
{
    //return ::operator delete(sz);
    z_anon_free(_p);
}
#endif // _COM_OPERATOR_NEW_THROWS

// Destruct this object
//
inline _xbstr_t::Data_t::~Data_t() noexcept
{
    _Free();
}

// Free up this object
//
inline void _xbstr_t::Data_t::_Free() throw()
{
    if (m_wstr != NULL) {
        ZComAPI::ZComSysFreeString(m_wstr);
        m_wstr = NULL;
    }

    if (m_str != NULL) {
        delete [] m_str;
        m_str = NULL;
    }
}

//////////////////////////////////////////////////////////////////////////////
//
// Wrapper class for VARIANT
//
//////////////////////////////////////////////////////////////////////////////

/*
 * VARENUM usage key,
 *
 * * [V] - may appear in a VARIANT
 * * [T] - may appear in a TYPEDESC
 * * [P] - may appear in an OLE property set
 * * [S] - may appear in a Safe Array
 * * [C] - supported by class _xvariant_t
 *
 *
 *  VT_EMPTY            [V]   [P]        nothing
 *  VT_NULL             [V]   [P]        SQL style Null
 *  VT_I2               [V][T][P][S][C]  2 byte signed int
 *  VT_I4               [V][T][P][S][C]  4 byte signed int
 *  VT_R4               [V][T][P][S][C]  4 byte real
 *  VT_R8               [V][T][P][S][C]  8 byte real
 *  VT_CY               [V][T][P][S][C]  currency
 *  VT_DATE             [V][T][P][S][C]  date
 *  VT_BSTR             [V][T][P][S][C]  OLE Automation string
 *  VT_DISPATCH         [V][T][P][S][C]  IDispatch *
 *  VT_ERROR            [V][T]   [S][C]  SCODE
 *  VT_BOOL             [V][T][P][S][C]  True=-1, False=0
 *  VT_VARIANT          [V][T][P][S]     VARIANT *
 *  VT_UNKNOWN          [V][T]   [S][C]  IUnknown *
 *  VT_DECIMAL          [V][T]   [S][C]  16 byte fixed point
 *  VT_I1                  [T]           signed char
 *  VT_UI1              [V][T][P][S][C]  unsigned char
 *  VT_UI2                 [T][P]        unsigned short
 *  VT_UI4                 [T][P]        unsigned short
 *  VT_I8                  [T][P]        signed 64-bit int
 *  VT_UI8                 [T][P]        unsigned 64-bit int
 *  VT_INT                 [T]           signed machine int
 *  VT_UINT                [T]           unsigned machine int
 *  VT_VOID                [T]           C style void
 *  VT_HRESULT             [T]           Standard return type
 *  VT_PTR                 [T]           pointer type
 *  VT_SAFEARRAY           [T]          (use VT_ARRAY in VARIANT)
 *  VT_CARRAY              [T]           C style array
 *  VT_USERDEFINED         [T]           user defined type
 *  VT_LPSTR               [T][P]        null terminated string
 *  VT_LPWSTR              [T][P]        wide null terminated string
 *  VT_FILETIME               [P]        FILETIME
 *  VT_BLOB                   [P]        Length prefixed bytes
 *  VT_STREAM                 [P]        Name of the stream follows
 *  VT_STORAGE                [P]        Name of the storage follows
 *  VT_STREAMED_OBJECT        [P]        Stream contains an object
 *  VT_STORED_OBJECT          [P]        Storage contains an object
 *  VT_BLOB_OBJECT            [P]        Blob contains an object
 *  VT_CF                     [P]        Clipboard format
 *  VT_CLSID                  [P]        A Class ID
 *  VT_VECTOR                 [P]        simple counted array
 *  VT_ARRAY            [V]              SAFEARRAY*
 *  VT_BYREF            [V]              void* for local use
 */

class _xvariant_t : public ::tagVARIANT {
public:
    long op_long() {
        return this->operator long();
    }

    bool op_eq(const tagVARIANT& other) {
        return operator==(other);
    }

    void _ctor_0(long lSrc, VARTYPE vtSrc) {
        new (this) _xvariant_t(lSrc, vtSrc);
    }


    // Constructors
    //
    _xvariant_t() throw();

    _xvariant_t(const VARIANT& varSrc) ;
    _xvariant_t(const VARIANT* pSrc) ;
    _xvariant_t(const _xvariant_t& varSrc) ;

    _xvariant_t(VARIANT& varSrc, bool fCopy) ;          // Attach VARIANT if !fCopy

    _xvariant_t(short sSrc, VARTYPE vtSrc = VT_I2) ;    // Creates a VT_I2, or a VT_BOOL
    _xvariant_t(long lSrc, VARTYPE vtSrc = VT_I4) ;     // Creates a VT_I4, a VT_ERROR, or a VT_BOOL
    _xvariant_t(float fltSrc) throw();                                   // Creates a VT_R4
    _xvariant_t(double dblSrc, VARTYPE vtSrc = VT_R8) ; // Creates a VT_R8, or a VT_DATE
    _xvariant_t(const CY& cySrc) throw();                                // Creates a VT_CY
    _xvariant_t(const _xbstr_t& bstrSrc) ;               // Creates a VT_BSTR
    _xvariant_t(const wchar_t *pSrc) ;                  // Creates a VT_BSTR
    _xvariant_t(const char* pSrc) ;                     // Creates a VT_BSTR
    _xvariant_t(IDispatch* pSrc, bool fAddRef = true) throw();           // Creates a VT_DISPATCH
    _xvariant_t(bool boolSrc) throw();                                   // Creates a VT_BOOL
    _xvariant_t(IUnknown* pSrc, bool fAddRef = true) throw();            // Creates a VT_UNKNOWN
    _xvariant_t(const DECIMAL& decSrc) throw();                          // Creates a VT_DECIMAL
    _xvariant_t(BYTE bSrc) throw();                                      // Creates a VT_UI1

    _xvariant_t(char cSrc) throw();                                      // Creates a VT_I1
    _xvariant_t(unsigned short usSrc) throw();                           // Creates a VT_UI2
    _xvariant_t(unsigned long ulSrc) throw();                            // Creates a VT_UI4
    _xvariant_t(int iSrc) throw();                                       // Creates a VT_INT
    _xvariant_t(unsigned int uiSrc) throw();                             // Creates a VT_UINT
#if (_WIN32_WINNT >= 0x0501)
    _xvariant_t(__int64 i8Src) throw();                                  // Creates a VT_I8
    _xvariant_t(unsigned __int64 ui8Src) throw();                        // Creates a VT_UI8
#endif

    // Destructor
    //
    ~_xvariant_t() noexcept;

    // Extractors
    //
    operator short() const ;                           // Extracts a short from a VT_I2
    operator long() const ;                            // Extracts a long from a VT_I4
    operator float() const ;                           // Extracts a float from a VT_R4
    operator double() const ;                          // Extracts a double from a VT_R8
    operator CY() const ;                              // Extracts a CY from a VT_CY
    operator _xbstr_t() const ;                         // Extracts a _xbstr_t from a VT_BSTR
    operator IDispatch*() const ;                      // Extracts a IDispatch* from a VT_DISPATCH
    operator bool() const ;                            // Extracts a bool from a VT_BOOL
    operator IUnknown*() const ;                       // Extracts a IUnknown* from a VT_UNKNOWN
    operator DECIMAL() const ;                         // Extracts a DECIMAL from a VT_DECIMAL
    operator BYTE() const ;                            // Extracts a BTYE (unsigned char) from a VT_UI1
    operator VARIANT() const throw();

    operator char() const ;                            // Extracts a char from a VT_I1
    operator unsigned short() const ;                  // Extracts a unsigned short from a VT_UI2
    operator unsigned long() const ;                   // Extracts a unsigned long from a VT_UI4
    operator int() const ;                             // Extracts a int from a VT_INT
    operator unsigned int() const ;                    // Extracts a unsigned int from a VT_UINT
#if (_WIN32_WINNT >= 0x0501)
    operator __int64() const ;                         // Extracts a __int64 from a VT_I8
    operator unsigned __int64() const ;                // Extracts a unsigned __int64 from a VT_UI8
#endif

    // Assignment operations
    //
    _xvariant_t& operator=(const VARIANT& varSrc) ;
    _xvariant_t& operator=(const VARIANT* pSrc) ;
    _xvariant_t& operator=(const _xvariant_t& varSrc) ;

    _xvariant_t& operator=(short sSrc) ;                // Assign a VT_I2, or a VT_BOOL
    _xvariant_t& operator=(long lSrc) ;                 // Assign a VT_I4, a VT_ERROR or a VT_BOOL
    _xvariant_t& operator=(float fltSrc) ;              // Assign a VT_R4
    _xvariant_t& operator=(double dblSrc) ;             // Assign a VT_R8, or a VT_DATE
    _xvariant_t& operator=(const CY& cySrc) ;           // Assign a VT_CY
    _xvariant_t& operator=(const _xbstr_t& bstrSrc) ;    // Assign a VT_BSTR
    _xvariant_t& operator=(const wchar_t* pSrc) ;       // Assign a VT_BSTR
    _xvariant_t& operator=(const char* pSrc) ;          // Assign a VT_BSTR
    _xvariant_t& operator=(IDispatch* pSrc) ;           // Assign a VT_DISPATCH
    _xvariant_t& operator=(bool boolSrc) ;              // Assign a VT_BOOL
    _xvariant_t& operator=(IUnknown* pSrc) ;            // Assign a VT_UNKNOWN
    _xvariant_t& operator=(const DECIMAL& decSrc) ;     // Assign a VT_DECIMAL
    _xvariant_t& operator=(BYTE bSrc) ;                 // Assign a VT_UI1

    _xvariant_t& operator=(char cSrc) ;                 // Assign a VT_I1
    _xvariant_t& operator=(unsigned short usSrc) ;      // Assign a VT_UI2
    _xvariant_t& operator=(unsigned long ulSrc) ;       // Assign a VT_UI4
    _xvariant_t& operator=(int iSrc) ;                  // Assign a VT_INT
    _xvariant_t& operator=(unsigned int uiSrc) ;        // Assign a VT_UINT
#if (_WIN32_WINNT >= 0x0501)
    _xvariant_t& operator=(__int64 i8Src) ;             // Assign a VT_I8
    _xvariant_t& operator=(unsigned __int64 ui8Src) ;   // Assign a VT_UI8
#endif

    // Comparison operations
    //
    bool operator==(const VARIANT& varSrc) const throw();
    bool operator==(const VARIANT* pSrc) const throw();

    bool operator!=(const VARIANT& varSrc) const throw();
    bool operator!=(const VARIANT* pSrc) const throw();

    // Low-level operations
    //
    void Clear() ;

    void Attach(VARIANT& varSrc) ;
    VARIANT Detach() throw();

    VARIANT& GetVARIANT() throw();
    VARIANT* GetAddress() ;

    void ChangeType(VARTYPE vartype, const _xvariant_t* pSrc = NULL) ;

    void SetString(const char* pSrc) ; // used to set ANSI string
};

//////////////////////////////////////////////////////////////////////////////////////////
//
// Constructors
//
//////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
//
inline _xvariant_t::_xvariant_t() throw()
{
    ::VariantInit(this);
}

// Construct a _xvariant_t from a const VARIANT&
//
inline _xvariant_t::_xvariant_t(const VARIANT& varSrc)
{
    ::VariantInit(this);
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(&varSrc)));
}

// Construct a _xvariant_t from a const VARIANT*
//
inline _xvariant_t::_xvariant_t(const VARIANT* pSrc)
{
    if (pSrc == NULL) {
        _com_issue_error(E_POINTER);
    }
    else {
        ::VariantInit(this);
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(pSrc)));
    }
}

// Construct a _xvariant_t from a const _xvariant_t&
//
inline _xvariant_t::_xvariant_t(const _xvariant_t& varSrc)
{
    ::VariantInit(this);
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc))));
}

// Construct a _xvariant_t from a VARIANT&.  If fCopy is false, give control of
// data to the _xvariant_t without doing a VariantCopy.
//
inline _xvariant_t::_xvariant_t(VARIANT& varSrc, bool fCopy)
{
    if (fCopy) {
        ::VariantInit(this);
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, &varSrc));
    }
    else {
        _COM_MEMCPY_S(this, sizeof(varSrc), &varSrc, sizeof(varSrc));
        V_VT(&varSrc) = VT_EMPTY;
    }
}

// Construct either a VT_I2 VARIANT or a VT_BOOL VARIANT from
// a short (the default is VT_I2)
//
inline _xvariant_t::_xvariant_t(short sSrc, VARTYPE vtSrc)
{
    if ((vtSrc != VT_I2) && (vtSrc != VT_BOOL)) {
        _com_issue_error(E_INVALIDARG);
        return;
    }

    if (vtSrc == VT_BOOL) {
        V_VT(this) = VT_BOOL;
        V_BOOL(this) = (sSrc ? VARIANT_TRUE : VARIANT_FALSE);
    }
    else {
        V_VT(this) = VT_I2;
        V_I2(this) = sSrc;
    }
}

// Construct either a VT_I4 VARIANT, a VT_BOOL VARIANT, or a
// VT_ERROR VARIANT from a long (the default is VT_I4)
//
inline _xvariant_t::_xvariant_t(long lSrc, VARTYPE vtSrc)
{
    if ((vtSrc != VT_I4) && (vtSrc != VT_ERROR) && (vtSrc != VT_BOOL)) {
        _com_issue_error(E_INVALIDARG);
        return;
    }

    if (vtSrc == VT_ERROR) {
        V_VT(this) = VT_ERROR;
        V_ERROR(this) = lSrc;
    }
    else if (vtSrc == VT_BOOL) {
        V_VT(this) = VT_BOOL;
        V_BOOL(this) = (lSrc ? VARIANT_TRUE : VARIANT_FALSE);
    }
    else {
        V_VT(this) = VT_I4;
        V_I4(this) = lSrc;
    }
}

// Construct a VT_R4 VARIANT from a float
//
inline _xvariant_t::_xvariant_t(float fltSrc) throw()
{
    V_VT(this) = VT_R4;
    V_R4(this) = fltSrc;
}

// Construct either a VT_R8 VARIANT, or a VT_DATE VARIANT from
// a double (the default is VT_R8)
//
inline _xvariant_t::_xvariant_t(double dblSrc, VARTYPE vtSrc)
{
    if ((vtSrc != VT_R8) && (vtSrc != VT_DATE)) {
        _com_issue_error(E_INVALIDARG);
        return;
    }

    if (vtSrc == VT_DATE) {
        V_VT(this) = VT_DATE;
        V_DATE(this) = dblSrc;
    }
    else {
        V_VT(this) = VT_R8;
        V_R8(this) = dblSrc;
    }
}

// Construct a VT_CY from a CY
//
inline _xvariant_t::_xvariant_t(const CY& cySrc) throw()
{
    V_VT(this) = VT_CY;
    V_CY(this) = cySrc;
}

// Construct a VT_BSTR VARIANT from a const _xbstr_t&
//
inline _xvariant_t::_xvariant_t(const _xbstr_t& bstrSrc)
{
    V_VT(this) = VT_BSTR;

    BSTR bstr = static_cast<wchar_t*>(bstrSrc);
    if (bstr == NULL) {
        V_BSTR(this) = NULL;
    }
    else {
        V_BSTR(this) = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(bstr),
                                                          ZComAPI::ZComSysStringByteLen(bstr));
        if (V_BSTR(this) == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }
}

// Construct a VT_BSTR VARIANT from a const wchar_t*
//
inline _xvariant_t::_xvariant_t(const wchar_t* pSrc)
{
    V_VT(this) = VT_BSTR;

    V_BSTR(this) = ZComAPI::ZComSysAllocString(pSrc);

    if (V_BSTR(this) == NULL && pSrc != NULL) {
        _com_issue_error(E_OUTOFMEMORY);
    }
}

// Construct a VT_BSTR VARIANT from a const char*
//
inline _xvariant_t::_xvariant_t(const char* pSrc)
{
    V_VT(this) = VT_BSTR;
    V_BSTR(this) = ZComAPI::ZtlConvertStringToBSTR(pSrc);
}

// Construct a VT_DISPATCH VARIANT from an IDispatch*
//
inline _xvariant_t::_xvariant_t(IDispatch* pSrc, bool fAddRef) throw()
{
    V_VT(this) = VT_DISPATCH;
    V_DISPATCH(this) = pSrc;

    // Need the AddRef() as VariantClear() calls Release(), unless fAddRef
    // false indicates we're taking ownership
    //
    if (fAddRef) {
        if (V_DISPATCH(this) != NULL) {
            V_DISPATCH(this)->AddRef();
        }
    }
}

// Construct a VT_BOOL VARIANT from a bool
//
inline _xvariant_t::_xvariant_t(bool boolSrc) throw()
{
    V_VT(this) = VT_BOOL;
    V_BOOL(this) = (boolSrc ? VARIANT_TRUE : VARIANT_FALSE);
}

// Construct a VT_UNKNOWN VARIANT from an IUnknown*
//
inline _xvariant_t::_xvariant_t(IUnknown* pSrc, bool fAddRef) throw()
{
    V_VT(this) = VT_UNKNOWN;
    V_UNKNOWN(this) = pSrc;

    // Need the AddRef() as VariantClear() calls Release(), unless fAddRef
    // false indicates we're taking ownership
    //
    if (fAddRef) {
        if (V_UNKNOWN(this) != NULL) {
            V_UNKNOWN(this)->AddRef();
        }
    }
}

// Construct a VT_DECIMAL VARIANT from a DECIMAL
//
inline _xvariant_t::_xvariant_t(const DECIMAL& decSrc) throw()
{
    // Order is important here! Setting V_DECIMAL wipes out the entire VARIANT
    //
    V_DECIMAL(this) = decSrc;
    V_VT(this) = VT_DECIMAL;
}

// Construct a VT_UI1 VARIANT from a BYTE (unsigned char)
//
inline _xvariant_t::_xvariant_t(BYTE bSrc) throw()
{
    V_VT(this) = VT_UI1;
    V_UI1(this) = bSrc;
}

// Construct a VT_I1 VARIANT from a char
//
inline _xvariant_t::_xvariant_t(char cSrc) throw()
{
    V_VT(this) = VT_I1;
    V_I1(this) = cSrc;
}

// Construct a VT_UI2 VARIANT from a unsigned short
//
inline _xvariant_t::_xvariant_t(unsigned short usSrc) throw()
{
    V_VT(this) = VT_UI2;
    V_UI2(this) = usSrc;
}

// Construct a VT_UI4 VARIANT from a unsigned long
//
inline _xvariant_t::_xvariant_t(unsigned long ulSrc) throw()
{
    V_VT(this) = VT_UI4;
    V_UI4(this) = ulSrc;
}

// Construct a VT_INT VARIANT from a int
//
inline _xvariant_t::_xvariant_t(int iSrc) throw()
{
    V_VT(this) = VT_INT;
    V_INT(this) = iSrc;
}

// Construct a VT_UINT VARIANT from a unsigned int
//
inline _xvariant_t::_xvariant_t(unsigned int uiSrc) throw()
{
    V_VT(this) = VT_UINT;
    V_UINT(this) = uiSrc;
}

#if (_WIN32_WINNT >= 0x0501)
// Construct a VT_I8 VARIANT from a __int64
//
inline _xvariant_t::_xvariant_t(__int64 i8Src) throw()
{
    V_VT(this) = VT_I8;
    V_I8(this) = i8Src;
}

// Construct a VT_UI8 VARIANT from a unsigned __int64
//
inline _xvariant_t::_xvariant_t(unsigned __int64 ui8Src) throw()
{
    V_VT(this) = VT_UI8;
    V_UI8(this) = ui8Src;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//
// Extractors
//
//////////////////////////////////////////////////////////////////////////////////////////

// Extracts a VT_I2 into a short
//
inline _xvariant_t::operator short() const
{
    if (V_VT(this) == VT_I2) {
        return V_I2(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_I2, this);

    return V_I2(&varDest);
}

// Extracts a VT_I4 into a long
//
inline _xvariant_t::operator long() const
{
    if (V_VT(this) == VT_I4) {
        return V_I4(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_I4, this);

    return V_I4(&varDest);
}

// Extracts a VT_R4 into a float
//
inline _xvariant_t::operator float() const
{
    if (V_VT(this) == VT_R4) {
        return V_R4(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_R4, this);

    return V_R4(&varDest);
}

// Extracts a VT_R8 into a double
//
inline _xvariant_t::operator double() const
{
    if (V_VT(this) == VT_R8) {
        return V_R8(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_R8, this);

    return V_R8(&varDest);
}

// Extracts a VT_CY into a CY
//
inline _xvariant_t::operator CY() const
{
    if (V_VT(this) == VT_CY) {
        return V_CY(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_CY, this);

    return V_CY(&varDest);
}

// Extracts a VT_BSTR into a _xbstr_t
//
inline _xvariant_t::operator _xbstr_t() const
{
    if (V_VT(this) == VT_BSTR) {
        return V_BSTR(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_BSTR, this);

    return V_BSTR(&varDest);
}

// Extracts a VT_DISPATCH into an IDispatch*
//
inline _xvariant_t::operator IDispatch*() const
{
    if (V_VT(this) == VT_DISPATCH) {
        if (V_DISPATCH(this) != NULL) {
            V_DISPATCH(this)->AddRef();
        }
        return V_DISPATCH(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_DISPATCH, this);

    if (V_DISPATCH(&varDest) != NULL) {
        V_DISPATCH(&varDest)->AddRef();
    }

    return V_DISPATCH(&varDest);
}

// Extract a VT_BOOL into a bool
//
inline _xvariant_t::operator bool() const
{
    if (V_VT(this) == VT_BOOL) {
        return V_BOOL(this) ? true : false;
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_BOOL, this);

    return (V_BOOL(&varDest) == VARIANT_TRUE) ? true : false;
}

// Extracts a VT_UNKNOWN into an IUnknown*
//
inline _xvariant_t::operator IUnknown*() const
{
    if (V_VT(this) == VT_UNKNOWN) {
        if (V_UNKNOWN(this) != NULL) {
            V_UNKNOWN(this)->AddRef();
        }
        return V_UNKNOWN(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_UNKNOWN, this);

    if (V_UNKNOWN(&varDest) != NULL) {
        V_UNKNOWN(&varDest)->AddRef();
    }

    return V_UNKNOWN(&varDest);
}

// Extracts a VT_DECIMAL into a DECIMAL
//
inline _xvariant_t::operator DECIMAL() const
{
    if (V_VT(this) == VT_DECIMAL) {
        return V_DECIMAL(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_DECIMAL, this);

    return V_DECIMAL(&varDest);
}

// Extracts a VT_UI1 into a BYTE (unsigned char)
//
inline _xvariant_t::operator BYTE() const
{
    if (V_VT(this) == VT_UI1) {
        return V_UI1(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_UI1, this);

    return V_UI1(&varDest);
}

// Extract the physical VARIANT
//
inline _xvariant_t::operator VARIANT() const throw()
{
    return *(VARIANT*) this;
}

// Extracts a VT_I1 into a char
//
inline _xvariant_t::operator char() const
{
    if (V_VT(this) == VT_I1) {
        return V_I1(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_I1, this);

    return V_I1(&varDest);
}

// Extracts a VT_UI2 into a unsigned short
//
inline _xvariant_t::operator unsigned short() const
{
    if (V_VT(this) == VT_UI2) {
        return V_UI2(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_UI2, this);

    return V_UI2(&varDest);
}

// Extracts a VT_UI4 into a unsigned long
//
inline _xvariant_t::operator unsigned long() const
{
    if (V_VT(this) == VT_UI4) {
        return V_UI4(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_UI4, this);

    return V_UI4(&varDest);
}

// Extracts a VT_INT into a int
//
inline _xvariant_t::operator int() const
{
    if (V_VT(this) == VT_INT) {
        return V_INT(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_INT, this);

    return V_INT(&varDest);
}

// Extracts a VT_UINT into a unsigned int
//
inline _xvariant_t::operator unsigned int() const
{
    if (V_VT(this) == VT_UINT) {
        return V_UINT(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_UINT, this);

    return V_UINT(&varDest);
}

#if (_WIN32_WINNT >= 0x0501)
// Extracts a VT_I8 into a __int64
//
inline _xvariant_t::operator __int64() const
{
    if (V_VT(this) == VT_I8) {
        return V_I8(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_I8, this);

    return V_I8(&varDest);
}

// Extracts a VT_UI8 into a unsigned __int64
//
inline _xvariant_t::operator unsigned __int64() const
{
    if (V_VT(this) == VT_UI8) {
        return V_UI8(this);
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_UI8, this);

    return V_UI8(&varDest);
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//
// Assignment operations
//
//////////////////////////////////////////////////////////////////////////////////////////

// Assign a const VARIANT& (ZComAPI::ZComVariantCopy handles everything)
//
inline _xvariant_t& _xvariant_t::operator=(const VARIANT& varSrc)
{
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(&varSrc)));

    return *this;
}

// Assign a const VARIANT* (ZComAPI::ZComVariantCopy handles everything)
//
inline _xvariant_t& _xvariant_t::operator=(const VARIANT* pSrc)
{
    if (pSrc == NULL) {
        _com_issue_error(E_POINTER);
    }
    else {
        _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(pSrc)));
    }

    return *this;
}

// Assign a const _xvariant_t& (ZComAPI::ZComVariantCopy handles everything)
//
inline _xvariant_t& _xvariant_t::operator=(const _xvariant_t& varSrc)
{
    _com_util::CheckError(ZComAPI::ZComVariantCopy(this, const_cast<VARIANT*>(static_cast<const VARIANT*>(&varSrc))));

    return *this;
}

// Assign a short creating either VT_I2 VARIANT or a
// VT_BOOL VARIANT (VT_I2 is the default)
//
inline _xvariant_t& _xvariant_t::operator=(short sSrc)
{
    if (V_VT(this) == VT_I2) {
        V_I2(this) = sSrc;
    }
    else if (V_VT(this) == VT_BOOL) {
        V_BOOL(this) = (sSrc ? VARIANT_TRUE : VARIANT_FALSE);
    }
    else {
        // Clear the VARIANT and create a VT_I2
        //
        Clear();

        V_VT(this) = VT_I2;
        V_I2(this) = sSrc;
    }

    return *this;
}

// Assign a long creating either VT_I4 VARIANT, a VT_ERROR VARIANT
// or a VT_BOOL VARIANT (VT_I4 is the default)
//
inline _xvariant_t& _xvariant_t::operator=(long lSrc)
{
    if (V_VT(this) == VT_I4) {
        V_I4(this) = lSrc;
    }
    else if (V_VT(this) == VT_ERROR) {
        V_ERROR(this) = lSrc;
    }
    else if (V_VT(this) == VT_BOOL) {
        V_BOOL(this) = (lSrc ? VARIANT_TRUE : VARIANT_FALSE);
    }
    else {
        // Clear the VARIANT and create a VT_I4
        //
        Clear();

        V_VT(this) = VT_I4;
        V_I4(this) = lSrc;
    }

    return *this;
}

// Assign a float creating a VT_R4 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(float fltSrc)
{
    if (V_VT(this) != VT_R4) {
        // Clear the VARIANT and create a VT_R4
        //
        Clear();

        V_VT(this) = VT_R4;
    }

    V_R4(this) = fltSrc;

    return *this;
}

// Assign a double creating either a VT_R8 VARIANT, or a VT_DATE
// VARIANT (VT_R8 is the default)
//
inline _xvariant_t& _xvariant_t::operator=(double dblSrc)
{
    if (V_VT(this) == VT_R8) {
        V_R8(this) = dblSrc;
    }
    else if(V_VT(this) == VT_DATE) {
        V_DATE(this) = dblSrc;
    }
    else {
        // Clear the VARIANT and create a VT_R8
        //
        Clear();

        V_VT(this) = VT_R8;
        V_R8(this) = dblSrc;
    }

    return *this;
}

// Assign a CY creating a VT_CY VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(const CY& cySrc)
{
    if (V_VT(this) != VT_CY) {
        // Clear the VARIANT and create a VT_CY
        //
        Clear();

        V_VT(this) = VT_CY;
    }

    V_CY(this) = cySrc;

    return *this;
}

// Assign a const _xbstr_t& creating a VT_BSTR VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(const _xbstr_t& bstrSrc)
{
    _COM_ASSERT(V_VT(this) != VT_BSTR || (BSTR) bstrSrc == NULL || V_BSTR(this) != (BSTR) bstrSrc);

    // Clear the VARIANT (This will SysFreeString() any previous occupant)
    //
    Clear();

    V_VT(this) = VT_BSTR;

    if (!bstrSrc) {
        V_BSTR(this) = NULL;
    }
    else {
        BSTR bstr = static_cast<wchar_t*>(bstrSrc);
        V_BSTR(this) = ZComAPI::ZComSysAllocStringByteLen(reinterpret_cast<char*>(bstr),
                                                          ZComAPI::ZComSysStringByteLen(bstr));

        if (V_BSTR(this) == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }

    return *this;
}

// Assign a const wchar_t* creating a VT_BSTR VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(const wchar_t* pSrc)
{
    _COM_ASSERT(V_VT(this) != VT_BSTR || pSrc == NULL || V_BSTR(this) != pSrc);

    // Clear the VARIANT (This will SysFreeString() any previous occupant)
    //
    Clear();

    V_VT(this) = VT_BSTR;

    if (pSrc == NULL) {
        V_BSTR(this) = NULL;
    }
    else {
        V_BSTR(this) = ZComAPI::ZComSysAllocString(pSrc);

        if (V_BSTR(this) == NULL) {
            _com_issue_error(E_OUTOFMEMORY);
        }
    }

    return *this;
}

// Assign a const char* creating a VT_BSTR VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(const char* pSrc)
{
    _COM_ASSERT(V_VT(this) != (VT_I1 | VT_BYREF) || pSrc == NULL || V_I1REF(this) != pSrc);

    // Clear the VARIANT (This will SysFreeString() any previous occupant)
    //
    Clear();

    V_VT(this) = VT_BSTR;
    V_BSTR(this) = ZComAPI::ZtlConvertStringToBSTR(pSrc);

    return *this;
}

// Assign an IDispatch* creating a VT_DISPATCH VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(IDispatch* pSrc)
{
    _COM_ASSERT(V_VT(this) != VT_DISPATCH || pSrc == 0 || V_DISPATCH(this) != pSrc);

    // Clear the VARIANT (This will Release() any previous occupant)
    //
    Clear();

    V_VT(this) = VT_DISPATCH;
    V_DISPATCH(this) = pSrc;

    if (V_DISPATCH(this) != NULL) {
        // Need the AddRef() as VariantClear() calls Release()
        //
        V_DISPATCH(this)->AddRef();
    }

    return *this;
}

// Assign a bool creating a VT_BOOL VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(bool boolSrc)
{
    if (V_VT(this) != VT_BOOL) {
        // Clear the VARIANT and create a VT_BOOL
        //
        Clear();

        V_VT(this) = VT_BOOL;
    }

    V_BOOL(this) = (boolSrc ? VARIANT_TRUE : VARIANT_FALSE);

    return *this;
}

// Assign an IUnknown* creating a VT_UNKNOWN VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(IUnknown* pSrc)
{
    _COM_ASSERT(V_VT(this) != VT_UNKNOWN || pSrc == NULL || V_UNKNOWN(this) != pSrc);

    // Clear VARIANT (This will Release() any previous occupant)
    //
    Clear();

    V_VT(this) = VT_UNKNOWN;
    V_UNKNOWN(this) = pSrc;

    if (V_UNKNOWN(this) != NULL) {
        // Need the AddRef() as VariantClear() calls Release()
        //
        V_UNKNOWN(this)->AddRef();
    }

    return *this;
}

// Assign a DECIMAL creating a VT_DECIMAL VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(const DECIMAL& decSrc)
{
    if (V_VT(this) != VT_DECIMAL) {
        // Clear the VARIANT
        //
        Clear();
    }

    // Order is important here! Setting V_DECIMAL wipes out the entire VARIANT
    V_DECIMAL(this) = decSrc;
    V_VT(this) = VT_DECIMAL;

    return *this;
}

// Assign a BTYE (unsigned char) creating a VT_UI1 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(BYTE bSrc)
{
    if (V_VT(this) != VT_UI1) {
        // Clear the VARIANT and create a VT_UI1
        //
        Clear();

        V_VT(this) = VT_UI1;
    }

    V_UI1(this) = bSrc;

    return *this;
}

// Assign a char creating a VT_I1 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(char cSrc)
{
    if (V_VT(this) != VT_I1) {
        // Clear the VARIANT and create a VT_I1
        //
        Clear();

        V_VT(this) = VT_I1;
    }

    V_I1(this) = cSrc;

    return *this;
}

// Assign a char creating a VT_UI2 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(unsigned short usSrc)
{
    if (V_VT(this) != VT_UI2) {
        // Clear the VARIANT and create a VT_UI2
        //
        Clear();

        V_VT(this) = VT_UI2;
    }

    V_UI2(this) = usSrc;

    return *this;
}

// Assign a char creating a VT_UI4 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(unsigned long ulSrc)
{
    if (V_VT(this) != VT_UI4) {
        // Clear the VARIANT and create a VT_UI4
        //
        Clear();

        V_VT(this) = VT_UI4;
    }

    V_UI4(this) = ulSrc;

    return *this;
}

// Assign a char creating a VT_INT VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(int iSrc)
{
    if (V_VT(this) != VT_INT) {
        // Clear the VARIANT and create a VT_INT
        //
        Clear();

        V_VT(this) = VT_INT;
    }

    V_INT(this) = iSrc;

    return *this;
}

// Assign a char creating a VT_UINT VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(unsigned int uiSrc)
{
    if (V_VT(this) != VT_UINT) {
        // Clear the VARIANT and create a VT_UINT
        //
        Clear();

        V_VT(this) = VT_UINT;
    }

    V_UINT(this) = uiSrc;

    return *this;
}

#if (_WIN32_WINNT >= 0x0501)
// Assign a char creating a VT_I8 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(__int64 i8Src)
{
    if (V_VT(this) != VT_I8) {
        // Clear the VARIANT and create a VT_I8
        //
        Clear();

        V_VT(this) = VT_I8;
    }

    V_I8(this) = i8Src;

    return *this;
}

// Assign a char creating a VT_UI8 VARIANT
//
inline _xvariant_t& _xvariant_t::operator=(unsigned __int64 ui8Src)
{
    if (V_VT(this) != VT_UI8) {
        // Clear the VARIANT and create a VT_UI8
        //
        Clear();

        V_VT(this) = VT_UI8;
    }

    V_UI8(this) = ui8Src;

    return *this;
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////
//
// Comparison operations
//
//////////////////////////////////////////////////////////////////////////////////////////

// Compare a _xvariant_t against a const VARIANT& for equality
//
inline bool _xvariant_t::operator==(const VARIANT& varSrc) const throw()
{
    return *this == &varSrc;
}

#pragma warning(push)
#pragma warning(disable: 4702) // unreachable code

// Compare a _xvariant_t against a const VARIANT* for equality
//
inline bool _xvariant_t::operator==(const VARIANT* pSrc) const throw()
{
    if (pSrc == NULL) {
        return false;
    }

    if (this == pSrc) {
        return true;
    }

    //
    // Variants not equal if types don't match
    //
    if (V_VT(this) != V_VT(pSrc)) {
        return false;
    }

    //
    // Check type specific values
    //
    switch (V_VT(this)) {
        case VT_EMPTY:
        case VT_NULL:
            return true;

        case VT_I2:
            return V_I2(this) == V_I2(pSrc);

        case VT_I4:
            return V_I4(this) == V_I4(pSrc);

        case VT_R4:
            return V_R4(this) == V_R4(pSrc);

        case VT_R8:
            return V_R8(this) == V_R8(pSrc);

        case VT_CY:
            return memcmp(&(V_CY(this)), &(V_CY(pSrc)), sizeof(CY)) == 0;

        case VT_DATE:
            return V_DATE(this) == V_DATE(pSrc);

        case VT_BSTR:
            return (ZComAPI::ZComSysStringByteLen(V_BSTR(this)) == ZComAPI::ZComSysStringByteLen(V_BSTR(pSrc))) &&
                   (memcmp(V_BSTR(this), V_BSTR(pSrc), ZComAPI::ZComSysStringByteLen(V_BSTR(this))) == 0);

        case VT_DISPATCH:
            return V_DISPATCH(this) == V_DISPATCH(pSrc);

        case VT_ERROR:
            return V_ERROR(this) == V_ERROR(pSrc);

        case VT_BOOL:
            return V_BOOL(this) == V_BOOL(pSrc);

        case VT_UNKNOWN:
            return V_UNKNOWN(this) == V_UNKNOWN(pSrc);

        case VT_DECIMAL:
            return memcmp(&(V_DECIMAL(this)), &(V_DECIMAL(pSrc)), sizeof(DECIMAL)) == 0;

        case VT_UI1:
            return V_UI1(this) == V_UI1(pSrc);

        case VT_I1:
            return V_I1(this) == V_I1(pSrc);

        case VT_UI2:
            return V_UI2(this) == V_UI2(pSrc);

        case VT_UI4:
            return V_UI4(this) == V_UI4(pSrc);

        case VT_INT:
            return V_INT(this) == V_INT(pSrc);

        case VT_UINT:
            return V_UINT(this) == V_UINT(pSrc);

#if (_WIN32_WINNT >= 0x0501)
        case VT_I8:
            return V_I8(this) == V_I8(pSrc);

        case VT_UI8:
            return V_UI8(this) == V_UI8(pSrc);
#endif

        default:
            _com_issue_error(E_INVALIDARG);
            // fall through
    }

    return false;
}

#pragma warning(pop)

// Compare a _xvariant_t against a const VARIANT& for in-equality
//
inline bool _xvariant_t::operator!=(const VARIANT& varSrc) const throw()
{
    return !(*this == &varSrc);
}

// Compare a _xvariant_t against a const VARIANT* for in-equality
//
inline bool _xvariant_t::operator!=(const VARIANT* pSrc) const throw()
{
    return !(*this == pSrc);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Low-level operations
//
//////////////////////////////////////////////////////////////////////////////////////////

// Clear the _xvariant_t
//
inline void _xvariant_t::Clear()
{
    _com_util::CheckError(ZComAPI::ZComVariantClear(this));
}

inline void _xvariant_t::Attach(VARIANT& varSrc)
{
    //
    // Free up previous VARIANT
    //
    Clear();

    //
    // Give control of data to _xvariant_t
    //
    _COM_MEMCPY_S(this, sizeof(varSrc), &varSrc, sizeof(varSrc));
    V_VT(&varSrc) = VT_EMPTY;
}

inline VARIANT _xvariant_t::Detach() throw()
{
    VARIANT varResult = *this;
    V_VT(this) = VT_EMPTY;

    return varResult;
}

inline VARIANT& _xvariant_t::GetVARIANT() throw()
{
    return *(VARIANT*) this;
}

inline VARIANT* _xvariant_t::GetAddress()
{
    Clear();
    return (VARIANT*) this;
}

// Change the type and contents of this _xvariant_t to the type vartype and
// contents of pSrc
//
inline void _xvariant_t::ChangeType(VARTYPE vartype, const _xvariant_t* pSrc)
{
    //
    // If pDest is NULL, convert type in place
    //
    if (pSrc == NULL) {
        pSrc = this;
    }

    if ((this != pSrc) || (vartype != V_VT(this))) {
        _com_util::CheckError(ZComAPI::ZComVariantChangeType(static_cast<VARIANT*>(this),
                                                             const_cast<VARIANT*>(static_cast<const VARIANT*>(pSrc)),
                                                             0, vartype));
    }
}

inline void _xvariant_t::SetString(const char* pSrc)
{
    operator=(pSrc);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Destructor
//
//////////////////////////////////////////////////////////////////////////////////////////

inline _xvariant_t::~_xvariant_t() noexcept
{
    ZComAPI::ZComVariantClear(this);
}

//////////////////////////////////////////////////////////////////////////////////////////
//
// Mutually-dependent definitions
//
//////////////////////////////////////////////////////////////////////////////////////////

// Construct a _xbstr_t from a const _xvariant_t&
//
inline _xbstr_t::_xbstr_t(const _xvariant_t &var)
        : m_Data(NULL)
{
    if (V_VT(&var) == VT_BSTR) {
        *this = V_BSTR(&var);
        return;
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_BSTR, &var);

    *this = V_BSTR(&varDest);
}

// Assign a const _xvariant_t& to a _xbstr_t
//
inline _xbstr_t& _xbstr_t::operator=(const _xvariant_t &var)
{
    if (V_VT(&var) == VT_BSTR) {
        *this = V_BSTR(&var);
        return *this;
    }

    _xvariant_t varDest;
    varDest.ChangeType(VT_BSTR, &var);

    *this = V_BSTR(&varDest);

    return *this;
}

extern _xvariant_t xvtMissing;

#ifndef _USE_RAW
#define xbstr_t _xbstr_t
#define xvariant_t _xvariant_t
#endif



#endif //XCOMDEF_H
