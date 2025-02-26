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
%module fesapi

%{
#if defined(__clang__) || defined(_MSC_VER)
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic ignored "-Wcast-qual"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif
%}

%include "stdint.i"
%include "std_string.i"

%include "../src/nsDefinitions.h"

//************************/
// JAVA
//************************/

#ifdef SWIGJAVA
	// https://stackoverflow.com/a/60208989
	%define PRIMITIVE_TYPEMAP(NEW_TYPE, TYPE)
	%clear NEW_TYPE;
	%apply TYPE { NEW_TYPE };
	%enddef // PRIMITIVE_TYPEMAP
#if defined(SWIGWORDSIZE64)
	PRIMITIVE_TYPEMAP(long int, long long);
	PRIMITIVE_TYPEMAP(unsigned long int, long long);
#endif
	// We don't want to use BigInteger in java.
	PRIMITIVE_TYPEMAP(unsigned long long int, long long);
#undef PRIMITIVE_TYPEMAP

	/*
	 When using multiple modules or the nspace feature it is common to invoke SWIG with a different -package command line option for each module.
	 However, by default the generated code may not compile if generated classes in one package use generated classes in another package.
	 The visibility of the getCPtr() and pointer constructor generated from the javabody typemaps needs changing.
	 The default visibility is protected but it needs to be public for access from a different package. Just changing 'protected' to 'public' in the typemap achieves this.
	 Two macros are available in java.swg to make this easier and using them is the preferred approach over simply copying the typemaps and modifying as this is forward compatible with any changes in the javabody typemap in future versions of SWIG.
	 The macros are for the proxy and typewrapper classes and can respectively be used to make the method and constructor public:
	  SWIG_JAVABODY_PROXY(public, public, SWIGTYPE)
	  SWIG_JAVABODY_TYPEWRAPPER(public, public, public, SWIGTYPE)
	*/
	SWIG_JAVABODY_PROXY(public, public, SWIGTYPE)
	SWIG_JAVABODY_TYPEWRAPPER(public, public, public, SWIGTYPE)
	
	// http://www.swig.org/Doc4.0/SWIGDocumentation.html#Java_proper_enums
	%include "enums.swg"
	%javaconst(1);
	
	%include "swigJavaInclude.i"
#endif

//************************/
// CSHARP
//************************/

#ifdef SWIGCSHARP
	SWIG_CSBODY_PROXY(public, public, SWIGTYPE)
	SWIG_CSBODY_TYPEWRAPPER(public, public, public, SWIGTYPE)
	
	%include "swigCsInclude.i"
#endif

//************************/
// PYTHON
//************************/

#ifdef SWIGPYTHON

	%{
	#define SWIG_FILE_WITH_INIT // In case we use Python Swig Wrapping
	%}
	%include "swigPythonInclude.i"

#endif


//************************/
// POD C ARRAYS
//************************/

%include "carrays_indexing64bits.i"
#ifdef SWIGJAVA // Use functions instead of classes in java in order to avoid premature garbage collection
	%array_functions(int64_t, Int64Array);
	%array_functions(uint64_t, UInt64Array);
	%array_functions(int32_t, Int32Array);
	%array_functions(uint32_t, UInt32Array);
	%array_functions(int16_t, Int16Array);
	%array_functions(uint16_t, UInt16Array);
	%array_functions(int8_t, Int8Array);
	%array_functions(uint8_t, UInt8Array);
	%array_functions(float, FloatArray);
	%array_functions(double, DoubleArray);
	%array_functions(bool, BoolArray);
#else // Use Dispose() (or GC.KeepAlive) on these arrays to ensure no premature garbage collection in C#
	%array_class(int64_t, Int64Array);
	%array_class(uint64_t, UInt64Array);
	%array_class(int32_t, Int32Array);
	%array_class(uint32_t, UInt32Array);
	%array_class(int16_t, Int16Array);
	%array_class(uint16_t, UInt16Array);
	%array_class(int8_t, Int8Array);
	%array_class(uint8_t, UInt8Array);
	%array_class(float, FloatArray);
	%array_class(double, DoubleArray);
	%array_class(bool, BoolArray);
#endif

// Example below of premature garbage collection
// DoubleArray myDoubleArray = new DoubleArray(100);
// myOperations(myDoubleArray.cast());
// myDoubleArray can be GC after cast and before myOperations has finished.

// Don't try to create vector of polymorphic data unless you really know what you are doing.
// Use C array approach instead.
%include "std_vector.i"

%template(StringVector) std::vector< std::string >;
%template(Int32Vector) std::vector< int32_t >;
%template(UInt32Vector) std::vector< uint32_t >;
// Vector of int64 and uint64 are tough to port because of long vs long long platform
// Indeed c++ long corresponds to int in java where c++ long long corresponds to long in java
%template(FloatVector) std::vector< float >;
%template(DoubleVector) std::vector< double >;
%template(BoolVector) std::vector< bool >;

//************************/
// EXCEPTIONS
//************************/

%include "exception.i"
%exception {
    try {
        $action
    }
    catch (std::invalid_argument & e) {
        SWIG_exception(SWIG_TypeError,const_cast<char*>(e.what()));
    } 
    catch (std::out_of_range& e) {
        SWIG_exception(SWIG_IndexError,const_cast<char*>(e.what()));
    }
    catch (std::logic_error & e) {
        SWIG_exception(SWIG_SyntaxError,const_cast<char*>(e.what()));
    } 
    catch(...) {
        SWIG_exception(SWIG_RuntimeError,"Unknown exception at F2I C++ API level");
    }
}

typedef long long 				time_t;

#if defined(SWIGJAVA) || defined(SWIGCSHARP)
	%nspace COMMON_NS::AbstractObject;
	%nspace COMMON_NS::DataObjectReference;
	%nspace COMMON_NS::DataObjectRepository;
	%nspace COMMON_NS::EnumStringMapper;
	%nspace COMMON_NS::EpcDocument;
	%nspace COMMON_NS::HdfProxyFactory;
#endif

namespace EML2_NS
{
	class Activity;
}

%module(directors="1") fesapi
%feature("director") COMMON_NS::HdfProxyFactory;

