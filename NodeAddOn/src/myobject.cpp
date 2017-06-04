#include "myobject.h"
#include <windows.h>
namespace demo {

	using v8::Context;
	using v8::Function;
	using v8::FunctionCallbackInfo;
	using v8::FunctionTemplate;
	using v8::Isolate;
	using v8::Local;
	using v8::Number;
	using v8::Object;
	using v8::Persistent;
	using v8::String;
	using v8::Value;
	using v8::Exception;

	Persistent<Function> MyObject::constructor;

	void RunCallback(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();
		Local<Function> cb = Local<Function>::Cast(args[0]);
		const unsigned argc = 1;
		Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "hello world") };
		cb->Call(Null(isolate), argc, argv);
		::Sleep(1000);
		cb->Call(Null(isolate), argc, argv);
		::Sleep(1000);
		cb->Call(Null(isolate), argc, argv);
		::Sleep(1000);
		cb->Call(Null(isolate), argc, argv);
		::Sleep(1000);
		cb->Call(Null(isolate), argc, argv);
	}


	MyObject::MyObject(double value) : value_(value) {
	}

	MyObject::~MyObject() {
	}

	void MyObject::Init(Local<Object> exports) {
		Isolate* isolate = exports->GetIsolate();

		// Prepare constructor template
		Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
		tpl->SetClassName(String::NewFromUtf8(isolate, "MyObject"));
		tpl->InstanceTemplate()->SetInternalFieldCount(1);

		// Prototype has state value_
		NODE_SET_PROTOTYPE_METHOD(tpl, "plusOne", PlusOne);
		
		//Static no state method
		NODE_SET_METHOD(exports, "add", Add);

		constructor.Reset(isolate, tpl->GetFunction());
		exports->Set(String::NewFromUtf8(isolate, "MyObject"),
			tpl->GetFunction());
	}

	void MyObject::New(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		if (args.IsConstructCall()) {
			// Invoked as constructor: `new MyObject(...)`
			double value = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
			MyObject* obj = new MyObject(value);
			obj->Wrap(args.This());
			args.GetReturnValue().Set(args.This());
		}
		else {
			// Invoked as plain function `MyObject(...)`, turn into construct call.
			const int argc = 1;
			Local<Value> argv[argc] = { args[0] };
			Local<Context> context = isolate->GetCurrentContext();
			Local<Function> cons = Local<Function>::New(isolate, constructor);
			Local<Object> result =
				cons->NewInstance(context, argc, argv).ToLocalChecked();
			args.GetReturnValue().Set(result);
		}
	}

	void MyObject::PlusOne(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		MyObject* obj = ObjectWrap::Unwrap<MyObject>(args.Holder());
		obj->value_ += 1;

		args.GetReturnValue().Set(Number::New(isolate, obj->value_));
	}

	void MyObject::Add(const FunctionCallbackInfo<Value>& args) {
		Isolate* isolate = args.GetIsolate();

		//Check the number of arguments passed.
		if (args.Length() < 2) {
			// Throw an Error that is passed back to JavaScript
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong number of arguments")));
			return;
		}



		// Check the argument types
		if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
			isolate->ThrowException(Exception::TypeError(
				String::NewFromUtf8(isolate, "Wrong arguments")));
			return;
		}


		// Perform the operation
		double value = args[0]->NumberValue() + args[1]->NumberValue();
		Local<Number> num = Number::New(isolate, value);

		// Set the return value (using the passed in
		// FunctionCallbackInfo<Value>&)
		args.GetReturnValue().Set(num);


	}

}  // namespace demo
