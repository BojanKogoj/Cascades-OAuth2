/*
 * Imgur.h
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#ifndef IMGUR_H_
#define IMGUR_H_

#include <QObject>
#include "OAuth.h"
#include "Singleton.h"

class Imgur: public OAuth
{
    Q_OBJECT
public:
    Imgur();
    virtual ~Imgur();
};

typedef Singleton<Imgur> OAuthx;

#endif /* IMGUR_H_ */