/*
Following csbody and javabody typemap definitions force the target language to not 
manage the memory of the C++ part of wrapped COMMON_NS::HdfProxyFactory and 
COMMON_NS::AbstractObject. This C++ parts will be free at the destruction
of the COMMON_NS::DataObjectRepository thanks to the 
DataObjectRepository::clear() method (called by DataObjectRepository 
destructor).
*/
%typemap(csbody) COMMON_NS::HdfProxyFactory %{
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public HdfProxyFactory(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = false;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  public static global::System.Runtime.InteropServices.HandleRef getCPtr(HdfProxyFactory obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }
%}
%typemap(javabody) COMMON_NS::HdfProxyFactory %{
  private transient long swigCPtr;
  protected transient boolean swigCMemOwn;

  public HdfProxyFactory(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = false;
    swigCPtr = cPtr;
  }

  public static long getCPtr(HdfProxyFactory obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }
%}

/* 
AbstractHdfProxy may be inherited in target languages. In such case, overriding
AbstractObject::getXmlNamespace() is required. Thus, AbstractObject needs to be
a director base class. 
Note: to not export the virtual destructor in a director base class leads to a 
warning (205) during the Swig processing.
*/
%feature("director") COMMON_NS::AbstractObject;
%typemap(csbody) COMMON_NS::AbstractObject %{
  private global::System.Runtime.InteropServices.HandleRef swigCPtr;
  protected bool swigCMemOwn;

  public AbstractObject(global::System.IntPtr cPtr, bool cMemoryOwn) {
    swigCMemOwn = false;
    swigCPtr = new global::System.Runtime.InteropServices.HandleRef(this, cPtr);
  }

  public static global::System.Runtime.InteropServices.HandleRef getCPtr(AbstractObject obj) {
    return (obj == null) ? new global::System.Runtime.InteropServices.HandleRef(null, global::System.IntPtr.Zero) : obj.swigCPtr;
  }
%}
namespace COMMON_NS
{
	%nodefaultctor; // Disable creation of default constructors
	
	class DataObjectReference
	{
	public:
		/**
		 * Checks if this reference is empty (i.e. it points to nothing)
		 *
		 * @returns	True if this reference empty, false if it is not.
		 */
		bool isEmpty() const;
		
		/**
		 * Gets the referenced data object UUID
		 *
		 * @returns	The UUID of the referenced data object if it exists, otherwise empty string.
		 */
		std::string getUuid() const;
		
		/**
		 * Gets the referenced data object title (i.e. its name)
		 *
		 * @returns	The title of the referenced data object if it exists, otherwise empty string.
		 */
		std::string getTitle() const;
		
		/**
		 * Gets the referenced data object version
		 *
		 * @returns	The version of the referenced data object if it exists, otherwise empty string.
		 */
		std::string getVersion() const;
		
		/**
		 * Gets the referenced data object type
		 *
		 * @returns	The content type of the referenced data object if it exists, otherwise empty string.
		 */
		std::string getContentType() const;
	};
	
	class AbstractObject
	{
	public:
		enum class numericalDatatypeEnum { UNKNOWN = 0, DOUBLE = 1, FLOAT = 2, INT64 = 3, UINT64 = 4, INT32 = 5, UINT32 = 6, INT16 = 7, UINT16 = 8, INT8 = 9, UINT8 = 10};
	
		COMMON_NS::DataObjectRepository* getRepository() const;
	
		virtual ~AbstractObject();
	
		bool isPartial() const;
	
		std::string getUuid() const;
		std::string getTitle() const;
		std::string getEditor() const;
		time_t getCreation() const;
		std::string getOriginator() const;
		std::string getDescription() const;
		time_t getLastUpdate() const;
		std::string getFormat() const;
		std::string getDescriptiveKeywords() const;
		std::string getVersion() const;
		
		void setMetadata(const std::string & title, const std::string & editor, time_t creation, const std::string & originator, const std::string & description, time_t lastUpdate, const std::string & descriptiveKeywords);
		void setTitle(const std::string & title);
		void setEditor(const std::string & editor);
		void setCreation(time_t creation);
		void setOriginator(const std::string & originator);
		void setDescription(const std::string & description);
		void setLastUpdate(time_t lastUpdate);
		static void setFormat(const std::string & vendor, const std::string & applicationName, const std::string & applicationVersionNumber);
		void setDescriptiveKeywords(const std::string & descriptiveKeywords);
		void setVersion(const std::string & version);
				
		std::string getXmlTag() const;
		virtual std::string getXmlNamespace() const = 0;
		std::string getXmlNamespaceVersion() const;
		
		void addAlias(const std::string & authority, const std::string & title);
		unsigned int getAliasCount() const;
		std::string getAliasAuthorityAtIndex(unsigned int index) const;
		std::string getAliasTitleAtIndex(unsigned int index) const;
		
		/**
		 * Pushes back an extra metadata (not a standard one) onto this instance
		 *
		 * @exception	std::logic_error	If the underlying gSOAP instance is not a RESQML2.0, EML2.1 or EML2.2 one.
		 *
		 * @param 	key  	The key of the metadata.
		 * @param 	value	The value of the metadata.
		 */
		void pushBackExtraMetadata(const std::string & key, const std::string & value);

		/**
		 * Gets an extra metadata of this instance according to its key
		 *
		 * @exception	std::logic_error	If the underlying gSOAP instance is not a RESQML2.0 one.
		 *
		 * @param 	key	The key of an extra metadata.
		 *
		 * @returns	The vector of all metadata values sharing this @p key (empty vector if there is no such value).
		 */
		std::vector<std::string> getExtraMetadata(const std::string & key) const;

		/**
		 * Get the count of extra metadata in this instance
		 *
		 * @exception	std::logic_error	If the underlying gSOAP instance is not a RESQML2.0 or EML2.2 one.
		 *
		 * @returns	The extra metadata count of this instance.
		 */
		unsigned int getExtraMetadataCount() const;

		/**
		 * Get the key of a key value pair at a particular index in the extra metadata set of this
		 * instance
		 *
		 * @exception	std::logic_error	If the underlying gSOAP instance is not a RESQML2.0 or EML2.2 one.
		 *
		 * @param 	index	Zero-based index of the asked key.
		 *
		 * @returns	The extra metadata key at @p index.
		 */
		std::string getExtraMetadataKeyAtIndex(unsigned int index) const;

		/**
		 * Get the value of a key value pair at a particular index in the extra metadata set of this
		 * instance
		 *
		 * @exception	std::logic_error	If the underlying gSOAP instance is not a RESQML2.0 or EML2.2 one.
		 *
		 * @param 	index	Zero-based index of the asked value.
		 *
		 * @returns	The extra metadata value at @p index.
		 */
		std::string getExtraMetadataStringValueAtIndex(unsigned int index) const;

		/**
		* Build and return an ETP1.2 URI from an Energistics dataobject.
		* @return	The ETP1.2 URI built from the Energistics dataobject
		*/
		std::string buildEtp12Uri() const;

		/**
		* Set the EPC document absolute path or the ETP dataspace URI where this dataobject comes from.
		*/
		void setUriSource(const std::string & uriSource);

		/**
		* Get the EPC document absolute path or the ETP dataspace URI where this dataobject comes from.
		*/
		const std::string& getUriSource() const;

		unsigned int getActivityCount() const;

		EML2_NS::Activity * getActivity (unsigned int index) const;		
	};

%warnfilter(473) HdfProxyFactory::make;
	class HdfProxyFactory
	{
	public:
		HdfProxyFactory() {}
		virtual ~HdfProxyFactory();
		
		virtual EML2_NS::AbstractHdfProxy* make(const DataObjectReference& dor);
		
		/*
		The following method leads to a warning (473: Returning a pointer or reference
		in a director method is not recommended) when it is processed by SWIG since
		there is no guarantee as to the lifetime of the object returned 
		in its target language redefinition. We ensure that the C++ part of all
		COMMON_NS::AbstractObject created from the target language side is never garbage 
		collected thanks to upper %typemap(csbody) COMMON_NS::AbstractObject definition.
		Then it is always safe to make use of these objects in fesapi methods.
		*/
		virtual EML2_NS::AbstractHdfProxy* make(COMMON_NS::DataObjectRepository * repo, const std::string & guid, const std::string & title,
			const std::string & packageDirAbsolutePath, const std::string & externalFilePath,
			COMMON_NS::DataObjectRepository::openingMode hdfPermissionAccess = COMMON_NS::DataObjectRepository::openingMode::READ_ONLY);
	};
}

#define CHECKER_PRESENCE_ATTRIBUTE_IN_VECTOR(gsoapClassName, proxyVariable, vectorName, attributeName) bool has##vectorName##attributeName(unsigned int index) const;

#define GETTER_SETTER_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoapClassName, proxyVariable, vectorName, attributeName, attributeDatatype)\
	void set##vectorName##attributeName(unsigned int index, const attributeDatatype& value);\
	attributeDatatype get##vectorName##attributeName(unsigned int index) const;\
	CHECKER_PRESENCE_ATTRIBUTE_IN_VECTOR(gsoapClassName, proxyVariable, vectorName, attributeName)
	
#define GETTER_SETTER_MEASURE_OPTIONAL_ATTRIBUTE_IN_VECTOR(gsoapClassName, proxyVariable, vectorName, attributeName, uomDatatype)\
	void set##vectorName##attributeName(unsigned int index, double value, uomDatatype uom);\
	double get##vectorName##attributeName##Value(unsigned int index) const;\
	uomDatatype get##vectorName##attributeName##Uom(unsigned int index) const;\
	CHECKER_PRESENCE_ATTRIBUTE_IN_VECTOR(gsoapClassName, proxyVariable, vectorName, attributeName)
	
%define SWIG_GETTER_DATAOBJECTS(returnedDataType, dataObjectName)
	unsigned int get ## dataObjectName ## Count() const;
	returnedDataType * get ## dataObjectName(unsigned int index) const;
%enddef

%include "swigEml2Include.i"
%include "swigResqml2Include.i"
%include "swigWitsml2Include.i"
%include "swigProdml2_2Include.i"
#ifdef WITH_ETP
%include "swigEtp1_2Include.i"
#endif

%{
#include "../src/common/EnumStringMapper.h"
#include "../src/common/EpcDocument.h"
#include "../src/common/HdfProxyFactory.h"
%}

namespace COMMON_NS
{
%typemap(javaimports) COMMON_NS::DataObjectRepository %{
import java.lang.AutoCloseable;
import com.f2i_consulting.fesapi.*;
%}
%typemap(javainterfaces) COMMON_NS::DataObjectRepository "AutoCloseable"
%typemap(javacode) COMMON_NS::DataObjectRepository %{
  @Override
  public void close() {
	clear();
    delete();
  }
%}
	class DataObjectRepository
	{
	public:

		enum class openingMode : std::int8_t {
			READ_ONLY = 0,
			READ_WRITE = 1,
			READ_WRITE_DO_NOT_CREATE = 2,
			OVERWRITE = 3
		};

		enum class EnergisticsStandard : std::int8_t {
			RESQML2_0_1 = 0,
			EML2_0 = 1,
			EML2_3 = 2,
			PRODML2_2 = 3,
			RESQML2_2 = 4,
			WITSML2_1 = 5
		};
		
		DataObjectRepository();
		DataObjectRepository(const std::string & propertyKindMappingFilesDirectory);

		void setDefaultStandard(EnergisticsStandard version);
		EnergisticsStandard getDefaultEmlVersion() const;
		EnergisticsStandard getDefaultProdmlVersion() const;
		EnergisticsStandard getDefaultResqmlVersion() const;
		EnergisticsStandard getDefaultWitsmlVersion() const;

		void clear();

		RESQML2_NS::AbstractLocal3dCrs* getDefaultCrs() const;
		void setDefaultCrs(RESQML2_NS::AbstractLocal3dCrs* crs);
		
		EML2_NS::AbstractHdfProxy* getDefaultHdfProxy() const;
		void setDefaultHdfProxy(EML2_NS::AbstractHdfProxy* hdfProxy);
		
		void setHdfProxyFactory(COMMON_NS::HdfProxyFactory * factory);
		
		/**
		 * Adds or replaces (based on Energistics XML definition) a data object in the repository. It
		 * does not update the relationships of the added or replaced data object
		 *
		 * @exception	std::invalid_argument	If, during a replacement, the content type of the data
		 * 										object has changed.
		 *
		 * @param 	xml		   	The XML which is the serialization of the Energistics data object to add
		 * 						or to replace.
		 * @param 	contentOrDataType	The content or qualified data type of the Energistics dataobject to add or to replace.
		 * @param	uriSource			The EPC document absolute path or the ETP dataspace URI where this dataobject comes from
		 *
		 * @returns	Null if the content type of the data object cannot be wrapped by fesapi, else a
		 * 			pointer the added or replaced data object.
		 */
		COMMON_NS::AbstractObject* addOrReplaceGsoapProxy(const std::string & xml, const std::string & contentType, const std::string& uriSource);
		
		SWIG_GETTER_DATAOBJECTS(EML2_NS::TimeSeries, TimeSeries)
		SWIG_GETTER_DATAOBJECTS(EML2_NS::AbstractHdfProxy, HdfProxy)

		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::AbstractSeismicLineFeature, SeismicLine)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::AbstractIjkGridRepresentation, IjkGridRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::BlockedWellboreRepresentation, BlockedWellboreRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::BoundaryFeature, Fault)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::BoundaryFeature, Fracture)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::BoundaryFeature, GeobodyBoundary)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::BoundaryFeature, Horizon)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::CmpLineFeature, CmpLine)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::CulturalFeature, Cultural)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::DeviationSurveyRepresentation, DeviationSurveyRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::DoubleTableLookup, DoubleTableLookup)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::Grid2dRepresentation, AllGrid2dRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::Grid2dRepresentation, HorizonGrid2dRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::IjkGridParametricRepresentation, IjkGridParametricRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::IjkGridExplicitRepresentation, IjkGridExplicitRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::IjkGridLatticeRepresentation, IjkSeismicCubeGridRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::LocalDepth3dCrs, LocalDepth3dCrs)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::LocalTime3dCrs, LocalTime3dCrs)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::Model, Model)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PointSetRepresentation, PointSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineRepresentation, AllPolylineRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineRepresentation, HorizonPolylineRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineRepresentation, SeismicLinePolylineRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineSetRepresentation, AllPolylineSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineSetRepresentation, HorizonPolylineSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineSetRepresentation, FaultPolylineSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineSetRepresentation, FracturePolylineSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::PolylineSetRepresentation, CulturalPolylineSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::RepresentationSetRepresentation, RepresentationSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::RockVolumeFeature, RockVolume)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::RockVolumeFeature, Geobody)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::SeismicLatticeFeature, SeismicLattice)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::SeismicLineSetFeature, SeismicLineSet)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::ShotPointLineFeature, ShotPointLine)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::StratigraphicColumn, StratigraphicColumn)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::StreamlinesFeature, StreamlinesFeature)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::StreamlinesRepresentation, StreamlinesRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::SubRepresentation, SubRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::TriangulatedSetRepresentation, AllTriangulatedSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::TriangulatedSetRepresentation, UnclassifiedTriangulatedSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::TriangulatedSetRepresentation, FaultTriangulatedSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::TriangulatedSetRepresentation, FractureTriangulatedSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::TriangulatedSetRepresentation, HorizonTriangulatedSetRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::UnstructuredGridRepresentation, UnstructuredGridRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::WellboreFeature, Wellbore)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::WellboreTrajectoryRepresentation, WellboreTrajectoryRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_NS::WellboreFrameRepresentation, WellboreFrameRepresentation)
		SWIG_GETTER_DATAOBJECTS(RESQML2_0_1_NS::PropertySet, PropertySet)

		SWIG_GETTER_DATAOBJECTS(WITSML2_NS::Well, WitsmlWell)
		SWIG_GETTER_DATAOBJECTS(WITSML2_NS::Wellbore, WitsmlWellbore)
		SWIG_GETTER_DATAOBJECTS(WITSML2_NS::Trajectory, WitsmlTrajectory)
				
		SWIG_GETTER_DATAOBJECTS(WITSML2_1_NS::WellCompletion, WellCompletion)
		SWIG_GETTER_DATAOBJECTS(WITSML2_1_NS::WellboreCompletion, WellboreCompletion)
		SWIG_GETTER_DATAOBJECTS(WITSML2_1_NS::WellboreGeometry, WellboreGeometry)
		SWIG_GETTER_DATAOBJECTS(WITSML2_1_NS::Log, Log)
		SWIG_GETTER_DATAOBJECTS(WITSML2_1_NS::ChannelSet, ChannelSet)
		SWIG_GETTER_DATAOBJECTS(WITSML2_1_NS::Channel, Channel)
				
		SWIG_GETTER_DATAOBJECTS(PRODML2_2_NS::FluidSystem, FluidSystem)
		SWIG_GETTER_DATAOBJECTS(PRODML2_2_NS::FluidCharacterization, FluidCharacterization)

		AbstractObject* getDataObjectByUuid(const std::string & uuid) const;
		AbstractObject* getDataObjectByUuidAndVersion(const std::string & uuid, const std::string & version) const;
		
		template <class valueType>
		std::vector<valueType*> getDataObjects() const
		{
			std::vector<valueType*> result;

			for (std::unordered_map< std::string, std::vector<AbstractObject*> >::const_iterator it = dataObjects.begin(); it != dataObjects.end(); ++it) {
				for (size_t i = 0; i < it->second.size(); ++i) {
					if (dynamic_cast<valueType*>(it->second[i]) != nullptr) {
						result.push_back(static_cast<valueType*>(it->second[i]));
					}
				}
			}

			return result;
		}
		
