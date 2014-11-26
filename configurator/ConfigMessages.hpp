#ifndef _CONFIG_MESSAGES_HPP_
#define _CONFIG_MESSAGES_HPP_

#include "jiface.hpp"

class CConfigMessages : public CInterface
{
public:

    IMPLEMENT_IINTERFACE

    virtual ~CConfigMessages();
    int getNumberOfMessages();




private:

    CConfigMessages();
    {
    }



};

#endif // _CONFIG_MESSAGES_HPP_
