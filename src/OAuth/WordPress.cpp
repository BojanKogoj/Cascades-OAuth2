/*
 * WordPress.cpp
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#include "WordPress.h"
//#include "ResponseType.h"

WordPress::WordPress()
{
    // API info
    setAuthorizeUrl("https://public-api.wordpress.com/oauth2/authorize");
    setRefreshUrl("https://public-api.wordpress.com/oauth2/token");

    // Client info
    setClientId("41987");
    setClientSecret("oBdJrdp1YBhBHrrA93jkToxVPZ0jFV5MihIUF7SK4yTCqL5flZnX8EnAhWtnYHqQ");
    setRedirectURI("http://darkenergy.si");
    setResponseType(CODE);
}

WordPress::~WordPress()
{
}

