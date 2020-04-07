#ifndef DICTIONARY_H
#define DICTIONARY_H

#include "AliRtcDefine.h"
#include "AliString.h"

namespace AliRtc {

class ALI_RTC_API Dictionary
{
public:
    Dictionary();
    virtual ~Dictionary();
	Dictionary(const Dictionary &other);

	Dictionary & operator=(const Dictionary& other);


	void setValue(const char *key, const char  *val);
    void setValue(const char *key, bool val);

	String getValue(const char *key);
	bool getBoolValue(const char *key, bool defVal);

private:
	void *data;
};

} // namespace AliRtc

#endif // DICTIONARY_H