#ifdef WITH_RESQML2_2
		%template(getGraphicalInformationSets) getDataObjects<EML2_3_NS::GraphicalInformationSet>;
#endif

		std::vector<std::string> getUuids() const;
		
		/**
		 * @brief	Creates a non parallel access to an HDF5 file for writing to it. Resulting HDF5 file
		 * 			proxy is stored into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid				  	The guid to set to the HDF5 file proxy. If empty then a new
		 * 									guid will be generated.
		 * @param 	title				  	The title to set to the HDF5 file proxy. If empty then
		 * 									\"unknown\" title will be set.
		 * @param 	packageDirAbsolutePath	Path of the directory where the HDF5 file must be created.
		 * @param 	filePath	  			Path of the HDF5 file relative to packageDirAbsolutePath.
		 * @param 	hdfPermissionAccess   	The HDF5 file permission access. It is read
		 * 										only by default.
		 *
		 * @returns	A pointer to an instantiated HDF5 file proxy.
		 */
		EML2_NS::AbstractHdfProxy* createHdfProxy(const std::string & guid, const std::string & title, const std::string & packageDirAbsolutePath, const std::string & filePath, DataObjectRepository::openingMode hdfPermissionAccess);

		//************ CRS *******************/

		RESQML2_NS::LocalDepth3dCrs* createLocalDepth3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, uint64_t projectedEpsgCode,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, unsigned int verticalEpsgCode, bool isUpOriented);


		RESQML2_NS::LocalDepth3dCrs* createLocalDepth3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, const std::string & projectedUnknownReason,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, const std::string & verticalUnknownReason, bool isUpOriented);


		RESQML2_NS::LocalDepth3dCrs* createLocalDepth3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, uint64_t projectedEpsgCode,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, const std::string & verticalUnknownReason, const bool & isUpOriented);


		RESQML2_NS::LocalDepth3dCrs* createLocalDepth3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, const std::string & projectedUnknownReason,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, unsigned int verticalEpsgCode, const bool & isUpOriented);


		RESQML2_NS::LocalTime3dCrs* createLocalTime3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, uint64_t projectedEpsgCode,
			gsoap_resqml2_0_1::eml20__TimeUom timeUom,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, unsigned int verticalEpsgCode, const bool & isUpOriented);


		RESQML2_NS::LocalTime3dCrs* createLocalTime3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, const std::string & projectedUnknownReason,
			gsoap_resqml2_0_1::eml20__TimeUom timeUom,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, const std::string & verticalUnknownReason, const bool & isUpOriented);


		RESQML2_NS::LocalTime3dCrs* createLocalTime3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, uint64_t projectedEpsgCode,
			gsoap_resqml2_0_1::eml20__TimeUom timeUom,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, const std::string & verticalUnknownReason, const bool & isUpOriented);


		RESQML2_NS::LocalTime3dCrs* createLocalTime3dCrs(const std::string & guid, const std::string & title,
			double originOrdinal1, double originOrdinal2, double originOrdinal3,
			double arealRotation,
			gsoap_resqml2_0_1::eml20__LengthUom projectedUom, const std::string & projectedUnknownReason,
			gsoap_resqml2_0_1::eml20__TimeUom timeUom,
			gsoap_resqml2_0_1::eml20__LengthUom verticalUom, unsigned int verticalEpsgCode, const bool & isUpOriented);

		RESQML2_NS::MdDatum* createMdDatum(const std::string & guid, const std::string & title,
			RESQML2_NS::AbstractLocal3dCrs * locCrs, gsoap_eml2_3::eml23__ReferencePointKind originKind,
			double referenceLocationOrdinal1, double referenceLocationOrdinal2, double referenceLocationOrdinal3);

		/* FEATURE */

		RESQML2_NS::BoundaryFeature* createBoundaryFeature(const std::string & guid, const std::string & title);

		RESQML2_NS::BoundaryFeature* createHorizon(const std::string & guid, const std::string & title);

		RESQML2_NS::BoundaryFeature* createGeobodyBoundaryFeature(const std::string & guid, const std::string & title);

		RESQML2_NS::RockVolumeFeature* createGeobodyFeature(const std::string & guid, const std::string & title);

		RESQML2_NS::BoundaryFeature* createFault(const std::string & guid, const std::string & title);

		RESQML2_NS::BoundaryFeature* createFracture(const std::string & guid, const std::string & title);

		RESQML2_NS::WellboreFeature* createWellboreFeature(const std::string & guid, const std::string & title);

		RESQML2_NS::SeismicLatticeFeature* createSeismicLattice(const std::string & guid, const std::string & title,
			int inlineIncrement, int crosslineIncrement,
			unsigned int originInline, unsigned int originCrossline,
			unsigned int inlineCount, unsigned int crosslineCount);

		RESQML2_0_1_NS::SeismicLineFeature* createSeismicLine(const std::string & guid, const std::string & title,
			int traceIndexIncrement, unsigned int firstTraceIndex, unsigned int traceCount);
#ifdef WITH_RESQML2_2	
		RESQML2_NS::CmpLineFeature* createCmpLine(const std::string & guid, const std::string & title,
			int nearestShotPointIndicesIncrement, int firstNearestShotPointIndex, unsigned int nearestShotPointCount);
			
		RESQML2_NS::ShotPointLineFeature* createShotPointLine(const std::string & guid, const std::string & title);
#endif
		RESQML2_NS::SeismicLineSetFeature* createSeismicLineSet(const std::string & guid, const std::string & title);

		RESQML2_NS::CulturalFeature* createCultural(const std::string & guid, const std::string & title,
			gsoap_eml2_3::resqml22__CulturalFeatureKind kind = gsoap_eml2_3::resqml22__CulturalFeatureKind__project_x0020boundaries);

		RESQML2_NS::RockVolumeFeature* createStratigraphicUnitFeature(const std::string & guid, const std::string & title);
#ifdef WITH_RESQML2_2
		RESQML2_NS::RockVolumeFeature* createRockVolumeFeature(const std::string & guid, const std::string & title);
		
		RESQML2_NS::Model* createModel(const std::string & guid, const std::string & title);
