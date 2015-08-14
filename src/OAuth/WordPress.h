/*
 * WordPress.h
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#ifndef WORDPRESS_H_
#define WORDPRESS_H_

#include <QObject>
#include "OAuth.h"
#include "Singleton.h"

class WordPress: public OAuth
{
    Q_OBJECT
public:
    WordPress();
    virtual ~WordPress();
};

typedef Singleton<WordPress> OAuthx;

#endif /* WORDPRESS_H_ */
