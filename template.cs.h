// C#
#ifndef TEMPLATE_CS_H
#define TEMPLATE_CS_H

#include "template.common.h"

// must correspond with KstDataType enum in template.common.h
string CsSerializerName[KstDataType::LENGTH_OF_KST_DATA_TYPES] =
{
	"Boolean",
	"Byte",
	"SByte",
	"Short",
	"UShort",
	"Int",
	"UInt",
	"Long",
	"ULong",
	"Float",
	"Double",
	"String"
};

// must correspond with KstDataType enum in template.common.h
string CsConvertPhrase[KstDataType::LENGTH_OF_KST_DATA_TYPES] =
{
	"Convert.ToBoolean", //bool
	"Convert.ToByte", //byte
	"Convert.ToSByte", //sbyte
	"Convert.ToChar", //short
	"Convert.ToChar", //ushort
	"Convert.ToInt32", //int
	"Convert.ToUInt32", //uint
	"(long)Convert.ToDouble", //long
	"(ulong)Convert.ToDouble", //ulong
	"(float)Convert.ToDouble", //float
	"Convert.ToDouble", //double
	"Convert.ToString" //string
};

string* lookupSerializerName(string* typeString)
{
	KstDataType::Enum kstDataType = lookupKstDataType(typeString);
    if (((int)kstDataType) == -1)
    {
        throw string("Data type not found during lookup SerializerName");
    }
    string* serializerName = &(CsSerializerName[kstDataType]);
    return serializerName;
}

string* lookupConvertPhrase(string* typeString)
{
	KstDataType::Enum kstDataType = lookupKstDataType(typeString);
    if (((int)kstDataType) == -1)
    {
        throw string("Data type not found during lookup ConvertPhrase");
    }
    string* convertPhrase = &(CsConvertPhrase[kstDataType]);
    return convertPhrase;
}


namespace CsParseAs
{
	enum Enum
	{
		Default = 0,
		GetLength,
		Write,
		Read,
		ParseJsonObjectData,
		ParseJsonArrayData,
		ToJsonPackageArray,
		ToJsonPackageDictionary,
		Initialization,
		SerializerName,
		ConvertPhrase,
		GenericType1,
		GenericType2,
		GenericType3,
		GenericTypeSerializerName1,
		GenericTypeSerializerName2,
		GenericTypeSerializerName3,
		GenericTypeConvertPhrase1,
		GenericTypeConvertPhrase2,
		GenericTypeConvertPhrase3,
		IsPrimitiveTypeValue // if primitive, return Non-NULL
	};
}

namespace CsNodeType
{
	enum Enum
	{
		kst = 0,
			include,
			packet,
				packetName,
				packetMembers,
					packetMember,
						packetMemberType,
							/* workaround */
							packetMemberTypePrimitive,
							packetMemberTypeReference,
							packetMemberTypeMap,
							packetMemberTypeList,
							packetMemberTypeStream,
						packetMemberName,
			enumeration,
				enumerationName,
				enumerators,
					enumerator,
						enumeratorName,
						enumeratorValue
	};
}


#define TCS_HEADER				"//generated from %1% by Krystal\n\n"

#define TCS_USINGS				"using System;\n" \
								"using System.Collections;\n" \
								"using System.IO;\n" \
								"using System.Collections.Generic;\n" \
								"using System.Net;\n" \
								"using System.Text;\n" \
								"\n" \
								"using Krystal;\n" \
								"\n"

#define TCS_USING_BY_INCLUDE	"using %1%;\n"

#define TCS_NAMESPACE_BEGIN		"namespace %1%\n" \
								"{\n"

#define TCS_NAMESPACE_END		"}\n"

#define TCS_PACKET_BEGIN		"public class %1% : Krystal.IMessageStream\n" \
								"{\n"

#define TCS_PACKET_END			"}\n"

#define TCS_PACKET_ID_TYPE		int // cast hash value to this type only in output

#define TCS_PACKET_ID_FIELD		"public static readonly int ID = %1%;\n"

