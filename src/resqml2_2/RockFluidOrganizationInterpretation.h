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

#include "../resqml2/RockFluidOrganizationInterpretation.h"

namespace RESQML2_2_NS
{
	/**
	* This class is a container for rock fluid units.
	*/
	class RockFluidOrganizationInterpretation final : public RESQML2_NS::RockFluidOrganizationInterpretation
	{
	public:

		/**
		* Only to be used in partial transfer context
		*/
		DLL_IMPORT_OR_EXPORT RockFluidOrganizationInterpretation(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : RESQML2_NS::RockFluidOrganizationInterpretation(partialObject) {}

		/**
		 * @brief	Creates an instance of this class in a gsoap context.
		 *
		 * @exception	std::invalid_argument	If <tt>orgFeat == nullptr</tt>.
		 *
		 * @param [in]	orgFeat	The feature the instance interprets. It must be a "fluid"
		 * 						organization feature.
		 * @param 	  	guid   	The guid to set to the interpretation. If empty then a new guid will be
		 * 						generated.
		 * @param 	  	title  	A title for the instance to create.
		 */
		RockFluidOrganizationInterpretation(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);

		/**
		* Creates an instance of this class by wrapping a gsoap instance.
		*/
		RockFluidOrganizationInterpretation(gsoap_eml2_3::_resqml22__RockFluidOrganizationInterpretation* fromGsoap) : RESQML2_NS::RockFluidOrganizationInterpretation(fromGsoap) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		~RockFluidOrganizationInterpretation() = default;

		DLL_IMPORT_OR_EXPORT void pushBackRockFluidUnitInterpretation(RESQML2_NS::RockFluidUnitInterpretation * rockFluidUnitInterpretation) final;

		DLL_IMPORT_OR_EXPORT uint64_t getRockFluidUnitInterpCount() const final;

		COMMON_NS::DataObjectReference getRockFluidUnitInterpretationDor(uint64_t index) const final;

		/** The standard XML tag without XML namespace for serializing this data object. */
		DLL_IMPORT_OR_EXPORT static const char* XML_TAG;

		DLL_IMPORT_OR_EXPORT virtual std::string getXmlTag() const final { return XML_TAG; }

		/**
		* The standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT static const char* XML_NS;

		/**
		* Get the standard XML namespace for serializing this data object.
		*/
		DLL_IMPORT_OR_EXPORT std::string getXmlNamespace() const final { return XML_NS; }
	};
}
