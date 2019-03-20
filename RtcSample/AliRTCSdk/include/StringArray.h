#ifndef STRINGARRAY_H
#define STRINGARRAY_H

#include "AliRtcDefine.h"
#include "AliString.h"

namespace AliRtc {

class ALI_RTC_API StringArray
{
public:
    StringArray();
    virtual ~StringArray();

    void AddString(const String &s);

	void clear();
	int size() const;
	String at(int index) const;

private:
	void *data;
};

} // namespace AliRtc

#endif // STRINGARRAY_H
