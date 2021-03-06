/* WEBagent - abstract inferface for WEB clients and servers

 * $Id: RdfDB.hpp,v 1.5 2008-10-14 12:02:57 eric Exp $

 * SWWEBagent is prefixed with SW to avoid collisions with
 * implementation names.
 */

#ifndef WEB_AGENT_H
#define WEB_AGENT_H

#include <string>
#include "SWObjects.hpp"

namespace w3c_sw {

    class SWWEBagent {
    protected:
	std::string mediaType;
    public:
	SWWEBagent () {  }
	virtual ~SWWEBagent () {  }
	std::string getMediaType () { return mediaType; }
	virtual std::string get(
#if REGEX_LIB == SWOb_DISABLED
				std::string host, std::string port, std::string path
#else /* !REGEX_LIB == SWOb_DISABLED */
				const char* url
#endif /* !REGEX_LIB == SWOb_DISABLED */
				) = 0;

	struct Parameter {
	    std::string attr;
	    std::string value;
	    Parameter (std::string attr, std::string value) : attr(attr), value(value) {  }
	};
	static std::string getURL (std::string service, const Parameter* start, size_t count) {
	    std::stringstream s;
	    s << service;
	    s << (service.find_first_of("?") == std::string::npos ? "?" : 
		  service.at(service.size()-1) == '&' ? "" : 
		  "&");
	    for (size_t i = 0; i < count; ++i)
		s << start[i].attr << "=" << start[i].value;
	    return s.str();
	}

	static std::string urlEncode (std::string encodeMe) {
	    std::stringstream s;
	    s.setf(std::ios::hex, std::ios::basefield);
	    s.setf(std::ios::uppercase);
	    for (std::string::const_iterator it = encodeMe.begin(); it != encodeMe.end(); ++it) {
		if (*it == ' ')
		    s << '+';
		else if ((*it >= 'a' && *it <= 'z') || 
			 (*it >= 'A' && *it <= 'Z') || 
			 (*it >= '0' && *it <= '9') || 
			 *it == '.' || *it == '-' || *it == '_')
		    s << *it;
		else if (*it < 0x10)
		    s << "%0" << (unsigned)*it;
		else
		    s << '%' << (unsigned)*it;
	    }
	    return s.str();
	}

	static std::string base64encode (std::string encodeMe) {
	    static const std::string base64_chars = 
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz"
		"0123456789+/";

	    unsigned char const* bytes_to_encode = (const unsigned char*)encodeMe.c_str();
	    size_t in_len = encodeMe.size();
	    std::string ret;
	    int i = 0;
	    int j = 0;
	    unsigned char char_array_3[3];
	    unsigned char char_array_4[4];

	    while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
		    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		    char_array_4[3] = char_array_3[2] & 0x3f;

		    for(i = 0; (i <4) ; i++)
			ret += base64_chars[char_array_4[i]];
		    i = 0;
		}
	    }

	    if (i)
		{
		    for(j = i; j < 3; j++)
			char_array_3[j] = '\0';

		    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
		    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
		    char_array_4[3] = char_array_3[2] & 0x3f;

		    for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		    while((i++ < 3))
			ret += '=';

		}

	    return ret;

	}



    };

} // namespace w3c_sw

#endif // !WEB_AGENT_H

