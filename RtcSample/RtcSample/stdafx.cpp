
// stdafx.cpp : source file that includes just the standard includes
// RtcSample.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


CString toCString(const std::string &s)
{
#ifdef _UNICODE
    USES_CONVERSION;
    CString cs(s.c_str());
    return cs;
#else
    CString cs;
    cs.Format("%s", str.c_str());
    return cs;
#endif
}

std::string toString(const CString &cs)
{
#ifdef _UNICODE
    USES_CONVERSION;
    std::string s(W2A(cs));
    return s;
#else
    std::string s(cs.GetBuffer());
    cs.ReleaseBuffer();
    return s;
#endif
}