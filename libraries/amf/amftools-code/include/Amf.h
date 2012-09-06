/*******************************************************************************
Copyright (c) 2012, Jonathan Hiller

This file is part of the AMF Tools suite. http://amf.wikispaces.com/
AMF Tools is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
AMF Tools is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
See <http://www.opensource.org/licenses/lgpl-3.0.html> for license details.
*******************************************************************************/

#ifndef AMF_H
#define AMF_H

#include <string> //inlude for std::string parameters

#if defined(_WIN32) || defined(_WIN64) /*Windows*/
	#ifdef DLL_EXPORT
	#define LIBSPEC __declspec(dllexport)
	#elif defined(DLL_IMPORT)
	#define LIBSPEC __declspec(dllimport)
	#else
	#define LIBSPEC
	#endif
#else /*non-windows*/
	#define LIBSPEC /*TODO: Allow this header file to generate (and be distributed) with non-windows shared objects*/
#endif

class AmfFile;

//!The Amf class encapsulates all the functionality of an AMF file, including the ability to load, save, import other mesh types, and output 3D openGL drawings and 2D color slices. Windows users have the option of #defining LIBSPEC as either (1) DLL_EXPORT to generate a dll using this header or (2) DLL_IMPORT to use this header in conjunction with a compiled dll.
class LIBSPEC Amf /*LIBSPEC macro allows windows users the option of using this class in DLL form */
{
public:
	Amf();
	~Amf();
	Amf(const Amf& In);
	Amf& operator=(const Amf& In);

	//!Defines a unit system
	enum aUnitSystem {
		aUNIT_MM, //!<Millimeters
		aUNIT_M, //!<Meters
		aUNIT_IN, //!<Inches
		aUNIT_FT, //!<Feet
		aUNIT_UM //!<Micrometers
	};
	//![DEPRECATED!] Parameters of the physical AMF size boundaries (envelope) in the current unit system
	enum aEnvelopeData {
		aENVL_XMIN, //!<Minimum X value
		aENVL_YMIN, //!<Minimum Y value
		aENVL_ZMIN, //!<Minimum Z value
		aENVL_XMAX, //!<Maximum X value
		aENVL_YMAX, //!<Maximum Y value
		aENVL_ZMAX, //!<Maximum Z value
		aENVL_XSIZE, //!<Size in the X direction
		aENVL_YSIZE, //!<Size in the Y direction
		aENVL_ZSIZE //!<Size in the Z direction
	};
	//!Parameters that define an AMF constellation instance
	enum aInstanceParamD {
		aINST_DX, //!<Offset in the X direction
		aINST_DY, //!<Offset in the Y direction
		aINST_DZ, //!<Offset in the Z direction
		aINST_RX, //!<Rotation about the X axis (according to right hand rule)
		aINST_RY, //!<Rotation about the Y axis (according to right hand rule)
		aINST_RZ //!<Rotation about the Z axis (according to right hand rule)
	};

	//Amf I/O
	bool Save(std::string AmfFilePath, bool Compressed = true); //!<Saves an AMF file at the specified file path. Returns true if successful save. If false, check GetLastErrorMsg() for failure reason. @param[in] AmfFilePath Full path of the AMF file to save. @param[in] Compressed Optionally compress AMF into binary format before saving.
	bool Load(std::string AmfFilePath, bool StrictLoad = true); //!<Loads an AMF file from file path. Returns true if successful load. If false, check GetLastErrorMsg() for failure reason. @param[in] AmfFilePath Full path of the AMF file to load. @param[in] StrictLoad When true the load will stop on error (recommended for checking amf files). Otherwise The AMF file will be fixed, but with the possibility of not loading some data.
	bool ImportAmf(std::string AmfFilePath, bool StrictLoad = true); //!<Imports an AMF file from file path to this AMF object. If false, check GetLastErrorMsg() for failure reason. @param[in] AmfFilePath Full path of the AMF file to import. @param[in] StrictLoad When true the load will stop on error (recommended for checking amf files). Otherwise The AMF file will be fixed, but with the possibility of not loading some data.
	void ClearAll(); //!<Clears all AMF data from this Amf object.
	
