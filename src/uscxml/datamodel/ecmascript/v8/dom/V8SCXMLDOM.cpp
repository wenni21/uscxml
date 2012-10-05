#include "V8SCXMLDOM.h"

namespace uscxml {
	
  using namespace Arabica::DOM;
  using namespace Arabica::XPath;
  
	V8SCXMLDOM::V8SCXMLDOM(Interpreter* interpreter) {
		_interpreter = interpreter;
	}

  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::getDocument() {
    v8::Handle<v8::ObjectTemplate> documentTmpl = v8::ObjectTemplate::New();
    documentTmpl->Set(v8::String::New("createElement"), v8::FunctionTemplate::New(jsDocumentCreateElement, v8::External::New(reinterpret_cast<void*>(_interpreter))));
    documentTmpl->Set(v8::String::New("evaluate"), v8::FunctionTemplate::New(jsDocumentEvaluate, v8::External::New(reinterpret_cast<void*>(_interpreter))));
    return documentTmpl;
  }

  v8::Handle<v8::Value> V8SCXMLDOM::jsDocumentCreateElement(const v8::Arguments& args) {
    assert(!args.Data().IsEmpty());
    assert(args.Data()->IsExternal());

    Interpreter* interpreter = static_cast<Interpreter*>(v8::External::Unwrap(args.Data()));
    v8::Handle<v8::ObjectTemplate> elementTmpl = v8::ObjectTemplate::New();
    elementTmpl->SetAccessor(v8::String::New("tagName"), V8SCXMLDOM::jsElementTagName);
    elementTmpl->Set(v8::String::New("getAttribute"), v8::FunctionTemplate::New(jsElementGetAttribute));
    elementTmpl->Set(v8::String::New("setAttribute"), v8::FunctionTemplate::New(jsElementSetAttribute));
    elementTmpl->SetInternalFieldCount(1);
    v8::Handle<v8::Object> elementJS = elementTmpl->NewInstance();

    assert(args.Length() == 1);
    assert(args[0]->IsString());
    
    v8::String::AsciiValue tagName(args[0]);
    Element<std::string>* element = new Element<std::string>(interpreter->getDocument().createElement(*tagName));

    elementJS->SetInternalField(0, v8::External::New(element));
    return elementJS;
  }
	
  v8::Handle<v8::Value> V8SCXMLDOM::jsDocumentEvaluate(const v8::Arguments& args) {
    assert(!args.Data().IsEmpty());
    assert(args.Data()->IsExternal());

    assert(args.Length() > 0);
    assert(args[0]->IsString());
    
    
    Interpreter* interpreter = static_cast<Interpreter*>(v8::External::Unwrap(args.Data()));
    Node<std::string> context;
    if (args.Length() > 1) {
      assert(args[1]->ToObject()->InternalFieldCount() == 1);
      context = *static_cast<Node<std::string>*>(v8::Local<v8::External>::Cast(args[1]->ToObject()->GetInternalField(0))->Value());
    } else {
      context = interpreter->getDocument();
    }
    v8::String::AsciiValue xpathExpr(args[0]);
    XPathValue<std::string>* xpathValue = new XPathValue<std::string>(interpreter->getXPath().evaluate(*xpathExpr, context));
    
    v8::Handle<v8::Object> xpathValueJS = getXPathValueTmpl()->NewInstance();
    xpathValueJS->SetInternalField(0, v8::External::New(xpathValue));
    return xpathValueJS;
  }
  