#define TCS_PACKET_URI			"public static readonly string URI = \"%1%\";\n"

#define TCS_PACKET_COOKIE_FIELD	"public string COOKIE = \"\";\n"

#define TCS_PACKET_MEMBER_AS_DEFAULT	"public %1% %2%%3%;\n" //type name initialization

#define TCS_PACKET_MEMBER_TYPE_MAP_AS_DEFAULT	"Dictionary<%1%, %2%>"

#define TCS_PACKET_MEMBER_TYPE_LIST_AS_DEFAULT	"List<%1%>"

#define TCS_PACKET_MEMBER_TYPE_STREAM_AS_DEFAULT		"Krystal.Stream"

#define TCS_PACKET_MEMBER_TYPE_NEW_AS_INITIALIZATION	" = new %1%()"

#define TCS_PACKET_MEMBER_TYPE_STREAM_AS_INITIALIZATION	" = new Krystal.Stream()"

#define TCS_PACKET_CONSTRUCTOR		"public %1%()\n" \
									"{\n" \
									"}\n"

#define TCS_PACKET_GET_ID			"public int GetID()\n" \
									"{\n" \
									"\treturn ID;\n" \
									"}\n"

#define TCS_PACKET_GET_URI			"public string GetURI()\n" \
									"{\n" \
									"\treturn URI;\n" \
									"}\n"

#define TCS_PACKET_SET_COOKIE		"public void SetCookie(string cookie)\n" \
									"{\n" \
									"\tCOOKIE = cookie;\n" \
									"}\n"

#define TCS_PACKET_GET_COOKIE		"public string GetCookie()\n" \
									"{\n" \
									"\treturn COOKIE;\n" \
									"}\n"

#define TCS_PACKET_GET_LENGTH_BEGIN		"public int GetLength(bool isInner = false)\n" \
										"{\n" \
										"\tint length = 0;\n" \
										"\tif ( false == isInner )\n" \
										"\t{\n" \
										"\t\tlength += Krystal.Serializer.String.GetLength( COOKIE );\n" \
										"\t}\n" \

#define TCS_PACKET_GET_LENGTH_END		"\treturn length;\n" \
										"}\n"

#define TCS_PACKET_CUSTOM_SERIALIZER_NAME			"Custom"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_PRIMITIVE	"length += Krystal.Serializer.%1%.GetLength(%2%);\n" // serializer member_name

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_REFERENCE	"length += Krystal.Serializer.Custom.GetLength(%1%, true);\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_MAP_PRIMITIVE_VALUE		"length += 4;\n" \
																"foreach ( KeyValuePair<%1%, %2%> %3%Temp in %3% )\n" \
																"{\n" \
																"\tlength += Krystal.Serializer.%4%.GetLength(%3%Temp.Key);\n" \
																"\tlength += Krystal.Serializer.%5%.GetLength(%3%Temp.Value);\n" \
																"}\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_MAP_REFERENCE_VALUE		"length += 4;\n" \
																"foreach ( KeyValuePair<%1%, %2%> %3%Temp in %3% )\n" \
																"{\n" \
																"\tlength += Krystal.Serializer.%4%.GetLength(%3%Temp.Key);\n" \
																"\tlength += Krystal.Serializer.Custom.GetLength(%3%Temp.Value);\n" \
																"}\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_LIST_PRIMITIVE_VALUE	"length += 4;\n" \
																"foreach ( %1% val in %2% )\n" \
																"{\n" \
																"\tlength += Krystal.Serializer.%3%.GetLength(val);\n" \
																"}\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_LIST_REFERENCE_VALUE	"length += 4;\n" \
																"foreach ( %1% val in %2% )\n" \
																"{\n" \
																"\tlength += Krystal.Serializer.Custom.GetLength(val, true);\n" \
																"}\n"

#define TCS_PACKET_GET_NAME		"public string GetName()\n" \
								"{\n" \
								"\treturn \"%1%\";\n" \
								"}\n"