	//importing meshes
	bool ImportMesh(std::string MeshFilePath, int AmfObjectIndex=0, int AmfMeshIndex=0); //!<Imports a mesh file into AMF. Currently only supports stl or x3d import (more coming). Optionally specify the AMF object and mesh to load the mesh into. Returns true if successful import. If false, check GetLastErrorMsg() for failure reason. @param[in] MeshFilePath Full path of the mesh file to load (*.stl, *.x3d). @param[in] AmfObjectIndex The AMF object index to import mesh into. Creates an object of this index if it does not exist. @param[in] AmfMeshIndex The AMF mesh index of the specified object index to import mesh into. Creates a mesh of this index if it does not exist.
	bool LoadStl(std::string StlFilePath); //!<Imports an stl into memory. This is an advanced function that allows information to be gathered about an stl file (see GetStlMeshSize()) before importing it into the AMF. After all information is gathered and import options set, call ImportStl() to import the internal data to the AMF. Returns true if successful load. If false, check GetLastErrorMsg() for failure reason. @param[in] StlFilePath Full path of the stl file to load (*.stl).
	bool GetStlMeshSize(double* XSize, double* YSize, double* ZSize); //!<Returns the size of a loaded stl file. The unitless size of the stl file most recently loaded into memory (see LoadStl()) is returned via XSize, YSize, and ZSize.  Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[out] XSize Pointer to a double. The X size of the stl will be written to this variable. @param[out] YSize Pointer to a double. The Y size of the stl will be written to this variable. @param[out] ZSize Pointer to a double. The Z size of the stl will be written to this variable.
	bool ImportStl(int AmfObjectIndex=0, int AmfMeshIndex=0); //!<Imports an stl previously loaded into memory (see LoadStl()). Optionally specify the AMF object and mesh to load the mesh into. @param[in] AmfObjectIndex The AMF object index to import mesh into. Creates an object of this index if it does not exist.  Returns true if successful import. If false, check GetLastErrorMsg() for failure reason. @param[in] AmfMeshIndex The AMF mesh index of the specified object index to import mesh into. Creates a mesh of this index if it does not exist.
	bool LoadX3d(std::string X3dFilePath, std::string ImagePath="", std::string* ImgPathErrorReturn = NULL); //!<Imports an x3d into memory. This is an advanced function that allows information to be gathered about an x3d file (see GetX3dMeshSize()) before importing it into the AMF. After all information is gathered and import options set, call ImportX3d() to import the internal data to the AMF. If the X3d file contains a reference to a texture that is not found, the expected file path is output to ImgPathErrorReturn. If ImagePath is set to anything besides an empty string, the importer will ignore any internal image path information and load the image at this path, if possible. @param[in] X3dFilePath Full path of the X3d file to load (*.x3d). @param[in] ImagePath If ImagePath is passed as anything besides an empty string (""), any image path inside the x3d will be replaced with this image path. @param[out] ImgPathErrorReturn In the event of an x3d load failure due to inability to locate texture image file, the string pointed to by this parameter will be filled with the erroneus file path in the x3d file.
	bool GetX3dMeshSize(double* XSize, double* YSize, double* ZSize);  //!<Returns the size of a loaded x3d file. The unitless size of the x3d file most recently loaded into memory (see LoadX3d()) is returned via XSize, YSize, and ZSize.  Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[out] XSize Pointer to a double. The X size of the x3d will be written to this variable. @param[out] YSize Pointer to a double. The Y size of the x3d will be written to this variable. @param[out] ZSize Pointer to a double. The Z size of the x3d will be written to this variable.
	bool ImportX3d(int AmfObjectIndex=0, int AmfMeshIndex=0); //!<Imports an x3d previously loaded into memory (see LoadX3d()). Optionally specify the AMF object and mesh to load the mesh into. @param[in] AmfObjectIndex The AMF object index to import mesh into. Creates an object of this index if it does not exist.  Returns true if successful import. If false, check GetLastErrorMsg() for failure reason. @param[in] AmfMeshIndex The AMF mesh index of the specified object index to import mesh into. Creates a mesh of this index if it does not exist.

	//exporting meshes
	bool ExportSTL(std::string StlFilePath); //!<Exports an stl file from the AMF at the specified file path. . Returns true if successful save. If false, check GetLastErrorMsg() for failure reason. @param[in] StlFilePath Full path of the stl file to save.

