#include "Main.h"
#include <node.h>
#include "myobject.h"

namespace demo {

	using v8::Local;
	using v8::Object;

	void InitMyObject(Local<Object> exports) {
		MyObject::Init(exports);
	}

	void InitCallbacks(Local<Object> exports, Local<Object> module) {
		NODE_SET_METHOD(module, "exports", RunCallback);
	}

	//Can only use one or the other below

	//user for registering object code
	NODE_MODULE(NodeAddOn, InitMyObject)
	
	//Use for registering code for call backs	
	//NODE_MODULE(NodeAddOn, InitCallbacks)

}  // namespace demo
