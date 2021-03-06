/**
 *  @file
 *  @author     This file has been generated by generate-bindings.pl. DO NOT MODIFY!
 *  @copyright  Simplified BSD
 *
 *  @cond
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the FreeBSD license as published by the FreeBSD
 *  project.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *  You should have received a copy of the FreeBSD license along with this
 *  program. If not, see <http://www.opensource.org/licenses/bsd-license>.
 *  @endcond
 */

#ifndef JSCElement_h
#define JSCElement_h

#include <string>
#include "DOM/Node.hpp"
#include "JSCNode.h"
#include <JavaScriptCore/JavaScriptCore.h>
#include "uscxml/plugins/datamodel/ecmascript/JavaScriptCore/JSCDOM.h"

namespace Arabica {
namespace DOM {

class JSCElement {
public:
	struct JSCElementPrivate {
		JSCDOM* dom;
		Arabica::DOM::Element<std::string>* nativeObj;
	};

	JSC_DESTRUCTOR(JSCElementPrivate);

	static JSValueRef getAttributeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef setAttributeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef removeAttributeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef getAttributeNodeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef setAttributeNodeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef removeAttributeNodeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef getElementsByTagNameCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef getAttributeNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef setAttributeNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef removeAttributeNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef getAttributeNodeNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef setAttributeNodeNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef getElementsByTagNameNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef hasAttributeCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);
	static JSValueRef hasAttributeNSCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObj, size_t argumentCount, const JSValueRef* arguments, JSValueRef* exception);

	static JSValueRef tagNameAttrGetter(JSContextRef ctx, JSObjectRef thisObj, JSStringRef propertyName, JSValueRef* exception);


	static JSStaticValue staticValues[];
	static JSStaticFunction staticFunctions[];

	static JSClassRef Tmpl;
	static JSClassRef getTmpl() {
		if (Tmpl == NULL) {
			JSClassDefinition classDef = kJSClassDefinitionEmpty;
			classDef.staticValues = staticValues;
			classDef.staticFunctions = staticFunctions;
			classDef.finalize = jsDestructor;
			classDef.parentClass = JSCNode::getTmpl();

			Tmpl = JSClassCreate(&classDef);
			JSClassRetain(Tmpl);
		}
		return Tmpl;
	}


};

}
}

#endif // JSCElement_h