	//Units
	void SetImportUnits(aUnitSystem Units); //!<Sets the imprt units. Unitless meshes will be imported according to the specified units. If no other geometry exists in the amf, the current AMF units will be set to this value. @param[in] Units The unit system to use.
	aUnitSystem GetUnits(void); //!<Returns the current units of the AMF.
	std::string GetUnitsString(void); //!<Returns a string of the current AMF units (i.e. "mm", "ft", etc.).
	std::string GetUnitsString(aUnitSystem Units); //!<Returns a string of the specifed units (i.e. "mm", "ft", etc.). @param[in] Units The unit system to convert to string form.
	void SetUnits(aUnitSystem Units); //!<Sets the AMF units to the specified unit system. Important: This function only changes the units label, and does not scale the numerical values of geometry (see Scale()). Therefore an AMF that was previously 1 inch wide will be 1 millimeter wide if the units are changed from inches to millimeters. @param[in] Units The new AMF units.
	double ConvertUnits(double Value, aUnitSystem OriginalUnits, aUnitSystem DesiredUnits); //!<Converts values between units. A value in its original unit system will be returned as the physically equivalent value in the desired unit system. For instance, a value of 1 with OriginalUnits of inches will return 25.4 if DesiredUnits is set to millimeters. @param[in] Value The value to be converted, in OriginalUnits. @param[in] OriginalUnits The unit system of the Value being passed in. @param[in] DesiredUnits The unit system to convert the value into.
	double ToCurrentUnits(double Value, aUnitSystem OriginalUnits); //!<Converts a value into current AMF units. Given the value in the specified OriginalUnits, the equivalent value in the current AMF unit system is returned. If the unit systems are the same, the value will be returned unchanged.  @param[in] Value The value to be converted, in OriginalUnits. @param[in] OriginalUnits The unit system of the Value being passed in.
	double FromCurrentUnits(double Value, aUnitSystem DesiredUnits); //!<Converts a value from current AMF units. The value is interpreted as being in the current AMF units, and will be converted in the specified DesiredUnits. @param[in] Value The value to be converted, in current AMF Units. @param[in] DesiredUnits The unit system to convert the value into.

