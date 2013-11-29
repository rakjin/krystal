// C#
#ifndef TEMPLATE_CS_H
#define TEMPLATE_CS_H

#include "template.common.h"

// must correspond with KstDataType enum in template.common.h
string CsSerializerName[KstDataType::LENGTH_OF_KST_DATA_TYPES] =
{
	"Bool",
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

string* lookupSerializerName(string* typeString)
{
	KstDataType::Enum kstDataType = lookupKstDataType(typeString);
    if (((int)kstDataType) == -1)
    {
        throw(runtime_error("Data type not found"));
    }
    string* serializerName = &(CsSerializerName[kstDataType]);
    return serializerName;
}

namespace CsParseAs
{
	enum Enum
	{
		Default = 0,
		GetLength,
		Write,
		Read,
		Initialization,
		SerializerName,
		GenericType1,
		GenericType2,
		GenericType3,
		GenericTypeSerializer1,
		GenericTypeSerializer2,
		GenericTypeSerializer3
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

#define TCS_NAMESPACE_BEGIN		"namespace %1%\n" \
								"{\n"

#define TCS_NAMESPACE_END		"}\n"

#define TCS_PACKET_BEGIN		"public class %1% : Krystal.IMessageStream\n" \
								"{\n"

#define TCS_PACKET_END			"}\n"

#define TCS_PACKET_ID_TYPE		int // cast hash value to this type only in output

#define TCS_PACKET_ID_FIELD		"public static readonly int ID = %1%;\n"

#define TCS_PACKET_COOKIE_FIELD	"public string COOKIE = \"\";\n"

#define TCS_PACKET_MEMBER_AS_DEFAULT	"public %1% %2%%3%;\n" //type name initialization

#define TCS_PACKET_MEMBER_TYPE_MAP_AS_DEFAULT	"Dictionary<%1%, %2%>"

#define TCS_PACKET_MEMBER_TYPE_LIST_AS_DEFAULT	"List<%1%>"

#define TCS_PACKET_MEMBER_TYPE_NEW_AS_INITIALIZATION	" = new %1%()"

#define TCS_PACKET_CONSTRUCTOR		"public %1%()\n" \
									"{\n" \
									"}\n"

#define TCS_PACKET_GET_ID			"public int GetID()\n" \
									"{\n" \
									"\treturn ID;\n" \
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

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_MAP			"length += 4;\n" \
													"foreach ( KeyValuePair<%1%, %2%> %3%Temp in %3% )\n" \
													"{\n" \
													"\tlength += Krystal.Serializer.%4%.GetLength(%3%Temp.Key);\n" \
													"\tlength += Krystal.Serializer.%5%.GetLength(%3%Temp.Value);\n" \
													"}\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_LIST		"length += 4;\n" \
													"foreach ( %1% val in %2% )\n" \
													"{\n" \
													"\tlength += Krystal.Serializer.%4%.GetLength(val, true);\n" \
													"}\n"

#endif // TEMPLATE_CS_H
