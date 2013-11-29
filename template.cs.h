// C#
#ifndef TEMPLATE_CS_H
#define TEMPLATE_CS_H


namespace CsParseAs
{
	enum Enum
	{
		Default = 0,
		GetLength,
		Write,
		Read
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

#define TCS_PACKET_ID_FIELD		"public static readonly int ID = %1%\n"

#define TCS_PACKET_MEMBER_AS_DEFAULT	"public %1% %2%;\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_PRIMITIVE	"length += Krystal.Serializer.%1%.GetLength(%2%);\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_REFERENCE	"length += Krystal.Serializer.Custom.GetLength(%1%, true);\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_MAP_BEGIN	"length += 4;\n" \
													"foreach ( KeyValuePair<%1%, %2%> %3%Temp in %3% )\n" \
													"{\n" \
													"\t%4%\t%5%}\n"

#define TCS_PACKET_MEMBER_AS_GET_LENGTH_LIST_BEGIN	""

#endif // TEMPLATE_CS_H