	//Size of Amf
	double GetEnvelopeData(aEnvelopeData Data); //!< [DEPRECATED: use GetEnvlMin(), GetEnvlMax(), and GetEnvlSize() instead] Returns information about the physical envelope of the AMF. @param[in] Data The desired parameter.
	bool GetEnvlMin(double* pXMinOut, double* pYMinOut, double* pZMinOut, int RenderIndex=-1); //!<Calculates the minimum extreme of the specified rendered instance. If RenderIndex is specified, the information pertains to only this rendered volume (after constellation transformations). Otherwise the information pertains to the entire AMF. Calculated values are in the base AMF units (See GetUnits()). Returns false if invalid RenderIndex specified, otherwise true. @param[out] pXMinOut Returns the calculated minimum X value to the location specified by this pointer. @param[out] pYMinOut Returns the calculated minimum Y value to the location specified by this pointer. @param[out] pZMinOut Returns the calculated minimum Z value to the location specified by this pointer. @param[in] RenderIndex UI index of the rendered instance.
	bool GetEnvlMax(double* pXMaxOut, double* pYMaxOut, double* pZMaxOut, int RenderIndex=-1);  //!<Calculates the maximum extreme of the specified rendered instance. If RenderIndex is specified, the information pertains to only this rendered volume (after constellation transformations). Otherwise the information pertains to the entire AMF. Calculated values are in the base AMF units (See GetUnits()). Returns false if invalid RenderIndex specified, otherwise true. @param[out] pXMaxOut Returns the calculated maximum X value to the location specified by this pointer. @param[out] pYMaxOut Returns the calculated maximum Y value to the location specified by this pointer. @param[out] pZMaxOut Returns the calculated maximum Z value to the location specified by this pointer. @param[in] RenderIndex UI index of the rendered instance.
	bool GetEnvlSize(double* pXSizeOut, double* pYSizeOut, double* pZSizeOut, int RenderIndex=-1); //!<Calculates the size of the specified rendered instance. If RenderIndex is specified, the information pertains to only this rendered volume (after constellation transformations). Otherwise the information pertains to the entire AMF. Calculated values are in the base AMF coordinate system (See GetEnvlDims()) and units (See GetUnits()). Returns false if invalid RenderIndex specified, otherwise true. @param[out] pXSizeOut Returns the calculated X size to the location specified by this pointer. @param[out] pYSizeOut Returns the calculated Y size to the location specified by this pointer. @param[out] pZSizeOut Returns the calculated Z size to the location specified by this pointer. @param[in] RenderIndex UI index of the rendered instance.
	bool GetEnvlRotQuat(double* pWRotOut, double* pXRotOut, double* pYRotOut, double* pZRotOut, int RenderIndex=-1); //!<Calculates the total quaternion rotation applied to the specified rendered instance. If RenderIndex is specified, the information pertains to only this rendered volume. Otherwise the information pertains to the entire AMF, and will return the unit quaternion. Returns false if invalid RenderIndex specified, otherwise true. See also: GetEnvlRotAngleAxis(), GetEnvlOrigin(), GerEnvlDims(). @param[out] pWRotOut Returns the calculated w quaternion component. @param[out] pXRotOut Returns the calculated x quaternion component. @param[out] pYRotOut Returns the calculated y quaternion component. @param[out] pZRotOut Returns the calculated z quaternion component.  @param[in] RenderIndex UI index of the rendered instance.
	bool GetEnvlRotAngleAxis(double* pAngleRadOut, double* pNXOut, double* pNYOut, double* pNZOut, int RenderIndex=-1); //!<Calculates the total angle-axis rotation applied to the specified rendered instance. If RenderIndex is specified, the information pertains to only this rendered volume. Otherwise the information pertains to the entire AMF, and will return zero angle rotation. Returns false if invalid RenderIndex specified, otherwise true. See also: GetEnvlRotQuat(), GetEnvlOrigin(), GerEnvlDims(). @param[out] pAngleRadOut Returns the angle in radians to apply about the returned normal. @param[out] pNXOut Returns the X component of the axis to rotate about. @param[out] pNYOut Returns the Y component of the axis to rotate about. @param[out] pNZOut Returns the Z component of the axis to rotate about. @param[in] RenderIndex UI index of the rendered instance.
	bool GetEnvlOrigin(double* pXOriginOut, double* pYOriginOut, double* pZOriginOut, int RenderIndex=-1); //!<Calculates the origin of a transformed rendered instance. This origin will always correspond to the minimum corner of a bounding box aligned with the rendered instance before any transformations. In conjuntion with GetEnvlDims() and GetEnvlRotAngleAxis() or GetEnvlRotQuat(), a bounding box aligned with a rendered instance can be drawn at the same location and angle by creating a box from [0,0,0] to the point returned by GetEnvlDims(), rotating it about the origin by the results of one of the GetEnvlRot functions, then translating by the values returned by this function. If RenderIndex is specified, the information pertains to only this rendered volume. Otherwise the information pertains to the entire AMF, and will return zeros. Returns false if invalid RenderIndex specified, otherwise true. See also: GetEnvlRotAngleAxis(), GetEnvlRotQuat(), GerEnvlDims(). @param[out] pXOriginOut Returns the X location of the local origin. @param[out] pYOriginOut Returns the Y location of the local origin. @param[out] pZOriginOut Returns the Z location of the local origin. @param[in] RenderIndex UI index of the rendered instance.
	bool GetEnvlDims(double* pIDimOut, double* pJDimOut, double* pKDimOut, int RenderIndex=-1);  //!<Calculates the size of the specified rendered instance in its original coordinate system. If RenderIndex is specified, the information pertains to only this rendered volume. Otherwise the information pertains to the entire AMF, and will return zero angle rotation. Returns false if invalid RenderIndex specified, otherwise true. See also: GetEnvlRotAngleAxis(), GetEnvlRotQuat(), GetEnvlOrigin(), @param[out] pIDimOut Returns the untransformed X size in AMF units  @param[out] pJDimOut Returns the untransformed Y size in AMF units @param[out] pKDimOut Returns the untransformed Z size in AMF units @param[in] RenderIndex UI index of the rendered instance.

	bool Scale(double ScaleFactor, bool ScaleConstellations=true, bool ScaleEquations=false); //!<Scales the entire AMF isotropically by the specified factor. Does not change the current unit system. Returns true if successful scale. If false, check GetLastErrorMsg() for failure reason. @param[in] ScaleFactor The factor to scale the AMF by. @param[in] ScaleConstellations Scales the offsets for all defined constellations by the same factor. @param[in] ScaleEquations Placeholder to scale equation. Not yet implemented.
	bool Scale(double XScaleFactor, double YScaleFactor, double ZScaleFactor, bool ScaleConstellations = true, bool ScaleEquations=false); //!<Scales the entire AMF anisotropically by the specified factors. Does not change the current unit system. Returns true if successful scale. If false, check GetLastErrorMsg() for failure reason. @param[in] XScaleFactor The factor to scale the AMF by in the X direction. @param[in] YScaleFactor The factor to scale the AMF by in the Y direction. @param[in] ZScaleFactor The factor to scale the AMF by in the Z direction. @param[in] ScaleConstellations Scales the offsets for all defined constellations by the same factor. @param[in] ScaleEquations Placeholder to scale equation. Not yet implemented.