#define TCS_PACKET_WRITE_BEGIN	"public void Write(BinaryWriter stream, bool isInner = false)\n" \
								"{\n" \
								"\tif ( false == isInner )\n" \
								"\t{\n" \
								"\t\tKrystal.Serializer.String.Write( stream, COOKIE );\n" \
								"\t}\n"

#define TCS_PACKET_WRITE_END	"}\n"

#define TCS_PACKET_MEMBER_AS_WRITE_PRIMITIVE	"Krystal.Serializer.%1%.Write( stream, %2% );\n" // serializer member_name

#define TCS_PACKET_MEMBER_AS_WRITE_REFERENCE	"Krystal.Serializer.Custom.Write( stream, %1%, true );\n"

#define TCS_PACKET_MEMBER_AS_WRITE_MAP_BEGIN	"Krystal.Serializer.Int.Write( stream, %1%.Count );\n" \
												"foreach( KeyValuePair<%2%, %3%> keyValueTemp in %1% )\n" \
												"{\n"

#define TCS_PACKET_MEMBER_AS_WRITE_MAP_END		"}\n"

#define TCS_PACKET_MEMBER_AS_WRITE_MAP_PRIMITIVE_VALUE	"Krystal.Serializer.%1%.Write( stream, keyValueTemp.Key );\n" \
														"Krystal.Serializer.%2%.Write( stream, keyValueTemp.Value );\n"

#define TCS_PACKET_MEMBER_AS_WRITE_MAP_REFERENCE_VALUE	"Krystal.Serializer.String.Write(stream, Convert.ToString(keyValueTemp.Key) );\n" \
														"Krystal.Serializer.Custom.Write( stream, Convert.ToString(keyValueTemp.Key), keyValueTemp.Value, true );\n"


#define TCS_PACKET_MEMBER_AS_WRITE_LIST_BEGIN	"Krystal.Serializer.Int.Write( stream, %1%.Count );\n" \
												"foreach( %2% val in %1% )\n" \
												"{\n"

#define TCS_PACKET_MEMBER_AS_WRITE_LIST_END		"}\n"

#define TCS_PACKET_MEMBER_AS_WRITE_LIST_PRIMITIVE_VALUE	"Krystal.Serializer.%1%.Write( stream, val );\n"

#define TCS_PACKET_MEMBER_AS_WRITE_LIST_REFERENCE_VALUE	"Krystal.Serializer.%1%.Write( stream, val, true );\n"

#define TCS_PACKET_READ_BEGIN	"public void Read( BinaryReader stream, bool isInner = false )\n" \
								"{\n" \
								"\tif ( false == isInner)\n" \
								"\t{\n" \
								"\t\tCOOKIE = Krystal.Serializer.String.Read( stream );\n" \
								"\t}\n"

#define TCS_PACKET_READ_END		"}\n"

#define TCS_PACKET_MEMBER_AS_READ_PRIMITIVE		"%1% = Krystal.Serializer.%2%.Read( stream );\n"

#define TCS_PACKET_MEMBER_AS_READ_REFERENCE		"Krystal.Serializer.Custom.Read( stream, %1%, true );\n"

#define TCS_PACKET_MEMBER_AS_READ_MAP_BEGIN		"%1%.Clear();\n" \
												"int %1%Count = Krystal.Serializer.Int.Read(stream);\n" \
												"for ( int %1%Index = 0; %1%Index < %1%Count; ++%1%Index )\n" \
												"{\n"

#define TCS_PACKET_MEMBER_AS_READ_MAP_END		"}\n"

#define TCS_PACKET_MEMBER_AS_READ_MAP_PRIMITIVE_VALUE	"%1% %2%Key = Krystal.Serializer.%3%.Read( stream );\n" \
														"%4% %2%Value = Krystal.Serializer.%5%.Read( stream );\n" \
														"%2%.Add( %2%Key, %2%Value );\n"
														//generic1 name generic1serializer generic2 generic2serializer