#endif
		RESQML2_NS::Model* createStructuralModel(const std::string & guid, const std::string & title);

		RESQML2_NS::Model* createStratigraphicModel(const std::string & guid, const std::string & title);

		RESQML2_NS::Model* createRockFluidModel(const std::string & guid, const std::string & title);

		RESQML2_NS::Model* createEarthModel(const std::string & guid, const std::string & title);

		RESQML2_0_1_NS::FluidBoundaryFeature* createFluidBoundaryFeature(const std::string & guid, const std::string & title, gsoap_resqml2_0_1::resqml20__FluidContact fluidContact);

		RESQML2_0_1_NS::RockFluidUnitFeature* createRockFluidUnit(const std::string & guid, const std::string & title, gsoap_resqml2_0_1::resqml20__Phase phase, RESQML2_0_1_NS::FluidBoundaryFeature* fluidBoundaryTop, RESQML2_0_1_NS::FluidBoundaryFeature* fluidBoundaryBottom);

		/* INTERPRETATION */

		RESQML2_NS::GenericFeatureInterpretation* createGenericFeatureInterpretation(RESQML2_NS::AbstractFeature * feature, const std::string & guid, const std::string & title);

		RESQML2_NS::BoundaryFeatureInterpretation* createBoundaryFeatureInterpretation(RESQML2_NS::BoundaryFeature * feature, const std::string & guid, const std::string & title);

		RESQML2_NS::HorizonInterpretation* createHorizonInterpretation(RESQML2_NS::BoundaryFeature * horizon, const std::string & guid, const std::string & title);

		RESQML2_NS::GeobodyBoundaryInterpretation* createGeobodyBoundaryInterpretation(RESQML2_NS::BoundaryFeature * geobodyBoundary, const std::string & guid, const std::string & title);

		RESQML2_NS::FaultInterpretation* createFaultInterpretation(RESQML2_NS::BoundaryFeature * fault, const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates a fluid Boundary interpretation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>fault == nullptr</tt>.
		 *
		 * @param [in]	boundary		The interpreted boundary. It cannot be null.
		 * @param 	  	guid 			The guid to set to the interpretation. If empty then a new guid
		 * 								will be generated.
		 * @param 	  	title			The title to set to the interpretation. If empty then \"unknown\"
		 * 								title will be set.
		 * @param 	  	fluidContact	The fluid contact this boundary is.
		 *
		 * @returns	A pointer to the new interpretation.
		 */
		RESQML2_NS::FluidBoundaryInterpretation* createFluidBoundaryInterpretation(RESQML2_NS::BoundaryFeature * boundary, const std::string & guid, const std::string & title, gsoap_eml2_3::resqml22__FluidContact fluidContact);

		RESQML2_NS::WellboreInterpretation* createWellboreInterpretation(RESQML2_NS::WellboreFeature * wellbore, const std::string & guid, const std::string & title, bool isDrilled);

		RESQML2_NS::EarthModelInterpretation* createEarthModelInterpretation(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);

		RESQML2_NS::StructuralOrganizationInterpretation* createStructuralOrganizationInterpretationInAge(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);
		RESQML2_NS::StructuralOrganizationInterpretation* createStructuralOrganizationInterpretationInApparentDepth(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);
		RESQML2_NS::StructuralOrganizationInterpretation* createStructuralOrganizationInterpretationInMeasuredDepth(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);

		RESQML2_NS::RockFluidOrganizationInterpretation* createRockFluidOrganizationInterpretation(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);
		RESQML2_NS::RockFluidUnitInterpretation* createRockFluidUnitInterpretation(RESQML2_NS::RockVolumeFeature * rockFluidUnitFeature, const std::string & guid, const std::string & title);

		RESQML2_NS::GeobodyInterpretation* createGeobodyInterpretation(RESQML2_NS::RockVolumeFeature * geobody, const std::string & guid, const std::string & title);

		RESQML2_NS::StratigraphicUnitInterpretation* createStratigraphicUnitInterpretation(RESQML2_NS::RockVolumeFeature * stratiUnitFeature, const std::string & guid, const std::string & title);
		RESQML2_NS::StratigraphicColumn* createStratigraphicColumn(const std::string & guid, const std::string & title);
		RESQML2_NS::StratigraphicColumnRankInterpretation* createStratigraphicColumnRankInterpretationInAge(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title, uint64_t rank);
		RESQML2_NS::StratigraphicColumnRankInterpretation* createStratigraphicColumnRankInterpretationInApparentDepth(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title, uint64_t rank);
		RESQML2_NS::StratigraphicOccurrenceInterpretation* createStratigraphicOccurrenceInterpretationInAge(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);
		RESQML2_NS::StratigraphicOccurrenceInterpretation* createStratigraphicOccurrenceInterpretationInApparentDepth(RESQML2_NS::Model * orgFeat, const std::string & guid, const std::string & title);

		/* REPRESENTATION */

		RESQML2_NS::TriangulatedSetRepresentation* createTriangulatedSetRepresentation(const std::string & guid, const std::string & title);
			
		RESQML2_NS::TriangulatedSetRepresentation* createTriangulatedSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		RESQML2_NS::PolylineSetRepresentation* createPolylineSetRepresentation(const std::string & guid, const std::string & title);

		RESQML2_NS::PolylineSetRepresentation* createPolylineSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		RESQML2_NS::PolylineSetRepresentation* createPolylineSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title, gsoap_eml2_3::resqml22__LineRole roleKind);

		RESQML2_NS::PointSetRepresentation* createPointSetRepresentation(const std::string & guid, const std::string & title);

		RESQML2_NS::PointSetRepresentation* createPointSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		RESQML2_NS::PlaneSetRepresentation* createPlaneSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		RESQML2_NS::PolylineRepresentation* createPolylineRepresentation(const std::string & guid, const std::string & title, bool isClosed = false);

		RESQML2_NS::PolylineRepresentation* createPolylineRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title, bool isClosed = false);

		RESQML2_NS::PolylineRepresentation* createPolylineRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title, gsoap_eml2_3::resqml22__LineRole roleKind, bool isClosed = false);

		RESQML2_NS::Grid2dRepresentation* createGrid2dRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		RESQML2_NS::WellboreTrajectoryRepresentation* createWellboreTrajectoryRepresentation(RESQML2_NS::WellboreInterpretation * interp, const std::string & guid, const std::string & title, RESQML2_NS::MdDatum * mdInfo);
		RESQML2_NS::WellboreTrajectoryRepresentation* createWellboreTrajectoryRepresentation(RESQML2_NS::WellboreInterpretation * interp, const std::string & guid, const std::string & title, RESQML2_NS::DeviationSurveyRepresentation * deviationSurvey);

		RESQML2_NS::DeviationSurveyRepresentation* createDeviationSurveyRepresentation(RESQML2_NS::WellboreInterpretation * interp, const std::string & guid, const std::string & title, const bool & isFinal, RESQML2_NS::MdDatum * mdInfo);

		RESQML2_NS::WellboreFrameRepresentation* createWellboreFrameRepresentation(RESQML2_NS::WellboreInterpretation* interp, const std::string& guid, const std::string& title, RESQML2_NS::WellboreTrajectoryRepresentation* traj);
#ifdef WITH_RESQML2_2
		RESQML2_NS::SeismicWellboreFrameRepresentation* createSeismicWellboreFrameRepresentation(
			RESQML2_NS::WellboreInterpretation* interp, 
			const std::string& guid, const std::string& title, 
			RESQML2_NS::WellboreTrajectoryRepresentation* traj,
			double seismicReferenceDatum,
			double weatheringVelocity,
			RESQML2_NS::LocalTime3dCrs* crs);