	//Amf Objects:
	int GetObjectCount(void); //!<Returns the number of object nodes in the AMF.
	std::string GetObjectName(int ObjectIndex); //!<Returns the name of the specified object index in the AMF. Returns "" if no name was found. @param[in] ObjectIndex Index of object to return the name of.
	void RenameObject(int ObjectIndex, std::string NewName); //!<Sets the name of the specified object index. @param[in] ObjectIndex Index of object to rename. @param[in] NewName the desired object name.
	int AddObject(std::string ObjectName = ""); //!<Adds a new object to the AMF. The name may be specified. Returns the index of the object just created. @param[in] ObjectName Optional name for the new object.
	void RemoveObject(int ObjectIndex); //!<Removes the object at the specified index. Note that this may change the index of other objects. @param[in] ObjectIndex Index of object to remove.
	void TranslateObject(int ObjectIndex, double dx, double dy, double dz); //!<Translates the specified object. Note that all instances this object will be affected. @param[in] ObjectIndex Index of object to translate. @param[in] dx Amount to translate in the X direction in current AMF units. @param[in] dy Amount to translate in the Y direction in current AMF units. @param[in] dz Amount to translate in the Z direction in current AMF units.
	void RotateObject(int ObjectIndex, double rx, double ry, double rz); //!<Rotates the specified object. Note that all instances this object will be affected. Rotations are applied as follows: The object is rotated by rx radians about the X axis, then ry radians about the Y axis, then rz radians about the Z axis. @param[in] ObjectIndex Index of object to translate. @param[in] rx Amount to rotate about the X axis in radians. @param[in] ry Amount to rotate about the Y axis in radians. @param[in] rz Amount to rotate about the Z axis in radians.

		//Amf Meshes
		int GetMeshCount(int ObjectIndex); //!<Returns the number of mesh nodes in the specified object index of the AMF. @param[in] ObjectIndex Index of the object to query number of mesh nodes within.

			//Amf Volumes
			int GetVolumeCount(int ObjectIndex, int MeshIndex); //!<Returns the number of volume nodes in the specified object index and mesh index of the AMF. @param[in] ObjectIndex Index of the object to query. @param[in] MeshIndex Index of the mesh to query for the number of volume nodes within.
			std::string GetVolumeName(int ObjectIndex, int MeshIndex, int VolumeIndex); //!<Returns the name of the volume index in the specified object index and mesh index of the AMF. Returns "" if no name was found. @param[in] ObjectIndex Index of the object to query. @param[in] MeshIndex Index of the mesh to query. @param[in] VolumeIndex Index of volume to return the name of.
			void RenameVolume(int ObjectIndex, int MeshIndex, int VolumeIndex, std::string NewName);  //!<Sets the name of the volume index in the specified object index and mesh index of the AMF. @param[in] ObjectIndex Index of the object containing the volume. @param[in] MeshIndex Index of the mesh containing the volume. @param[in] VolumeIndex Index of volume to rename. @param[in] NewName the desired volume name.
			int GetVolumeMaterialIndex(int ObjectIndex, int MeshIndex, int VolumeIndex); //!<Returns the index of the material assigned to the specified volume. Returns -1 if the volume was not found or no material is specified for it. @param[in] ObjectIndex Index of the object containing the volume. @param[in] MeshIndex Index of the mesh containing the volume. @param[in] VolumeIndex Index of volume to return material index of. 
			bool SetVolumeMaterialIndex(int ObjectIndex, int MeshIndex, int VolumeIndex, int MaterialIndex); //!<Assigns a material by index to the specified volume. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] ObjectIndex Index of the object containing the volume. @param[in] MeshIndex Index of the mesh containing the volume. @param[in] VolumeIndex Index of volume to set material index of. @param[in] MaterialIndex Index of the material to assign to this volume.

