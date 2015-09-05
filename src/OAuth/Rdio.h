/*
 * WordPress.h
 *
 *  Created on: 12. avg. 2015
 *      Author: Bojan
 */

#ifndef RDIO_H_
#define RDIO_H_

#include <QObject>
#include "OAuth.h"
#include "Singleton.h"

class Rdio: public OAuth
{
    Q_OBJECT
public:
    Rdio();
    virtual ~Rdio();
};

typedef Singleton<Rdio> OAuthx;

#endif /* RDIO_H_ */
