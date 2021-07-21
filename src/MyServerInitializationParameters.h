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

	std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> makeSupportedDataObjects() const final
	{
		std::vector<Energistics::Etp::v12::Datatypes::SupportedDataObject> result;

		Energistics::Etp::v12::Datatypes::SupportedDataObject supportedDataObject;
		supportedDataObject.qualifiedType = "resqml20.*";
		Energistics::Etp::v12::Datatypes::DataValue value;
		value.item.set_boolean(false);
		supportedDataObject.dataObjectCapabilities["SupportsDelete"] = value;
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "resqml22.*";
		result.push_back(supportedDataObject);

		supportedDataObject.qualifiedType = "eml20.EpcExternalPartReference";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml21.PropertyKind";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml23.Activity";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml23.ActivityTemplate";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml23.GraphicalInformationSet";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml23.PropertyKind";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml23.TimeSeries";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "eml23.EpcExternalPartReference";
		result.push_back(supportedDataObject);

		supportedDataObject.qualifiedType = "witsml20.Channel";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "witsml20.Trajectory";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "witsml20.Well";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "witsml20.Wellbore";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "witsml20.WellboreCompletion";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "witsml20.WellboreGeometry";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "witsml20.WellCompletion";
		result.push_back(supportedDataObject);

		supportedDataObject.qualifiedType = "prodml21.FluidCharacterization";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "prodml21.FluidSystem";
		result.push_back(supportedDataObject);
		supportedDataObject.qualifiedType = "prodml21.TimeSeriesData";
		result.push_back(supportedDataObject);

		return result;
	}

	void postSessionCreationOperation(ETP_NS::AbstractSession* session) const final {
		session->setCoreProtocolHandlers(std::make_shared<ETP_NS::CoreHandlers>(session));
		session->setDiscoveryProtocolHandlers(std::make_shared<MyOwnDiscoveryProtocolHandlers>(session, repo_));
		session->setStoreProtocolHandlers(std::make_shared<MyOwnStoreProtocolHandlers>(session, repo_));
		session->setStoreNotificationProtocolHandlers(std::make_shared<MyOwnStoreNotificationProtocolHandlers>(session, repo_));
		session->setDataArrayProtocolHandlers(std::make_shared<MyOwnDataArrayProtocolHandlers>(session, repo_));

		repo_->sessions.push_back(session);
	}
};