	//Amf Constellations:
	int GetConstellationCount(void); //!<Returns the number of constellation nodes in the AMF.
	std::string GetConstellationName(int ConstellationIndex); //!<Returns the name of the specified constellation index in the AMF. Returns "" if no name was found.  @param[in] ConstellationIndex Index of constellation to return the name of.
	void RenameConstellation(int ConstellationIndex, std::string NewName); //!<Sets the name of the specified constellation index. @param[in] ConstellationIndex Index of constellation to rename. @param[in] NewName the desired constellation name.
	int AddConstellation(std::string ConstellationName = ""); //!<Adds a new constellation to the AMF. The name may be specified. Returns the index of the constellation just created. @param[in] ConstellationName Optional name for the new constellation.
	void RemoveConstellation(int ConstellationIndex); //!<Removes the constellation at the specified index. Note that this may change the index of other constellations. @param[in] ConstellationIndex Index of constellation to remove.
	bool IsConstellationReferencedBy(int ConstellationIndex, int ConstellationIndexToCheck); //!< Returns true if the "constellation to check" references the specified constellation. This function is used to prevent self-referential constellations. Returns false if the "constellation to check" does not include any references (direct or iterative) to the specified constellation, implying that it is elegible to be referenced in this constellation. @param[in] ConstellationIndex Index of a constellation. @param[in] ConstellationIndexToCheck Index of the constellation to check for any references to the first constellation.

		//Amf Instances
		int GetInstanceCount(int ConstellationIndex); //!<Returns the number of instance nodes in the specified constellation index of the AMF. @param[in] ConstellationIndex Index of the constellation to query number of instance nodes within.
		int AddInstance(int ConstellationIndex); //!<Adds a new instance to the specified AMF constellation. Returns the index of the instance just created. @param[in] ConstellationIndex Index of the constellation to add instance too.
		void RemoveInstance(int ConstellationIndex, int InstanceIndex); //!<Removes the instance at the specified index. Note that this may change the index of other instances. @param[in] ConstellationIndex Index of the constellation to remove instance from. @param[in] InstanceIndex Index of instance to remove.
		bool SetInstanceObjectIndex(int ConstellationIndex, int InstanceIndex, int InstanceObjectIndex); //!<Assigns an object by index for the specified instance. Returns true if successful. If false, check GetLastErrorMsg() for failure reason.  @param[in] ConstellationIndex Index of the constellation to locate instance in. @param[in] InstanceIndex Index of instance to set object reference for. @param[in] VolumeIndex Index of volume to set material index of. @param[in] InstanceObjectIndex Index of the object to assign to this instance.
		bool SetInstanceConstellationIndex(int ConstellationIndex, int InstanceIndex, int InstanceConstellationIndex); //!<Assigns a constellation by index for the specified instance. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] ConstellationIndex Index of the constellation to locate instance in. @param[in] InstanceIndex Index of instance to set constellation reference for. @param[in] VolumeIndex Index of volume to set material index of. @param[in] InstanceConstellationIndex Index of the constellation to assign to this instance.
		int GetInstanceObjectIndex(int ConstellationIndex, int InstanceIndex); //!<Returns the index of the object assigned to the specified instance. Returns -1 if the instance was not found or no object is specified for it. Use GetInstanceConstellationIndex() to check for a reference to a constellation instead of an object. @param[in] ConstellationIndex Index of the constellation to locate instance in. @param[in] InstanceIndex Index of instance to get object reference from. 
		int GetInstanceConstellationIndex(int ConstellationIndex, int InstanceIndex); //!<Returns the index of the constellation assigned to the specified instance. Returns -1 if the instance was not found or no constellation is specified for it. Use GetInstanceObjectIndex() to check for a reference to a object instead of a constellation. @param[in] ConstellationIndex Index of the constellation to locate instance in. @param[in] InstanceIndex Index of instance to get constellation reference from. 
		bool SetInstanceParam(int ConstellationIndex, int InstanceIndex, aInstanceParamD ParamD, double Value); //!<Sets a value (offset, rotation) for the specified instance. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] ConstellationIndex Index of the constellation to locate instance in. @param[in] InstanceIndex Index of instance to set parameter for. @param[in] ParamD Specifies which instance parameter is being set. @param[in] Value The value to be set. For offsets this value is in the current units. For rotations, the value is always in degrees.
		double GetInstanceParam(int ConstellationIndex, int InstanceIndex, aInstanceParamD ParamD); //!<Returns the value in AMF units or angular degrees of the specified parameter of the instance.  @param[in] ConstellationIndex Index of the constellation to locate instance in. @param[in] InstanceIndex Index of instance to return a parameter for. @param[in] ParamD Specifies the desired instance parameter.