  v8::Handle<v8::Value> V8SCXMLDOM::jsElementTagName(v8::Local<v8::String> property,
                                                     const v8::AccessorInfo &info) {
    Element<std::string>* element = static_cast<Element<std::string>*>(v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0))->Value());
    return v8::String::New(element->getTagName().c_str());
  }

  v8::Handle<v8::Value> V8SCXMLDOM::jsElementGetAttribute(const v8::Arguments& args) {
    assert(!args.Data().IsEmpty());
    assert(args.Data()->IsExternal());

    assert(args.Length() == 1);
    assert(args[0]->IsString());

    Element<std::string>* element = static_cast<Element<std::string>*>(v8::External::Unwrap(args.Data()));
    
    v8::String::AsciiValue attribute(args[0]);
    if (element->hasAttribute(*attribute)) {
      return v8::String::New(element->getAttribute(*attribute).c_str());
    }
    return v8::String::New("");
  }

  v8::Handle<v8::Value> V8SCXMLDOM::jsElementSetAttribute(const v8::Arguments& args) {
    v8::Local<v8::Object> self = args.Holder();
    assert(self->InternalFieldCount() == 1);
    
    assert(args.Length() == 2);
    assert(args[0]->IsString());
    assert(args[1]->IsString());
    
    v8::String::AsciiValue attribute(args[0]);
    v8::String::AsciiValue value(args[1]);

    Element<std::string>* element = static_cast<Element<std::string>*>(v8::External::Unwrap(self->GetInternalField(0)));
    element->setAttribute(*attribute, *value);
    return v8::Undefined();
  }

  v8::Handle<v8::Value> V8SCXMLDOM::jsXPathValueAsNodeSet(const v8::Arguments& args) {
    v8::Local<v8::Object> self = args.Holder();
    assert(self->InternalFieldCount() == 1);
    XPathValue<std::string>* xPathValue = static_cast<XPathValue<std::string>*>(v8::External::Unwrap(self->GetInternalField(0)));

    v8::Persistent<v8::Object> nodeSetJS = v8::Persistent<v8::Object>::New(getNodeSetTmpl()->NewInstance());
    nodeSetJS->SetInternalField(0, v8::External::New(new NodeSet<std::string>(xPathValue->asNodeSet())));
    nodeSetJS.MakeWeak(NULL, jsNodeSetDestructor);
    return nodeSetJS;

  }
  
  void V8SCXMLDOM::jsNodeSetDestructor(v8::Persistent<v8::Value> object, void* data) {
    NodeSet<std::string>* nodeSet = static_cast<NodeSet<std::string>*>(v8::Local<v8::External>::Cast(object->ToObject()->GetInternalField(0))->Value());
    delete nodeSet;
  }
  
  v8::Handle<v8::Value> V8SCXMLDOM::jsNodeSetGetIndex(uint32_t index, const v8::AccessorInfo &info) {
    v8::Local<v8::Object> self = info.Holder();
    assert(self->InternalFieldCount() == 1);
    NodeSet<std::string>* nodeSet = static_cast<NodeSet<std::string>*>(v8::Local<v8::External>::Cast(info.Holder()->GetInternalField(0))->Value());

    if (nodeSet->size() >= index) {
      Node<std::string>* node = new Node<std::string>((*nodeSet)[index]);
      v8::Handle<v8::Object> nodeJS = getNodeTmpl()->NewInstance();
      nodeJS->SetInternalField(0, v8::External::New(node));
      return nodeJS;
    }
    return v8::Undefined();    
  }

  v8::Handle<v8::Value> V8SCXMLDOM::jsNodeSetLength(const v8::Arguments& args) {
    v8::Local<v8::Object> self = args.Holder();
    assert(self->InternalFieldCount() == 1);
    NodeSet<std::string>* nodeSet = static_cast<NodeSet<std::string>*>(v8::External::Unwrap(self->GetInternalField(0)));
    return v8::Integer::New(nodeSet->size());
  }

  v8::Handle<v8::Value> V8SCXMLDOM::jsNodeAppendChild(const v8::Arguments& args) {
    v8::Local<v8::Object> self = args.Holder();
    assert(self->InternalFieldCount() == 1);
    Node<std::string>* node = static_cast<Node<std::string>*>(v8::External::Unwrap(self->GetInternalField(0)));

    assert(args.Length() == 1);
    assert(args[0]->IsObject());

    Node<std::string>* childToAppend = static_cast<Node<std::string>*>(v8::External::Unwrap(args[0]->ToObject()->GetInternalField(0)));
    node->appendChild(*childToAppend);
    
//    std::cout << *childToAppend << std::endl;
    
    return v8::Undefined();
  }
  
  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::xPathValueTmpl;
  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::getXPathValueTmpl() {
    if (xPathValueTmpl.IsEmpty()) {
      xPathValueTmpl = v8::ObjectTemplate::New();
      xPathValueTmpl->SetInternalFieldCount(1);
      xPathValueTmpl->Set(v8::String::New("asNodeSet"), v8::FunctionTemplate::New(jsXPathValueAsNodeSet));
    }
    return xPathValueTmpl;
  }

  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::nodeSetTmpl;
  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::getNodeSetTmpl() {
    if (nodeSetTmpl.IsEmpty()) {
      nodeSetTmpl = v8::ObjectTemplate::New();
      nodeSetTmpl->SetInternalFieldCount(1);
      nodeSetTmpl->SetIndexedPropertyHandler(jsNodeSetGetIndex);
      nodeSetTmpl->Set(v8::String::New("length"), v8::FunctionTemplate::New(jsNodeSetLength));
    }
    return nodeSetTmpl;
  }

  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::nodeTmpl;
  v8::Handle<v8::ObjectTemplate> V8SCXMLDOM::getNodeTmpl() {
    if (nodeTmpl.IsEmpty()) {
      nodeTmpl = v8::ObjectTemplate::New();
      nodeTmpl->SetInternalFieldCount(1);
      nodeTmpl->Set(v8::String::New("appendChild"), v8::FunctionTemplate::New(jsNodeAppendChild));
    }
    return nodeTmpl;
  }

}