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

#include "../common/AbstractObject.h"

#include "../MacroDefinitions.h"

namespace RESQML2_0_1_NS
{
	class WellboreFeature;
}

namespace WITSML2_1_NS
{
	class WellboreCompletion;
	class WellboreGeometry;
	class Log;
}

namespace WITSML2_NS
{
	class Trajectory;
	class Well;

	/** 
	 * @brief	A wellbore represents the path from surface to a unique bottomhole location.
	 */
	class Wellbore : public COMMON_NS::AbstractObject
	{
	public:

		/**
		 * Only to be used in partial transfer context
		 *
		 * @param [in,out]	partialObject	If non-null, the partial object.
		 */
		DLL_IMPORT_OR_EXPORT Wellbore(gsoap_resqml2_0_1::eml20__DataObjectReference* partialObject) : COMMON_NS::AbstractObject(partialObject) {}

		/** Destructor does nothing since the memory is managed by the gsoap context. */
		virtual ~Wellbore() = default;

		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, Number)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, SuffixAPI)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(std::string, NumGovt)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_3::eml23__WellStatus, StatusWellbore)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_3::witsml21__WellPurpose, PurposeWellbore)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_3::witsml21__WellboreType, TypeWellbore)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_3::witsml21__WellboreShape, Shape)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(gsoap_eml2_3::witsml21__WellFluid, FluidWellbore)
		ABSTRACT_GETTER_AND_SETTER_GENERIC_OPTIONAL_ATTRIBUTE(bool, AchievedTD)
		ABSTRACT_GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(Md, gsoap_eml2_3::eml23__LengthUom)
		ABSTRACT_GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdBit, gsoap_eml2_3::eml23__LengthUom)
		ABSTRACT_GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdKickoff, gsoap_eml2_3::eml23__LengthUom)
		ABSTRACT_GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdPlanned, gsoap_eml2_3::eml23__LengthUom)
		ABSTRACT_GETTER_AND_SETTER_DEPTH_MEASURE_OPTIONAL_ATTRIBUTE(MdSubSeaPlanned, gsoap_eml2_3::eml23__LengthUom)

		/**
		 * @brief	Gets the well
		 *
		 * @returns	Nullptr if it fails, else the well.
		 */
		DLL_IMPORT_OR_EXPORT Well* getWell() const;

		/**
		 * @brief	Gets well dor
		 *
		 * @returns	The well dor.
		 */
		virtual COMMON_NS::DataObjectReference getWellDor() const = 0;

		/**
		 * Sets a well
		 *
		 * @param [in,out]	witsmlWell	If non-null, the witsml well.
		 */
		DLL_IMPORT_OR_EXPORT virtual void setWell(Well* witsmlWell) = 0;

		GETTER_DATAOBJECTS(RESQML2_NS::WellboreFeature, ResqmlWellboreFeature);

		GETTER_DATAOBJECTS(WITSML2_NS::Trajectory, Trajectory)
		GETTER_DATAOBJECTS(WITSML2_1_NS::WellboreCompletion, WellboreCompletion)
		GETTER_DATAOBJECTS(WITSML2_1_NS::WellboreGeometry, WellboreGeometry)
		GETTER_DATAOBJECTS(WITSML2_1_NS::Log, Log)

		/**
		 * The standard XML tag without XML namespace for serializing this data object.
		 *
		 * @returns	The XML tag.
		 */
		DLL_IMPORT_OR_EXPORT static constexpr char const* XML_TAG = "Wellbore";

		/**
		 * Get the standard XML tag without XML namespace for serializing this data object.
		 *
		 * @returns	The XML tag.
		 */
		DLL_IMPORT_OR_EXPORT virtual std::string getXmlTag() const final { return XML_TAG; }

		/**
		* Resolve all relationships of the object in the repository.
		*/
		void loadTargetRelationships() final;

	protected:

		/** Default constructor does nothing */
		Wellbore() = default;

		/**
		 * Creates an instance of this class by wrapping a gsoap instance.
		 *
		 * @param [in,out]	fromGsoap	If non-null, from gsoap.
		 */
		Wellbore(gsoap_eml2_3::witsml21__Wellbore* fromGsoap) : COMMON_NS::AbstractObject(fromGsoap) {}

		/**
		 * Creates an instance of this class by wrapping a gsoap instance.
		 *
		 * @param [in,out]	fromGsoap	If non-null, from gsoap.
		 */
		//Trajectory(gsoap_eml2_3::_resqml22__Trajectory* fromGsoap) : COMMON_NS::AbstractObject(fromGsoap) {}
	};
}