#define TCS_PACKET_MEMBER_AS_READ_MAP_REFERENCE_VALUE	"%1% %2%Key = Krystal.Serializer.%3%.Read( stream );\n" \
														"%4% %2%Temp = new %4%();\n" \
														"Krystal.Serializer.Custom.Read( stream, %2%Temp, true );\n" \
														"%2%.Add( %2%Key, %2%Temp );\n"
														//generic1 name generic1serializer generic2

#define TCS_PACKET_MEMBER_AS_READ_LIST_BEGIN	"%1%.Clear();\n" \
												"int %1%Count = Krystal.Serializer.Int.Read(stream);\n" \
												"for ( int i = 0; i< %1%Count; ++i )\n" \
												"{\n"

#define TCS_PACKET_MEMBER_AS_READ_LIST_END		"}\n"


#define TCS_PACKET_MEMBER_AS_READ_LIST_PRIMITIVE_VALUE	"%1% item = Krystal.Serializer.%2%.Read( stream );\n" \
														"%3%.Add(item);\n"
														//generic1 generic1serializer name

#define TCS_PACKET_MEMBER_AS_READ_LIST_REFERENCE_VALUE	"%1% item = new %1%();\n" \
														"Krystal.Serializer.Custom.Read(stream, item, true);\n" \
														"%2%.Add(item);\n"
														//generic1 name


#define TCS_PACKET_FIXED_METHODS_AFTER_READ		"public byte[] ToArray()\n" \
												"{\n" \
												"\tusing(MemoryStream ms = new MemoryStream())\n" \
												"\t{\n" \
												"\t\tusing(BinaryWriter bw = new BinaryWriter(ms))\n" \
												"\t\t{\n" \
												"\t\t\tWrite(bw);\n" \
												"\t\t\treturn ms.ToArray();\n" \
												"\t\t}\n" \
												"\t}\n" \
												"}\n" \
												"public void FromArray(byte[] bytes)\n" \
												"{\n" \
												"\tusing(MemoryStream ms = new MemoryStream(bytes))\n" \
												"\t{\n" \
												"\t\tusing(BinaryReader br = new BinaryReader(ms))\n" \
												"\t\t{\n" \
												"\t\t\tRead(br);\n" \
												"\t\t}\n" \
												"\t}\n" \
												"}\n" \
												"public void FromMemoryStream(MemoryStream ms)\n" \
												"{\n" \
												"\tFromArray(ms.GetBuffer());\n" \
												"}\n" \
												"public bool FromJSon(string userData, bool isInner = false)\n" \
												"{\n" \
												"\tstring webDecodedString = Uri.UnescapeDataString(userData);\n" \
												"\tDictionary<string, object> jsonList = (Dictionary<string, object>)fastJSON.JSON.Instance.Parse( webDecodedString );\n" \
												"\tobject requestObject = null;\n" \
												"\tif ( false == jsonList.TryGetValue(\"request\", out requestObject) )\n" \
												"\t{\n" \
												"\t\treturn false;\n" \
												"\t}\n" \
												"\tif (requestObject is Dictionary<string,object>)\n" \
												"\t{\n" \
												"\t\treturn ParseJsonObjectData(requestObject, isInner);\n" \
												"\t}\n" \
												"\telse if (requestObject is ArrayList)\n" \
												"\t{\n" \
												"\t\treturn ParseJsonArrayData(requestObject, isInner);\n" \
												"\t}\n" \
												"\treturn false;\n" \
												"}\n"

#define TCS_PACKET_PARSE_JSON_OBJECT_DATA_BEGIN	"public bool ParseJsonObjectData(object requestObject, bool isInner = false)\n" \
												"{\n" \
												"\tDictionary<string,object> jsonArrayList_Overall = requestObject as Dictionary<string,object>;\n" \
												"\tif ( %1% > jsonArrayList_Overall.Count )\n" \
												"\t{\n" \
												"\t\treturn false;\n" \
												"\t}\n" \
												"\tif ( false == isInner )\n" \
												"\t{\n" \
												"\t\tCOOKIE = Krystal.Serializer.JSon.String.Read(\"COOKIE\", jsonArrayList_Overall );\n" \
												"\t}\n"