	//Amf Materials:
	int GetMaterialCount(void); //!<Returns the number of material nodes in the AMF.
	std::string GetMaterialName(int MaterialIndex); //!<Returns the name of the specified material index in the AMF. Returns "" if no name was found.  @param[in] MaterialIndex Index of material to return the name of.
	void RenameMaterial(int MaterialIndex, std::string NewName);  //!<Sets the name of the specified material index. @param[in] MaterialIndex Index of material to rename. @param[in] NewName the desired material name.
	int AddMaterial(std::string MaterialName = ""); //!<Adds a new material to the AMF. The name may be specified. Returns the index of the material just created. @param[in] MaterialName Optional name for the new material.
	int AddMaterial(std::string MaterialName, int Red, int Green, int Blue); //!<Adds a new material to the AMF. The name must be specified. Returns the index of the material just created. Input color values should range from 0 to 255. Values outside this range will be truncated. @param[in] MaterialName Optional name for the new material. @param[in] Red Red component of color.  @param[in] Green Red component of color.  @param[in] Blue Blue component of color.
	int AddMaterial(std::string MaterialName, double Red, double Green, double Blue); //!<Adds a new material to the AMF. The name must be specified. Returns the index of the material just created. Input color values should range from 0.0 to 1.0. Values outside this range will be truncated. @param[in] MaterialName Optional name for the new material. @param[in] Red Red component of color.  @param[in] Green Red component of color.  @param[in] Blue Blue component of color.
	void RemoveMaterial(int MaterialIndex); //!<Removes the material at the specified index. Note that this may change the index of other materials. @param[in] MaterialIndex Index of material to remove.
	bool IsMaterialReferencedBy(int MaterialIndex, int MaterialIndexToCheck); //!< Returns true if the "material to check" references the specified material. This function is used to prevent self-referential composite materials. Returns false if the "material to check" does not include any references (direct or iterative) to the specified material, implying that it is elegible to be referenced in this material. @param[in] MaterialIndex Index of a material. @param[in] MaterialIndexToCheck Index of the material to check for any references to the first material.
	bool SetMaterialColorD(int MaterialIndex, double Red, double Green, double Blue); //!<Sets the color of the specified material. Input color values should range from 0.0 to 1.0. Values outside this range will be truncated. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of material to set color of. @param[in] Red Red component of color.  @param[in] Green Red component of color.  @param[in] Blue Blue component of color.
	bool SetMaterialColorI(int MaterialIndex, int Red, int Green, int Blue); //!<Sets the color of the specified material. Input color values should range from 0 to 255. Values outside this range will be truncated. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of material to set color of. @param[in] Red Red component of color. @param[in] Green Green component of color.  @param[in] Blue Blue component of color.
	bool GetMaterialColorD(int MaterialIndex, double *Red, double *Green, double *Blue); //!<Returns the color of the specified material via the double pointer arguments. Color values will range from 0.0 to 1.0. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of material to set color of. @param[out] Red Red component of color. @param[out] Green Green component of color.  @param[out] Blue Blue component of color.
	bool GetMaterialColorI(int MaterialIndex, int *Red, int *Green, int *Blue); //!<Returns the color of the specified material via the integer pointer arguments. Color values will range from 0 to 255. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of material to set color of. @param[out] Red Red component of color. @param[out] Green Green component of color.  @param[out] Blue Blue component of color.

		//Amf Composites
		int GetCompositeCount(int MaterialIndex); //!<Returns the number of composite nodes in the specified material index of the AMF. @param[in] MaterialIndex Index of the material to query number of composite nodes within.
		void ClearComposites(int MaterialIndex); //!<Clears all composite nodes from the specified material index of the AMF. @param[in] MaterialIndex Index of the material to erase all composite nodes within.
		int AddComposite(int MaterialIndex, int MaterialIndexToComposite = 0); //!<Adds a new composite node to the specified AMF material. Optionally sets the material index this composite will reference. Returns the index of the composite just created, or -1 if unable to create. If so, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of the material to add a composite too. @param[in] MaterialIndexToComposite Index of material to composite. Self referential indices will error out.
		void RemoveComposite(int MaterialIndex, int CompositeIndex); //!<Removes the composite at the specified index. Note that this may change the index of other composites. @param[in] MaterialIndex Index of the material to remove composite from. @param[in] CompositeIndex Index of composite to remove.
		bool SetCompositeMaterialIndex(int MaterialIndex, int CompositeIndex, int CompositeMaterialIndex); //!<Assigns a material by index for the specified composite. A composite material index of zero always refers to void (empty) material. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of the material to locate composite in. @param[in] CompositeIndex Index of composite to set material reference for. @param[in] CompositeMaterialIndex Index of the material to assign to this composite.
		int GetCompositeMaterialIndex(int MaterialIndex, int CompositeIndex); //!<Returns the index of the material assigned to the specified composite. Returns -1 if the composite was not found.  @param[in] MaterialIndex Index of the material to locate composite in. @param[in] CompositeIndex Index of composite to return material index of. 
		std::string GetCompositeEquation(int MaterialIndex, int CompositeIndex); //!<Returns the AMF-formatted equation associated with the specified composite. Returns "" if no equation exisits.  @param[in] MaterialIndex Index of the material to locate composite in. @param[in] CompositeIndex Index of composite to return equation of. 
		bool SetCompositeEquation(int MaterialIndex, int CompositeIndex, std::string Equation); //!<Assigns equation to the specified composite. This will determine the spatial percentage of this material with respect to other materials. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] MaterialIndex Index of the material to locate composite in. @param[in] CompositeIndex Index of composite to set equation of. @param[in] Equation AMF-formatted equation to define spatial distribution of this composite material.

