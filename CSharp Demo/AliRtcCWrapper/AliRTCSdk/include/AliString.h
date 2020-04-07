#ifndef STRING_H
#define STRING_H

#include "AliRtcDefine.h"

/* String: a wrapper class for API
 * Performance is not considered here.
 * Unicode is not supported
 * Not thread safe
 */

namespace AliRtc {

class ALI_RTC_API String
{
public:
    String(const char* str=0);
    String(const String &other);
    String & operator=(const String& other);
    String & operator=(const char *str);
    String operator+(const String &other) const;
    String & operator+=(const String& other);

    virtual ~String(void)
    {
        delete[] mpData;
    }

	// get c string
	const char *c_str() const;

	// empty check
	bool isEmpty() const;

    // compare
    bool operator==(const String&) const;

    // get char
    char& operator[](unsigned int);

    // get string length
	int size() const;

private:
    char *mpData; // never be NULL
    int mnDataLen; // always > 0
};

} // namespace AliRtc

#endif // STRING_H