#define TCS_PACKET_PARSE_JSON_OBJECT_DATA_END	"\treturn true;\n" \
												"}\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_PRIMITIVE	"%1% = Krystal.Serializer.JSon.%2%.Read( \"%1%\", jsonArrayList_Overall );\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_REFERENCE	"if ( jsonArrayList_Overall[\"%1%\"] is Dictionary<string, object> )\n" \
																"{\n" \
																"\tDictionary<string, object> %1%Value1 = jsonArrayList_Overall[\"%1%\"] as Dictionary<string, object>;\n" \
																"\tif ( null == %1%Value1 || 0 == %1%Value1.Count )\n" \
																"\t{\n" \
																"\t\treturn false;\n" \
																"\t}\n" \
																"\tKrystal.Serializer.JSon.Custom.Read( %1%Value1, \"%1%\", %1%, true );\n" \
																"}\n" \

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_BEGIN	"%1%.Clear();\n" \
																"if ( jsonArrayList_Overall[\"%1%\"] is Dictionary<string, object> )\n" \
																"{\n" \
																"\tDictionary<string, object> %1%Value1 = jsonArrayList_Overall[\"%1%\"] as Dictionary<string, object>;\n" \
																"\tforeach ( KeyValuePair<string, object> %1%Value7 in %1%Value1 )\n" \
																"\t{\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_END		"\t}\n" \
																"}\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_PRIMITIVE_VALUE		"%1% keyVal = %2%(%3%Value7.Key);\n" \
																			"%4% valueVal = Krystal.Serializer.JSon.%5%.Read( %3%Value7.Key, %3%Value1 );\n" \
																			"%3%.Add( keyVal, valueVal );\n"
																			// generic1, convert1, name, generic2, serializer2

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_MAP_REFERENCE_VALUE		"%1% keyVal = %2%(%3%Value7.Key);\n" \
																			"%4% valueVal = new %4%();\n" \
																			"Krystal.Serializer.JSon.Custom.Read( %3%Value7.Value, %3%Value7.Key, valueVal, true );\n" \
																			"%3%.Add( keyVal, valueVal );\n"
																			// generic1, convert1, name, generic2

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_LIST_BEGIN	"%1%.Clear();\n" \
																"ArrayList jsonArrayList_%1% = jsonArrayList_Overall[\"%1%\"] as ArrayList;\n"

//#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_LIST_END // no end string

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_LIST_PRIMITIVE_VALUE	"Krystal.Serializer.JSon.%1%.Read( \"%2%\", %2%, jsonArrayList_%2% );\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_OBJECT_DATA_LIST_REFERENCE_VALUE	"foreach ( var jsonArrayListIter_%1% in jsonArrayList_%1% )\n" \
																			"{\n" \
																			"\t%2% dataItem = new %2%();\n" \
																			"\tKrystal.Serializer.JSon.Custom.Read( jsonArrayListIter_%1%, \"%2%\", dataItem, true );\n" \
																			"\t%1%.Add( dataItem );\n" \
																			"}\n"

#define TCS_PACKET_PARSE_JSON_ARRAY_DATA_BEGIN	"public bool ParseJsonArrayData(object requestObject, bool isInner = false)\n" \
												"{\n" \
												"\tArrayList jsonArrayList_Overall = requestObject as ArrayList;\n" \
												"\tif ( %1% > jsonArrayList_Overall.Count )\n" \
												"\t{\n" \
												"\t\treturn false;\n" \
												"\t}\n" \
												"\tint arrayIndex = 0;\n" \
												"\tif ( false == isInner )\n" \
												"\t{\n" \
												"\t\tCOOKIE = Krystal.Serializer.JSon.String.Read(\"COOKIE\", jsonArrayList_Overall[arrayIndex++] );\n" \
												"\t}\n"

#define TCS_PACKET_PARSE_JSON_ARRAY_DATA_END	"\treturn true;\n" \
												"}\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_PRIMITIVE	"%1% = Krystal.Serializer.JSon.%2%.Read( \"%1%\", jsonArrayList_Overall[arrayIndex] );\n" \
																"arrayIndex += 1;\n"
																// name, serializer

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_REFERENCE	"if ( jsonArrayList_Overall[arrayIndex] is Dictionary<string, object> )\n" \
																"{\n" \
																"\tDictionary<string, object> %1%Value1 = jsonArrayList_Overall[arrayIndex] as Dictionary<string, object>;\n" \
																"\tif ( null == %1%Value1 || 0 == %1%Value1.Count )\n" \
																"\t{\n" \
																"\t\treturn false;\n" \
																"\t}\n" \
																"\tobject %1%Value2 = null;\n" \
																"\tif ( true == %1%Value1.TryGetValue( \"%1%\", out %1%Value2 ) )\n" \
																"\t{\n" \
																"\t\tKrystal.Serializer.JSon.Custom.Read( %1%Value2, \"%1%\", %1%, true );\n" \
																"\t}\n" \
																"}\n" \
																"arrayIndex += 1;\n"
																// name

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_MAP_BEGIN	"%1%.Clear();\n" \
																"if ( jsonArrayList_Overall[arrayIndex] is Dictionary<string, object> )\n" \
																"{\n" \
																"\tDictionary<string, object> %1%Value1 = jsonArrayList_Overall[arrayIndex] as Dictionary<string, object>;\n" \
																"\tobject %1%Value2 = null;\n" \
																"\tif ( true == %1%Value1.TryGetValue(\"%1%\", out %1%Value2 ) )\n" \
																"\t{\n" \
																"\t\tArrayList %1%Value3 = %1%Value2 as ArrayList;\n" \
																"\t\tArrayList %1%Value4 = %1%Value3 as ArrayList;\n" \
																"\t\tforeach ( ArrayList %1%Value5 in %1%Value4 )\n" \
																"\t\t{\n" \
																"\t\t\tforeach ( Dictionary<string, object> %1%Value6 in %1%Value5 )\n" \
																"\t\t\t{\n" \
																"\t\t\t\tforeach ( KeyValuePair<string, object> %1%Value7 in %1%Value6 )\n" \
																"\t\t\t\t{\n"
																// name

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_MAP_END		"\t\t\t\t}\n" \
																"\t\t\t}\n" \
																"\t\t}\n" \
																"\t}\n" \
																"}\n" \
																"arrayIndex += 1;\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_MAP_PRIMITIVE_VALUE		"%1% keyVal = %2%(%3%Value7.Key);\n" \
																			"%4% valueVal = Krystal.Serializer.JSon.%5%.Read( %3%Value7.Key, %3%Value6 );\n" \
																			"%3%.Add( keyVal, valueVal );\n"
																			// generic1, generic1convert, name, generic2, generic2serializer

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_MAP_REFERENCE_VALUE		"%1% keyVal = %2%(%3%Value7.Key);\n" \
																			"%4% valueVal = new %4%();\n" \
																			"Krystal.Serializer.JSon.Custom.Read( %3%Value7.Value, %3%Value7.Key, valueVal, true );\n" \
																			"%3%.Add( keyVal, valueVal );\n"
																			// generic1, generic1convert, name, generic2

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_LIST_BEGIN	"%1%.Clear();\n" \
																"ArrayList jsonArrayList_%1% = jsonArrayList_Overall[arrayIndex] as ArrayList;\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_LIST_END		"arrayIndex += 1;\n"

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_LIST_PRIMITIVE_VALUE		"Krystal.Serializer.JSon.%1%.Read( \"%2%\", %2%, jsonArrayList_%2% );\n"
																			// generic1serializer, name

#define TCS_PACKET_MEMBER_AS_PARSE_JSON_ARRAY_DATA_LIST_REFERENCE_VALUE		"foreach ( ArrayList jsonArrayListIter_%1% in jsonArrayList_%1% )\n" \
																			"{\n" \
																			"\tforeach ( Dictionary<string, object> jsonDictionaryInList_%1% in jsonArrayListIter_%1% )\n" \
																			"\t{\n" \
																			"\t\tobject objectItem_%1% = null;\n" \
																			"\t\tif ( true == jsonDictionaryInList_%1%.TryGetValue( \"%1%\", out objectItem_%1% ) )\n" \
																			"\t\t{\n" \
																			"\t\t\t%2% dataItem = new %2%();\n" \
																			"\t\t\tKrystal.Serializer.JSon.Custom.Read( objectItem_%1%, \"%2%\", dataItem, true );\n" \
																			"\t\t\t%1%.Add( dataItem );\n" \
																			"\t\t}\n" \
																			"\t}\n" \
																			"}\n"
																			// name, generic1

#define TCS_PACKET_FROM_JSON_PACKAGE	"public bool FromJSonPackage(object objList, bool isInner = false)\n" \
										"{\n" \
										"\tif (objList is Dictionary<string, object>)\n" \
										"\t{\n" \
										"\treturn ParseJsonObjectData(objList, isInner);\n" \
										"\t}\n" \
										"\telse if (objList is ArrayList)\n" \
										"\t{\n" \
										"\treturn ParseJsonArrayData(objList, isInner);\n" \
										"\t}\n" \
										"\treturn false;\n" \
										"}\n"

#define TCS_PACKET_TO_JSON_PACKAGE_ARRAY_BEGIN			"public void ToJSonPackage( ArrayList objList, bool isInner = false )\n" \
														"{\n" \
														"\tif ( false == isInner )\n" \
														"\t{\n" \
														"\t\tKrystal.Serializer.JSon.String.Write( objList, \"COOKIE\", COOKIE );\n" \
														"\t}\n"

#define TCS_PACKET_TO_JSON_PACKAGE_ARRAY_END			"}\n"

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_ARRAY_PRIMITIVE		"Krystal.Serializer.JSon.%1%.Write( objList, \"%2%\", %2% );\n"
																	// serializer, name

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_ARRAY_REFERENCE		"Krystal.Serializer.JSon.Custom.Write( objList, \"%1%\", %1%, true );\n"
																	// name

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_ARRAY_MAP_PRIMITIVE_VALUE			"ArrayList %1%Value1 = new ArrayList();\n" \
																				"foreach ( KeyValuePair<%2%, %3%> val in %1% )\n" \
																				"{\n" \
																				"\tArrayList %1%Value2 = new ArrayList();\n" \
																				"\tKrystal.Serializer.JSon.%4%.Write( %1%Value2, val.Key.ToString(), val.Value );\n" \
																				"\t%1%Value1.Add( %1%Value2 );\n" \
																				"}\n" \
																				"Dictionary<string, object> %1%Value3 = new Dictionary<string, object>();\n" \
																				"%1%Value3.Add( \"%1%\", %1%Value1 );\n" \
																				"objList.Add( %1%Value3 );\n"
																				// name, generic1, generic2, generic2serializer

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_ARRAY_MAP_REFERENCE_VALUE			"ArrayList %1%Value1 = new ArrayList();\n" \
																				"foreach ( KeyValuePair<%2%, %3%> val in %1% )\n" \
																				"{\n" \
																				"\tArrayList %1%Value2 = new ArrayList();\n" \
																				"\tKrystal.Serializer.JSon.Custom.Write( %1%Value2, val.Key.ToString(), val.Value, true );\n" \
																				"\t%1%Value1.Add( %1%Value2 );\n" \
																				"}\n" \
																				"Dictionary<string, object> %1%Value3 = new Dictionary<string, object>();\n" \
																				"%1%Value3.Add( \"%1%\", %1%Value1 );\n" \
																				"objList.Add( %1%Value3 );\n"
																				// name, generic1, generic2

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_ARRAY_LIST_PRIMITIVE_VALUE			"Krystal.Serializer.JSon.%1%.Write( objList, \"%2%\", %2% );\n"
																				// serializer1, name

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_ARRAY_LIST_REFERENCE_VALUE			"ArrayList %1%Value1 = new ArrayList();\n" \
																				"foreach ( %2% val in %1% )\n" \
																				"{\n" \
																				"\tArrayList %1%Value2 = new ArrayList();\n" \
																				"\tKrystal.Serializer.JSon.Custom.Write( %1%Value2, \"%1%\", val, true );\n" \
																				"\t%1%Value1.Add( %1%Value2 );\n" \
																				"}\n" \
																				"objList.Add( %1%Value1 );\n"
																				// name, generic1

#define TCS_PACKET_TO_JSON_PACKAGE_DICTIONARY_BEGIN		"public void ToJSonPackage( Dictionary<string,object> objDic, bool isInner = false )\n" \
														"{\n" \
														"\tif ( false == isInner )\n" \
														"\t{\n" \
														"\t\tKrystal.Serializer.JSon.String.Write( objDic, \"COOKIE\", COOKIE );\n" \
														"\t}\n" \

#define TCS_PACKET_TO_JSON_PACKAGE_DICTIONARY_END		"}\n"

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_DICTIONARY_PRIMITIVE	"Krystal.Serializer.JSon.%1%.Write( objDic, \"%2%\", %2% );\n"

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_DICTIONARY_REFERENCE	"Krystal.Serializer.JSon.Custom.Write( objDic, \"%1%\", %1%, true );\n"

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_DICTIONARY_MAP_PRIMITIVE_VALUE		"Dictionary<string,object> %1%Value1 = new Dictionary<string,object>();\n" \
																				"foreach ( KeyValuePair<%2%, %3%> val in %1% )\n" \
																				"{\n" \
																				"\tKrystal.Serializer.JSon.%4%.Write( %1%Value1, val.Key.ToString(), val.Value );\n" \
																				"}\n" \
																				"objDic.Add( \"%1%\", %1%Value1 );\n"
																				// name, generic1, generic2, generic2serializer

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_DICTIONARY_MAP_REFERENCE_VALUE		"Dictionary<string,object> %1%Value1 = new Dictionary<string,object>();\n" \
																				"foreach ( KeyValuePair<%2%, %3%> val in %1% )\n" \
																				"{\n" \
																				"\tKrystal.Serializer.JSon.Custom.Write( %1%Value1, val.Key.ToString(), val.Value, true );\n" \
																				"}\n" \
																				"objDic.Add( \"%1%\", %1%Value1 );\n"
																				// name, generic1, generic2

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_DICTIONARY_LIST_PRIMITIVE_VALUE	"Krystal.Serializer.JSon.%1%.Write( objDic, \"%2%\", %2% );\n"
																				// serializer1, name

#define TCS_PACKET_MEMBER_AS_TO_JSON_PACKAGE_DICTIONARY_LIST_REFERENCE_VALUE	"ArrayList %1%Value1 = new ArrayList();\n" \
																				"foreach ( %2% val in %1% )\n" \
																				"{\n" \
																				"\tDictionary<string,object> %1%Value2 = new Dictionary<string,object>();\n" \
																				"\tKrystal.Serializer.JSon.Custom.WriteForList( %1%Value2, \"%1%\", val, true );\n" \
																				"\t%1%Value1.Add( %1%Value2 );\n" \
																				"}\n" \
																				"objDic.Add(\"%1%\", %1%Value1 );\n" \
																				// name, generic1

#define TCS_PACKET_TO_JSON	"public byte[] ToJSon(bool isInner = false)\n" \
							"{\n" \
							"\tstring response = \"\";\n" \
							"\tDictionary<string,object> objDic = new Dictionary<string,object>();\n" \
							"\tToJSonPackage( objDic, isInner );\n" \
							"\tDictionary<string, object> jsonList = new Dictionary<string, object>();\n" \
							"\tjsonList.Add( \"request\", objDic );\n" \
							"\tfastJSON.JSON.Instance.Parameters.UseExtensions = false;\n" \
							"\tresponse = fastJSON.JSON.Instance.ToJSON(jsonList);\n" \
							"\treturn Encoding.ASCII.GetBytes(response);\n" \
							"}\n"


#endif // TEMPLATE_CS_H