#endif
		RESQML2_NS::WellboreMarkerFrameRepresentation* createWellboreMarkerFrameRepresentation(RESQML2_NS::WellboreInterpretation * interp, const std::string & guid, const std::string & title, RESQML2_NS::WellboreTrajectoryRepresentation * traj);

		RESQML2_NS::WellboreMarker* createWellboreMarker(RESQML2_NS::WellboreMarkerFrameRepresentation* wellboreMarkerFrame, const std::string& guid, const std::string& title);

		RESQML2_NS::WellboreMarker* createWellboreMarker(RESQML2_NS::WellboreMarkerFrameRepresentation* wellboreMarkerFrame, const std::string& guid, const std::string& title, gsoap_resqml2_0_1::resqml20__GeologicBoundaryKind geologicBoundaryKind);

		RESQML2_NS::BlockedWellboreRepresentation* createBlockedWellboreRepresentation(RESQML2_NS::WellboreInterpretation * interp,
			const std::string & guid, const std::string & title, RESQML2_NS::WellboreTrajectoryRepresentation * traj);

		RESQML2_NS::RepresentationSetRepresentation* createRepresentationSetRepresentation(
			RESQML2_NS::AbstractOrganizationInterpretation* interp,
			const std::string & guid,
			const std::string & title);

		RESQML2_NS::RepresentationSetRepresentation* createRepresentationSetRepresentation(
			const std::string & guid,
			const std::string & title);

		RESQML2_NS::NonSealedSurfaceFrameworkRepresentation* createNonSealedSurfaceFrameworkRepresentation(
			RESQML2_NS::StructuralOrganizationInterpretation* interp,
			const std::string & guid,
			const std::string & title);

		RESQML2_NS::SealedSurfaceFrameworkRepresentation* createSealedSurfaceFrameworkRepresentation(
			RESQML2_NS::StructuralOrganizationInterpretation* interp,
			const std::string & guid,
			const std::string & title);

		RESQML2_NS::SealedVolumeFrameworkRepresentation* createSealedVolumeFrameworkRepresentation(
			RESQML2_NS::StratigraphicColumnRankInterpretation* interp,
			const std::string & guid,
			const std::string & title,
			RESQML2_NS::SealedSurfaceFrameworkRepresentation* ssf);

		/**
		 * Creates a partial ijk grid representation into this repository
		 *
		 * @param 	guid 	The guid to set to the sealed volume framework representation. If empty
		 * 					then a new guid will be generated.
		 * @param 	title	The title to set to the sealed volume framework representation.
		 *
		 * @returns	A pointer to the new partial ijk grid representation.
		 */
		RESQML2_NS::AbstractIjkGridRepresentation* createPartialIjkGridRepresentation(const std::string & guid, const std::string & title);

		/**
		 * Creates a partial truncated ijk grid representation into this repository
		 *
		 * @param 	guid 	The guid to set to the sealed volume framework representation.
		 * @param 	title	The title to set to the sealed volume framework representation.
		 *
		 * @returns	A pointer to the new partial truncated ijk grid representation.
		 */
		RESQML2_NS::AbstractIjkGridRepresentation* createPartialTruncatedIjkGridRepresentation(const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates an ijk grid explicit representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param		guid  	The guid to set to the ijk grid explicit representation. If empty then a new
		 * 						guid will be generated.
		 * @param		title 	The title to set to the ijk grid explicit representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param		iCount	Count of cells in the I direction in the grid.
		 * @param 		jCount	Count of cells in the J direction in the grid.
		 * @param		kCount	Number of layers in the grid.
		 * @param		kGaps	(Optional) Boolean array of length KCellCount-1.
		 *						TRUE if there is a gap after the corresponding layer.
		 *						Won't be freed by FESAPI.
		 * @param [in]	proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 						values. If @c nullptr (default), then the default HDF proxy will be
		 * 						used.
		 *
		 * @returns	A pointer to the new ijk grid explicit representation.
		 */
		RESQML2_NS::IjkGridExplicitRepresentation* createIjkGridExplicitRepresentation(const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr);

		/**
		 * @brief	Creates an ijk grid explicit representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>interp == nullptr</tt>.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null.
		 * @param 	  	guid  	The guid to set to the ijk grid explicit representation. If empty then a
		 * 						new guid will be generated.
		 * @param 	  	title 	The title to set to the ijk grid explicit representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param 	  	iCount	Count of cells in the I direction in the grid.
		 * @param 	  	jCount	Count of cells in the J direction in the grid.
		 * @param 	  	kCount	Number of layers in the grid.
		 * @param		kGaps	(Optional) Boolean array of length KCellCount-1.
		 *						TRUE if there is a gap after the corresponding layer.
		 *						Won't be freed by FESAPI.
		 * @param [in]	proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 						values. If @c nullptr (default), then the default HDF proxy will be
		 * 						used.
		 *
		 * @returns	A pointer to the new ijk grid explicit representation.
		 */
		RESQML2_NS::IjkGridExplicitRepresentation* createIjkGridExplicitRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr);

		/**
		 * @brief	Creates an ijk grid parametric representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param		guid  	The guid to set to the ijk grid parametric representation. If empty then a
		 * 						new guid will be generated.
		 * @param		title 	The title to set to the ijk grid parametric representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param		iCount	Count of cells in the I direction in the grid.
		 * @param		jCount	Count of cells in the J direction in the grid.
		 * @param		kCount	Number of layers in the grid.
		 * @param		kGaps	(Optional) Boolean array of length KCellCount-1.
		 *						TRUE if there is a gap after the corresponding layer.
		 *						Won't be freed by FESAPI.
		 * @param [in]	proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 						values. If @c nullptr (default), then the default HDF proxy will be
		 * 						used.
		 *
		 * @returns	A pointer to the new ijk grid parametric representation.
		 */
		RESQML2_NS::IjkGridParametricRepresentation* createIjkGridParametricRepresentation(const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr);

		/**
		 * @brief	Creates an ijk grid parametric representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>interp == nullptr</tt>.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null.
		 * @param 	  	guid  	The guid to set to the ijk grid parametric representation. If empty then
		 * 						a new guid will be generated.
		 * @param 	  	title 	The title to set to the ijk grid parametric representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param 	  	iCount	Count of cells in the I direction in the grid.
		 * @param 	  	jCount	Count of cells in the J direction in the grid.
		 * @param 	  	kCount	Number of layers in the grid.
		 * @param		kGaps	(Optional) Boolean array of length KCellCount-1.
		 *						TRUE if there is a gap after the corresponding layer.
		 *						Won't be freed by FESAPI.
		 * @param [in]	proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 						values. If @c nullptr (default), then the default HDF proxy will be
		 * 						used.
		 *
		 * @returns	A pointer to the new ijk grid parametric representation.
		 */
		RESQML2_NS::IjkGridParametricRepresentation* createIjkGridParametricRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr);

		/**
		 * @brief	Creates an ijk grid lattice representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid  	The guid to set to the ijk grid lattice representation. If empty then a new
		 * 					guid will be generated.
		 * @param 	title 	The title to set to the ijk grid lattice representation. If empty then
		 * 					\"unknown\" title will be set.
		 * @param 	iCount	Count of cells in the I direction in the grid.
		 * @param 	jCount	Count of cells in the J direction in the grid.
		 * @param 	kCount	Number of layers in the grid.
		 *
		 * @returns	A pointer to the new ijk grid lattice representation.
		 */
		RESQML2_NS::IjkGridLatticeRepresentation* createIjkGridLatticeRepresentation(const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount);

		/**
		 * @brief	Creates an ijk grid lattice representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>interp == nullptr</tt>.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null.
		 * @param 	  	guid  	The guid to set to the ijk grid lattice representation. If empty then a
		 * 						new guid will be generated.
		 * @param 	  	title 	The title to set to the ijk grid lattice representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param 	  	iCount	Count of cells in the I direction in the grid.
		 * @param 	  	jCount	Count of cells in the J direction in the grid.
		 * @param 	  	kCount	Number of layers in the grid.
		 *
		 * @returns	A pointer to the new ijk grid lattice representation.
		 */
		RESQML2_NS::IjkGridLatticeRepresentation* createIjkGridLatticeRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount);

		/**
		 * @brief	Creates an ijk grid with no geometry representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param		guid  	The guid to set to the ijk grid with no geometry representation. If empty
		 * 						then a new guid will be generated.
		 * @param		title 	The title to set to the ijk grid with no geometry representation. If empty
		 * 						then \"unknown\" title will be set.
		 * @param		iCount	Count of cells in the I direction in the grid.
		 * @param		jCount	Count of cells in the J direction in the grid.
		 * @param		kCount	Number of layers in the grid.
		 * @param		kGaps	(Optional) Boolean array of length KCellCount-1.
		 *						TRUE if there is a gap after the corresponding layer.
		 *						Won't be freed by FESAPI.
		 * @param [in]	proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 						values. If @c nullptr (default), then the default HDF proxy will be
		 * 						used.
		 *
		 * @returns	A pointer to the new ijk grid with no geometry representation.
		 */
		RESQML2_NS::IjkGridNoGeometryRepresentation* createIjkGridNoGeometryRepresentation(
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr);

		/**
		 * @brief	Creates an ijk grid with no geometry representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>interp == nullptr</tt>.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null.
		 * @param 	  	guid  	The guid to set to the ijk grid with no geometry representation. If empty
		 * 						then a new guid will be generated.
		 * @param 	  	title 	The title to set to the ijk grid with no geometry representation. If
		 * 						empty then \"unknown\" title will be set.
		 * @param 	  	iCount	Count of cells in the I direction in the grid.
		 * @param 	  	jCount	Count of cells in the J direction in the grid.
		 * @param 	  	kCount	Number of layers in the grid.
		 * @param		kGaps	(Optional) Boolean array of length KCellCount-1.
		 *						TRUE if there is a gap after the corresponding layer.
		 *						Won't be freed by FESAPI.
		 * @param [in]	proxy	(Optional) The HDF proxy for writing the @p enabledCells
		 * 						values. If @c nullptr (default), then the default HDF proxy will be
		 * 						used.
		 *
		 * @returns	A pointer to the new ijk grid with no geometry representation.
		 */
		RESQML2_NS::IjkGridNoGeometryRepresentation* createIjkGridNoGeometryRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title,
			unsigned int iCount, unsigned int jCount, unsigned int kCount, bool* kGaps = nullptr, EML2_NS::AbstractHdfProxy* proxy = nullptr);

		/**
		 * @brief	Creates an unstructured grid representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid	 	The guid to set to the unstructured grid representation. If empty then a
		 * 						new guid will be generated.
		 * @param 	title	 	The title to set to the unstructured grid representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param 	cellCount	Number of cells in the grid.
		 *
		 * @returns	A pointer to the new unstructured grid representation.
		 */
		RESQML2_NS::UnstructuredGridRepresentation* createUnstructuredGridRepresentation(const std::string & guid, const std::string & title, uint64_t cellCount);
		
		/**
		 * @brief	Creates an unstructured grid representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null.
		 * @param 	guid	 	The guid to set to the unstructured grid representation. If empty then a
		 * 						new guid will be generated.
		 * @param 	title	 	The title to set to the unstructured grid representation. If empty then
		 * 						\"unknown\" title will be set.
		 * @param 	cellCount	Number of cells in the grid.
		 *
		 * @returns	A pointer to the new unstructured grid representation.
		 */
		RESQML2_NS::UnstructuredGridRepresentation* createUnstructuredGridRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp, const std::string & guid, const std::string & title, uint64_t cellCount);

		/**
		 * @brief	Creates a sub-representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid 	The guid to set to the sub-representation. If empty then a new guid will be
		 * 					generated.
		 * @param 	title	The title to set to the sub-representation. If empty then \"unknown\" title
		 * 					will be set.
		 *
		 * @returns	A pointer to the new sub-representation.
		 */
		RESQML2_NS::SubRepresentation* createSubRepresentation(
			const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates a sub-representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>interp == nullptr</tt>.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null. You can alternatively
		 * 						use {@link createSubRepresentation} if no interpretation is associated
		 * 						to this representation.
		 * @param 	  	guid  	The guid to set to the sub-representation. If empty then a new guid will
		 * 						be generated.
		 * @param 	  	title 	The title to set to the sub-representation. If empty then \"unknown\"
		 * 						title will be set.
		 *
		 * @returns	A pointer to the new sub-representation.
		 */
		RESQML2_NS::SubRepresentation* createSubRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates a grid connection set representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid 	The guid to set to the grid connection set representation. If empty then a
		 * 					new guid will be generated.
		 * @param 	title	The title to set to the grid connection set representation. If empty then
		 * 					\"unknown\" title will be set.
		 *
		 * @returns	A pointer to the new grid connection set representation.
		 */
		RESQML2_NS::GridConnectionSetRepresentation* createGridConnectionSetRepresentation(const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates a grid connection set representation into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If <tt>interp == nullptr</tt>.
		 *
		 * @param [in]	interp	The represented interpretation. It cannot be null. You can alternatively
		 * 						use {@link createGridConnectionSetRepresentation} if no interpretation
		 * 						is associated to this representation.
		 * @param 	  	guid  	The guid to set to the grid connection set representation. If empty then
		 * 						a new guid will be generated.
		 * @param 	  	title 	The title to set to the grid connection set representation. If empty then
		 * 						\"unknown\" title will be set.
		 *
		 * @returns	A pointer to the new grid connection set representation.
		 */
		RESQML2_NS::GridConnectionSetRepresentation* createGridConnectionSetRepresentation(RESQML2_NS::AbstractFeatureInterpretation* interp,
			const std::string & guid, const std::string & title);

		/* PROPERTIES */

		/**
		 * @brief	Creates a time series into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid 	The guid to set to the time series. If empty then a new guid will be
		 * 					generated.
		 * @param 	title	The title to set to the time series. If empty then \"unknown\" title will be
		 * 					set.
		 *
		 * @returns	A pointer to the new time series.
		 */
		EML2_NS::TimeSeries* createTimeSeries(const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates a double table lookup into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid 	The guid to set to the double table lookup. If empty then a new guid will be
		 * 					generated.
		 * @param 	title	The title to set to the double table lookup. If empty then \"unknown\" title
		 * 					will be set.
		 *
		 * @returns	A pointer to the new double table lookup.
		 */
		RESQML2_NS::DoubleTableLookup* createDoubleTableLookup(const std::string & guid, const std::string & title);

		/**
		 * @brief	Creates a string table lookup into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 *
		 * @param 	guid 	The guid to set to the string table lookup. If empty then a new guid will be
		 * 					generated.
		 * @param 	title	The title to set to the string table lookup. If empty then \"unknown\" title
		 * 					will be set.
		 *
		 * @returns	A pointer to the new string table lookup.
		 */
		 RESQML2_NS::StringTableLookup* createStringTableLookup(const std::string & guid, const std::string & title);

		RESQML2_0_1_NS::PropertyKind* createPropertyKind(const std::string & guid, const std::string & title,
			const std::string & namingSystem, gsoap_resqml2_0_1::resqml20__ResqmlUom uom, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind parentEnergisticsPropertyKind);

		RESQML2_0_1_NS::PropertyKind* createPropertyKind(const std::string & guid, const std::string & title,
			const std::string & namingSystem, gsoap_resqml2_0_1::resqml20__ResqmlUom uom, EML2_NS::PropertyKind * parentPropType);

		RESQML2_0_1_NS::PropertyKind* createPropertyKind(const std::string & guid, const std::string & title,
			const std::string & namingSystem, const std::string & nonStandardUom, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind parentEnergisticsPropertyKind);

		RESQML2_0_1_NS::PropertyKind* createPropertyKind(const std::string & guid, const std::string & title,
			const std::string & namingSystem, const std::string & nonStandardUom, EML2_NS::PropertyKind * parentPropType);

		EML2_NS::PropertyKind* createPropertyKind(const std::string & guid, const std::string & title,
			gsoap_eml2_3::eml23__QuantityClassKind quantityClass, bool isAbstract = false, EML2_NS::PropertyKind* parentPropertyKind = nullptr);

		RESQML2_0_1_NS::PropertySet* createPropertySet(const std::string & guid, const std::string & title,
			bool hasMultipleRealizations, bool hasSinglePropertyKind, gsoap_resqml2_0_1::resqml20__TimeSetKind timeSetKind);

		RESQML2_0_1_NS::CommentProperty* createCommentProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind);

		RESQML2_NS::CommentProperty* createCommentProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, EML2_NS::PropertyKind * localPropType);

		RESQML2_0_1_NS::ContinuousProperty* createContinuousProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, gsoap_resqml2_0_1::resqml20__ResqmlUom uom, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind);

		RESQML2_NS::ContinuousProperty* createContinuousProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, gsoap_resqml2_0_1::resqml20__ResqmlUom uom, EML2_NS::PropertyKind * localPropType);

		RESQML2_0_1_NS::ContinuousProperty* createContinuousProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, std::string nonStandardUom, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind);

		RESQML2_NS::ContinuousProperty* createContinuousProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, const std::string & nonStandardUom, EML2_NS::PropertyKind * localPropType);

		RESQML2_0_1_NS::DiscreteProperty* createDiscreteProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind);

		RESQML2_NS::DiscreteProperty* createDiscreteProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, EML2_NS::PropertyKind * localPropType);

		/**
		 * Creates a categorical property (which is of a standard Energistics property kind) into this
		 * repository
		 *
		 * @exception	std::invalid_argument	If @p rep or @p strLookup is null.
		 *
		 * @param [in]	rep					   	The representation on which this property is attached to.
		 * 										It cannot be null.
		 * @param 	  	guid				   	The guid to set to the property. If empty then a new guid
		 * 										will be generated.
		 * @param 	  	title				   	The title to set to the property. If empty then
		 * 										\"unknown\" title will be set.
		 * @param 	  	dimension			   	The dimension of each value of this property. Dimension
		 * 										is 1 for a scalar property.
		 * @param 	  	attachmentKind		   	The topological element on which the property values are
		 * 										attached to.
		 * @param [in]	strLookup			   	The string lookup which defines the possible string
		 * 										values and their keys.
		 * @param 	  	energisticsPropertyKind	The property kind of these property values which must be
		 * 										defined in the standard Energistics property type
		 * 										dictionary.
		 *
		 * @returns A pointer to new categorical property.
		 */
		RESQML2_0_1_NS::CategoricalProperty* createCategoricalProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind,
			RESQML2_NS::StringTableLookup* strLookup, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind);

		/**
		 * Creates a categorical property (which is of a standard Energistics property kind) into this
		 * repository
		 *
		 * @exception	std::invalid_argument	If @p rep or @p strLookup is null.
		 *
		 * @param [in]	rep					   	The representation on which this property is attached to.
		 * 										It cannot be null.
		 * @param 	  	guid				   	The guid to set to the property. If empty then a new guid
		 * 										will be generated.
		 * @param 	  	title				   	The title to set to the property. If empty then
		 * 										\"unknown\" title will be set.
		 * @param 	  	dimension			   	The dimension of each value of this property. Dimension
		 * 										is 1 for a scalar property.
		 * @param 	  	attachmentKind		   	The topological element on which the property values are
		 * 										attached to.
		 * @param [in]	dblLookup			   	The double lookup which defines a discrete function associated with the property values.
		 * @param 	  	energisticsPropertyKind	The property kind of these property values which must be
		 * 										defined in the standard Energistics property type
		 * 										dictionary.
		 *
		 * @returns A pointer to new categorical property.
		 */
		RESQML2_0_1_NS::CategoricalProperty* createCategoricalProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind,
			RESQML2_NS::DoubleTableLookup* dblLookup, gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind);

		/**
		 * Creates a categorical property (which is of a local property kind) into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If @p rep, @p strLookup or @p localPropKind is null.
		 *
		 * @param [in]	rep			  	The representation on which this property is attached to. It
		 * 								cannot be null.
		 * @param 	  	guid		  	The guid to set to the property. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the property. If empty then \"unknown\" title
		 * 								will be set.
		 * @param 	  	dimension	  	The dimension of each value of this property. Dimension is 1 for
		 * 								a scalar property.
		 * @param 	  	attachmentKind	The topological element on which the property values are attached
		 * 								to.
		 * @param [in]	strLookup	  	The string lookup which defines the possible string values and
		 * 								their keys. It cannot be null.
		 * @param [in]	localPropType 	The property kind of these property values which must be defined
		 * 								in the EPC document as a local property kind. It cannot be null.
		 *
		 * @returns	A pointer to the new categorical property.
		 */
		RESQML2_NS::CategoricalProperty* createCategoricalProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind,
			RESQML2_NS::StringTableLookup* strLookup, EML2_NS::PropertyKind * localPropType);

		/**
		 * Creates a categorical property (which is of a local property kind) into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If @p rep, @p strLookup or @p localPropKind is null.
		 *
		 * @param [in]	rep			  	The representation on which this property is attached to. It
		 * 								cannot be null.
		 * @param 	  	guid		  	The guid to set to the property. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the property. If empty then \"unknown\" title
		 * 								will be set.
		 * @param 	  	dimension	  	The dimension of each value of this property. Dimension is 1 for
		 * 								a scalar property.
		 * @param 	  	attachmentKind	The topological element on which the property values are attached
		 * 								to.
		 * @param [in]	dblLookup		The double lookup which defines a discrete function associated with the property values.
		 * @param [in]	localPropType 	The property kind of these property values which must be defined
		 * 								in the EPC document as a local property kind. It cannot be null.
		 *
		 * @returns	A pointer to the new categorical property.
		 */
		RESQML2_NS::CategoricalProperty* createCategoricalProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind,
			RESQML2_NS::DoubleTableLookup* dblLookup, EML2_NS::PropertyKind * localPropType);
			
		/**
		 * Creates a points property (which is of a well known Energistics property kind) into this
		 * repository
		 *
		 * @exception	std::invalid_argument	If @p rep is null.
		 *
		 * @param [in]	rep					   	The representation on which this property is attached to.
		 * 										It cannot be null.
		 * @param 	  	guid				   	The guid to set to the property. If empty then a new guid
		 * 										will be generated.
		 * @param 	  	title				   	The title to set to the property. If empty then
		 * 										\"unknown\" title will be set.
		 * @param 	  	dimension			   	The dimension of each value of this property. Dimension
		 * 										is 1 for a scalar property.
		 * @param 	  	attachmentKind		   	The topological element on which the property values are
		 * 										attached to.
		 * @param 	  	energisticsPropertyKind	The property kind of these property values which must be
		 * 										defined in the standard Energistics property type
		 * 										dictionary. Defautl is length
		 *
		 * @returns	A pointer to the new points property.
		 */
		RESQML2_0_1_NS::PointsProperty* createPointsProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, RESQML2_NS::AbstractLocal3dCrs* localCrs,
			gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind = gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind__length);

		/**
		 * Creates a points property (which is of a local property kind) into this repository
		 *
		 * @exception	std::invalid_argument	If the default RESQML version is unrecognized.
		 * @exception	std::invalid_argument	If @p or @p localPropKind is null.
		 *
		 * @param [in]	rep			  	The representation on which this property is attached to. It
		 * 								cannot be null.
		 * @param 	  	guid		  	The guid to set to the property. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the property. If empty then \"unknown\" title
		 * 								will be set.
		 * @param 	  	dimension	  	The dimension of each value of this property. Dimension is 1 for
		 * 								a scalar property.
		 * @param 	  	attachmentKind	The topological element on which the property values are attached
		 * 								to.
		 * @param [in]	localPropType 	The property kind of these property values which must be defined
		 * 								in the EPC document as a local property kind. It cannot be null.
		 *
		 * @returns	A pointer to the new points property.
		 */
		RESQML2_NS::PointsProperty* createPointsProperty(RESQML2_NS::AbstractRepresentation * rep, const std::string & guid, const std::string & title,
			unsigned int dimension, gsoap_eml2_3::eml23__IndexableElement attachmentKind, RESQML2_NS::AbstractLocal3dCrs* localCrs,
			EML2_NS::PropertyKind * localPropType);

		//************* ACTIVITIES ***********/

		EML2_NS::ActivityTemplate* createActivityTemplate(const std::string & guid, const std::string & title);

		EML2_NS::Activity* createActivity(EML2_NS::ActivityTemplate* activityTemplate, const std::string & guid, const std::string & title);

		//*************** WITSML *************/

		/**
		 * Creates a well into this repository
		 *
		 * @param 	guid 	The guid to set to the well. If empty then a new guid will be generated.
		 * @param 	title	The title to set to the well. If empty then \"unknown\" title will be set.
		 * @param 	isActive	True if is active, false if not.
		 *
		 * @returns	A pointer to the new well.
		 */
		WITSML2_NS::Well* createWell(const std::string & guid,
			const std::string & title, bool isActive);

		/**
		 * Creates a well into this repository
		 *
		 * @param 	guid		 	The guid to set to the well. If empty then a new guid will be
		 * 							generated.
		 * @param 	title		 	The title to set to the well. If empty then \"unknown\" title will be
		 * 							set.
		 * @param 	isActive		True if is active, false if not.
		 * @param 	statusWell   	POSC well status.
		 * @param 	directionWell	POSC well direction. The direction of the flow of the fluids in a
		 * 							well facility (generally, injected or produced, or some combination).
		 *
		 * @returns	A pointer to the new well.
		 */
		WITSML2_NS::Well* createWell(const std::string & guid,
			const std::string & title,
			bool isActive,
			gsoap_eml2_3::eml23__WellStatus statusWell,
			gsoap_eml2_3::witsml21__WellDirection directionWell
		);

		/**
		 * @brief	Creates a wellbore into this repository.
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWell == nullptr</tt>.
		 *
		 * @param [in]	witsmlWell	The well associated to this wellbore. It cannot be null.
		 * @param 	  	guid	  	The guid to set to the wellbore. If empty then a new guid will be
		 * 							generated.
		 * @param 	  	title	  	The title to set to the wellbore. If empty then \"unknown\" title
		 * 							will be set.
		 * @param 	  	isActive	  	True if is active, false if not.
		 *
		 * @returns	A pointer to the new wellbore.
		 */
		WITSML2_NS::Wellbore* createWellbore(WITSML2_NS::Well* witsmlWell,
			const std::string & guid,
			const std::string & title,
			bool isActive);

		/**
		 * @brief	Creates a wellbore into this repository.
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWell == nullptr</tt>.
		 *
		 * @param [in]	witsmlWell	  	The well associated to this wellbore. It cannot be null.
		 * @param 	  	guid		  	The guid to set to the wellbore. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the wellbore. If empty then \"unknown\" title
		 * 								will be set.
		 * @param 	  	statusWellbore	POSC wellbore status.
		 * @param 	  	isActive	  	True if is active, false if not.
		 * @param 	  	achievedTD	  	True indicates that the wellbore has achieved total depth. That
		 * 								is, drilling has completed. False indicates otherwise.  
		 *
		 * @returns	A pointer to the new wellbore.
		 */
		WITSML2_NS::Wellbore* createWellbore(WITSML2_NS::Well* witsmlWell,
			const std::string & guid,
			const std::string & title,
			gsoap_eml2_3::eml23__WellStatus statusWellbore,
			bool isActive,
			bool achievedTD
		);

		/**
		 * @brief	Creates a well completion into this repository
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWell == nullptr</tt>.
		 *
		 * @param [in]	witsmlWell	The well associated to this well completion. It cannot be null.
		 * @param 	  	guid	  	The guid to set to the well completion. If empty then a new guid will
		 * 							be generated.
		 * @param 	  	title	  	The title to set to the well completion. If empty then \"unknown\"
		 * 							title will be set.
		 *
		 * @returns	A pointer to the new well completion.
		 */
		WITSML2_1_NS::WellCompletion* createWellCompletion(WITSML2_NS::Well* witsmlWell,
			const std::string & guid,
			const std::string & title);

		/**
		 * Creates a wellbore completion into this repository
		 *
		 * @param [in]	witsmlWellbore	  	The wellbore associated to this wellbore completion. It
		 * 									cannot be null.
		 * @param 	  	guid			  	The guid to set to the wellbore completion. If empty then a new guid will be
		 * 									generated.
		 * @param 	  	title			  	The title to set to the wellbore completion. If empty then \"unknown\" title
		 * 									will be set.
		 *
		 * @returns	A pointer to the new wellbore completion.
		 */
		WITSML2_1_NS::WellboreCompletion* createWellboreCompletion(WITSML2_NS::Wellbore* witsmlWellbore,
			const std::string & guid,
			const std::string & title);

		/**
		 * @brief	Creates a wellbore geometry into this repository. It is used to capture information
		 * 			about the configuration of the permanently installed components in a wellbore
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWellbore == nullptr</tt>.
		 *
		 * @param [in]	witsmlWellbore	The wellbore associated to this wellbore geometry. It cannot be
		 * 								null.
		 * @param 	  	guid		  	The guid to set to the geometry. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the geometry. If empty then \"unknown\" title
		 * 								will be set.
		 * @param 		isActive	  	True if is active, false if not.
		 *
		 * @returns	A pointer to the new wellbore geometry.
		 */
		WITSML2_1_NS::WellboreGeometry* createWellboreGeometry(WITSML2_NS::Wellbore* witsmlWellbore,
			const std::string & guid,
			const std::string & title,
			bool isActive);

		/**
		 * @brief	Creates a wellbore trajectory into this repository
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWellbore == nullptr</tt>.
		 *
		 * @param [in]	witsmlWellbore	The wellbore associated to this wellbore trajectory. It cannot be
		 * 								null.
		 * @param 	  	guid		  	The guid to set to the trajectory. If empty then a new guid will
		 * 								be generated.
		 * @param 	  	title		  	The title to set to the trajectory. If empty then \"unknown\"
		 * 								title will be set.
		 * @param 		isActive	  	True if is active, false if not.
		 *
		 * @returns	A pointer to the new trajectory.
		 */
		WITSML2_1_NS::Trajectory* createTrajectory(WITSML2_NS::Wellbore* witsmlWellbore,
			const std::string & guid,
			const std::string & title,
			bool isActive);

		/**
		 * @brief	Creates a wellbore log into this repository
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWellbore == nullptr</tt>.
		 *
		 * @param [in]	witsmlWellbore	The wellbore associated to this log. It cannot be null.
		 * @param 		guid			The guid to set to the log. If empty then a new guid will be
		 * 								generated.
		 * @param 		title			The title to set to the log. If empty then \"unknown\" title will
		 * 								be set.
		 * @param 		isActive		True if is active, false if not.
		 *
		 * @returns	A pointer to the new log.
		 */
		WITSML2_1_NS::Log* createLog(WITSML2_NS::Wellbore* witsmlWellbore,
			const std::string & guid,
			const std::string & title,
			bool isActive);

		/**
		 * Creates a channel set into this repository
		 *
		 * @param 	guid 	The guid to set to the channel set. If empty then a new guid will be generated.
		 * @param 	title	The title to set to the channel set. If empty then \"unknown\" title will be set.
		 * @param 	isActive	True if is active, false if not.
		 *
		 * @returns	A pointer to the new channel set.
		 */
		WITSML2_1_NS::ChannelSet* createChannelSet(
			const std::string & guid,
			const std::string & title,
			bool isActive);

		/**
		 * @brief	Creates a channel into this repository
		 *
		 * @exception	std::invalid_argument	If <tt>propertyKind == nullptr</tt>.
		 *
		 * @param [in]	propertyKind	  	The property kind associated to this channel. It cannot be
		 * 									null.
		 * @param 	  	guid			  	The guid to set to the channel. If empty then a new guid will
		 * 									be generated.
		 * @param 	  	title			  	The title to set to the channel. If empty then \"unknown\"
		 * 									title will be set.
		 * @param 	  	mnemonic		  	The mnemonic name to set to this channel.
		 * @param 	  	uom				  	The underlying unit of measure of the value.
		 * @param 	  	dataKind		  	The underlying data kind of the value.
		 * @param 	  	isActive	  	True if is active, false if not.
		 *
		 * @returns	A pointer to the new channel.
		 */
		WITSML2_1_NS::Channel* createChannel(EML2_NS::PropertyKind * propertyKind,
			const std::string & guid, const std::string & title,
			const std::string & mnemonic, gsoap_eml2_3::eml23__UnitOfMeasure uom, gsoap_eml2_3::witsml21__ChannelDataKind dataKind,
			bool isActive);
			
			/**
		 * @brief	Creates a WITSML2.0 Wellbore Marker into this repository
		 *
		 * @param 	  	guid		  	The guid to set to the marker. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the marker. If empty then \"unknown\" title will
		 * 								be set.
		 * @param 	  	md		  		The Measured Depth to set to this marker.
		 * @param 	  	mdUom			The underlying unit of measure of the MD value.
		 *
		 * @returns	A pointer to the new Wellbore Marker.
		 */
		WITSML2_1_NS::WellboreMarker* createWellboreMarker(
			const std::string & guid, const std::string & title,
			double md, gsoap_eml2_3::eml23__LengthUom mdUom);

		/**
		 * @brief	Creates a WITSML2.0 Wellbore Marker into this repository
		 *
		 * @exception	std::invalid_argument	If <tt>witsmlWellbore == nullptr</tt>.
		 *
		 * @param [in]	witsmlWellbore	The wellbore associated to this marker. It cannot be null.
		 * @param 	  	guid		  	The guid to set to the marker. If empty then a new guid will be
		 * 								generated.
		 * @param 	  	title		  	The title to set to the marker. If empty then \"unknown\" title will
		 * 								be set.
		 * @param 	  	md		  		The Measured Depth to set to this marker.
		 * @param 	  	mdUom			The underlying unit of measure of the MD value.
		 *
		 * @returns	A pointer to the new Wellbore Marker.
		 */
		WITSML2_1_NS::WellboreMarker* createWellboreMarker(WITSML2_NS::Wellbore* witsmlWellbore,
			const std::string & guid, const std::string & title,
			double md, gsoap_eml2_3::eml23__LengthUom mdUom);
			
		//*************** PRODML *************/	
		
		/**
		 * Creates a fluid system into this repository
		 *
		 * @param 	guid			  	The guid to set to the fluid system. If empty then a new guid
		 * 								will be generated.
		 * @param 	title			  	The title to set to the fluid system. If empty then \"unknown\"
		 * 								title will be set.
		 * @param 	temperatureValue  	The temperature value.
		 * @param 	temperatureUom	  	The temperature unit of measure.
		 * @param 	pressureValue	  	The pressure value.
		 * @param 	pressureUom		  	The pressure unit of measure.
		 * @param 	reservoirFluidKind	The kind of the reservoir fluid.
		 * @param 	gasOilRatio		  	The gas oil ratio.
		 * @param 	gasOilRatioUom	  	The gas oil ratio unit of measure.
		 *
		 * @returns	A pointer to the new fluid system.
		 */
		PRODML2_2_NS::FluidSystem* createFluidSystem(const std::string & guid,
			const std::string & title,
			double temperatureValue, gsoap_eml2_3::eml23__ThermodynamicTemperatureUom temperatureUom,
			double pressureValue, gsoap_eml2_3::eml23__PressureUom pressureUom,
			gsoap_eml2_3::prodml22__ReservoirFluidKind reservoirFluidKind,
			double gasOilRatio, gsoap_eml2_3::eml23__VolumePerVolumeUom gasOilRatioUom);

		/**
		 * Creates a fluid characterization into this repository
		 *
		 * @param 	guid 	The guid to set to the fluid characterization. If empty then a new guid will
		 * 					be generated.
		 * @param 	title	The title to set to the fluid characterization. If empty then \"unknown\"
		 * 					title will be set.
		 *
		 * @returns	A pointer to the new fluid characterization.
		 */
		PRODML2_2_NS::FluidCharacterization* createFluidCharacterization(const std::string & guid, const std::string & title);

		/**
		 * Creates a time series data into this repository
		 *
		 * @param 	guid 	The guid to set to the time series data. If empty then a new guid will
		 * 					be generated.
		 * @param 	title	The title to set to the time series data. If empty then \"unknown\"
		 * 					title will be set.
		 *
		 * @returns	A pointer to the new time series data.
		 */
		PRODML2_2_NS::TimeSeriesData* createTimeSeriesData(const std::string & guid, const std::string & title);

