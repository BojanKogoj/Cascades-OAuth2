/*
 * Slack.h
 *
 *  Created on: 23. jun. 2016
 *      Author: Bojan
 */

#ifndef SLACK_H_
#define SLACK_H_

#include <QObject>
#include "OAuth.h"
#include "Singleton.h"

class Slack: public OAuth
{
    Q_OBJECT
public:
    Slack();
    virtual ~Slack();
};

typedef Singleton<Slack> OAuthx;

#endif /* SLACK_H_ */
