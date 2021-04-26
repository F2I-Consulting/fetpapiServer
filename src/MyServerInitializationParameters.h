/*-----------------------------------------------------------------------
Licensed to the Apache Software Foundation (ASF) under one
or more contributor license agreements.  See the NOTICE file
distributed with this work for additional information
regarding copyright ownership.  The ASF licenses this file
to you under the Apache License, Version 2.0 (the
"License"; you may not use this file except in compliance
with the License.  You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing,
software distributed under the License is distributed on an
"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
KIND, either express or implied.  See the License for the
specific language governing permissions and limitations
under the License.
-----------------------------------------------------------------------*/
#pragma once

#include <fetpapi/etp/ServerInitializationParameters.h>

#include <vector>

#include <boost/uuid/random_generator.hpp>

#include <fetpapi/etp/ProtocolHandlers/CoreHandlers.h>

#include "MyDataObjectRepository.h"
#include "MyOwnDiscoveryProtocolHandlers.h"
#include "MyOwnStoreProtocolHandlers.h"
#include "MyOwnStoreNotificationProtocolHandlers.h"
#include "MyOwnDataArrayProtocolHandlers.h"

class MyServerInitializationParameters final : public ETP_NS::ServerInitializationParameters
{
private:
	MyDataObjectRepository* repo_;

public:
	MyServerInitializationParameters(MyDataObjectRepository* repo, boost::uuids::uuid instanceUuid, const std::string & host, unsigned short port) : ETP_NS::ServerInitializationParameters(instanceUuid, host, port), repo_(repo) {}
	~MyServerInitializationParameters() = default;

	void postSessionCreationOperation(ETP_NS::AbstractSession* session) const final {
		session->setCoreProtocolHandlers(std::make_shared<ETP_NS::CoreHandlers>(session));
		session->setDiscoveryProtocolHandlers(std::make_shared<MyOwnDiscoveryProtocolHandlers>(session, repo_));
		session->setStoreProtocolHandlers(std::make_shared<MyOwnStoreProtocolHandlers>(session, repo_));
		session->setStoreNotificationProtocolHandlers(std::make_shared<MyOwnStoreNotificationProtocolHandlers>(session, repo_));
		session->setDataArrayProtocolHandlers(std::make_shared<MyOwnDataArrayProtocolHandlers>(session, repo_));

		repo_->sessions.push_back(session);
	}
};