#ifdef WITH_RESQML2_2
		//************************************/
		//************ EML2.3 ****************/
		//************************************/

		EML2_NS::GraphicalInformationSet* createGraphicalInformationSet(const std::string & guid, const std::string & title);

		RESQML2_NS::DiscreteColorMap* createDiscreteColorMap(const std::string& guid, const std::string& title);

		RESQML2_NS::ContinuousColorMap* createContinuousColorMap(const std::string& guid, const std::string& title,
			gsoap_eml2_3::resqml22__InterpolationDomain interpolationDomain, gsoap_eml2_3::resqml22__InterpolationMethod interpolationMethod);
#endif
		//************************************/
		//***** STANDARD PROP KIND ***********/
		//************************************/
		
		RESQML2_0_1_NS::PropertyKindMapper* getPropertyKindMapper() const;

		//************************************/
		//************* WARNINGS *************/
		//************************************/

		void clearWarnings();
		const std::vector<std::string> & getWarnings() const;
		
		template <class valueType>
		valueType* createPartial(const std::string & guid, const std::string & title, const std::string & version = "")
		{
			gsoap_resqml2_0_1::eml20__DataObjectReference* dor = gsoap_resqml2_0_1::soap_new_eml20__DataObjectReference(gsoapContext);
			dor->UUID = guid.empty() ? generateRandomUuidAsString() : guid;
			dor->Title = title;
			if (!version.empty()) {
				dor->VersionString = gsoap_resqml2_0_1::soap_new_std__string(gsoapContext);
				dor->VersionString->assign(version);
			}
			valueType* result = new valueType(dor);
			addOrReplaceDataObject(result);
			return result;
		}
		
		// Template for partial RESQML2.2
