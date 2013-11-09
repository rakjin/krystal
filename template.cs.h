// C#
#ifndef TEMPLATE_CS_H
#define TEMPLATE_CS_H

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

#define TCS_PACKET_END			"}"


#endif // TEMPLATE_CS_H