	//Amf Textures:	
	int GetTextureCount(void); //!<Returns the number of texture nodes in the AMF.

	//Output utilities
	bool SetSubdivisionLevel(int Level=4); //!<Set the mesh subdivision level for generating curved triangles. A level of 0 results in the original mesh. A level of 1 divides each triangle into 4, level 2 divides each triangle into 16, etc. Caution: setting the level higher results in an exponential number of triangles that must be generated. Returns true if successful. If false, check GetLastErrorMsg() for failure reason. @param[in] Level Number of recursive subdivisions to perform on mesh.
	void DrawGL(); //!<Draws the current 3D AMF in an initialized OpenGL window. This function should be called from somewhere within the OpenGL scene drawing function.
	unsigned char* GetSliceBitmapRGBA(double PixelSizeX, double PixelSizeY, double SliceHeightZ, int* XSizeOut, int* YSizeOut, double SurfaceDepth = 0.0); //!<Returns a color slice of the AMF. The physical size of each desired pixel is specified in current AMF units, along with the height (Z level) of the slice. A surface color extrusion depth may also be specified, which gives depth to surface colors and textures. The data is return as a 32bpp array of R, G, B, A values (four bytes per pixel) beginning with the upper left corner of the bitmap. Pixels are stored in left to right rows from top to bottom. A pointer to this internally allocated array is returned, which is valid until another slice is requested. The slicing algorithm is internally optimized for ascending Z height calls. @param[in] PixelSizeX The X size (width) in current amf units of the pixels of the desired bitmap. @param[in] PixelSizeY The Y size (height) in current amf units of the pixels of the desired bitmap. @param[in] SliceHeightZ The slice hight (Z level) of the desired slice bitmap. @param[out] XSizeOut The number of X pixels (in width) of the returned bitmap. @param[out] YSizeOut The number of Y pixels (in height) of the returned bitmap. @param[in] SurfaceDepth Depth of surface colors and textures to be extruded into volumes, in current AMF units.
	int* GetSliceSegmentsXY(double ZHeight, int* NumSegmentsOut); //!<Returns a list of 2D slice boundary segments. Not yet implemented.

	//Errors and information
	std::string GetInfoString(bool MeshInfo = true); //!<Returns a string with some helpful statistical information about the AMF. @param[in] MeshInfo Include information about the meshes.
	std::string* pLastErrorMsg(); //!<Returns a pointer to an internal string containing the last error message.
	std::string GetLastErrorMsg(); //!<Return the last error message encountered.

	//Real time status info on long i/o operations
	bool* pCancelIO(); //!<Returns a pointer to a boolean flag that can be used to cancel long I/O operations (i.e. Load(), Save(), ImportMesh()) from another thread. Setting the variable of the returned pointer to true will halt all I/O operations immediately.
	int* pCurTick(); //!<Returns a pointer to the current tick progress of a long I/O operation (i.e. Load(), Save(), ImportMesh()). This can be queried from another thread to provide information for a real-time status bar.
	int* pMaxTick();  //!<Returns a pointer to the maximum ticks of a long I/O operation (i.e. Load(), Save(), ImportMesh()). This can be queried from another thread to provide information for a real-time status bar.
	std::string* pStatusMsg(); //!< Returns a pointer to a string that contains the current work being done in I/O operations (i.e. Load(), Save(), ImportMesh()). This can be queried from another thread to provide information for a real-time status bar.





protected:
	AmfFile* pData; //!<Internal data
};

#undef LIBSPEC
#endif //AMF_WIN_H