#ifdef WITH_RESQML2_2		
		%template(createPartialDiscreteColorMap) createPartial<RESQML2_2_NS::DiscreteColorMap>;
		%template(createPartialContinuousColorMap) createPartial<RESQML2_2_NS::ContinuousColorMap>;
		%template(createPartialSeismicWellboreFrameRepresentation) createPartial<RESQML2_2_NS::SeismicWellboreFrameRepresentation>;
#endif
		// Template for partial RESQML2.0.1
		
		%template(createPartialLocalDepth3dCrs) createPartial<RESQML2_0_1_NS::LocalDepth3dCrs>;
		%template(createPartialLocalTime3dCrs) createPartial<RESQML2_0_1_NS::LocalTime3dCrs>;
		%template(createPartialMdDatum) createPartial<RESQML2_0_1_NS::MdDatum>;

		%template(createPartialHorizon) createPartial<RESQML2_0_1_NS::Horizon>;
		%template(createPartialTectonicBoundaryFeature) createPartial<RESQML2_0_1_NS::TectonicBoundaryFeature>;
		%template(createPartialFrontierFeature) createPartial<RESQML2_0_1_NS::FrontierFeature>;
		%template(createPartialWellboreFeature) createPartial<RESQML2_0_1_NS::WellboreFeature>;
		%template(createPartialSeismicLineFeature) createPartial<RESQML2_0_1_NS::SeismicLineFeature>;
		%template(createPartialSeismicLineSetFeature) createPartial<RESQML2_0_1_NS::SeismicLineSetFeature>;
		%template(createPartialSeismicLatticeFeature) createPartial<RESQML2_0_1_NS::SeismicLatticeFeature>;
		%template(createPartialOrganizationFeature) createPartial<RESQML2_0_1_NS::OrganizationFeature>;
		%template(createPartialStratigraphicUnitFeature) createPartial<RESQML2_0_1_NS::StratigraphicUnitFeature>;
		%template(createPartialGeobodyFeature) createPartial<RESQML2_0_1_NS::GeobodyFeature>;
		%template(createPartialFluidBoundaryFeature) createPartial<RESQML2_0_1_NS::FluidBoundaryFeature>;

		%template(createPartialGenericFeatureInterpretation) createPartial<RESQML2_0_1_NS::GenericFeatureInterpretation>;
		%template(createPartialHorizonInterpretation) createPartial<RESQML2_0_1_NS::HorizonInterpretation>;
		%template(createPartialFaultInterpretation) createPartial<RESQML2_0_1_NS::FaultInterpretation>;
		%template(createPartialWellboreInterpretation) createPartial<RESQML2_0_1_NS::WellboreInterpretation>;
		%template(createPartialStratigraphicUnitInterpretation) createPartial<RESQML2_0_1_NS::StratigraphicUnitInterpretation>;
		%template(createPartialStructuralOrganizationInterpretation) createPartial<RESQML2_0_1_NS::StructuralOrganizationInterpretation>;
		%template(createPartialStratigraphicColumnRankInterpretation) createPartial<RESQML2_0_1_NS::StratigraphicColumnRankInterpretation>;
		%template(createPartialStratigraphicOccurrenceInterpretation) createPartial<RESQML2_0_1_NS::StratigraphicOccurrenceInterpretation>;
		%template(createPartialEarthModelInterpretation) createPartial<RESQML2_0_1_NS::EarthModelInterpretation>;
		%template(createPartialGeobodyBoundaryInterpretation) createPartial<RESQML2_0_1_NS::GeobodyBoundaryInterpretation>;
		%template(createPartialGeobodyInterpretation) createPartial<RESQML2_0_1_NS::GeobodyInterpretation>;

		%template(createPartialPolylineSetRepresentation) createPartial<RESQML2_0_1_NS::PolylineSetRepresentation>;
		%template(createPartialPointSetRepresentation) createPartial<RESQML2_0_1_NS::PointSetRepresentation>;
		%template(createPartialPlaneSetRepresentation) createPartial<RESQML2_0_1_NS::PlaneSetRepresentation>;
		%template(createPartialPolylineRepresentation) createPartial<RESQML2_0_1_NS::PolylineRepresentation>;
		%template(createPartialGrid2dRepresentation) createPartial<RESQML2_0_1_NS::Grid2dRepresentation>;
		%template(createPartialTriangulatedSetRepresentation) createPartial<RESQML2_0_1_NS::TriangulatedSetRepresentation>;
		%template(createPartialWellboreTrajectoryRepresentation) createPartial<RESQML2_0_1_NS::WellboreTrajectoryRepresentation>;
		%template(createPartialDeviationSurveyRepresentation) createPartial<RESQML2_0_1_NS::DeviationSurveyRepresentation>;
		%template(createPartialRepresentationSetRepresentation) createPartial<RESQML2_0_1_NS::RepresentationSetRepresentation>;
		%template(createPartialNonSealedSurfaceFrameworkRepresentation) createPartial<RESQML2_0_1_NS::NonSealedSurfaceFrameworkRepresentation>;
		%template(createPartialSealedSurfaceFrameworkRepresentation) createPartial<RESQML2_0_1_NS::SealedSurfaceFrameworkRepresentation>;

		%template(createPartialUnstructuredGridRepresentation) createPartial<RESQML2_0_1_NS::UnstructuredGridRepresentation>;
		%template(createPartialSubRepresentation) createPartial<RESQML2_0_1_NS::SubRepresentation>;
		%template(createPartialGridConnectionSetRepresentation) createPartial<RESQML2_0_1_NS::GridConnectionSetRepresentation>;

		%template(createPartialTimeSeries) createPartial<RESQML2_0_1_NS::TimeSeries>;

		%template(createPartialPropertyKind) createPartial<RESQML2_0_1_NS::PropertyKind>;
		%template(createPartialPropertySet) createPartial<RESQML2_0_1_NS::PropertySet>;
		%template(createPartialDoubleTableLookup) createPartial<RESQML2_0_1_NS::DoubleTableLookup>;
		%template(createPartialStringTableLookup) createPartial<RESQML2_0_1_NS::StringTableLookup>;
		%template(createPartialDiscreteProperty) createPartial<RESQML2_0_1_NS::DiscreteProperty>;
		%template(createPartialCategoricalProperty) createPartial<RESQML2_0_1_NS::CategoricalProperty>;
		%template(createPartialCommentProperty) createPartial<RESQML2_0_1_NS::CommentProperty>;
		%template(createPartialContinuousProperty) createPartial<RESQML2_0_1_NS::ContinuousProperty>;

		%template(createPartialActivity) createPartial<RESQML2_0_1_NS::Activity>;
		%template(createPartialActivityTemplate) createPartial<RESQML2_0_1_NS::ActivityTemplate>;
		
		// Template for partial WITSML2.0
		
		%template(createPartialWell) createPartial<WITSML2_1_NS::Well>;
		%template(createPartialWellCompletion) createPartial<WITSML2_1_NS::WellCompletion>;
		%template(createPartialWellbore) createPartial<WITSML2_1_NS::Wellbore>;
		%template(createPartialWellboreCompletion) createPartial<WITSML2_1_NS::WellboreCompletion>;
		%template(createPartialWellboreGeometry) createPartial<WITSML2_1_NS::WellboreGeometry>;
		%template(createPartialTrajectory) createPartial<WITSML2_1_NS::Trajectory>;
		%template(createPartialLog) createPartial<WITSML2_1_NS::Log>;
		%template(createPartialChannelSet) createPartial<WITSML2_1_NS::ChannelSet>;
		%template(createPartialChannel) createPartial<WITSML2_1_NS::Channel>;
	};
	
%typemap(javaimports) COMMON_NS::EpcDocument %{
import java.lang.AutoCloseable;
%}
%typemap(javainterfaces) COMMON_NS::EpcDocument "AutoCloseable"
%typemap(javacode) COMMON_NS::EpcDocument %{
  @Override
  public void close() {
	closeNativeResource();
    delete();
  }
%}
	class EpcDocument
	{
	public:
	
		EpcDocument(const std::string & fileName);
		
		void setFilePath(const std::string & fp);

		virtual void serializeFrom(const DataObjectRepository & repo, bool useZip64 = false);
		virtual std::string deserializeInto(DataObjectRepository & repo, DataObjectRepository::openingMode hdfPermissionAccess = DataObjectRepository::openingMode::READ_ONLY);

// JAVA uses autocloseable and consequently cannot use a second "close" method
#ifdef SWIGJAVA
		%javamethodmodifiers close() "private";
		%rename(closeNativeResource) close();
#endif
		void close();

		std::string getStorageDirectory() const;
		std::string getName() const;
		
		/**
		 * Sets or adds an extended core property
		 *
		 * @param 	key  	The key of the property.
		 * @param 	value	The value of the property.
		 */
		void setExtendedCoreProperty(const std::string & key, const std::string & value);
		
		/**
		 * Gets extended core property count.
		 *
		 * @returns	The count of extended core properties in this EPC document
		 */
		uint64_t getExtendedCorePropertyCount() const;
		
		/**
		 * Gets an extended core property value according to its key.
		 *
		 * @param 	key	The key of the property.
		 *
		 * @returns	An empty string if the extended core property does not exist. Or the extended core
		 * 			property value if it exists.
		 */
		std::string getExtendedCoreProperty(const std::string & key);
	};
	
	class EnumStringMapper
	{
	public :
		EnumStringMapper();

		std::string getEnergisticsPropertyKindName(gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind energisticsPropertyKind) const;
		gsoap_resqml2_0_1::resqml20__ResqmlPropertyKind getEnergisticsPropertyKind(const std::string & energisticsPropertyKindName) const;

		std::string getEnergisticsUnitOfMeasureName(gsoap_resqml2_0_1::resqml20__ResqmlUom energisticsUom) const;
		gsoap_resqml2_0_1::resqml20__ResqmlUom getEnergisticsUnitOfMeasure(const std::string & energisticsUomName) const;

		/**
		 * Get the name of a facet as a string based on the enumerated facet enumerated value
		 *
		 * @param 	facet	An enumerated facet.
		 *
		 * @returns	A string corresponding to @p facet if successful. The empty string if no
		 * 			correspondence is found.
		 */
		std::string getFacet(gsoap_eml2_3::eml23__FacetKind facet) const;

		/**
		 * Get the enumerated value from the name of a facet.
		 *
		 * @param 	facet	The name of a facet.
		 *
		 * @returns	The facet enumerated value corresponding to @p facet if successful. The @c
		 * 			what facet enumerated value if no correspendance is found.
		 */
		gsoap_eml2_3::eml23__FacetKind getFacet(const std::string & facet) const;

		std::string lengthUomToString(gsoap_eml2_3::eml23__LengthUom witsmlUom) const;

		std::string verticalCoordinateUomToString(gsoap_eml2_3::eml23__VerticalCoordinateUom witsmlUom) const;

		std::string planeAngleUomToString(gsoap_eml2_3::eml23__PlaneAngleUom witsmlUom) const;
	};
}

